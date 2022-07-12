#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QVector>
#include <QString>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>
<<<<<<< HEAD
=======

>>>>>>> b0a19df139c766a16cc9d28ee812b1d181e94553

class Server: public QTcpServer
{
    Q_OBJECT

public:
    Server();
    QTcpSocket *socket;
    void TestConnect(QStringList data);
    void SetDb(QSqlDatabase database, QString name,
               QString group, QString teachers, QString subject);
<<<<<<< HEAD
=======

>>>>>>> b0a19df139c766a16cc9d28ee812b1d181e94553
private:
    QVector <QTcpSocket*> Sockets;
    QByteArray Data;
    QSqlDatabase db;
    QSqlQuery *query;
    quint16 nextBlockSize;
<<<<<<< HEAD
    QStringList decode(QString str);
    void SendToClient(QString str);
=======

    QStringList decode(QString str);
    void SendToClient(QString str);

>>>>>>> b0a19df139c766a16cc9d28ee812b1d181e94553
public slots:
    void incomingConnection(qintptr socketDeskriptor);
    void slotReadyRead();

};

#endif // SERVER_H
