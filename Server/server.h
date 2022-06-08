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


class Server: public QTcpServer
{
    Q_OBJECT

public:
    Server();
    QTcpSocket *socket;
    void TestConnect();
private:
    QVector <QTcpSocket*> Sockets;
    QByteArray Data;
    void SendToClient(QString str);
    quint16 nextBlockSize;
    QSqlDatabase db;
        QSqlQuery *query;
public slots:
    void incomingConnection(qintptr socketDeskriptor);
    void slotReadyRead();

};

#endif // SERVER_H
