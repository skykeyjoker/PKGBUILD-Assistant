#include "decompressthread.h"

DecompressThread::DecompressThread(QObject *parent)
    :QObject(parent)
{

}

void DecompressThread::startWork()
{
    emit workStart();

    startDecompress();
}

void DecompressThread::startDecompress()
{
    QStringList shellList;
    shellList<<"tar -zxvf /home/skykey/Downloads/PKGBUILD-Assistant.tar.gz -C /home/skykey/test/src";
    for(int i=0; i<_shellList.size(); ++i)
    {
        QString _shell = _shellList.at(i);

        QProcess process;

        qDebug()<<_shell;

        process.start(_shell);

        process.waitForFinished();
    }

    emit workFinished();
}
