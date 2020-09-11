#ifndef MODEM_H
#define MODEM_H

#include <QObject>
#include "math.h"
#include "fftw3.h"

#include "iqnco.h"
#include "abstractudp.h"
#include "iqcontainer.h"
#include "iqbuffer.h"

/**
 * @brief The IQAnalyzer class
 * 1) Принимает входящий поток IQ отсчётов по UDP
 * 2) Обрабатывает этот поток и его основе генерирует наборы точек для построения графиков основных параметров сигнала:
 *      - амплитуды;
 *      - частоты;
 *      - спектра;
 *      - созвуздия.
 * 3) Определяет основные параметры сигнала:
 *      - максимальную амплитуду
 *      - среднюю амплитуду
 *      - частоту с наибольшей амплитудой
 */
class IQAnalyzer : public AbstractUdp
{
    Q_OBJECT

public:
    /**
     * @brief The IQAnalyzerMode enum
     * Режим генерирования точек для построения графика
     */
    enum IQAnalyzerMode
    {
        UNKNOW          = 0,
        ///Режим построения амплитуды
        AMPLITUDE       = 1,
        ///Режим построения частоты
        FREQUENCY       = 2,
        ///Режим построения спектра
        SPECTRUM        = 3,
        ///Режим построения созвездия
        CONSTELLATION   = 4
    };

    Q_ENUM(IQAnalyzerMode)

    /**
     * @brief The IQAnalyzerRange struct
     * Область отображения
     */
    struct IQAnalyzerRange
    {
    private:
        int m_minX;
        int m_maxX;
        int m_minY;
        int m_maxY;

    public:
        IQAnalyzerRange()
        {
            m_minX = 0;
            m_maxX = 0;
            m_minY = 0;
            m_maxY = 0;
        }
        IQAnalyzerRange(int minX, int maxX, int minY, int maxY)
        {
            m_minX = minX;
            m_maxX = maxX;
            m_minY = minY;
            m_maxY = maxY;
        }

        int minX() const        {return m_minX;}
        void setMinX(int minX)  {m_minX = minX;}
        int maxX() const        {return m_maxX;}
        void setMaxX(int maxX)  {m_maxX = maxX;}
        int minY() const        {return m_minY;}
        void setMinY(int minY)  {m_minY = minY;}
        int maxY() const        {return m_maxY;}
        void setMaxY(int maxY)  {m_maxY = maxY;}

    };

    /**
     * @brief The IQAnalyzerPlot struct
     * Контейнер для набора точек для построения графика
     */
    struct IQAnalyzerPlot
    {
        QVector <double> X, Y;

        void clear()
        {
            X.resize(0);
            Y.resize(0);
        }

        /**
         * @brief spectrumRearrangement
         * Упородочивание точек амплитуд спектра после преобразования Фурье
         * @param vector
         * @return
         */
        static QVector <double> spectrumRearrangement(QVector <double> vector)
        {
            double temp1 = 0.0;
            for(int i=0; i<vector.length()/2; i++)
            {
                temp1 = vector[i];
                vector[i] = vector[vector.length()/2+i];
                vector[vector.length()/2+i] = temp1;
            }
            return vector;
        }

        /**
         * @brief maxValueIndex
         * Возвращает индекс элемента с максимальным значением
         * @param vector
         * @return
         */
        static int maxValueIndex(QVector <double> vector)
        {
            int maxValueIndex = -1;
            double maxY = -1000000;

            for (int i = 0; i < vector.count(); i++)
            {
                if(vector.at(i) > maxY)
                {
                    maxY = vector.at(i);
                    maxValueIndex = i;
                }
            }
            return maxValueIndex;
        }
    };

    /**
     * @brief The IQAnalyzerSignalParametrs struct
     * Контейнер для хранения параметров сигнала
     */
    struct IQAnalyzerSignalParametrs
    {
    private:
        double m_midAmplitude;
        double m_maxAmplitude;
        double m_frequency;

    public:
        IQAnalyzerSignalParametrs()
        {
            clear();
        }
        void clear()
        {
            m_midAmplitude = 0;
            m_maxAmplitude = 0;
            m_frequency = 0;
        }
        double midAmplitude() const {return m_midAmplitude;}
        void setMidAmplitude(double amplitude) {m_midAmplitude = amplitude;}
        double maxAmplitude() const {return m_maxAmplitude;}
        void setMaxAmplitude(double amplitude) {m_maxAmplitude = amplitude;}
        double frequency() const {return m_frequency;}
        void setFrequency(double frequency) {m_frequency = frequency;}
    };

    explicit IQAnalyzer(int id, QObject *parent = nullptr);
    ~IQAnalyzer();

private:
    int m_id;
    IQAnalyzerMode m_mode = UNKNOW;
    IQContainer::IQType m_inputIQType;
    IQAnalyzerRange m_amplitudeRange;
    IQAnalyzerRange m_frequencyRange;
    IQAnalyzerRange m_spectrumRange;
    IQAnalyzerRange m_constellationRange;

    IQNco *m_nco;
    fftw_plan m_fftw_plan;
    int m_nvl;
    fftw_complex *m_in, *m_out;
    int m_spectrCount;
    int m_Xcount;

    IQAnalyzerPlot m_amplitudePlot;
    IQAnalyzerPlot m_frequencyPlot;
    IQAnalyzerPlot m_spectrumPlot;
    IQAnalyzerPlot m_constellationPlot;

    IQAnalyzerSignalParametrs m_signalParametrs;

    int m_offsetFrequency;
    double oldI = 0, oldQ = 0;

    int m_iqStartNumberByte = 0;



signals:
    /**
     * @brief plotRangeChanged
     * Сообщает о необходимости изменения разрешения области графика
     * @param range
     */
    void plotRangeChanged(IQAnalyzer::IQAnalyzerRange range);

    /**
     * @brief newPlot
     * Сообщает новые точки для построения графика
     * @param X
     * @param Y
     */
    void newPlot(QVector<double> X, QVector<double> Y);

    /**
     * @brief newSignalParametrs
     * Сообщает новые параметры сигнала
     * @param signalParametrs
     */
    void newSignalParametrs(IQAnalyzer::IQAnalyzerSignalParametrs signalParametrs);

private slots:
    void readPacket(QByteArray data);

public slots:
    /**
     * @brief setMode
     * Задаёт режим генерирования точек для построения графика
     * @param mode
     */
    void setMode(IQAnalyzer::IQAnalyzerMode mode);

    /**
     * @brief setInputIQType
     * Устанавливает тип входящего потока отсчетов IQ
     * @param iqType
     */
    void setInputIQType(IQContainer::IQType iqType);

    /**
     * @brief setFftwPlan
     * @param plan
     */
    void setFftwPlan(fftw_plan plan);

    /**
     * @brief setNvl
     * Устанавливает разрешение спектра сигнала (количество точек для построения спектра)
     * @param nvl
     */
    void setNvl(int nvl);

    /**
     * @brief setIQStartNumberByte
     * Устанавливает номер байта, начина с которого берутся отсчёты в полученном UDP пакете
     * @param number
     */
    void setIQStartNumberByte(int number);

};

#endif // MODEM_H
