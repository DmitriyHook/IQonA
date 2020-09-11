#include "iqvisualizer.h"
#include "ui_iqvisualizer.h"


IQVisualizer::IQVisualizer(int id, QString name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IQVisualizer),
    m_id {id},
    m_qcpCurve {nullptr},
    m_mode {IQAnalyzer::UNKNOW}
{
    ui->setupUi(this);
    setName(name);

    //инициализация группы кнопок изменения отображения сигнала
    ui->mode_buttonGroup->setId(ui->mode_amplitude_pushButton,      IQAnalyzer::AMPLITUDE);
    ui->mode_buttonGroup->setId(ui->mode_frequency_pushButton,      IQAnalyzer::FREQUENCY);
    ui->mode_buttonGroup->setId(ui->mode_spectrum_pushButton,       IQAnalyzer::SPECTRUM);
    ui->mode_buttonGroup->setId(ui->mode_constellation_pushButton,  IQAnalyzer::CONSTELLATION);

    //инициализация группы кнопок изменения типа IQ
    ui->iqType_buttonGroup->setId(ui->iqType_uint8_pushButton,      IQContainer::UINT8);
    ui->iqType_buttonGroup->setId(ui->iqType_uint16_pushButton,     IQContainer::UINT16);
    ui->iqType_buttonGroup->setId(ui->iqType_pcm24_pushButton,      IQContainer::PCM24);

    //инициализация группы кнопок запуска/остановки сигнала
    ui->channalState_buttonGroup->setId(ui->stop_pushButton,        0);
    ui->channalState_buttonGroup->setId(ui->play_pushButton,        1);

    m_qcpCurve = new QCPCurve(ui->plot_widget->xAxis, ui->plot_widget->yAxis);
    plotInit();

    waterfallInit();

    connect(&m_replotTimer, &QTimer::timeout, this, &IQVisualizer::replotTimer_tick);
    m_replotTimer.start(1000);

    connect(&m_signalParametrsRefreshTimer, &QTimer::timeout, this, &IQVisualizer::signalParametrsRefreshTimer_tick);
    m_signalParametrsRefreshTimer.start(500);

    ui->mode_amplitude_pushButton->setToolTip("Амплитуда");
    ui->mode_frequency_pushButton->setToolTip("Частота");
    ui->mode_spectrum_pushButton->setToolTip("Спектр");
    ui->mode_constellation_pushButton->setToolTip("Созвездие");
    ui->autoRescale_pushButton->setToolTip("Подогнать размер");
    ui->waterfall_pushButton->setToolTip("Водопад");
    ui->settings_pushButton->setToolTip("Настройки канала");

    ui->stackedWidget->setCurrentIndex(0);
    ui->nvl_comboBox->setCurrentIndex(3);
    ui->visibleParametrs_refreshFrequency_comboBox->setCurrentIndex(2);

    ui->nvl_comboBox->setVisible(false);
    ui->nvl_label->setVisible(false);
    ui->waterfall_widget->setVisible(false);

    ui->iqType_pcm24_pushButton->setChecked(true);
}

IQVisualizer::~IQVisualizer()
{
    delete ui;
}

void IQVisualizer::plotInit()
{
    //ui->plot_widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->plot_widget->axisRect()->setupFullAxesBox();
    ui->plot_widget->rescaleAxes();


    m_qcpCurve->setSelectable(QCP::SelectionType::stNone);
    m_qcpCurve->setPen(QPen(QColor(18, 130, 255, 230)));//10, 70, 140, 230
    m_qcpCurve->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 1));
    m_qcpCurve->setAntialiased(true);

    // set some pens, brushes and backgrounds:
    ui->plot_widget->xAxis->setBasePen(QPen(Qt::white, 1));
    ui->plot_widget->yAxis->setBasePen(QPen(Qt::white, 1));
    ui->plot_widget->xAxis->setTickPen(QPen(Qt::white, 1));
    ui->plot_widget->yAxis->setTickPen(QPen(Qt::white, 1));
    ui->plot_widget->xAxis->setSubTickPen(QPen(Qt::white, 1));
    ui->plot_widget->yAxis->setSubTickPen(QPen(Qt::white, 1));
    ui->plot_widget->xAxis->setTickLabelColor(Qt::white);
    ui->plot_widget->yAxis->setTickLabelColor(Qt::white);
    ui->plot_widget->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    ui->plot_widget->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    ui->plot_widget->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    ui->plot_widget->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    ui->plot_widget->xAxis->grid()->setSubGridVisible(true);
    ui->plot_widget->yAxis->grid()->setSubGridVisible(true);
    ui->plot_widget->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    ui->plot_widget->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    ui->plot_widget->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    ui->plot_widget->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    ui->plot_widget->setBackground(plotGradient);

    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
    ui->plot_widget->axisRect()->setBackground(axisRectGradient);

    ui->plot_widget->rescaleAxes();

    ui->plot_widget->replot();
}

