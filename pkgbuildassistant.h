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
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QGroupBox>
#include <QDebug>



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
    QLineEdit *le_debPath;
    QTextEdit *le_preview;
    QTreeWidget *tree_source;
    QTreeWidget *tree_pkg;


    QVBoxLayout *layout_createTab;
    QLineEdit *le_savePath;
    QTextEdit *le_savePreview;
    QGroupBox *group_savePreview;

    void scanSoFiles();
    QString scanPackageNames(QString filename);
    void createFile();

public slots:

private:
    Ui::PKGBUILDASSISTANT *ui;
};
#endif // PKGBUILDASSISTANT_H
