#include "tilestream.h"
#include "tilestreamthread.h"

#include <QDebug>
#include <stdlib.h>

TileStream::TileStream(QObject *parent) :
    QTcpServer(parent)
{
}

void TileStream::incomingConnection(int socketDescriptor)
{
    /*
    TileStreamThread *thread = new TileStreamThread(socketDescriptor, QString(""), this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
    */

    QTcpSocket* s = new QTcpSocket(this);
    connect(s, SIGNAL(readyRead()), this, SLOT(readClient()));
    s->setSocketDescriptor(socketDescriptor);
}

void TileStream::readClient() {
    QTcpSocket* socket = (QTcpSocket*)sender();
    if (socket->canReadLine()) {
        QStringList tokens = QString(socket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));
        if (tokens[0] == "GET") {
            QStringList path = QString(tokens[1]).split("/");
            QTextStream os(socket);
            os.setAutoDetectUnicode(true);
            os << "HTTP/1.0 200 Ok\r\n"
                  "Content-Type: text/html; charset=\"utf-8\"\r\n"
                  "\r\n"
                  "<h1>Welcome to QTileStream</h1>\n";

            /* @TODO: url dispatching
            for (int i=0; i < path.size(); ++i) {
                os << path.at(i) << "\n";
            }
            */

            socket->close();

            if (socket->state() == QTcpSocket::UnconnectedState) {
                delete socket;
            }
        }
    }
}
