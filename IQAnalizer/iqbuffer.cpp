#include "iqbuffer.h"

IQBuffer::IQBuffer(IQContainer::IQType iqType) : QList(),
    m_iqType{iqType}
{

}

/*void IQBuffer::setIQContainerData(int index, QByteArray data)
{
    if(index >= this->count()-1)return;
    this->replace(index, IQContainer(m_iqType, data));
}*/

void IQBuffer::setData(QByteArray data)
{
    this->clear();
    for (int i = 0; i < data.count(); i+=IQContainer::getIQBlockSize(m_iqType))
    {
        this->append(IQContainer(m_iqType, QByteArray(&data.data()[i], IQContainer::getIQBlockSize(m_iqType))));
    }
}
