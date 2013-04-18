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
    void run();

signals:
    void error(QTcpSocket::SocketError socketError);

private:
    QRegExp rx;
    QVector<QSqlDatabase> *db;
    int socketDescriptor;
};

#endif // QTILESTREAMTHREAD_H
