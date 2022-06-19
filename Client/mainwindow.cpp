#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    nextBlockSize = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    socket->connectToHost("127.0.0.1", 2323);
}



void MainWindow::SendToServer(QString str)
{
    Data.clear();
    QDataStream out(&Data,QIODevice::WriteOnly);
    //out.setVersion(QDataStream::Qt_6_2);
    out << quint16(0)<<str;
    out.device()->seek(0);
    out << quint16(Data.size() - sizeof(quint16));
    socket->write(Data);
    ui->lineEdit->clear();
}

void MainWindow::slotReadyRead()
{
    QDataStream in(socket);
    //in.setVersion(QDataStream::Qt_2);
    if (in.status() == QDataStream::Ok)
    {

        /*QString str;
        in >> str;
        ui->textBrowser->append(str);*/
        for(; ;)
        {
            if(nextBlockSize==0)
            {
                if (socket->bytesAvailable()<2)
                {
                    break;
                }
                in>>nextBlockSize;
            }
            if (socket->bytesAvailable()<nextBlockSize)
            {
                break;
            }

            QString str;
            in >> str;
            nextBlockSize = 0;
            ui->textBrowser->append(str);

        }
    }
    else
    {
         ui->textBrowser->append("read error");
    }

}


void MainWindow::on_pushButton_2_clicked()
{
    SendToServer(QString("[new]%1|%2|%3").arg(ui->lineEdit->text(), ui->lineEdit_2->text(), ui->lineEdit_3->text()));
}


void MainWindow::on_lineEdit_returnPressed()
{
    SendToServer(ui->lineEdit->text());
}

