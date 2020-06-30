#ifndef PACKAGEQUERYTHREAD_H
#define PACKAGEQUERYTHREAD_H

#include <QObject>
#include <QProcess>
#include <QDebug>

class PackageQueryThread : public QObject
{
    Q_OBJECT
public:
    explicit PackageQueryThread(QObject *parent = nullptr);

    void setFileName(QStringList fileNameList)
    {
        _fileNameLists = fileNameList;
    }


public slots:
    void startWork();
    void startQuery();

signals:
    void workStarted();
    void workFinished();
    void ret(QStringList pkgList);

private:
    QStringList _fileNameLists;
    QStringList _pkgLists;

signals:

};

#endif // PACKAGEQUERYTHREAD_H
