#include "packagequerythread.h"

PackageQueryThread::PackageQueryThread(QObject *parent) : QObject(parent)
{

}

void PackageQueryThread::startWork()
{
    emit workStarted();

    startQuery();
}

void PackageQueryThread::startQuery()
{
    for(int i=0; i<_fileNameLists.size(); i++)
    {
        QString fileName = _fileNameLists.at(i);
        qDebug()<<fileName;

        QProcess process;

        process.start(tr("pacman -Fx %1").arg(fileName));

        process.waitForFinished();

        QByteArray buf = process.readAllStandardOutput();

        QString ret(buf);

        _pkgLists.append(ret);
    }

    emit ret(_pkgLists);
    emit workFinished();
}
