#ifndef QTILESTREAM_H
#define QTILESTREAM_H

#include <QtSql>
#include <QRegExp>
#include <QString>
#include <QTcpServer>
#include <QByteArray>
#include <QStringList>

class QTileStream : public QTcpServer
{
    Q_OBJECT
public:
    explicit QTileStream(QString path, QObject *parent = 0);
    ~QTileStream();

protected:
    void incomingConnection(qintptr socketDescriptor);

private:
    QSqlDatabase db;
};

#endif // QTILESTREAM_H
