#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit_2->setVisible(false);
    ui->lineEdit_3->setVisible(false);
    ui->textBrowser->setVisible(false);
    ui->lineEdit->setVisible(false);
    ui->pushButton_2->setVisible(false);
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    nextBlockSize = 0;

    socket->connectToHost("127.0.0.1", 2323);  // подключение к хосту
    // change Visible buttins

    // pushButton_2 - добваить в список
    // pushButton_3 - препод
    // pushButton_4 - нектс
    // pushButton_5 - получить список
    // pushButton_6 - назад

    ui->pushButton_4->setVisible(false);
    ui->pushButton_5->setVisible(false);
    ui->pushButton_6->setVisible(false);
    ui->label->setVisible(false);
    ui->label_2->setVisible(false);
    ui->label_3->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    // студент
    ui->label->setVisible(true);
    ui->label_2->setVisible(true);
    ui->label_3->setVisible(true);
    ui->label->setText("ФИО студента");
    ui->label_2->setText("Номер группы");
    ui->label_3->setText("ФИО препода");

    ui->lineEdit->setVisible(true);
    ui->lineEdit_2->setVisible(true);
    ui->lineEdit_3->setVisible(true);
    ui->textBrowser->setVisible(true);
    ui->pushButton_2->setVisible(true);
    ui->pushButton_6->setVisible(true);


    ui->pushButton->setVisible(false);
    ui->pushButton_3->setVisible(false);


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

        QString str;
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

            in >> str;
            nextBlockSize = 0;
            // Если ответ от сервера аунтентичен, то править интерфейс
            // Если ответ от сервера что авторизован ученик, то заполнить интерфейс с номером группы и ФИО
            //else
            ui->textBrowser->append(str);
        }
        //ui->textBrowser->append(str);
    }
    else
    {
         ui->textBrowser->append("read error");
    }

}


void MainWindow::on_pushButton_2_clicked()
{
    // встать в очередь
    SendToServer(QString("[new]%1|%2|%3").arg(ui->lineEdit->text(), ui->lineEdit_2->text(), ui->lineEdit_3->text()));
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
}


void MainWindow::on_lineEdit_returnPressed()
{
    SendToServer(ui->lineEdit->text());
}


void MainWindow::on_pushButton_3_clicked()
{
    // кликаем на препода
    ui->label->setVisible(false);
    ui->label_2->setVisible(true);
    ui->label_3->setVisible(true);
    ui->label_2->setText("ФИО прeпода");
    ui->label_3->setText("Номер группы");

    ui->lineEdit_2->setVisible(true);
    ui->lineEdit_3->setVisible(true);
    ui->textBrowser->setVisible(true);
    ui->pushButton_4->setVisible(true);
    ui->pushButton_5->setVisible(true);
    ui->pushButton_6->setVisible(true);


    ui->pushButton->setVisible(false);
    ui->lineEdit->setVisible(false);
    ui->pushButton_2->setVisible(false);
    ui->pushButton_3->setVisible(false);
}


void MainWindow::on_pushButton_5_clicked()
{
    // получаем список
    ui->textBrowser->clear();
    SendToServer(QString("[pre]%1|%2").arg(ui->lineEdit_2->text(), ui->lineEdit_3->text()));
    ui->lineEdit->clear();
    //ui->lineEdit_2->clear();
    //ui->lineEdit_3->clear();
}


void MainWindow::on_pushButton_6_clicked()
{
    ui->lineEdit_2->setVisible(false);
    ui->lineEdit_3->setVisible(false);
    ui->textBrowser->setVisible(false);
    ui->lineEdit->setVisible(false);
    ui->pushButton_2->setVisible(false);
    ui->pushButton_4->setVisible(false);
    ui->pushButton_5->setVisible(false);
    ui->pushButton_6->setVisible(false);
    ui->label->setVisible(false);
    ui->label_2->setVisible(false);
    ui->label_3->setVisible(false);

    ui->pushButton->setVisible(true);
    ui->pushButton_3->setVisible(true);

    ui->textBrowser->clear();
}


void MainWindow::on_pushButton_4_clicked()
{
    // next
    if (ui->textBrowser->toPlainText() != "") {
        auto name = ui->textBrowser->toPlainText().split(QRegExp("[\n]"),QString::SkipEmptyParts)[0];
        SendToServer(QString("[nex]%1|%2|%3").arg(name, ui->lineEdit_2->text(), ui->lineEdit_3->text()));
        ui->textBrowser->clear();
    }
}

