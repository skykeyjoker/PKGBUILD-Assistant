#include "pkgbuildassistant.h"
#include "ui_pkgbuildassistant.h"

PKGBUILDASSISTANT::PKGBUILDASSISTANT(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PKGBUILDASSISTANT)
{
    ui->setupUi(this);

    ui->tabWidget->setCurrentIndex(0);

    /* BaseInfo Tab */
    // Init
    layout_baseinfoTab = new QGridLayout(ui->tab_baseinfo);

    le_pkgname = new QLineEdit;
    le_pkgver = new QLineEdit;
    le_pkgrel = new QLineEdit;
    le_pkgdesc = new QTextEdit;
    le_arch = new QLineEdit;
    le_url = new QLineEdit;


    layout_baseinfoTab->addWidget(new QLabel("包名:"),0,0);
    layout_baseinfoTab->addWidget(le_pkgname,0,1);

    layout_baseinfoTab->addWidget(new QLabel("版本号:"),1,0);
    layout_baseinfoTab->addWidget(le_pkgver,1,1);

    layout_baseinfoTab->addWidget(new QLabel("发布号:"),2,0);
    layout_baseinfoTab->addWidget(le_pkgrel,2,1);

    layout_baseinfoTab->addWidget(new QLabel("包描述:"),3,0);
    layout_baseinfoTab->addWidget(le_pkgdesc,3,1);

    layout_baseinfoTab->addWidget(new QLabel("架构:"),4,0);
    layout_baseinfoTab->addWidget(le_arch,4,1);

    layout_baseinfoTab->addWidget(new QLabel("网址:"),5,0);
    layout_baseinfoTab->addWidget(le_url,5,1);


    /* Source Tab */
    // Init
    layout_sourceTab = new QGridLayout(ui->tab_source);
    le_source = new QTextEdit;
    hlayout_radios = new QHBoxLayout;
    radio_md5sums = new QRadioButton("md5sums");
    radio_sha1sums = new QRadioButton("sha1sums");
    radio_sha256sums = new QRadioButton("sha256sums");
    radio_sha512sums = new QRadioButton("sha512sums");
    le_sum = new QTextEdit;

    layout_sourceTab->addWidget(new QLabel("源链接:"),0,0);
    layout_sourceTab->addWidget(le_source,0,1);

    layout_sourceTab->addWidget(new QLabel("校验方式:"),1,0);
    layout_sourceTab->addLayout(hlayout_radios,1,1);
    hlayout_radios->addWidget(radio_md5sums);
    hlayout_radios->addWidget(radio_sha1sums);
    hlayout_radios->addWidget(radio_sha256sums);
    hlayout_radios->addWidget(radio_sha512sums);

    layout_sourceTab->addWidget(new QLabel("校验和:"),2,0);
    layout_sourceTab->addWidget(le_sum,2,1);

    radio_md5sums->setChecked(true);

    le_source->setPlaceholderText("${pkgname}.deb::https://cdn.jin10.com/linuxclient/TraderMaster_20_04.deb");
    le_sum->setPlaceholderText("SKIP\n9846035301fd6fe7298ee26686c74fefe5ebe6fc15b967ab26ee3cd98c0645f4ea3443411eb324f2a0df302e758548a150fb06755e634a631fb5fdf363827d39");

    /* Relations Tab */
    // Init
    layout_relationsTab = new QGridLayout(ui->tab_relations);
    le_provides = new QLineEdit;
    le_conflicts = new QLineEdit;

    layout_relationsTab->addWidget(new QLabel("提供功能:"),0,0);
    layout_relationsTab->addWidget(le_provides,0,1);

    layout_relationsTab->addWidget(new QLabel("冲突:"),1,0);
    layout_relationsTab->addWidget(le_conflicts,1,1);


    /* Depends Tab */
    // Init
    layout_dependsTab = new QGridLayout(ui->tab_depends);
    le_dependsDir = new QLineEdit;
    table_depends = new QTableWidget;
    le_depends = new QTextEdit;
    le_optdepends = new QTextEdit;

    QPushButton *btn_dependsDir = new QPushButton("选择库目录");
    QPushButton *btn_scanDepends = new QPushButton("分析库依赖");

    layout_dependsTab->addWidget(new QLabel("链接库目录:"),0,0);
    layout_dependsTab->addWidget(le_dependsDir,0,1);
    layout_dependsTab->addWidget(btn_dependsDir,0,2);
    layout_dependsTab->addWidget(btn_scanDepends,0,3);

    layout_dependsTab->addWidget(new QLabel("库文件:"),1,0);
    layout_dependsTab->addWidget(table_depends,1,1,1,3);

    layout_dependsTab->addWidget(new QLabel("依赖:"),2,0);
    layout_dependsTab->addWidget(le_depends,2,1,1,3);

    layout_dependsTab->addWidget(new QLabel("可选依赖:"),3,0);
    layout_dependsTab->addWidget(le_optdepends,3,1,1,3);

    table_depends->setColumnCount(2);
    table_depends->setHorizontalHeaderLabels(QStringList()<<"文件名"<<"依赖");

    table_depends->setColumnWidth(0,140);
    table_depends->setColumnWidth(1,180);

    table_depends->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);

    le_depends->setPlaceholderText("pkg1\npkg2\npkg3");
    le_optdepends->setPlaceholderText("pkg1\npkg2\npkg3\n");

    connect(btn_dependsDir, &QPushButton::clicked, [=](){
        QString dir = QFileDialog::getExistingDirectory(this,"选择包库目录");
        if(!dir.isEmpty())
        {
            le_dependsDir->setText(dir);
            dependsDir = new QDir(dir);
        }
    });

    connect(btn_scanDepends, &QPushButton::clicked, [=](){
        if(!le_dependsDir->text().isEmpty())
        {
            scanSoFiles();
        }
        else
        {
            QMessageBox::critical(this,"错误！","请选择库文件目录后再进行选择!");
        }
    });



    /* Function Tab */
    // Init
    layout_functionTab = new QVBoxLayout(ui->tab_function);
    group_setFakeDir = new QGroupBox("设置模拟目录");
    group_operation = new QGroupBox("操作");
    group_trees = new QGroupBox("文件树");
    group_preview = new QGroupBox("命令预览");
    le_fakeDirPath = new QLineEdit;
    le_debPath = new QLineEdit;
    tree_source = new QTreeWidget;
    tree_pkg = new QTreeWidget;
    le_preview = new QTextEdit;

    layout_functionTab->addWidget(group_setFakeDir);
    QVBoxLayout *vlay_setFakeDir = new QVBoxLayout(group_setFakeDir);

    QHBoxLayout *hlay_fakeDir = new QHBoxLayout;
    QPushButton *btn_fakedir = new QPushButton("选择目录");
    hlay_fakeDir->addWidget(new QLabel("模拟目录:"));
    hlay_fakeDir->addWidget(le_fakeDirPath);
    hlay_fakeDir->addWidget(btn_fakedir);
    vlay_setFakeDir->addLayout(hlay_fakeDir);

    QHBoxLayout *hlay_debPath = new QHBoxLayout;
    QPushButton *btn_debPath = new QPushButton("选择Deb包");
    hlay_debPath->addWidget(new QLabel("Deb包目录:"));
    hlay_debPath->addWidget(le_debPath);
    hlay_debPath->addWidget(btn_debPath);

    vlay_setFakeDir->addLayout(hlay_fakeDir);
    vlay_setFakeDir->addLayout(hlay_debPath);

    layout_functionTab->addWidget(group_operation);
    QHBoxLayout *hlay_operation = new QHBoxLayout(group_operation);

    QPushButton *btn_installFile = new QPushButton("拷贝文件");
    QPushButton *btn_installDir = new QPushButton("拷贝文件夹");
    QPushButton *btn_removeFile = new QPushButton("删除文件");
    QPushButton *btn_removeDir = new QPushButton("删除文件夹");

    hlay_operation->addStretch();
    hlay_operation->addWidget(btn_installFile);
    hlay_operation->addStretch();
    hlay_operation->addWidget(btn_installDir);
    hlay_operation->addStretch();
    hlay_operation->addWidget(btn_removeFile);
    hlay_operation->addStretch();
    hlay_operation->addWidget(btn_removeDir);
    hlay_operation->addStretch();

    layout_functionTab->addWidget(group_trees);
    QHBoxLayout *hlay_trees = new QHBoxLayout(group_trees);

    hlay_trees->addWidget(tree_source);
    hlay_trees->addWidget(tree_pkg);

    layout_functionTab->addWidget(group_preview);
    QVBoxLayout *vlay_preview = new QVBoxLayout(group_preview);

    vlay_preview->addWidget(le_preview);

    connect(btn_fakedir, &QPushButton::clicked, [=](){
        QString dir = QFileDialog::getExistingDirectory(this,"选择模拟文件夹");
        if(dir.isEmpty())
        {
            QMessageBox::critical(this,"错误","请选择模拟文件夹");
        }
        else
        {
            le_fakeDirPath->setText(dir);
        }
    });

    connect(btn_debPath, &QPushButton::clicked, [=](){
        if(le_fakeDirPath->text().isEmpty())
        {
            QMessageBox::critical(this,"错误","请先选择模拟文件夹");
        }
        else
        {
            QString fileName = QFileDialog::getOpenFileName(this,"选择Deb安装包","","Deb File (*.deb)");
            if(fileName.isEmpty())
            {
                QMessageBox::critical(this,"错误","请选择deb安装包");
            }
            else
            {
                le_debPath->setText(fileName);
            }
        }
    });


    /* Create Tab */
    layout_createTab = new QVBoxLayout(ui->tab_create);
    le_savePath = new QLineEdit;
    le_savePreview = new QTextEdit;
    group_savePreview = new QGroupBox("PKGBUILD预览");

    QHBoxLayout *hlay_saveFile = new QHBoxLayout;
    QPushButton *btn_saveFile = new QPushButton("选择路径");
    hlay_saveFile->addWidget(new QLabel("保存文件路径:"));
    hlay_saveFile->addWidget(le_savePath);
    hlay_saveFile->addWidget(btn_saveFile);
    layout_createTab->addLayout(hlay_saveFile);

    layout_createTab->addWidget(group_savePreview);
    QVBoxLayout *vlay_savePrew = new QVBoxLayout(group_savePreview);

    QHBoxLayout *hlay_savePreviewBtn = new QHBoxLayout;
    QPushButton *btn_openPKG = new QPushButton("打开PKGBUILD");
    QPushButton *btn_savePKG = new QPushButton("保存PKGBUILD");
    hlay_savePreviewBtn->addStretch();
    hlay_savePreviewBtn->addWidget(btn_openPKG);
    hlay_savePreviewBtn->addStretch();
    hlay_savePreviewBtn->addWidget(btn_savePKG);
    hlay_savePreviewBtn->addStretch();
    vlay_savePrew->addLayout(hlay_savePreviewBtn);


    QHBoxLayout *hlay_savePreviewEdit = new QHBoxLayout;
    hlay_savePreviewEdit->addWidget(le_savePreview);
    vlay_savePrew->addLayout(hlay_savePreviewEdit);

    connect(btn_saveFile, &QPushButton::clicked, [=](){
        QString fileName = QFileDialog::getSaveFileName(this,"选择PKGBUILD保存位置");
        if(fileName.isEmpty())
        {
            QMessageBox::critical(this,"错误!","请选择PKGBUILD文件保存位置!");
        }
        else
        {
            le_savePath->setText(fileName);
        }
    });

}

