#include "cserveurtcp.h"

CServeurTcp::CServeurTcp(QObject *parent) :
    QTcpServer(parent)
{
    m_noPort = PORTPARDEFAUT;
    init();
}

CServeurTcp::CServeurTcp(QObject *parent, quint16 noPort) :
    QTcpServer(parent)
{
    m_noPort = noPort;
    init();
}

CServeurTcp::~CServeurTcp()
{
    // destruction optionnelle car déjà pris en charge par le serveur
    for (int i=0 ; i<listeClients.size() ; i++) {
        listeClients.at(i)->close();
        delete listeClients.at(i);
    } // for i
}

int CServeurTcp::emettreVersClients(QString mess)
{
    for (int i=0 ; i<listeClients.size() ; i++) {
        listeClients.at(i)->write(mess.toStdString().c_str());
        qDebug() << "Envoi vers " << listeClients.at(i);
    } // for i
    return 1;
}

int CServeurTcp::init()
{
    listen(QHostAddress::Any, m_noPort);
    connect(this,SIGNAL(newConnection()), this, SLOT(onNewConnectionClient()));
    connect(this, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(onErreurReseau(QAbstractSocket::SocketError)));
    return 1;
}

///////////////////  SLOTs ////////////////////////

void CServeurTcp::onNewConnectionClient()
{
    QString mess="Un client vient de se connecter";
    qDebug() << mess;
    QTcpSocket *newClient = this->nextPendingConnection();
    qDebug() << "Nouvelle connexion : " << newClient;
    if (newClient == NULL)
        emit sigErreur(QAbstractSocket::ConnectionRefusedError);
    connect(newClient, SIGNAL(readyRead()), this, SLOT(onReadyReadClient()));
    connect(newClient, SIGNAL(disconnected()), this, SLOT(onDisconnectedClient()));
    listeClients.append(newClient);  // sauve l'adresse de l'objet dans la liste
    emit sigEvenementServeur("CON");
    emit sigAdrClient(newClient->localAddress().toString());
    emit sigMaJClients(listeClients);  // pour IHM
}

void CServeurTcp::onDisconnectedClient()
{
    QTcpSocket *client = (QTcpSocket *)sender(); // Déterminer quel client ?
    emit sigEvenementServeur("DEC");
    listeClients.removeOne(client);
    delete client;
    emit sigMaJClients(listeClients);
}

void CServeurTcp::onErreurReseau(QAbstractSocket::SocketError err)
{
    qDebug() << "Erreur réseau !";
    emit sigErreur(err);
}

void CServeurTcp::onReadyReadClient()
{
    QByteArray ba;
    // Déterminer quel client ?
    QTcpSocket *client = (QTcpSocket *)sender();
    ba=client->readAll();
    int nb = client->write("OK");
    if (nb == -1)
        qDebug() << "ERREUR ENVOI";
    qDebug() << "Client : " << client << ba.size() << " Caractères reçus.";
    emit sigDataClient(client->localAddress().toString(), QString(ba));
}
