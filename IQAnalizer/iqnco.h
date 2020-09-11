#ifndef QINTEGRALCOSSIN_H
#define QINTEGRALCOSSIN_H

class IQNco
{
public:
    IQNco(int cosSinCount);
    ~IQNco();
    double getSin() const;
    double getCos() const;
    int getNcoCount() const;
    void setControl(double control);
    double getControl(){return m_control;}
private:
    void setNco();
    double *m_cosTable = nullptr;
    double *m_sinTable = nullptr;
    int m_cosSinCount;
    double m_control;
};

#endif // QINTEGRALCOSSIN_H
