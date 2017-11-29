#ifndef CSERVEURTCP_H
#define CSERVEURTCP_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>

#define PORTPARDEFAUT 2222

class CServeurTcp : public QTcpServer
{
    Q_OBJECT
public:
    explicit CServeurTcp(QObject *parent = 0);
    explicit CServeurTcp(QObject *parent = 0, quint16 noPort = 2222);
    ~CServeurTcp();
    int emettreVersClients(QString mess);

signals:
    void sigEvenementServeur(QString eve);
    void sigErreur(QAbstractSocket::SocketError err);
    void sigDataClient(QString adrIpClient, QString data);
    void sigAdrClient(QString adrClient);
    void sigMaJClients(QList<QTcpSocket *> liste);

public slots:
    void onNewConnectionClient();
    void onDisconnectedClient();
    void onErreurReseau(QAbstractSocket::SocketError err);
    void onReadyReadClient();

private:
    int init();
    quint16 m_noPort;
    QList<QTcpSocket *> listeClients;
};

#endif // CSERVEURTCP_H
