#include "qtilestreamthread.h"

#include <QtNetwork>

QTileStreamThread::QTileStreamThread(int socketDescriptor, QSqlDatabase *db, QObject *parent)
    : QThread(parent), socketDescriptor(socketDescriptor)
{
    // regexp matches "/1/2/3.png"
    rx = QRegExp("/\\d+/\\d+/\\d+.png");

    this->db = db;
}

void QTileStreamThread::readClient() {
    QTcpSocket* socket = (QTcpSocket*)sender();
    if (socket->canReadLine()) {
        QString req = QString(socket->readLine());
        QStringList tokens = req.split(QRegExp("[ \r\n][ \r\n]*"));
        qDebug() << tokens[1];
        if (tokens[0] == "GET") {
            if (rx.exactMatch(tokens[1])) {
                QStringList path = QString(tokens.at(1)).split("/");
                unsigned int zoom = path.at(1).toInt();
                unsigned int column = path.at(2).toInt();
                QStringList rowAndImageType = path.at(3).split(".");
                unsigned int row = rowAndImageType.at(0).toInt();

                QSqlQuery query;
                query.prepare("SELECT tile_data FROM tiles WHERE zoom_level = :zoom AND tile_column = :column AND tile_row = :row");
                query.bindValue(":zoom", zoom);
                query.bindValue(":column", column);
                query.bindValue(":row", row);

                if (query.exec()) {
                    while (query.next()) {
                        QByteArray data = query.value(0).toByteArray();
                        if (data.isEmpty()) {
                            QTextStream os(socket);
                            os.setAutoDetectUnicode(true);
                            os << "HTTP/1.1 404 Not Found\r\n"
                                  "Content-Type: text/html; charset=\"utf-8\"\r\n"
                                  "Connection: close\r\n";
                        }
                        else {
                            socket->write(data);
                        }
                    }
                }
            }
            else {
                QTextStream os(socket);
                os.setAutoDetectUnicode(true);
                os << "HTTP/1.1 404 Not Found\r\n"
                      "Content-Type: text/html; charset=\"utf-8\"\r\n"
                      "Connection: close\r\n\n"
                      "There is no spoon.";
            }
        }

        socket->close();
        if (socket->state() == QTcpSocket::UnconnectedState) {
            delete socket;
        }
    }
}
