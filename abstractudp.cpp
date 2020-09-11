#include "abstractudp.h"

AbstractUdp::AbstractUdp(QHostAddress IP, quint16 dstPort, quint16 srcPort) : QObject (),
    m_udpSocket {nullptr}
{
    setNetworkSetting(IP, dstPort, srcPort);
}

AbstractUdp::AbstractUdp()  : QObject (),
    m_udpSocket {nullptr}
{

}

AbstractUdp::~AbstractUdp()
{
    unbind();
}

void AbstractUdp::readPacket()
{
    int len = static_cast<int>(m_udpSocket->pendingDatagramSize());
    if(len<1)
        return;

    QByteArray data;
    data.resize(len);

    QHostAddress address;
    m_udpSocket->readDatagram(data.data(), len, &address);

    readPacket(data);
}

void AbstractUdp::sendUdpData(char *buf, int size)
{
    if(m_udpSocket != nullptr)
        m_udpSocket->writeDatagram(buf, size, m_IP, m_dstPort);
}

void AbstractUdp::sendUdpData(QByteArray data)
{
    if(m_udpSocket != nullptr)
        m_udpSocket->writeDatagram(data, m_IP, m_dstPort);
}

void AbstractUdp::setNetworkSetting(QHostAddress IP, quint16 dstPort, quint16 srcPort)
{
    m_IP = IP;
    m_dstPort = dstPort;
    m_srcPort = srcPort;
}

void AbstractUdp::bindUdpPort(quint16 port)
{
    m_srcPort = port;
    bind();
}

void AbstractUdp::bind()
{
    if(m_udpSocket != nullptr)
        delete m_udpSocket;

    m_udpSocket = new QUdpSocket();
    connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(readPacket()));

    if(!m_udpSocket->bind(QHostAddress::Any, m_srcPort))
    {
        qWarning() << "Не удалось открыть UDP порт " + QString::number(m_srcPort);
    }
    else
    {
        qInfo() << "Открыт UDP порт" << m_srcPort << "|" <<"Текущий поток" << QThread::currentThread();
    }
}

void AbstractUdp::unbind()
{
    if(m_udpSocket != nullptr)
    {
        //if(m_udpSocket->isOpen())
            qInfo() << "Закрыт UDP порт" << m_srcPort;
        delete m_udpSocket;
        m_udpSocket = nullptr;
    }
}

QHostAddress AbstractUdp::getIP()
{
    return m_IP;
}

quint16 AbstractUdp::getDstPort()
{
    return m_dstPort;
}

quint16 AbstractUdp::getSrcPort()
{
    return m_srcPort;
}
