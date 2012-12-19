#include "tilestream.h"

#include <QtSql>
#include <QDebug>
#include <QCoreApplication>

TileStream::TileStream(const QString path) :
    QTcpServer()
{
    this->db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);
    if (!db.open()) {
        qDebug() << db.lastError().text();
    }
}

TileStream::~TileStream() {
    this->db.close();
}

void TileStream::incomingConnection(int socketDescriptor)
{
    QTcpSocket* s = new QTcpSocket(this);
    connect(s, SIGNAL(readyRead()), this, SLOT(readClient()));
    s->setSocketDescriptor(socketDescriptor);
}

void TileStream::readClient() {
    QTcpSocket* socket = (QTcpSocket*)sender();
    if (socket->canReadLine()) {
        QStringList tokens = QString(socket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));
        if (tokens[0] == "GET") {
            qDebug() << tokens.at(1);
            if (tokens.at(1) == "/favicon.ico") {
                QTextStream os(socket);
                os.setAutoDetectUnicode(true);
                os << "HTTP/1.0 200 Ok\r\n"
                      "Content-Type: text/html; charset=\"utf-8\"\r\n";
            }
            else {
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
                            os << "HTTP/1.0 404 Not Found\r\n"
                                  "Content-Type: text/html; charset=\"utf-8\"\r\n";
                        }
                        else {
                            socket->write(data);
                        }
                    }
                }
            }

            socket->close();
            if (socket->state() == QTcpSocket::UnconnectedState) {
                delete socket;
            }
        }
    }
}
