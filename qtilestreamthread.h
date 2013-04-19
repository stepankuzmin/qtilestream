#ifndef QTILESTREAMTHREAD_H
#define QTILESTREAMTHREAD_H

#include <QtSql>
#include <QRegExp>
#include <QThread>
#include <QString>
#include <QTcpSocket>

class QTileStreamThread : public QThread
{
    Q_OBJECT
public:
    QTileStreamThread(int socketDescriptor, QVector<QSqlDatabase> *databases, QObject *parent);
    QTileStreamThread(int socketDescriptor, QVector<QSqlDatabase> *databases, QByteArray *notFoundImageData, QObject *parent);
    void run();

signals:
    void error(QTcpSocket::SocketError socketError);

private:
    QRegExp rx;
    int socketDescriptor;
    QVector<QSqlDatabase> *db;
    QByteArray *notFoundImage;
};

#endif // QTILESTREAMTHREAD_H
