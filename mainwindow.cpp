#include "mainwindow.h"
#include "ui_mainwindow.h"

mainwindow::mainwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mainwindow)
{
    ui->setupUi(this);
    hostBtn = ui->hostBtn;
    clientBtn = ui->clientBtn;
    hostBack = ui->hostBack;
    clientStart = ui->clientStart;
    clientBack = ui->clientBack;
    ipEdit = ui->ipEdit;
    portEdit = ui->portEdit;
    receiveMsg = ui->receiveMsg;
    sendMsg = ui->sendMsg;
    backBtn = ui->backBtn;
    sendBtn = ui->sendBtn;

    startBox = ui->startBox;
    hostBox = ui->hostBox;
    clientBox = ui->clientBox;
    sessionBox = ui->sessionBox;
    status = ui->status;

    clientBox->hide();
    hostBox->hide();
    sessionBox->hide();

    tcpServer = new QTcpServer(this);
    tcpSocket = new QTcpSocket(this);
    checkConnect = new QTimer(this);
    checkConnect->start(100);

    connect(hostBtn, SIGNAL(clicked()), this, SLOT(hostPart()));
    connect(clientBtn, SIGNAL(clicked()), this, SLOT(clientPart()));
    connect(hostBack, SIGNAL(clicked()), this, SLOT(backToMenu()));
    connect(clientBack, SIGNAL(clicked()), this, SLOT(backToMenu()));
    connect(clientStart, SIGNAL(clicked()), this, SLOT(prepareStart()));
    connect(sendBtn, SIGNAL(clicked()), this, SLOT(sendMessage()));
    connect(backBtn, SIGNAL(clicked()),this, SLOT(backToMenu()));

}

mainwindow::~mainwindow()
{
    delete ui;
}

void mainwindow::hostPart()
{
    tcpServer->listen();
    QList<QHostAddress> ipList = QNetworkInterface::allAddresses();
    status->setText("IP : \n");

    foreach(QHostAddress i, ipList){
        if(i.toIPv4Address())  status->setText(status->text() + i.toString() + '\n');
    }

    status->setText(status->text() + "\nport : " + QString::number(tcpServer->serverPort()) + '\n');

    startBox->hide();
    hostBox->show();
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(prepareStart()));
    character = "host";
}

void mainwindow::clientPart()
{
    startBox->hide();
    clientBox->show();
    ipEdit->setFocus();
    character = "client";
}


void mainwindow::backToMenu()
{
    startBox->show();
    clientBox->hide();
    sessionBox->hide();
    hostBox->hide();
    if(character == "host"){
        tcpServer->close();
        disconnect(tcpServer);
    }

}

void mainwindow::prepareStart()
{
    if(character == "client"){
        if(ipEdit->text().isEmpty() || portEdit->text().isEmpty()) return;

        host = ipEdit->text();
        port = portEdit->text().toInt();

        tcpSocket->connectToHost(host,port);
        connect(checkConnect, SIGNAL(timeout()), this, SLOT(connectSuccess()));
        connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readMessage()));

    }
    else if(character == "host"){
        hostBox->hide();
        sessionBox->show();
        delete tcpSocket;
        tcpSocket = tcpServer->nextPendingConnection();
        connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readMessage()));
    }
}

void mainwindow::readMessage()
{
    QDataStream in(tcpSocket);

    in.startTransaction();
    if(!in.commitTransaction()){
        return;
    }
    QString data;
    in>>data;
    qDebug()<<data;

    receiveMsg->setText(receiveMsg->toPlainText() + '\n' + data);
}

void mainwindow::sendMessage()
{
    QByteArray byte;
    QDataStream stream(&byte, QIODevice::WriteOnly);


    stream<<sendMsg->text();
    sendMsg->clear();
    sendMsg->setFocus();
    tcpSocket->write(byte);
}

void mainwindow::connectSuccess()
{
    if(tcpSocket->state() == QAbstractSocket::ConnectedState){
        sessionBox->show();
        clientBox->hide();

        disconnect(checkConnect);
    }
}

void mainwindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return){
        if(sessionBox->isEnabled())
            sendMessage();
    }
}
