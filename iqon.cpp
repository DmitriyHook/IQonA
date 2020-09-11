#include "iqon.h"

IQon::IQon(QObject *parent) : QObject(parent),
    m_start_nvl {256}
{
    qRegisterMetaType<IQAnalyzer::IQAnalyzerMode>("IQAnalyzer::IQAnalyzerMode");
    qRegisterMetaType<IQAnalyzer::IQAnalyzerRange>("IQAnalyzer::IQAnalyzerRange");
    qRegisterMetaType<IQAnalyzer::IQAnalyzerSignalParametrs>("IQAnalyzer::IQAnalyzerSignalParametrs");
    qRegisterMetaType<IQContainer::IQType>("IQContainer::IQType");
    qRegisterMetaType<QHostAddress>("QHostAddress");


    m_start_fft_plan = fftw_plan_dft_1d(m_start_nvl, nullptr, nullptr, FFTW_BACKWARD, FFTW_ESTIMATE);
}

IQon::~IQon()
{
    clear();
}

void IQon::setChannalState(int iqAnalyzerID, bool state)
{
    if(state)
        m_threads[iqAnalyzerID]->start();
    else
        m_threads[iqAnalyzerID]->exit();
}

void IQon::clear()
{
    //остановка всех запущенных потоков
    //ожидание завершения потоков
    //разрушение потоков
    foreach(QThread *thread, m_threads)
    {
        thread->exit();
        while (thread->isRunning()){}
        delete thread;
    }
    //разрушение визуализаторов
    foreach(IQVisualizer *iqVisualizer, m_iqVisualizer)
    {
        delete iqVisualizer;
    }
    //разрушение визуализаторов
    foreach(IQAnalyzer *iqAnalizer, m_iqAnalyzers)
    {
        delete iqAnalizer;
    }
    m_threads.resize(0);
    m_iqVisualizer.resize(0);
    m_iqAnalyzers.resize(0);
}

QVector<IQVisualizer*> IQon::iqVisualizers()
{
    return m_iqVisualizer;
}

void IQon::setCountChannals(int value)
{
    clear();

    //создание отдельных потоков для анализаторов
    for (int i = 0; i < value; i++) m_threads.append(new QThread());

    //создание анализаторов
    for (int i = 0; i < value; i++)
    {
        m_iqAnalyzers.append(new IQAnalyzer(i));
        m_iqAnalyzers.last()->setFftwPlan(m_start_fft_plan);
        m_iqAnalyzers.last()->setNvl(m_start_nvl);
        m_iqAnalyzers.last()->setNetworkSetting(QHostAddress("192.168.105.30"), 0, 38100+i);
        m_iqAnalyzers.last()->setIQStartNumberByte(36);
        m_iqAnalyzers.last()->moveToThread(m_threads[i]);
    }

    //создание визуализаторов
    for (int i = 0; i < value; i++)
    {
        m_iqVisualizer.append(new IQVisualizer(i, "Канал №"+QString::number(i+1)));
    }

    //соединение сигналов-слотов анализаторов и визуализаторов
    for (int i = 0; i < value; i++)
    {
        QObject::connect(m_threads[i], &QThread::started, m_iqAnalyzers[i], &IQAnalyzer::bind);
        QObject::connect(m_threads[i], &QThread::finished, m_iqAnalyzers[i], &IQAnalyzer::unbind);

        QObject::connect(m_iqAnalyzers[i], &IQAnalyzer::newPlot,               m_iqVisualizer[i], &IQVisualizer::setPlot);
        QObject::connect(m_iqAnalyzers[i], &IQAnalyzer::newSignalParametrs,    m_iqVisualizer[i], &IQVisualizer::setSignalParametrs);
        QObject::connect(m_iqAnalyzers[i], &IQAnalyzer::plotRangeChanged,      m_iqVisualizer[i], &IQVisualizer::setPlotRange);
        QObject::connect(m_iqVisualizer[i],&IQVisualizer::mode_changed,        m_iqAnalyzers[i], &IQAnalyzer::setMode);
        QObject::connect(m_iqVisualizer[i],&IQVisualizer::iqType_changed,      m_iqAnalyzers[i], &IQAnalyzer::setInputIQType);
        QObject::connect(m_iqVisualizer[i],&IQVisualizer::networkSetting_changed,  m_iqAnalyzers[i], &IQAnalyzer::setNetworkSetting);
        QObject::connect(m_iqVisualizer[i],&IQVisualizer::iqStartNumber_changed,  m_iqAnalyzers[i], &IQAnalyzer::setIQStartNumberByte);

        QObject::connect(m_iqVisualizer[i],&IQVisualizer::nvl_changed,         this, &IQon::setNvl);
        QObject::connect(m_iqVisualizer[i],&IQVisualizer::channalState_changed,this, &IQon::setChannalState);
    }

    emit countChannals_changed(m_iqVisualizer);
}

void IQon::setNvl(int iqAnalyzerID, int nvl)
{
    fftw_plan fft_plan = fftw_plan_dft_1d(nvl, nullptr, nullptr, FFTW_BACKWARD, FFTW_ESTIMATE);

    m_iqAnalyzers[iqAnalyzerID]->setFftwPlan(fft_plan);
    m_iqAnalyzers[iqAnalyzerID]->setNvl(nvl);

}
