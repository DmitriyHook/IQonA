#include "iqwaterfall.h"

IQWaterfall::IQWaterfall(QWidget *parent) : QWidget(parent),
    m_backgroundBrush {},
    m_colorMod      {1},
    m_colorR        {1},
    m_colorG        {1},
    m_colorB        {2},
    m_leftX         {0},
    m_rightX        {255},
    m_minC          {0},
    m_maxC          {255},
    m_paddingLeft   {35},
    m_paddingRight  {15}
{
    m_sliderColorR      = new QSlider(Qt::Orientation::Vertical);
    m_sliderColorG      = new QSlider(Qt::Orientation::Vertical);
    m_sliderColorB      = new QSlider(Qt::Orientation::Vertical);
    m_sliderLightness   = new QSlider(Qt::Orientation::Vertical);

    m_sliderColorR->setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Preferred);
    m_sliderColorR->setMinimum(0);
    m_sliderColorR->setMaximum(500);
    m_sliderColorR->setValue(m_colorR*100);

    m_sliderColorG->setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Preferred);
    m_sliderColorG->setMinimum(0);
    m_sliderColorG->setMaximum(500);
    m_sliderColorG->setValue(m_colorG*100);

    m_sliderColorB->setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Preferred);
    m_sliderColorB->setMinimum(0);
    m_sliderColorB->setMaximum(500);
    m_sliderColorB->setValue(m_colorB*100);

    m_sliderLightness->setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Preferred);
    m_sliderLightness->setMinimum(0);
    m_sliderLightness->setMaximum(500);
    m_sliderLightness->setValue(m_colorMod*100);

    QVBoxLayout *vLay1 = new QVBoxLayout;
    QLabel *label = new QLabel;
    QVBoxLayout *vLay2 = new QVBoxLayout;
    QHBoxLayout *hlay = new QHBoxLayout;

    //vLay1->addWidget(new QLabel("R"));
    vLay1->addWidget(m_sliderColorR);
    //vLay1->addWidget(new QLabel("G"));
    vLay1->addWidget(m_sliderColorG);
    //vLay1->addWidget(new QLabel("B"));
    vLay1->addWidget(m_sliderColorB);

    vLay2->addWidget(m_sliderLightness);

    hlay->setContentsMargins(5,0,0,0);
    hlay->addLayout(vLay1);
    hlay->addWidget(label);
    hlay->addLayout(vLay2);

    this->setLayout(hlay);

    connect(m_sliderColorR, &QSlider::valueChanged, this, &IQWaterfall::setColorR);
    connect(m_sliderColorG, &QSlider::valueChanged, this, &IQWaterfall::setColorG);
    connect(m_sliderColorB, &QSlider::valueChanged, this, &IQWaterfall::setColorB);
    connect(m_sliderLightness, &QSlider::valueChanged, this, &IQWaterfall::setLightness);
}

void IQWaterfall::setBackground(const QBrush &brush)
{
    m_backgroundBrush = brush;
}

void IQWaterfall::setRange(double leftX, double rightX)
{
    m_leftX = leftX;
    m_rightX = rightX;
}

void IQWaterfall::setColorRange(double minC, double maxC)
{
    m_minC = minC;
    m_maxC = maxC;
}

void IQWaterfall::replot()
{
    //ничего не делать, если количество точек в X и Y разное
    if(m_tempX.count() != m_tempY.count()) return;
    //ничего не делать, если точек нет
    if(m_tempX.isEmpty()) return;

    IQWaterfallLine line;
    line.m_X = m_tempX;

    //определение цветов точек и добавление их в линию
    for (int pointCounter = 0; pointCounter < line.m_X.count(); pointCounter++)
    {
        //сдвиг значений в область 0+
        double color = m_tempY.at(pointCounter) - m_minC;
        //приведение значений к диапазону 0..255
        color *= 255.0/m_maxC;
        //настройка яркости
        color *= m_colorMod;

        //добавление точки в линию в соответствии с текущей цветовой схемой
        line.m_C.append(QColor(getUint8Color(color*m_colorR), getUint8Color(color*m_colorG), getUint8Color(color*m_colorB)));
    }

    //добавление линии в список
    m_lines.push_front(line);

    //определение текущего размера точек
    double width = size().width();
    double count = m_lines.last().m_X.count();
    int sizeRect = static_cast<int>(round(width/count + 0.5));

    //удаление всех точек, которые не помещаются в область вывода текущего размера
    while(size().height() < (m_lines.count()*sizeRect)) m_lines.removeLast();

    m_tempX.resize(0);
    m_tempY.resize(0);

    update();
}

void IQWaterfall::clear()
{
    m_tempX.resize(0);
    m_tempY.resize(0);
    m_lines.clear();
}

void IQWaterfall::setData(QVector<double> X, QVector<double> Y)
{
    m_tempX = X;
    m_tempY = Y;
}

void IQWaterfall::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    //заполнение фона виджета
    painter.fillRect(QRect(0, 0, size().width(), size().height()), m_backgroundBrush);

    //расчёт модификатора положения точки в зависимости от размера виджета
    double xMod = (double)(m_rightX - m_leftX)/(size().width()-m_paddingLeft-m_paddingRight);

    int yLinePosition = 0;

    //вывод линий
    for (int lineCounter = 0; lineCounter < m_lines.count(); lineCounter++)
    {
        //определение текущего размера точек в линии
        double width = size().width();
        double count = m_lines.at(lineCounter).m_X.count();
        int sizeRect = round(width/count + 0.5);

        //вывод точек в линии
        for (int pointCounter = 0; pointCounter < m_lines.at(lineCounter).m_X.count(); pointCounter++)
        {
            //расчёт текущего положения точки
            int x = (m_lines.at(lineCounter).m_X.at(pointCounter)-m_leftX)/xMod;
            painter.fillRect(x+m_paddingLeft, yLinePosition, sizeRect, sizeRect, m_lines.at(lineCounter).m_C.at(pointCounter));
        }
        yLinePosition += sizeRect;
    }
}

void IQWaterfall::setLightness(int lightness)
{
    m_colorMod = lightness/100.0;
}

void IQWaterfall::setColorR(int value)
{
    m_colorR = value/100.0;
}

void IQWaterfall::setColorG(int value)
{
    m_colorG = value/100.0;
}

void IQWaterfall::setColorB(int value)
{
    m_colorB = value/100.0;
}

quint8 IQWaterfall::getUint8Color(double color)
{
    quint8 retColor;
    if(color > 255) retColor = 255;
    else if(color < 0) retColor = 0;
    else retColor = static_cast<quint8>(color);
    return retColor;
}
