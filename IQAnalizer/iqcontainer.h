#ifndef IQCONTAINER_H
#define IQCONTAINER_H

#include <QObject>

#define IQCONTAINER_MAX_SIZE 6

/**
 * @brief The IQContainer class
 * Класс-контейнер для одного отсчёта IQ определённого типа
 */
class IQContainer
{
public:
    /**
     * @brief The IQType enum
     * Тип отсчёта IQ
     */
    enum IQType
    {
      ///8-ми битные части I и Q формата I0 Q0
      UINT8,
      ///16-ти битные части I и Q формата I0 I1 Q0 Q1
      UINT16,
      ///24-х битные части I и Q формата I1 I2 Q1 Q2 I0 Q0
      PCM24
    };

    /**
     * @brief getIQBlockSize
     * Возвращает размер блока одного отсчёта IQ
     * @param iqType
     * @return
     */
    static unsigned int getIQBlockSize(IQType iqType);

    IQContainer();
    IQContainer(IQType iqType);
    IQContainer(IQType iqType, const QByteArray &iqBlock);
    ~IQContainer();

    /**
     * @brief iqBlock
     * Возвращает блок текущего отсчёта IQ
     * @return
     */
    QByteArray iqBlock() const;

    /**
     * @brief setIqBlock
     * Задаёт блок отсчёта IQ
     * @param iqBlock
     */
    void setIqBlock(const QByteArray &iqBlock);

    /**
     * @brief I
     * Возвращает I часть текущего отсчёта
     * @return
     */
    double I() const;

    /**
     * @brief setI
     * Задаёт I часть отсчёта
     * @param I
     */
    void setI(double I);

    /**
     * @brief Q
     * Возвращает Q часть текущего отсчёта
     * @return
     */
    double Q() const;

    /**
     * @brief setQ
     * Задаёт Q часть отсчёта
     * @param Q
     */
    void setQ(double Q);

    /**
     * @brief setIQType
     * Задаёт формат обработки отсчёта
     * @param iqType
     */
    void setIQType(IQType iqType);

    /**
     * @brief clear
     * Очистка текущего отсчёта
     */
    void clear();

private:
    IQType m_iqType;
    uchar m_iqBlock[IQCONTAINER_MAX_SIZE];
    unsigned int m_iqBlockSize;
    double m_I;
    double m_Q;
};

#endif // IQCONTAINER_H
