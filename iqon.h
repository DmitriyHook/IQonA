#ifndef IQONA_H
#define IQONA_H

#include <QObject>
#include <QVector>
#include "IQAnalizer/iqanalyzer.h"
#include "IQVisualizer/iqvisualizer.h"

/**
 * @brief The IQon class
 * Связка анализаторов и визуализаторов сигналов
 */
class IQon : public QObject
{
    Q_OBJECT
public:
    explicit IQon(QObject *parent = nullptr);
    ~IQon();

private:
    QVector<IQAnalyzer*>    m_iqAnalyzers;
    QVector<IQVisualizer*>  m_iqVisualizer;
    QVector<QThread*>       m_threads;
    fftw_plan m_start_fft_plan;
    int m_start_nvl;
    void setChannalState(int iqAnalyzerID, bool state);
    void setNvl(int iqAnalyzerID, int nvl);
    void clear();

signals:
    /**
     * @brief countChannals_changed
     * Сообщает об изменении количества каналов и передаёт указатели на их визуализаторы
     * @param m_iqVisualizer
     */
    void countChannals_changed(QVector<IQVisualizer*> m_iqVisualizer);

public slots:
    /**
     * @brief iqVisualizers
     * Возвращает вектор указателей на объекты визуализаторов
     * @return
     */
    QVector<IQVisualizer*> iqVisualizers();

    /**
     * @brief setCountChannals
     * Задаёт количество каналов
     * @param value
     */
    void setCountChannals(int value);

};

#endif // IQONA_H
