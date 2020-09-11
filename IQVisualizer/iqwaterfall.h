#ifndef IQWATERFALL_H
#define IQWATERFALL_H

#include <QWidget>
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSlider>
#include <QLabel>
#include "math.h"

/**
 * @brief The IQWaterfall class
 * Виджет для отображения изменения сигнала во времени (водопад)
 */
class IQWaterfall : public QWidget
{
    Q_OBJECT
public:
    explicit IQWaterfall(QWidget *parent = nullptr);

    /**
     * @brief setBackground
     * Установка фона виджета
     * @param brush
     */
    void setBackground(const QBrush &brush);

    /**
     * @brief setRange
     * Установка области отображения по ширине
     * @param leftX
     * @param rightX
     */
    void setRange(double leftX, double rightX);

    /**
     * @brief setColorRange
     * Установка максимальных и минимальных значений для определения цвета
     * @param minC
     * @param maxC
     */
    void setColorRange(double minC, double maxC);

    /**
     * @brief setData
     * Установка данных для отображения
     * @param X
     * @param Y
     */
    void setData(QVector<double> X, QVector<double> Y);

    /**
     * @brief replot
     * Обновление области отображения
     */
    void replot();

    /**
     * @brief clear
     * Очистка области отображения
     */
    void clear();

protected:
    void    paintEvent(QPaintEvent *);

private slots:
    /**
     * @brief setLightness
     * Установка яркости цветов
     * @param lightness
     */
    void setLightness(int lightness);

    /**
     * @brief setLightness
     * Установка модификатора красного цвета
     * @param lightness
     */
    void setColorR(int value);

    /**
     * @brief setColorG
     * Установка модификатора зелёного цвета
     * @param value
     */
    void setColorG(int value);

    /**
     * @brief setColorB
     * Установка модификатора синего цвета
     * @param value
     */
    void setColorB(int value);

private:
    QBrush m_backgroundBrush;
    double m_colorMod;
    double m_colorR;
    double m_colorG;
    double m_colorB;

    double m_leftX;
    double m_rightX;
    double m_minC;
    double m_maxC;
    int m_paddingLeft;
    int m_paddingRight;

    QSlider *m_sliderLightness;
    QSlider *m_sliderColorR;
    QSlider *m_sliderColorG;
    QSlider *m_sliderColorB;

    struct IQWaterfallLine
    {
        QVector<double> m_X;
        QVector<QColor> m_C;
    };
    QVector <IQWaterfallLine> m_lines;

    QVector<double> m_tempX;
    QVector<double> m_tempY;

    quint8 getUint8Color(double color);


signals:

public slots:
};

#endif // IQWATERFALL_H
