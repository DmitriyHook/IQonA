#include "iqnco.h"
#include "math.h"
#include "QTime"

IQNco::IQNco(int cosSinCount) :
    m_cosSinCount {cosSinCount},
    m_control {0}
{
    m_cosTable = new double[m_cosSinCount];
    m_sinTable = new double[m_cosSinCount];
    setNco();
}

IQNco::~IQNco()
{
    if(m_cosTable != nullptr)
        delete[] m_cosTable;
    if(m_sinTable != nullptr)
        delete[] m_sinTable;
}

void IQNco::setNco(){
    for(int i=0;i<getNcoCount();i++)
    {
        m_cosTable[i] = (cos(i*2*M_PI/getNcoCount()));
        m_sinTable[i] = (sin(i*2*M_PI/getNcoCount()));
    }
}
int IQNco::getNcoCount() const
{
    return m_cosSinCount;
}

void IQNco::setControl(double control)
{
    m_control += control;
    m_control = fmod(m_control, getNcoCount()-1);
    if(m_control < 0)
    {
        m_control += static_cast<double>(m_cosSinCount);
    }
}

double IQNco::getSin() const
{
    if(m_control < getNcoCount() && m_control >= 0)
    {
        return m_sinTable[static_cast<int>(floor(m_control))];
    }
    else
    {
        qDebug("bad element getIntegralCos");
        throw;
    }
}

double IQNco::getCos() const
{
    if(m_control < getNcoCount() && m_control >= 0)
    {
        return m_cosTable[static_cast<int>(floor(m_control))];
    }
    else
    {
        qDebug("bad element getIntegralCos");
        return -1;
    }
}
