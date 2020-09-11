#ifndef CHANNALANALYZERWIDGET_H
#define CHANNALANALYZERWIDGET_H

#include <QWidget>
#include <QTimer>
#include <IQVisualizer/qcustomplot.h>
#include <IQAnalizer/iqanalyzer.h>

namespace Ui {
class IQVisualizer;
}

/**
 * @brief The IQVisualizer class
 * 1) Отображает сигнал методом построения его графика по точкам, а также построение его временнЫх отображения (водопада)
 * 2) Предоставляет элементы управления для анализатора (генератора) этих сигналов
 */
class IQVisualizer : public QWidget
{
    Q_OBJECT

public:
    explicit IQVisualizer(int id, QString name = "Канал без номера", QWidget *parent = nullptr);
    ~IQVisualizer();

private:
    Ui::IQVisualizer *ui;

    int m_id;
    QTimer m_replotTimer;
    QTimer m_signalParametrsRefreshTimer;
    QCPCurve *m_qcpCurve;
    IQAnalyzer::IQAnalyzerMode m_mode;
    IQAnalyzer::IQAnalyzerSignalParametrs m_signalParametrs;

    /**
     * @brief plotInit
     * Инициализация графика
     */
    void plotInit();

    /**
     * @brief waterfallInit
     * Инициализация "водопада"
     */
    void waterfallInit();

signals:
    /**
     * @brief mode_changed
     * Сообщает о необходимости изменения режима отображения
     * @param mode
     */
    void mode_changed(IQAnalyzer::IQAnalyzerMode mode);

    /**
     * @brief iqType_changed
     * Сообщает о необходимости изменения режима обработки IQ
     * @param iqType
     */
    void iqType_changed(IQContainer::IQType iqType);

    /**
     * @brief nvl_changed
     * Сообщает о необходимости изменить разрешение спектра
     * @param iqVisualizerID
     * @param nvl
     */
    void nvl_changed(int iqVisualizerID, int nvl);

    /**
     * @brief srcUdpPort_changed
     * Сообщает о необходимости изменить входящий порт IQ
     * @param value
     */
    void networkSetting_changed(QHostAddress IP, quint16 dstPort, quint16 srcPort);

    /**
     * @brief channalState_changed
     * Сообщает о необходимости запуска или приостановки работы анализатора/генератора сигнала
     * @param play
     */
    void channalState_changed(int iqVisualizerID, bool play);

    /**
     * @brief iqStartNumber_changed
     * Сообщает о необходимости изменить номер байта, с которого начинаются отсчёты IQ в UDP пакете
     * @param number
     */
    void iqStartNumber_changed(int number);

private slots:
    void on_mode_buttonGroup_buttonClicked(QAbstractButton *button);
    void on_iqType_buttonGroup_buttonClicked(QAbstractButton *button);
    void on_channalState_buttonGroup_buttonClicked(QAbstractButton *button);
    void replotTimer_tick();
    void signalParametrsRefreshTimer_tick();

    void on_visibleParametrs_refreshFrequency_comboBox_currentTextChanged(const QString &arg1);
    void on_autoRescale_pushButton_clicked();
    void on_visibleParametrs_pointsSize_horizontalSlider_valueChanged(int value);
    void on_visibleParametrs_line_checkBox_stateChanged(int arg1);
    void on_nvl_comboBox_currentTextChanged(const QString &arg1);
    void on_settings_pushButton_clicked();
    void on_apply_pushButton_clicked();
    void on_cancel_pushButton_clicked();
    void on_waterfall_pushButton_toggled(bool checked);

public slots:
    /**
     * @brief setName
     * Задает название текущего визуализатора
     * @param name
     */
    void setName(QString name);

    /**
     * @brief setPlotRange
     * Задает область отображения графика
     * @param range
     */
    void setPlotRange(IQAnalyzer::IQAnalyzerRange range);

    /**
     * @brief setPlot
     * Задаёт набор точек для отображения графика
     * @param X
     * @param Y
     */
    void setPlot(QVector<double> X, QVector<double> Y);

    /**
     * @brief setSignalParametrs
     * Задает значения основных параметров для отображения
     * @param signalParametrs
     */
    void setSignalParametrs(IQAnalyzer::IQAnalyzerSignalParametrs signalParametrs);
};

#endif // CHANNALANALYZERWIDGET_H
