#ifndef ABSTRACTUDP_H
#define ABSTRACTUDP_H

#include <QObject>
#include <QUdpSocket>
#include <QThread>

/**
 * @brief The AbstractUdp class
 */
class AbstractUdp : public QObject
{
    Q_OBJECT
public:
    explicit AbstractUdp(QHostAddress IP, quint16 dstPort, quint16 srcPort);
    explicit AbstractUdp();
    ~AbstractUdp();

private:
    QHostAddress m_IP;
    quint16 m_dstPort;
    quint16 m_srcPort;
    QUdpSocket *m_udpSocket;

private slots:
    void readPacket();

protected:
    void sendUdpData(char *buf, int size);
    void sendUdpData(QByteArray data);
    virtual void readPacket(QByteArray data) = 0;

signals:

public slots:
    /**
     * @brief setNetworkSetting
     * Задаёт сетевые настройки
     * @param IP
     * @param dstPort
     * @param srcPort
     */
    void setNetworkSetting(QHostAddress IP, quint16 dstPort, quint16 srcPort);

    /**
     * @brief bindUdpPort
     * Задаёт отдельно UDP порт и начинает его слушать
     * @param port
     */
    void bindUdpPort(quint16 port);

    /**
     * @brief bind
     * Запускает прослушиваение заданного входящего порта
     */
    void bind();

    /**
     * @brief unbind
     * Останавливает прослушиваение заданного входящего порта
     */
    void unbind();

    /**
     * @brief getIP
     * Возвращает заданный IP
     * @return
     */
    QHostAddress    getIP();

    /**
     * @brief getDstPort
     * Возвращает заданный исходящий порт
     * @return
     */
    quint16         getDstPort();

    /**
     * @brief getSrcPort
     * Возвращает заданный входящий порт
     * @return
     */
    quint16         getSrcPort();
};

#endif // ABSTRACTUDP_H
