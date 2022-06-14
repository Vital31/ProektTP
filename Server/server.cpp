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
            if (str.left(5)=="[new]")
            {
                qDebug()<<decode(str.mid(5, str.length()));
                TestConnect(decode(str.mid(5, str.length())));

            }else {
                qDebug()<<str;
            }
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
void Server::TestConnect(QStringList data)
{
    /// У нас ничего не вышло
    /// Мы это сделаем в скором времени
    /// doxygen

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("12345.db");
    if (db.open()) {
        qDebug("database opened");
        SetDb(db, data[0], data[1], data[2], "0");
       /* query = new QSqlQuery(db);
        query -> exec("select * from Queue");
        while (query->next()) {
            qDebug() << QString("[%1%5%2%5%3%5%4]").arg(query->value("Name").toString(),
                                                        query->value("Groups").toString(),
                                                        query->value("Teachers").toString(),
                                                        query->value("id_num").toString(),
                                                        "    |");
        } */
    } else {
        qDebug("database doesn't open");
    }

    qDebug("Db end");
}

void Server::SetDb(QSqlDatabase database, QString name,
                   QString group, QString teachers, QString subject)
{
    query = new QSqlQuery(database);
    qDebug()<<QString("insert into Queue values ('%1', '%2', '%3', %4);").arg(name, group, teachers, subject);
    query -> exec(QString("insert into Queue values ('%1', '%2', '%3', %4);").arg(name, group, teachers, subject));
}

QStringList Server::decode(QString str)
{
    return str.split("|");
}

