#include "qtilestreamthread.h"

QTileStreamThread::QTileStreamThread(int socketDescriptor, QVector<QSqlDatabase> *databases, QObject *parent)
    : QThread(parent), socketDescriptor(socketDescriptor)
{
    rx = QRegExp("/\\d+/\\d+/\\d+.png"); // regexp matches "/1/2/3.png"
    this->db = databases;
    notFoundImage = new QByteArray(); // @TODO: fixit
}

QTileStreamThread::QTileStreamThread(int socketDescriptor, QVector<QSqlDatabase> *databases, QByteArray *notFoundImageData, QObject *parent)
    : QThread(parent), socketDescriptor(socketDescriptor)
{
    db = databases;
    notFoundImage = notFoundImageData;
    rx = QRegExp("/\\d+/\\d+/\\d+.png"); // regexp matches "/1/2/3.png"
}

void QTileStreamThread::run()
{
    QTcpSocket tcpSocket;
    if (!tcpSocket.setSocketDescriptor(socketDescriptor)) {
        emit error(tcpSocket.error());
        return;
    }

    QByteArray data;
    tcpSocket.waitForReadyRead(-1);
    if (tcpSocket.canReadLine()) {
        QString req = QString(tcpSocket.readLine());
        QStringList tokens = req.split(QRegExp("[ \r\n][ \r\n]*"));
        qDebug() << tokens[1];
        if (tokens[0] == "GET") {
            if (rx.exactMatch(tokens[1])) {
                QStringList path = QString(tokens.at(1)).split("/");
                unsigned int zoom = path.at(1).toInt();
                unsigned int column = path.at(2).toInt();
                QStringList rowAndImageType = path.at(3).split(".");
                unsigned int row = rowAndImageType.at(0).toInt();

                for (int i = 0; i < db->size(); ++i) {
                    QSqlQuery query(db->at(i));
                    query.prepare("SELECT tile_data FROM tiles WHERE zoom_level = :zoom AND tile_column = :column AND tile_row = :row");
                    query.bindValue(":zoom", zoom);
                    query.bindValue(":column", column);
                    query.bindValue(":row", row);

                    if (query.exec()) {
                        while (query.next()) {
                            data = query.value(0).toByteArray();
                        }
                    }

                    if (!data.isEmpty()) {
                        continue;
                    }
                }

            }
        }
    }

    if (data.isEmpty()) {
        if (notFoundImage->isEmpty()) {
            tcpSocket.write("HTTP/1.0 404 Not Found\r\n"
                                    "Content-Type: text/html; charset=\"utf-8\"\r\n"
                                    "Connection: close\r\n"
                                    "\r\n"
                                    "There is no spoon");
        }
        else {
            tcpSocket.write(QString("HTTP/1.1 200 OK\r\n"
                    "Content-Type: image/png\r\n"
                    "Content-Length: %1\r\n"
                    "Connection: close\r\n"
                    "\r\n").arg(notFoundImage->length()).toAscii());
            tcpSocket.write(*notFoundImage);
        }
    }
    else {
        tcpSocket.write(QString("HTTP/1.1 200 OK\r\n"
                "Content-Type: image/png\r\n"
                "Content-Length: %1\r\n"
                "Connection: close\r\n"
                "\r\n").arg(data.length()).toAscii());
        tcpSocket.write(data);
    }

    tcpSocket.disconnectFromHost();
    tcpSocket.waitForDisconnected();
}