void IQVisualizer::waterfallInit()
{
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));

    ui->waterfall_widget->setBackground(plotGradient);
}

void IQVisualizer::on_mode_buttonGroup_buttonClicked(QAbstractButton *button)
{
    ui->waterfall_widget->clear();

    m_mode = IQAnalyzer::IQAnalyzerMode(ui->mode_buttonGroup->id(button));

    if(m_mode == IQAnalyzer::SPECTRUM)
    {
        ui->nvl_comboBox->setVisible(true);
        ui->nvl_label->setVisible(true);
    }
    else
    {
        ui->nvl_comboBox->setVisible(false);
        ui->nvl_label->setVisible(false);
    }

    emit mode_changed(m_mode);
}

void IQVisualizer::on_iqType_buttonGroup_buttonClicked(QAbstractButton *button)
{
    //emit iqType_changed(IQContainer::IQType(ui->iqType_buttonGroup->id(button)));
}

void IQVisualizer::on_channalState_buttonGroup_buttonClicked(QAbstractButton *button)
{
    emit channalState_changed(m_id, ui->channalState_buttonGroup->id(button));
}

void IQVisualizer::replotTimer_tick()
{
    if(ui->play_pushButton->isChecked())
    {
        //обновлять график
        ui->plot_widget->replot();

        //обновлять "водопад" только если он отображается
        if(ui->waterfall_pushButton->isChecked())
            ui->waterfall_widget->replot();
    }
}

void IQVisualizer::signalParametrsRefreshTimer_tick()
{
    ui->signalParametrs_middleAmplitude_label->setText(QString::number(m_signalParametrs.midAmplitude()));
    ui->signalParametrs_maxAmplitude_label->setText(QString::number(m_signalParametrs.maxAmplitude()));
    ui->signalParametrs_frequency_label->setText(QString::number(m_signalParametrs.frequency()));
}

void IQVisualizer::setName(QString name)
{
    ui->channalAnalyzerWidget_groupBox->setTitle(name);
}

void IQVisualizer::setPlotRange(IQAnalyzer::IQAnalyzerRange range)
{
    ui->plot_widget->xAxis->setRange(range.minX(), range.maxX());
    ui->plot_widget->yAxis->setRange(range.minY(), range.maxY());

    ui->waterfall_widget->setRange(range.minX(), range.maxX());
    ui->waterfall_widget->setColorRange(range.minY(), range.maxY());
}

void IQVisualizer::setPlot(QVector<double> X, QVector<double> Y)
{
    m_qcpCurve->setData(X, Y);
    ui->waterfall_widget->setData(X, Y);
}

void IQVisualizer::setSignalParametrs(IQAnalyzer::IQAnalyzerSignalParametrs signalParametrs)
{
    m_signalParametrs = signalParametrs;
}

void IQVisualizer::on_visibleParametrs_refreshFrequency_comboBox_currentTextChanged(const QString &arg1)
{
    m_replotTimer.setInterval(1000/arg1.toInt());
}

void IQVisualizer::on_autoRescale_pushButton_clicked()
{
    if(m_mode == IQAnalyzer::IQAnalyzerMode::CONSTELLATION)
    {
        double r = (m_signalParametrs.maxAmplitude())*16375.0;
        IQAnalyzer::IQAnalyzerRange range(-r, r, -r, r);
        setPlotRange(range);
    }
    else
    {
        ui->plot_widget->rescaleAxes();
        ui->waterfall_widget->setRange(ui->plot_widget->xAxis->range().lower, ui->plot_widget->xAxis->range().upper);
        ui->waterfall_widget->setColorRange(ui->plot_widget->yAxis->range().lower-(ui->plot_widget->yAxis->range().lower*0.2),
                                            ui->plot_widget->yAxis->range().upper+(ui->plot_widget->yAxis->range().upper*0.2));
    }
}

void IQVisualizer::on_visibleParametrs_pointsSize_horizontalSlider_valueChanged(int value)
{
    m_qcpCurve->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, value));
}

void IQVisualizer::on_visibleParametrs_line_checkBox_stateChanged(int arg1)
{
    if(arg1)
        m_qcpCurve->setLineStyle(QCPCurve::lsLine);
    else
        m_qcpCurve->setLineStyle(QCPCurve::lsNone);
}

void IQVisualizer::on_nvl_comboBox_currentTextChanged(const QString &arg1)
{
    //ui->waterfall_widget->clear();
    emit nvl_changed(m_id, arg1.toInt());
}

void IQVisualizer::on_settings_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void IQVisualizer::on_apply_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);

    emit iqStartNumber_changed(ui->iqStartNumberByte_spinBox->value());
    emit iqType_changed(IQContainer::IQType(ui->iqType_buttonGroup->checkedId()));
    emit networkSetting_changed(QHostAddress::Any, 0, ui->srcUdpPort_spinBox->value());
}

void IQVisualizer::on_cancel_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void IQVisualizer::on_waterfall_pushButton_toggled(bool checked)
{
    ui->waterfall_widget->setVisible(checked);
}
