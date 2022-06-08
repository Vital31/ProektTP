#include <server.h>

Server::Server()
{
    if(this->listen(QHostAddress::Any, 2323))
    {
        qDebug()<<"start";
    }
    else
    {
        qDebug()<<"error";
    }
    nextBlockSize = 0;
}

void Server::incomingConnection(qintptr socketDesckriptor)
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDesckriptor);
    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    Sockets.push_back(socket);
    qDebug()<<"client connecnted"<< socketDesckriptor;
}

void Server::slotReadyRead()
{
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);                 //почему-то не работает данный метод
    //in.setVersion(QDataStream::Qt_6_2);
    if (in.status() == QDataStream::Ok)
    {
        qDebug()<<"read.....";
        /*QString str;
        in >> str;
        qDebug()<<str;
        SendToClient(str);*/
        for(; ;)
        {
            if(nextBlockSize==0)
            {
                qDebug() << "nextBlockSize - 0";
                if (socket->bytesAvailable()<2)
                {
                    qDebug() << "Data < 2, break";
                    break;
                }
                in>>nextBlockSize;
                qDebug() << "nextBlockSize ="<<nextBlockSize;
            }
            if (socket->bytesAvailable()<nextBlockSize)
            {
                qDebug() << "Data not full, break";
                break;
            }

            QString str;
            in >> str;
            nextBlockSize = 0;
            qDebug()<<str;
            //ui->textBrowser->append(str);
            SendToClient(str);
            break;
        }
    }
    else
    {
        qDebug()<<"DataStream Error";
    }

}


void Server::SendToClient(QString str)
{
        Data.clear();
        QDataStream out(&Data,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_6);
        out << quint16(0)<<str;
        out.device()->seek(0);
        out << quint16(Data.size() - sizeof(quint16));
        //socket->write(Data);
        for(int i=0; i < Sockets.size(); i++)
        {
            Sockets[i]->write(Data);
        }
}

void Server::TestConnect()
{
    /// У нас ничего не вышло
    /// Мы это сделаем в скором времени
    /// doxygen

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("Database.db");
    if (db.open()) {
        qDebug("database opened");

        query = new QSqlQuery(db);
        query -> exec("select * from Queue");
        query -> next();

        qDebug() << query->size();
        while (query->next()) {
            qDebug() << query;

        }
    } else {
        qDebug("database doesn't open");
    }

    qDebug("Db end");
}
