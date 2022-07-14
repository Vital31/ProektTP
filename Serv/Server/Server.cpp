#include <Server.h>

Server::Server()
{
    UnitTest();  // вызов тестов
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
                SendToClient(str.mid(5, str.length()).replace("|", " -- "));
                TestConnect(decode(str.mid(5, str.length())));

            } else if (str.left(5)=="[pre]") {
                auto __data__ = decode(str.mid(5, str.length()));
                Read_Deque(__data__[0], __data__[1]);

            } else if (str.left(5)=="[nex]") {
                auto __data__ = decode(str.mid(5, str.length()));
                next(__data__[0], __data__[1], __data__[2]);
                Read_Deque(__data__[1], __data__[2]);
            } else {
                qDebug()<<str;
            }
            //ui->textBrowser->append(str);
            break;
        }
    }
    else
    {
        qDebug()<<"DataStream Error";
    }

}

void Server::UnitTest()
{
    ///          Unit tests
    /// Testing the <decode> function
    qDebug() << decode("a|b|c");
    qDebug() << typeid(decode("kieill|ann|go")).name() << "\n============";
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

void Server::Read_Deque(QString prepod, QString group)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("12345.db");
    if (db.open()) {
        qDebug("database opened [Read_Deque]");
        query = new QSqlQuery(db);
        qDebug() << QString("select Name from Queue WHERE Teachers='%1' AND Groups='%2' AND id_num=0").arg(prepod, group);
        query -> exec( QString("select Name from Queue WHERE Teachers='%1' AND Groups='%2' AND id_num=0").arg(prepod, group) );
        while (query->next()) {
            qDebug() << "+ " << query->value("Name").toString();
            SendToClient(query->value("Name").toString());
        }
    } else {
        qDebug("database doesn't open");
    }

    qDebug("Db end");

    db.close();
}

void Server::next(QString name, QString prepod, QString group)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("12345.db");
    if (db.open()) {
        qDebug("database opened [next]");
        query = new QSqlQuery(db);
        qDebug() << QString("UPDATE Queue SET id_num = 1 WHERE Name == '%1' AND Groups == '%2' AND Teachers == '%3' AND id_num == 0;").arg(name, group, prepod);
        query -> exec(QString("UPDATE Queue SET id_num = 1 WHERE Name == '%1' AND Groups == '%2' AND Teachers == '%3' AND id_num == 0;").arg(name, group, prepod));
    } else {
        qDebug("database doesn't open");
    }

    qDebug("Db end");

    db.close();
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
    } else {
        qDebug("database doesn't open");
    }

    qDebug("Db end");

    db.close();
}

void Server::SetDb(QSqlDatabase database, QString name,
                   QString group, QString teachers, QString id_num)
{
    query = new QSqlQuery(database);
    qDebug()<<QString("insert into Queue values ('%1', '%2', '%3', %4);").arg(name, group, teachers, id_num);
    query -> exec(QString("insert into Queue values ('%1', '%2', '%3', %4);").arg(name, group, teachers, id_num));
}

QStringList Server::decode(QString str)
{
    return str.split("|");
}


