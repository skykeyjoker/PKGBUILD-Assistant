#ifndef PKGBUILDASSISTANT_H
#define PKGBUILDASSISTANT_H

#include <QMainWindow>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QDir>
#include <QProcess>
#include <QGridLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QTextBrowser>
#include <QGroupBox>
#include <QDebug>

#include "dialogfilecopy.h"
#include "dialogfiledelete.h"
#include "dialogfilechmod.h"
#include "decompressthread.h"
#include "packagequerythread.h"


QT_BEGIN_NAMESPACE
namespace Ui { class PKGBUILDASSISTANT; }
QT_END_NAMESPACE

class PKGBUILDASSISTANT : public QMainWindow
{
    Q_OBJECT

public:
    PKGBUILDASSISTANT(QWidget *parent = nullptr);
    ~PKGBUILDASSISTANT();

    QGridLayout *layout_baseinfoTab;
    QLineEdit *le_pkgname;
    QLineEdit *le_pkgver;
    QLineEdit *le_pkgrel;
    QTextEdit *le_pkgdesc;
    QLineEdit *le_arch;
    QLineEdit *le_url;


    QGridLayout *layout_sourceTab;
    QTextEdit *le_source;
    QHBoxLayout *hlayout_radios;
    QRadioButton *radio_md5sums;
    QRadioButton *radio_sha1sums;
    QRadioButton *radio_sha256sums;
    QRadioButton *radio_sha512sums;
    QTextEdit *le_sum;


    QGridLayout* layout_relationsTab;
    QLineEdit *le_provides;
    QLineEdit *le_conflicts;
    QLineEdit *le_maintainerName;
    QLineEdit *le_maintainerEmail;


    QGridLayout *layout_dependsTab;
    QLineEdit *le_dependsDir;
    QDir *dependsDir;
    QTableWidget *table_depends;
    QTextEdit *le_depends;
    QTextEdit *le_optdepends;
    QStringList list_so;


    QVBoxLayout *layout_functionTab;
    QGroupBox *group_setFakeDir;
    QGroupBox *group_operation;
    QGroupBox *group_trees;
    QGroupBox *group_preview;
    QLineEdit *le_fakeDirPath;
    QLineEdit *le_archPath;
    QTextEdit *le_preview;
    QTextBrowser *tree_source;
    QTextBrowser *tree_pkg;


    QVBoxLayout *layout_createTab;
    QLineEdit *le_savePath;
    QTextEdit *le_savePreview;
    QGroupBox *group_savePreview;

    QByteArray pkgBuf;

    void scanSoFiles();
    QString scanPackageNames(QString filename);
    void decompress(QString fileName);
    void updateTree();
    bool copyDirectoryFiles(const QString &fromDir, const QString &toDir);
    bool DeleteDirectory(const QString &path);
    void createFile();

    void checkFile();

    void checkPkg();

public slots:
    void slotAddOperation(QString operation);
    void insertPackageNames(QStringList pkgNameList);
private:
    Ui::PKGBUILDASSISTANT *ui;
};
#endif // PKGBUILDASSISTANT_H
