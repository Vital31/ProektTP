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
    void TestConnect(QStringList data);
    void SetDb(QSqlDatabase database, QString name,
               QString group, QString teachers, QString subject);

private:
    QVector <QTcpSocket*> Sockets;
    QByteArray Data;
    QSqlDatabase db;
    QSqlQuery *query;
    quint16 nextBlockSize;

    QStringList decode(QString str);
    void SendToClient(QString str);

public slots:
    void incomingConnection(qintptr socketDeskriptor);
    void slotReadyRead();

};

#endif // SERVER_H
