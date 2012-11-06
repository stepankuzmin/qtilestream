#ifndef TILESTREAMTHREAD_H
#define TILESTREAMTHREAD_H

#include <QThread>
#include <QTcpSocket>

class TileStreamThread : public QThread
{
    Q_OBJECT

public:
    TileStreamThread(int socketDescriptor, const QString &fortune, QObject *parent);

    void run();

signals:
    void error(QTcpSocket::SocketError socketError);

private:
    int socketDescriptor;
    QString text;
};

#endif // TILESTREAMTHREAD_H
