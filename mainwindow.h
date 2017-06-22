#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QPushButton>
#include <QGroupBox>
#include <QLineEdit>
#include <QList>
#include <QLabel>
#include <QTextBrowser>
#include <QKeyEvent>

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QByteArray>
#include <QNetworkInterface>
#include <QHostInfo>
#include <QTimer>

namespace Ui {
class mainwindow;
}

class mainwindow : public QDialog
{
    Q_OBJECT

public:
    explicit mainwindow(QWidget *parent = 0);
    ~mainwindow();
    QLineEdit *ipEdit;
    QLineEdit *portEdit;

private:
    Ui::mainwindow *ui;
    QPushButton *hostBtn;
    QPushButton *clientBtn;
    QPushButton *clientBack;
    QPushButton *clientStart;
    QPushButton *hostBack;
    QPushButton *backBtn;
    QPushButton *sendBtn;
    QLabel *status;
    QTextBrowser *receiveMsg;
    QLineEdit *sendMsg;
    QString host;
    quint16 port;

    QGroupBox *hostBox;
    QGroupBox *startBox;
    QGroupBox *clientBox;
    QGroupBox *sessionBox;

    QString character;
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    QTcpSocket *receive;
    QTimer *checkConnect;

private slots:
    void hostPart();
    void clientPart();
    void backToMenu();
    void prepareStart();
    void readMessage();
    void sendMessage();
    void connectSuccess();

    virtual void keyPressEvent(QKeyEvent *event);

};

#endif // MAINWINDOW_H
