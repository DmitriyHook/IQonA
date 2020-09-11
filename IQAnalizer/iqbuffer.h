#ifndef IQBUFFER_H
#define IQBUFFER_H

#include <QList>
#include "iqcontainer.h"

/**
 * @brief The IQBuffer class
 * Класс-контейнер контейнеров отсчётов IQ определённого типа
 */
class IQBuffer : public QList <IQContainer>
{
public:
    IQBuffer(IQContainer::IQType iqType);

    //void setIQContainerData(int index, QByteArray data);

    /**
     * @brief setData
     * Инициализирует весь контейнер данными data
     * @param data
     */
    void setData(QByteArray data);

private:
    IQContainer::IQType m_iqType;
};

#endif // IQBUFFER_H