PKGBUILDASSISTANT::~PKGBUILDASSISTANT()
{
    delete ui;
}

void PKGBUILDASSISTANT::scanSoFiles()
{
    // Set filter
    dependsDir->setFilter(QDir::Files);
    dependsDir->setNameFilters(QStringList()<<"*.so"<<"*.so.*");

    // Get filename list
    list_so = dependsDir->entryList();
    qDebug()<<list_so;
    qDebug()<<list_so.size();

    // Insert Item
    for(int i=0; i<list_so.size(); i++)
    {

        QTableWidgetItem* itemName = new QTableWidgetItem;

        itemName->setText(list_so.at(i));

        table_depends->setRowCount(table_depends->rowCount()+1);
        table_depends->insertRow(-1);

        table_depends->setItem(table_depends->rowCount()-1,0,itemName);
    }


    // Query the package name
    for(int i=0; i<list_so.size(); i++)
    {
        QString pkgname = scanPackageNames(list_so.at(i));
        qDebug()<<pkgname;

        QTableWidgetItem *itemPackage = new QTableWidgetItem;
        itemPackage->setText(pkgname);

        table_depends->setItem(i,1,itemPackage);
    }




}

QString PKGBUILDASSISTANT::scanPackageNames(QString filename)
{
    QProcess process;

    process.start(tr("pacman -Fx %1").arg(filename));

    process.waitForFinished();

    QByteArray buf = process.readAllStandardOutput();

    QString ret(buf);

    return buf;
}


