#include "iqcontainer.h"

unsigned int IQContainer::getIQBlockSize(IQContainer::IQType iqType)
{
    unsigned int size = 0;
    switch (iqType)
    {
        case UINT8:
            size = 2;
        break;
        case UINT16:
            size = 4;
        break;
        case PCM24:
            size = 6;
        break;
    }
    return size;
}

IQContainer::IQContainer()
{
    setIQType(IQContainer::IQType::UINT8);
}

IQContainer::IQContainer(IQContainer::IQType iqType, const QByteArray &iqBlock)
{
    setIQType(iqType);
    setIqBlock(iqBlock);
}

IQContainer::~IQContainer()
{

}

QByteArray IQContainer::iqBlock() const
{
    return QByteArray(m_iqBlock[0], m_iqBlockSize);
}

void IQContainer::setIqBlock(const QByteArray &iqBlock)
{
    clear();
    memcpy(m_iqBlock, iqBlock.data(), m_iqBlockSize);

    switch (m_iqType)
    {
        case UINT8:
            m_I = static_cast<int>(m_iqBlock[0]<<24) >> 24;
            m_Q = static_cast<int>(m_iqBlock[1]<<24) >> 24;
        break;
        case UINT16:
            m_I = static_cast<int>((m_iqBlock[0]<<16 | m_iqBlock[1]<<24)>>16);
            m_Q = static_cast<int>((m_iqBlock[2]<<16 | m_iqBlock[3]<<24)>>16);
        break;
        case PCM24:
            m_I = static_cast<int>(((m_iqBlock[4] << 8) | (m_iqBlock[0] << 16) | (m_iqBlock[1] << 24)) >> 8);
            m_Q = static_cast<int>(((m_iqBlock[5] << 8) | (m_iqBlock[2] << 16) | (m_iqBlock[3] << 24)) >> 8);
        break;
    }
}

double IQContainer::I() const
{
    return m_I;
}

void IQContainer::setI(double I)
{
    m_I = I;
}

double IQContainer::Q() const
{
    return m_Q;
}

void IQContainer::setQ(double doubleQ)
{
    m_Q = doubleQ;
}

void IQContainer::setIQType(IQContainer::IQType iqType)
{
    m_iqType = iqType;
    m_iqBlockSize = getIQBlockSize(m_iqType);

    clear();
}

void IQContainer::clear()
{
    memset(m_iqBlock, 0, m_iqBlockSize);
    m_I = 0;
    m_Q = 0;
}
