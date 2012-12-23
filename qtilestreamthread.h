#ifndef QTILESTREAMTHREAD_H
#define QTILESTREAMTHREAD_H

#include <QtSql>
#include <QThread>
#include <QTcpSocket>

class QTileStreamThread : public QThread
{
    Q_OBJECT
public:
    explicit QTileStreamThread(int socketDescriptor, QSqlDatabase *db, QObject *parent);

public slots:
    void readClient();

signals:
    void error(QTcpSocket::SocketError socketError);

private:
    QRegExp rx;
    QSqlDatabase *db;
    int socketDescriptor;
};

#endif // QTILESTREAMTHREAD_H
