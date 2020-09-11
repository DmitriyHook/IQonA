#include "iqanalyzer.h"

IQAnalyzer::IQAnalyzer(int id, QObject *parent) :
    m_id {id},
    m_mode {IQAnalyzerMode::UNKNOW},
    m_inputIQType{IQContainer::IQType::PCM24},
    m_amplitudeRange{0,192,0,4},
    m_frequencyRange{0,96,-4800,4800},
    m_spectrumRange{-4800, 4800, -50, 120},
    m_constellationRange{-2000,2000,-2000,2000},
    m_nvl {0},
    m_in {nullptr},
    m_out {nullptr},
    m_spectrCount {0},
    m_Xcount {0},
    m_offsetFrequency {0}

{
    m_nco = new IQNco(static_cast<int>(pow(2.0,16.0)));
}

IQAnalyzer::~IQAnalyzer()
{
    delete m_nco;

    if(m_in != nullptr)
        delete[] m_in;
    if(m_out != nullptr)
        delete[] m_out;
}

void IQAnalyzer::readPacket(QByteArray data)
{
    if(data.count() -1  < m_iqStartNumberByte) return;

    m_signalParametrs.clear();
    IQBuffer iqBuffer(m_inputIQType);
    iqBuffer.setData(QByteArray(&data.data()[m_iqStartNumberByte], data.count()-m_iqStartNumberByte));

    //сдвиг сигнала на определённую частоту
    for (int i=0; i<iqBuffer.count(); i++)
    {
        int _i_=0, _q_=0;
        _i_ = static_cast<int>(iqBuffer.at(i).I());
        _q_ = static_cast<int>(iqBuffer.at(i).Q());

        iqBuffer[i].setI(_q_ * m_nco->getSin() + _i_ * m_nco->getCos());
        iqBuffer[i].setQ(_q_ * m_nco->getCos() - _i_ * m_nco->getSin());

        m_nco->setControl(m_offsetFrequency);
    }

    for (int i = 0; i < iqBuffer.count(); i++)
    {
        double currentI = iqBuffer.at(i).I();
        double currentQ = iqBuffer.at(i).Q();
        double currentAmplitude = sqrt(pow(currentI,2) + pow(currentQ,2));
        double currentIdivAmpl = currentI / currentAmplitude;
        double currentQdivAmpl = currentQ / currentAmplitude;

        //построение графика амплитуды
        m_amplitudePlot.X.append(i);
        m_amplitudePlot.Y.append(currentAmplitude / 16375.0);
        m_signalParametrs.setMidAmplitude(m_signalParametrs.midAmplitude() + m_amplitudePlot.Y.last());
        if((currentAmplitude / 16375.0) > m_signalParametrs.maxAmplitude()) m_signalParametrs.setMaxAmplitude((currentAmplitude / 16375.0));

        //построение графика скорости изменения частоты
        m_frequencyPlot.X.append(i/2);
        m_frequencyPlot.Y.append((currentIdivAmpl*oldQ - currentQdivAmpl*oldI)*9600/(M_PI * 2));
        oldI = currentIdivAmpl;
        oldQ = currentQdivAmpl;

        //построение графика спектра
        if(m_nvl)
        {
            m_spectrumPlot.X[m_Xcount] = m_Xcount*(9600.0/m_nvl) - 4800;
            m_in[m_spectrCount][0] = currentI;
            m_in[m_spectrCount][1] = currentQ;
            m_spectrCount++;
            if(++m_Xcount == m_nvl) m_Xcount = 0;

            if (m_spectrCount >= (m_nvl))
            {
                fftw_execute_dft(m_fftw_plan, m_in, m_out);
                for (int j=0;j<m_nvl;j++)
                {
                    m_spectrumPlot.Y[j] = 0.0;
                    m_spectrumPlot.Y[j] += 20.0*log10(sqrt(pow(m_out[j][0],2.0) + pow(m_out[j][1],2.0))/16000/10/M_SQRT2);
                }
                m_spectrCount = 0;
                emit newPlot(m_spectrumPlot.X, IQAnalyzerPlot::spectrumRearrangement(m_spectrumPlot.Y));
            }
        }

        //построение графика созвездия
        m_constellationPlot.X.append(currentI);
        m_constellationPlot.Y.append(currentQ);

    }

    //заполнение структуры с основными параметрами сигнала
    m_signalParametrs.setMidAmplitude(m_signalParametrs.midAmplitude() / iqBuffer.count());
    m_signalParametrs.setFrequency(m_spectrumPlot.X.at(IQAnalyzerPlot::maxValueIndex(m_spectrumPlot.spectrumRearrangement(m_spectrumPlot.Y))));
    emit newSignalParametrs(m_signalParametrs);

    switch (m_mode)
    {
        case UNKNOW:break;
        case AMPLITUDE:
        {
            emit newPlot(m_amplitudePlot.X, m_amplitudePlot.Y);
            m_amplitudePlot.clear();
        }
        break;
        case FREQUENCY:
        {
            QVector <double> newX, newY;
            for (int i = 0; i < m_frequencyPlot.X.count(); i++)
            {
                if(i%2)
                {
                    newX.append(m_frequencyPlot.X.at(i));
                    newY.append(m_frequencyPlot.Y.at(i));
                }
            }
            emit newPlot(newX, newY);
            m_frequencyPlot.clear();
        }
        break;
        case SPECTRUM:
            //emit newPlot(m_spectrumPlot.X, IQAnalyzerPlot::perestanovka(m_spectrumPlot.Y));
        break;
        case CONSTELLATION:
            emit newPlot(m_constellationPlot.X, m_constellationPlot.Y);
            m_constellationPlot.clear();
        break;
    }
}

void IQAnalyzer::setMode(IQAnalyzer::IQAnalyzerMode mode)
{
    m_mode = mode;

    m_amplitudePlot.clear();
    m_frequencyPlot.clear();
    m_constellationPlot.clear();

    switch (m_mode)
    {
        case UNKNOW:break;
        case AMPLITUDE:
        {
            emit plotRangeChanged(m_amplitudeRange);
        }
        break;
        case FREQUENCY:
        {
            emit plotRangeChanged(m_frequencyRange);
        }
        break;
        case SPECTRUM:
        {
            emit plotRangeChanged(m_spectrumRange);

        }
        break;
        case CONSTELLATION:
        {
            emit plotRangeChanged(m_constellationRange);
        }
        break;
    }

}

void IQAnalyzer::setInputIQType(IQContainer::IQType iqType)
{
    m_inputIQType = iqType;
}

void IQAnalyzer::setFftwPlan(fftw_plan plan)
{
    m_fftw_plan = plan;
}

void IQAnalyzer::setNvl(int nvl)
{
    m_nvl = nvl;

    if(m_in != nullptr)
        delete[] m_in;
    if(m_out != nullptr)
        delete[] m_out;
    m_in = new fftw_complex[m_nvl];
    m_out = new fftw_complex[m_nvl];

    m_spectrumPlot.X.resize(static_cast<int>(m_nvl));
    m_spectrumPlot.Y.resize(static_cast<int>(m_nvl));
    m_spectrumPlot.X.fill(0);
    m_spectrumPlot.Y.fill(0);
    m_Xcount = 0;
    m_spectrCount = 0;
}

void IQAnalyzer::setIQStartNumberByte(int number)
{
    if(number < 0) return;
    m_iqStartNumberByte = number;
}
