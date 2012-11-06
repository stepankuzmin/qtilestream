#include <QtGui>
#include <QDebug>
#include <QtNetwork>

#include <QWebView>
#include <QUrl>

#include <stdlib.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "tilestream.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (!server.listen(QHostAddress::Any, 8080)) {
        QMessageBox::critical(this, tr("Threaded Fortune Server"),
                              tr("Unable to start the server: %1.")
                              .arg(server.errorString()));
        close();
        return;
    }

    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    qDebug() << tr("The server is running on\n\nIP: %1\nport: %2\n\n")
                .arg(ipAddress).arg(server.serverPort());

    QUrl url;

    url.setScheme("http");
    url.setHost(ipAddress);
    url.setPort(server.serverPort());

    ui->webView->setUrl(url);
}

MainWindow::~MainWindow()
{
    delete ui;
}
