#ifndef DECOMPRESSTHREAD_H
#define DECOMPRESSTHREAD_H

#include <QObject>
#include <QThread>
#include <QProcess>
#include <QDebug>

class DecompressThread : public QObject
{
    Q_OBJECT
public:
    DecompressThread(QObject *parent = nullptr);

    void setShell(QStringList const & shellList)
    {
        _shellList=shellList;
    }

public slots:
    void startWork();
    void startDecompress();

signals:

    void workStart();
    void workFinished();

private:
    QStringList _shellList;
};

#endif // DECOMPRESSTHREAD_H
