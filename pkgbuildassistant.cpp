#include "pkgbuildassistant.h"
#include "ui_pkgbuildassistant.h"

PKGBUILDASSISTANT::PKGBUILDASSISTANT(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PKGBUILDASSISTANT)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/ico.png"));

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


    layout_baseinfoTab->addWidget(new QLabel("PkgName:"),0,0);
    layout_baseinfoTab->addWidget(le_pkgname,0,1);

    layout_baseinfoTab->addWidget(new QLabel("PkgVer:"),1,0);
    layout_baseinfoTab->addWidget(le_pkgver,1,1);

    layout_baseinfoTab->addWidget(new QLabel("PkgRel:"),2,0);
    layout_baseinfoTab->addWidget(le_pkgrel,2,1);

    layout_baseinfoTab->addWidget(new QLabel("PkgDesc:"),3,0);
    layout_baseinfoTab->addWidget(le_pkgdesc,3,1);

    layout_baseinfoTab->addWidget(new QLabel("PkgArch:"),4,0);
    layout_baseinfoTab->addWidget(le_arch,4,1);

    layout_baseinfoTab->addWidget(new QLabel("PkgUrl:"),5,0);
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

    layout_sourceTab->addWidget(new QLabel("Source:"),0,0);
    layout_sourceTab->addWidget(le_source,0,1);

    layout_sourceTab->addWidget(new QLabel("Sums:"),1,0);
    layout_sourceTab->addLayout(hlayout_radios,1,1);
    hlayout_radios->addWidget(radio_md5sums);
    hlayout_radios->addWidget(radio_sha1sums);
    hlayout_radios->addWidget(radio_sha256sums);
    hlayout_radios->addWidget(radio_sha512sums);

    layout_sourceTab->addWidget(new QLabel("Sums:"),2,0);
    layout_sourceTab->addWidget(le_sum,2,1);

    radio_md5sums->setChecked(true);

    le_source->setPlaceholderText("${pkgname}.deb::https://cdn.jin10.com/linuxclient/TraderMaster_20_04.deb");
    le_sum->setPlaceholderText("SKIP\n9846035301fd6fe7298ee26686c74fefe5ebe6fc15b967ab26ee3cd98c0645f4ea3443411eb324f2a0df302e758548a150fb06755e634a631fb5fdf363827d39");

    /* Relations Tab */
    // Init
    layout_relationsTab = new QGridLayout(ui->tab_relations);
    le_maintainerName = new QLineEdit;
    le_maintainerEmail = new QLineEdit;
    le_provides = new QLineEdit;
    le_conflicts = new QLineEdit;

    layout_relationsTab->addWidget(new QLabel("Maintainer Name:"),0,0);
    layout_relationsTab->addWidget(le_maintainerName,0,1);

    layout_relationsTab->addWidget(new QLabel("Maintainer Email:"),1,0);
    layout_relationsTab->addWidget(le_maintainerEmail,1,1);

    layout_relationsTab->addWidget(new QLabel("Provides:"),2,0);
    layout_relationsTab->addWidget(le_provides,2,1);

    layout_relationsTab->addWidget(new QLabel("Conflicts:"),3,0);
    layout_relationsTab->addWidget(le_conflicts,3,1);



    /* Depends Tab */
    // Init
    layout_dependsTab = new QGridLayout(ui->tab_depends);
    le_dependsDir = new QLineEdit;
    table_depends = new QTableWidget;
    le_depends = new QTextEdit;
    le_optdepends = new QTextEdit;

    QPushButton *btn_dependsDir = new QPushButton("Choose");
    QPushButton *btn_scanDepends = new QPushButton("Analyse");

    layout_dependsTab->addWidget(new QLabel("Lib Dir:"),0,0);
    layout_dependsTab->addWidget(le_dependsDir,0,1);
    layout_dependsTab->addWidget(btn_dependsDir,0,2);
    layout_dependsTab->addWidget(btn_scanDepends,0,3);

    layout_dependsTab->addWidget(new QLabel("Libs:"),1,0);
    layout_dependsTab->addWidget(table_depends,1,1,1,3);

    layout_dependsTab->addWidget(new QLabel("Depends:"),2,0);
    layout_dependsTab->addWidget(le_depends,2,1,1,3);

    layout_dependsTab->addWidget(new QLabel("OptDepends:"),3,0);
    layout_dependsTab->addWidget(le_optdepends,3,1,1,3);

    table_depends->setColumnCount(2);
    table_depends->setHorizontalHeaderLabels(QStringList()<<"File Name"<<"Pkg Names");

    table_depends->setColumnWidth(0,140);
    table_depends->setColumnWidth(1,180);

    table_depends->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);

    le_depends->setPlaceholderText("pkg1\npkg2\npkg3");
    le_optdepends->setPlaceholderText("pkg1\npkg2\npkg3\n");

    connect(btn_dependsDir, &QPushButton::clicked, [=](){
        QString dir = QFileDialog::getExistingDirectory(this,"Choose Libs Dir");
        if(!dir.isEmpty())
        {
            le_dependsDir->setText(dir);
            dependsDir = new QDir(dir);

            scanSoFiles();

        }
    });

    connect(btn_scanDepends, &QPushButton::clicked, [=](){
        if(!le_dependsDir->text().isEmpty())
        {
            int ret = QMessageBox::question(this,"Confirming","Quering packages names will start straight away, and it may cost a long time, please be patient.");

            // Query the package name
            if(ret == QMessageBox::Yes)
            {
                /*for(int i=0; i<list_so.size(); i++)
                {
                    QString pkgname = scanPackageNames(list_so.at(i));
                    qDebug()<<pkgname;

                    QTableWidgetItem *itemPackage = new QTableWidgetItem;
                    itemPackage->setText(pkgname);

                    table_depends->setItem(i,1,itemPackage);
                }*/
                QThread *thread_query = new QThread;
                PackageQueryThread *queryThread = new PackageQueryThread;
                queryThread->setFileName(list_so);
                queryThread->moveToThread(thread_query);

                // Start Thread
                connect(thread_query, &QThread::started, queryThread, &PackageQueryThread::startWork);


                // Destroy Thread
                void (PackageQueryThread::*pRet)(QStringList) = &PackageQueryThread::ret;
                void (PKGBUILDASSISTANT::*pInsertPackageNames)(QStringList) = &PKGBUILDASSISTANT::insertPackageNames;
                connect(queryThread, pRet, this, pInsertPackageNames);
                connect(queryThread, &PackageQueryThread::workFinished, queryThread, &PackageQueryThread::deleteLater);
                connect(queryThread, &PackageQueryThread::destroyed, thread_query, &QThread::quit);
                connect(thread_query, &QThread::finished, thread_query, &QThread::deleteLater);

                thread_query->start();
            }
        }
        else
        {
            QMessageBox::critical(this,"Wrong!","Please choose a Libs dir at first!");
        }
    });



    /* Function Tab */
    // Init
    layout_functionTab = new QVBoxLayout(ui->tab_function);
    group_setFakeDir = new QGroupBox("Set Fake Dir");
    group_operation = new QGroupBox("Operations");
    group_trees = new QGroupBox("Tree");
    group_preview = new QGroupBox("Preview");
    le_fakeDirPath = new QLineEdit;
    le_archPath = new QLineEdit;
    tree_source = new QTextBrowser;
    tree_pkg = new QTextBrowser;
    le_preview = new QTextEdit;

    layout_functionTab->addWidget(group_setFakeDir);
    QVBoxLayout *vlay_setFakeDir = new QVBoxLayout(group_setFakeDir);

    QHBoxLayout *hlay_fakeDir = new QHBoxLayout;
    QPushButton *btn_fakedir = new QPushButton("Choose");
    hlay_fakeDir->addWidget(new QLabel("Fake Dir:"));
    hlay_fakeDir->addWidget(le_fakeDirPath);
    hlay_fakeDir->addWidget(btn_fakedir);
    vlay_setFakeDir->addLayout(hlay_fakeDir);

    QHBoxLayout *hlay_debPath = new QHBoxLayout;
    QPushButton *btn_debPath = new QPushButton("Choose");
    hlay_debPath->addWidget(new QLabel("Achive Path:"));
    hlay_debPath->addWidget(le_archPath);
    hlay_debPath->addWidget(btn_debPath);

    vlay_setFakeDir->addLayout(hlay_fakeDir);
    vlay_setFakeDir->addLayout(hlay_debPath);

    layout_functionTab->addWidget(group_operation);
    QHBoxLayout *hlay_operation = new QHBoxLayout(group_operation);

    QPushButton *btn_installFile = new QPushButton("Copy File");
    QPushButton *btn_installDir = new QPushButton("Copy Dir");
    QPushButton *btn_removeFile = new QPushButton("Delete File");
    QPushButton *btn_removeDir = new QPushButton("Delete Dir");
    QPushButton *btn_chmodFile = new QPushButton("Chmod File");
    QPushButton *btn_chmodDir = new QPushButton("Chmod Dir");

    hlay_operation->addStretch();
    hlay_operation->addWidget(btn_installFile);
    hlay_operation->addStretch();
    hlay_operation->addWidget(btn_installDir);
    hlay_operation->addStretch();
    hlay_operation->addWidget(btn_removeFile);
    hlay_operation->addStretch();
    hlay_operation->addWidget(btn_removeDir);
    hlay_operation->addStretch();
    hlay_operation->addWidget(btn_chmodFile);
    hlay_operation->addStretch();
    hlay_operation->addWidget(btn_chmodDir);
    hlay_operation->addStretch();



    layout_functionTab->addWidget(group_trees);
    QHBoxLayout *hlay_trees = new QHBoxLayout(group_trees);

    hlay_trees->addWidget(tree_source);
    hlay_trees->addWidget(tree_pkg);

    layout_functionTab->addWidget(group_preview);
    QVBoxLayout *vlay_preview = new QVBoxLayout(group_preview);

    vlay_preview->addWidget(le_preview);

    layout_functionTab->setStretchFactor(group_setFakeDir,1);
    layout_functionTab->setStretchFactor(group_operation,1);
    layout_functionTab->setStretchFactor(group_trees,2);
    layout_functionTab->setStretchFactor(group_preview,1);

    connect(btn_fakedir, &QPushButton::clicked, [=](){
        QString dir = QFileDialog::getExistingDirectory(this,"Choose Fake Dir");
        if(dir.isEmpty())
        {
            QMessageBox::critical(this,"Wrong!","Please choose a fake dir!");
        }
        else
        {
            le_fakeDirPath->setText(dir);
            updateTree();
        }
    });

    connect(btn_debPath, &QPushButton::clicked, [=](){
        if(le_fakeDirPath->text().isEmpty())
        {
            QMessageBox::critical(this,"Wrong!","Please choose a fake dir!");
        }
        else
        {
            QString fileName = QFileDialog::getOpenFileName(this,"Choose an archive","","deb archive(*.deb);;tar archive(*.tar.gz)");
            if(fileName.isEmpty())
            {
                QMessageBox::critical(this,"Wrong!","Please choose an archive");
            }
            else
            {
                le_archPath->setText(fileName);

                decompress(fileName);

            }
        }
    });

    connect(btn_installFile, &QPushButton::clicked, [=](){
       if(!le_fakeDirPath->text().isEmpty())
       {
           DialogFileCopy *dlg = new DialogFileCopy(this,0);

           void (DialogFileCopy::*pSignalAddOperation)(QString) = &DialogFileCopy::signalAddOperation;
           void (PKGBUILDASSISTANT::*pSlotAddOperation)(QString) = &PKGBUILDASSISTANT::slotAddOperation;
           connect(dlg, pSignalAddOperation, this, pSlotAddOperation);

           dlg->exec();
       }
       else
       {
           QMessageBox::critical(this,"Wrong!","Please choose a fake dir!");
       }


    });

    connect(btn_installDir, &QPushButton::clicked,[=](){
        if(!le_fakeDirPath->text().isEmpty())
        {
            DialogFileCopy *dlg = new DialogFileCopy(this,1);

            void (DialogFileCopy::*pSignalAddOperation)(QString) = &DialogFileCopy::signalAddOperation;
            void (PKGBUILDASSISTANT::*pSlotAddOperation)(QString) = &PKGBUILDASSISTANT::slotAddOperation;
            connect(dlg, pSignalAddOperation, this, pSlotAddOperation);

            dlg->exec();
        }
        else
        {
            QMessageBox::critical(this,"Wrong!","Please choose a fake dir!");
        }

    });

    connect(btn_removeFile, &QPushButton::clicked, [=](){
        if(!le_fakeDirPath->text().isEmpty())
        {
            DialogFileDelete *dlg = new DialogFileDelete(this, 0);
            void (DialogFileDelete::*pSignalAddOperation)(QString) = &DialogFileDelete::signalAddOperation;
            void (PKGBUILDASSISTANT::*pSlotAddOperation)(QString) = &PKGBUILDASSISTANT::slotAddOperation;
            connect(dlg, pSignalAddOperation, this, pSlotAddOperation);

            dlg->exec();
        }
        else
        {
            QMessageBox::critical(this,"Wrong!","Please choose a fake dir!");
        }
    });

    connect(btn_removeDir, &QPushButton::clicked,[=](){
        if(!le_fakeDirPath->text().isEmpty())
        {
            DialogFileDelete *dlg = new DialogFileDelete(this,1);
            void (DialogFileDelete::*pSignalAddOperation)(QString) = &DialogFileDelete::signalAddOperation;
            void (PKGBUILDASSISTANT::*pSlotAddOperation)(QString) = &PKGBUILDASSISTANT::slotAddOperation;
            connect(dlg, pSignalAddOperation, this, pSlotAddOperation);
            dlg->exec();
        }
        else
        {
            QMessageBox::critical(this,"Wrong!","Please choose a fake dir!");
        }
    });

    connect(btn_chmodFile, &QPushButton::clicked, [=](){
        if(!le_fakeDirPath->text().isEmpty())
        {
            DialogFileChmod *dlg = new DialogFileChmod(this,0);
            void (DialogFileChmod::*pSignalAddOperation)(QString) = &DialogFileChmod::signalAddOperation;
            void (PKGBUILDASSISTANT::*pSlotAddOperation)(QString) = &PKGBUILDASSISTANT::slotAddOperation;
            connect(dlg, pSignalAddOperation, this, pSlotAddOperation);

            dlg->exec();
        }
        else
        {
            QMessageBox::critical(this,"Wrong!","Please choose a fake dir!");
        }
    });

    connect(btn_chmodDir, &QPushButton::clicked, [=](){
        if(!le_fakeDirPath->text().isEmpty())
        {
            DialogFileChmod *dlg = new DialogFileChmod(this, 1);
            void (DialogFileChmod::*pSignalAddOperation)(QString) = &DialogFileChmod::signalAddOperation;
            void (PKGBUILDASSISTANT::*pSlotAddOperation)(QString) = &PKGBUILDASSISTANT::slotAddOperation;
            connect(dlg, pSignalAddOperation, this, pSlotAddOperation);

            dlg->exec();
        }
        else
        {
            QMessageBox::critical(this,"Wrong!","Please choose a fake dir!");
        }
    });


    /* Create Tab */
    layout_createTab = new QVBoxLayout(ui->tab_create);
    le_savePath = new QLineEdit;
    le_savePreview = new QTextEdit;
    group_savePreview = new QGroupBox("PKGBUILD Preview");

    QHBoxLayout *hlay_saveFile = new QHBoxLayout;
    QPushButton *btn_saveFile = new QPushButton("Choose");
    hlay_saveFile->addWidget(new QLabel("PKGBUILD Save Dir:"));
    hlay_saveFile->addWidget(le_savePath);
    hlay_saveFile->addWidget(btn_saveFile);
    layout_createTab->addLayout(hlay_saveFile);

    layout_createTab->addWidget(group_savePreview);
    QVBoxLayout *vlay_savePrew = new QVBoxLayout(group_savePreview);

    QHBoxLayout *hlay_savePreviewBtn = new QHBoxLayout;
    QPushButton *btn_prevPKG = new QPushButton("Make PKGBUILD");
    QPushButton *btn_checkPKG = new QPushButton("Check PKGBUILD");
    QPushButton *btn_savePKG = new QPushButton("Save PKGBUILD");
    hlay_savePreviewBtn->addStretch();
    hlay_savePreviewBtn->addWidget(btn_prevPKG);
    hlay_savePreviewBtn->addStretch();
    hlay_savePreviewBtn->addWidget(btn_checkPKG);
    hlay_savePreviewBtn->addStretch();
    hlay_savePreviewBtn->addWidget(btn_savePKG);
    hlay_savePreviewBtn->addStretch();
    vlay_savePrew->addLayout(hlay_savePreviewBtn);


    QHBoxLayout *hlay_savePreviewEdit = new QHBoxLayout;
    hlay_savePreviewEdit->addWidget(le_savePreview);
    vlay_savePrew->addLayout(hlay_savePreviewEdit);

    connect(btn_saveFile, &QPushButton::clicked, [=](){
        QString fileName = QFileDialog::getSaveFileName(this,"Choose a location to save PKGBUILD","","PKGBUILD(PKGBUILD)");
        if(fileName.isEmpty())
        {
            QMessageBox::critical(this,"Wrong!","Please a location to save PKGBUILD!");
        }
        else
        {
            le_savePath->setText(fileName);
        }
    });

    connect(btn_prevPKG, &QPushButton::clicked, this, &PKGBUILDASSISTANT::createFile);

    connect(btn_savePKG, &QPushButton::clicked, [=](){
        if(le_savePath->text().isEmpty())
        {
            QMessageBox::critical(this,"Wrong!","Please a location to save PKGBUILD!");

            return;
        }

        QFile file(le_savePath->text());
        file.open(QFile::ReadWrite | QFile::Truncate);

        file.write(le_savePreview->toPlainText().toUtf8());

        file.close();

        ui->statusbar->showMessage("PKGBUILD saved...",2000);
    });

    connect(btn_checkPKG, &QPushButton::clicked, [=](){
        if(le_savePreview->toPlainText().isEmpty())
        {
            QMessageBox::critical(this,"Wrong!","Please fill out related information and make PKGBUILD！");

            return;
        }

        if(le_savePath->text().isEmpty())
        {
            QMessageBox::critical(this,"Wrong!","Please a location to save PKGBUILD!");

            return;
        }

        QFile file(le_savePath->text());
        file.open(QFile::ReadWrite | QFile::Truncate);

        file.write(le_savePreview->toPlainText().toUtf8());

        file.close();

        ui->statusbar->showMessage("PKGBUILD saved...",500);

        checkFile();

    });



    /* Check Tab */
    layout_checkTab = new QVBoxLayout(ui->tab_check);
    group_checkBtns = new QGroupBox;
    le_checkLog = new QTextEdit;

    layout_checkTab->addWidget(group_checkBtns);
    layout_checkTab->addWidget(le_checkLog);

    QHBoxLayout *hlay_checkBtns = new QHBoxLayout(group_checkBtns);
    QPushButton *btn_checkBuild = new QPushButton("Build");
    QPushButton *btn_checkCheck = new QPushButton("Check");

    hlay_checkBtns->addStretch();
    hlay_checkBtns->addWidget(btn_checkBuild);
    hlay_checkBtns->addStretch();
    hlay_checkBtns->addWidget(btn_checkCheck);
    hlay_checkBtns->addStretch();

    le_checkLog->setReadOnly(true);

    connect(btn_checkBuild, &QPushButton::clicked, [=](){
        if(le_savePath->text().isEmpty())
        {
            QMessageBox::critical(this, "Build Error","Please choose a location to save PKGBUILD at Make TAB at first.");

            return;
        }

        buildFile();
    });

    connect(btn_checkCheck, &QPushButton::clicked, this, &PKGBUILDASSISTANT::checkPkg);

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

    ui->statusbar->showMessage(tr("Scanning finished, There totally are %1 libs").arg(list_so.size()),2000);

}

void PKGBUILDASSISTANT::insertPackageNames(QStringList pkgList)
{
    for(int i=0; i<list_so.size(); i++)
    {

        QTableWidgetItem* pkgNames = new QTableWidgetItem;

        pkgNames->setText(pkgList.at(i));


        table_depends->setItem(i,1,pkgNames);
    }
}

void PKGBUILDASSISTANT::decompress(QString fileName)
{
    QFileInfo info(fileName);
    QDir dir(le_fakeDirPath->text());

    if(!dir.exists("src"))
        dir.mkdir("src");

    if(!dir.exists("pkg"))
        dir.mkdir("pkg");

    if(info.suffix()=="gz")
    {
        qDebug()<<"tar.gz";

        // decompress tar.gz file
        QThread *thread_decompress = new QThread;

        DecompressThread *deTar = new DecompressThread;
        deTar->setShell(QStringList()<<tr("tar -zxvf %1 -C %2/src").arg(le_archPath->text()).arg(le_fakeDirPath->text()));
        deTar->moveToThread(thread_decompress);

        // Start thread
        connect(thread_decompress, &QThread::started, deTar, &DecompressThread::startWork);
        // Destroy thread
        connect(deTar, &DecompressThread::workFinished, this, &PKGBUILDASSISTANT::updateTree);
        connect(deTar, &DecompressThread::workFinished, deTar,&DecompressThread::deleteLater);
        connect(deTar, &DecompressThread::destroyed, thread_decompress, &QThread::quit);
        connect(thread_decompress, &QThread::finished, thread_decompress, &QThread::deleteLater);

        thread_decompress->start();

        updateTree();

        return;
    }

    if(info.suffix()=="deb")
    {
        qDebug()<<"deb";

        //QProcess process;

        // first decompress .deb file        
        QThread *thread_decompress = new QThread;
        DecompressThread *deDeb = new DecompressThread;
        deDeb->setShell(QStringList()<<tr("ar -vx %1 --output=%2/src").arg(le_archPath->text()).arg(le_fakeDirPath->text())
                        <<tr("tar -xvJf %1/src/data.tar.xz -C %2/pkg").arg(le_fakeDirPath->text()).arg(le_fakeDirPath->text()));
        deDeb->moveToThread(thread_decompress);

        // Start thread
        connect(thread_decompress, &QThread::started, deDeb, &DecompressThread::startWork);

        // Destroy thread
        connect(deDeb, &DecompressThread::workFinished, this, &PKGBUILDASSISTANT::updateTree);
        connect(deDeb, &DecompressThread::workFinished, deDeb,&DecompressThread::deleteLater);
        connect(deDeb, &DecompressThread::destroyed, thread_decompress, &QThread::quit);
        connect(thread_decompress, &QThread::finished, thread_decompress, &QThread::deleteLater);

        thread_decompress->start();

        updateTree();  //update file trees

        return;

    }

}

void PKGBUILDASSISTANT::updateTree()
{
    QDir dir_src(le_fakeDirPath->text()+"/src");
    QDir dir_pkg(le_fakeDirPath->text()+"/pkg");

    //qDebug()<<dir_src.entryList();
    // Src Dirs and Files

    QProcess process_src;
    QProcess process_pkg;
    QByteArray buf;

    process_src.start(tr("tree %1").arg(le_fakeDirPath->text()+"/src"));

    process_src.waitForFinished();

    buf = process_src.readAllStandardOutput();

    tree_source->setText(buf);

    process_pkg.start(tr("tree %1").arg(le_fakeDirPath->text()+"/pkg"));

    process_pkg.waitForFinished();

    buf = process_pkg.readAllStandardOutput();

    tree_pkg->setText(buf);
}

void PKGBUILDASSISTANT::slotAddOperation(QString operation)
{
    QString opeShell = operation.section(' ',0,0);
    qDebug()<<"opeShell = " <<opeShell;

    QString realSrcDir = le_fakeDirPath->text()+"/src";
    QString realPkgDir = le_fakeDirPath->text()+"/pkg";



    qDebug()<<"realSrcDir: "<<realSrcDir;
    qDebug()<<"realPkgDir: "<<realPkgDir;

    qDebug()<<"operation: "<<operation;

    // Copy file or dir
    if(opeShell == "install")
    {
        if(!operation.section(' ',2,2).contains("*")) //Copy file
        {
            //install -Dm644 ${srcdir}/.. ${pkgdir}/usr/
            qDebug()<<"copy file";
            QString newOperation = operation;
            le_preview->insertPlainText(newOperation.replace("${srcdir}","\"${srcdir}\"").replace("${pkgdir}","\"${pkgdir}\"")+"\n");

            QString srcStr = operation.section(" ",2,2);
            QString destStr = operation.section(" ",3,3);

            // Firstly build the fake dir
            QProcess mkdir;

            if(destStr.mid(0,9)=="${srcdir}")
                mkdir.start(tr("mkdir -p %1").arg(realSrcDir+destStr.mid(9,destStr.lastIndexOf("/")-9+1)));
            else
                mkdir.start(tr("mkdir -p %1").arg(realPkgDir+destStr.mid(9,destStr.lastIndexOf("/")-9+1)));

            mkdir.waitForFinished();

            // Copy the file to fake dir
            QProcess cp;

            if(srcStr.mid(0,9)=="${srcdir}")  // Source:${srcdir}
            {
                if(destStr.mid(0,9)=="${srcdir}")
                    cp.start(tr("cp %1 %2").arg(realSrcDir+srcStr.mid(9,srcStr.length()-9)).arg(realSrcDir+destStr.mid(9,destStr.length()-9)));
                if(destStr.mid(0,9)=="${pkgdir}")
                    cp.start(tr("cp %1 %2").arg(realSrcDir+srcStr.mid(9,srcStr.length()-9)).arg(realPkgDir+destStr.mid(9,destStr.length()-9)));

                cp.waitForFinished();
                qDebug()<<"copy file: "<<tr("cp %1 %2").arg(realSrcDir+srcStr.mid(9,srcStr.length()-9)).arg(realPkgDir+destStr.mid(9,destStr.length()-9));

            }
            else // Source:${pkgdir}
            {
                if(destStr.mid(0,9)=="${srcdir}")
                    cp.start(tr("cp %1 %2").arg(realPkgDir+srcStr.mid(9,srcStr.length()-9)).arg(realSrcDir+destStr.mid(9,destStr.length()-9)));
                if(destStr.mid(0,9)=="${pkgdir}")
                    cp.start(tr("cp %1 %2").arg(realPkgDir+srcStr.mid(9,srcStr.length()-9)).arg(realPkgDir+destStr.mid(9,destStr.length()-9)));

                cp.waitForFinished();
                qDebug()<<"copy file: "<<tr("cp %1 %2").arg(realPkgDir+srcStr.mid(9,srcStr.length()-9)).arg(realPkgDir+destStr.mid(9,destStr.length()-9));
            }


        }
        else  //Copy dir
        {
            qDebug()<<"copy dir";
            //install -Dm644 ${srcdir}/.. ${pkgdir}/
            QDir tmpDir(le_fakeDirPath->text());
            QString srcStr = operation.section(" ",2,2);
            QString destStr = operation.section(" ",3,3);

            if(destStr.mid(0,9)=="${srcdir}")  // Destination: ${srcdir}
            {
                // if dir exists
                if(tmpDir.exists("src"+destStr.mid(9,destStr.length()-9)))
                {
                    le_preview->insertPlainText(operation.replace("${srcdir}","\"${srcdir}\"").replace("${pkgdir}","\"${pkgdir}\"")+"\n");

                    // copy dir
                    if(srcStr.mid(0,9)=="${srcdir}") // Source: ${srcdir}
                    {
                        qDebug()<<copyDirectoryFiles(realSrcDir+srcStr.mid(9,srcStr.length()-10), realSrcDir+destStr.mid(9,destStr.length()-9));

                        qDebug()<<"copy dir: "<<tr("cp %1* %2").arg(realSrcDir+srcStr.mid(9,srcStr.length()-10)).arg(realSrcDir+destStr.mid(9,destStr.length()-9));
                    }
                    else  // Source: ${pkgdir}
                    {
                        qDebug()<<copyDirectoryFiles(realPkgDir+srcStr.mid(9,srcStr.length()-10),realSrcDir+destStr.mid(9,destStr.length()-9));

                        qDebug()<<"copy dir: "<<tr("cp %1* %2").arg(realPkgDir+srcStr.mid(9,srcStr.length()-10)).arg(realSrcDir+destStr.mid(9,destStr.length()-9));
                    }

                }
                else  // dir doesn't exist
                {
                    // mkdir the dir
                    QProcess mkdir;

                    mkdir.start(tr("mkdir -p %1").arg(realSrcDir+destStr.mid(9,destStr.length()-9)));

                    mkdir.waitForFinished();

                    le_preview->insertPlainText(tr("mkdir -p %1\n").arg("\"${srcdir}\""+destStr.mid(9,destStr.length()-9)));
                    QString newOperation = operation;
                    le_preview->insertPlainText(newOperation.replace("${srcdir}","\"${srcdir}\"").replace("${pkgdir}","\"${pkgdir}\"")+"\n");

                    // copy the dir
                    if(srcStr.mid(0,9)=="${srcdir}")  // Source: ${srcdir}
                    {

                        qDebug()<<copyDirectoryFiles(realSrcDir+srcStr.mid(9,srcStr.length()-10), realSrcDir+destStr.mid(9,destStr.length()-9));


                        qDebug()<<"copy new dir: "<<tr("cp %1* %2").arg(realSrcDir+srcStr.mid(9,srcStr.length()-10)).arg(realSrcDir+destStr.mid(9,destStr.length()-9));

                    }
                    else  // Source: ${pkgdir}
                    {
                        qDebug()<<copyDirectoryFiles(realPkgDir+srcStr.mid(9,srcStr.length()-10), realSrcDir+destStr.mid(9,destStr.length()-9));

                        qDebug()<<"copy new dir: "<<tr("cp %1* %2").arg(realPkgDir+srcStr.mid(9,srcStr.length()-10)).arg(realSrcDir+destStr.mid(9,destStr.length()-9));
                    }
                }
            }
            else  // Destination:${pkgdir}
            {
                if(tmpDir.exists("pkg"+destStr.mid(12,destStr.length()-12)))  // dir exists
                {
                    QString newOperation = operation;
                    le_preview->insertPlainText(newOperation.replace("${srcdir}","\"${srcdir}\"").replace("${pkgdir}","\"${pkgdir}\"")+"\n");

                    // copy the dir
                    if(srcStr.mid(0,9)=="${srcdir}")  // Source: ${srcdir}
                    {
                        qDebug()<<copyDirectoryFiles(realSrcDir+srcStr.mid(9,srcStr.length()-10), realPkgDir+destStr.mid(9,destStr.length()-9));


                        qDebug()<<"copy dir: "<<tr("cp %1* %2").arg(realSrcDir+srcStr.mid(9,srcStr.length()-10)).arg(realPkgDir+destStr.mid(9,destStr.length()-9));
                    }
                    else  // Source: ${pkgdir}
                    {
                        qDebug()<<copyDirectoryFiles(realPkgDir+srcStr.mid(9,srcStr.length()-10),realPkgDir+destStr.mid(9,destStr.length()-9));

                        qDebug()<<"copy dir: "<<tr("cp %1* %2").arg(realPkgDir+srcStr.mid(9,srcStr.length()-10)).arg(realPkgDir+destStr.mid(9,destStr.length()-9));
                    }

                }
                else // dir not exists
                {
                    // mkdir the dir
                    QProcess mkdir;

                    mkdir.start(tr("mkdir -p %1").arg(realPkgDir+destStr.mid(9,destStr.length()-9)));

                    mkdir.waitForFinished();

                    le_preview->insertPlainText(tr("mkdir -p %1\n").arg("\"${pkgdir}\""+destStr.mid(9,destStr.length()-9)));
                    QString newOperation = operation;
                    le_preview->insertPlainText(newOperation.replace("${srcdir}","\"${srcdir}\"").replace("${pkgdir}","\"${pkgdir}\"")+"\n");

                    // copy the dir
                    if(srcStr.mid(0,9)=="${srcdir}")  // Source: ${srcdir}
                    {

                        qDebug()<<copyDirectoryFiles(realSrcDir+srcStr.mid(9,srcStr.length()-10),realPkgDir+destStr.mid(9,destStr.length()-9));

                        qDebug()<<"copy new dir: "<<tr("cp %1* %2").arg(realSrcDir+srcStr.mid(9,srcStr.length()-10)).arg(realPkgDir+destStr.mid(9,destStr.length()-9));
                    }
                    else  // Source: ${pkgdir}
                    {
                        qDebug()<<copyDirectoryFiles(realPkgDir+srcStr.mid(9,srcStr.length()-10),realPkgDir+destStr.mid(9,destStr.length()-9));


                        qDebug()<<"copy new dir: "<<tr("cp %1* %2").arg(realPkgDir+srcStr.mid(9,srcStr.length()-10)).arg(realPkgDir+destStr.mid(9,destStr.length()-9));
                    }
                }
            }
        }

        updateTree();

        return;
    }

    // delete file or dir
    if(opeShell == "rm")
    {
        QString dirStr = operation.section(" ",2,2);

        // delete file
        // rm -f ${pkgdir}/usr/filename
        if(operation.section(" ",1,1) == "-f")
        {
            qDebug()<<"delete file";
            QString newOperation = operation;
            le_preview->insertPlainText(newOperation.replace("${srcdir}","\"${srcdir}\"").replace("${pkgdir}","\"${pkgdir}\"")+"\n");


            if(dirStr.mid(0,9)=="${srcdir}")  // delete file from ${srcdir}
            {
                QDir tmpDir(realSrcDir);

                qDebug()<<tmpDir.remove(dirStr.mid(10, dirStr.length()-10));
            }

            if(dirStr.mid(0,9) == "${pkgdir}")  // delete file from ${srcdir}
            {
                QDir tmpDir(realPkgDir);

                qDebug()<<tmpDir.remove(dirStr.mid(10, dirStr.length()-10));
            }
        }

        // delete dir
        // rm -rf ${pkgdir}/usr/filename
        if(operation.section(" ",1,1) == "-rf")
        {
            qDebug()<<"delete file";
            QString newOperation = operation;
            le_preview->insertPlainText(newOperation.replace("${srcdir}","\"${srcdir}\"").replace("${pkgdir}","\"${pkgdir}\"")+"\n");


            if(dirStr.mid(0,9)=="${srcdir}")  // delete dir from ${srcdir}
            {
                QDir tmpDir(realSrcDir);

                qDebug()<<DeleteDirectory(realSrcDir+"/"+dirStr.mid(10, dirStr.length()-10));
            }

            if(dirStr.mid(0,9) == "${pkgdir}")  // delete dir from ${srcdir}
            {
                QDir tmpDir(realPkgDir);

                qDebug()<<DeleteDirectory(realPkgDir+"/"+dirStr.mid(10, dirStr.length()-10));
            }
        }

        updateTree();

        return;
    }

    // chmod file or dir
    if(opeShell == "chmod")
    {
        qDebug()<<"chmod";
        QString newOperation = operation;
        le_preview->insertPlainText(newOperation.replace("${srcdir}","\"${srcdir}\"").replace("${pkgdir}","\"${pkgdir}\"")+"\n");

        if(operation.section(" ",1,1) != "-R")  // chmod file
        {
            // chmod 0755 ${pkgdir}/usr/name

            qDebug()<<"chmod file";
            QString mod = operation.section(" ",1,1);
            QString dirStr = operation.section(" ",2,2);

            QProcess chmod;

            if(dirStr.mid(0,9)=="${srcdir}")  // chmod file from ${srcdir}
            {
                chmod.start(tr("chmod %1 %2").arg(mod).arg(realSrcDir+"/"+dirStr.mid(10, dirStr.length()-10)));

                chmod.waitForFinished();
            }

            if(dirStr.mid(0,9)=="${pkgdir}")  // chmod file from ${pkgdir}
            {
                chmod.start(tr("chmod %1 %2").arg(mod).arg(realPkgDir+"/"+dirStr.mid(10, dirStr.length()-10)));

                chmod.waitForFinished();
            }
        }
        else  //chmod dir
        {
            // chmod -R 0755 ${pkgdir}/usr/name
            QString mod = operation.section(" ",2,2);
            QString dirStr = operation.section(" ",3,3);

            QProcess chmod;

            if(dirStr.mid(0,9)=="${srcdir}")  // chmod file from ${srcdir}
            {
                chmod.start(tr("chmod -R %1 %2").arg(mod).arg(realSrcDir+"/"+dirStr.mid(10, dirStr.length()-10)));

                chmod.waitForFinished();
            }

            if(dirStr.mid(0,9)=="${pkgdir}")  // chmod file from ${pkgdir}
            {
                chmod.start(tr("chmod -R %1 %2").arg(mod).arg(realPkgDir+"/"+dirStr.mid(10, dirStr.length()-10)));

                chmod.waitForFinished();
            }
        }


        updateTree();

        return;
    }


}

bool PKGBUILDASSISTANT::copyDirectoryFiles(const QString &fromDir, const QString &toDir)
{
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);
    if(!targetDir.exists())
    {    /**< 如果目标目录不存在，则进行创建 */
        if(!targetDir.mkdir(targetDir.absolutePath()))
            return false;
    }

    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        if(fileInfo.isDir())
        {    /**< 当为目录时，递归的进行copy */
            if(!copyDirectoryFiles(fileInfo.filePath(),targetDir.filePath(fileInfo.fileName())))
                return false;
        }
        else
        {            /**< 当允许覆盖操作时，将旧文件进行删除操作 */
            if(targetDir.exists(fileInfo.fileName()))
            {
                targetDir.remove(fileInfo.fileName());
            }

            /// 进行文件copy
            if(!QFile::copy(fileInfo.filePath(),
                targetDir.filePath(fileInfo.fileName()))){
                    return false;
            }
        }
    }
    return true;

}

bool PKGBUILDASSISTANT::DeleteDirectory(const QString &path)
{
    if (path.isEmpty())
    {
        return false;
    }

    QDir dir(path);
    if(!dir.exists())
    {
        return true;
    }

    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();
    foreach (QFileInfo fi, fileList)
    {
        if (fi.isFile())
        {
            fi.dir().remove(fi.fileName());
        }
        else
        {
            DeleteDirectory(fi.absoluteFilePath());
        }
    }
    return dir.rmpath(dir.absolutePath());
}

void PKGBUILDASSISTANT::createFile()
{
    if(le_pkgname->text().isEmpty() || le_pkgver->text().isEmpty() || le_pkgrel->text().isEmpty()
            || le_pkgdesc->toPlainText().isEmpty() || le_arch->text().isEmpty() || le_url->text().isEmpty()
            || le_source->toPlainText().isEmpty() || le_sum->toPlainText().isEmpty() || le_maintainerName->text().isEmpty()
            || le_maintainerEmail->text().isEmpty() || le_provides->text().isEmpty())
    {
        QMessageBox::critical(this, "Make Error", "Can't make PKGBUILD, as missing some necessary information!");
        return;
    }

    pkgBuf.clear();

    // Maintainer Info
    pkgBuf.append(tr("#Maintainer: %1 <%2>\n\n").arg(le_maintainerName->text()).arg(le_maintainerEmail->text()));

    // Pkgname
    pkgBuf.append(tr("pkgname=(\'%1\')\n").arg(le_pkgname->text()));

    // Pkgver
    pkgBuf.append(tr("pkgver=%1\n").arg(le_pkgver->text()));

    // Pkgrel
    pkgBuf.append(tr("pkgrel=%1\n").arg(le_pkgrel->text()));

    // Pkgdec
    pkgBuf.append(tr("pkgdesc=\"%1\"\n").arg(le_pkgdesc->toPlainText()));

    // Provides
    pkgBuf.append(tr("provides=(\"%1\")\n").arg(le_provides->text()));

    // Conflicts
    if(!le_conflicts->text().isEmpty())
        pkgBuf.append(tr("conflicts=(\"%1\")\n").arg(le_conflicts->text()));

    // Url
    pkgBuf.append(tr("url=\'%1\'\n").arg(le_url->text()));

    // Arch
    pkgBuf.append(tr("arch=(\'%1\')\n").arg(le_arch->text()));

    // Depends
    int count = 0;
    QString str = le_depends->toPlainText();
    qDebug()<<str;
    QString depensStr;

    count = str.count("\n")+1;
    qDebug()<<count;
    for(int i=0; i<count; ++i)
    {
        depensStr += tr("\'%1\' ").arg(str.section("\n",i,i).trimmed());
    }
    depensStr = depensStr.trimmed();  // trimmed

    pkgBuf.append(tr("depends=(%1)\n").arg(depensStr));

    // Optdepends
    if(!le_optdepends->toPlainText().isEmpty())
    {
        count = 0;
        str = le_optdepends->toPlainText();
        QString optdependsStr;

        count = str.count("\n")+1;
        for(int i=0; i<count; ++i)
        {
            optdependsStr += tr("\'%1\' ").arg(str.section("\n",i,i).trimmed());
        }
        optdependsStr = optdependsStr.trimmed();  // trimmed

        pkgBuf.append(tr("optdepends=(%1)\n").arg(optdependsStr));
    }

    // Source
    count = 0;
    str = le_source->toPlainText();
    QString sourceStr;

    count = str.count("\n")+1;
    for(int i=0; i<count; ++i)
    {
        sourceStr += tr("\"%1\"\n").arg(str.section("\n",i,i).trimmed());
    }
    pkgBuf.append(tr("source=(%1)\n").arg(sourceStr));

    // Sums
    count = 0;
    str = le_sum->toPlainText();
    QString sumStr;

    count = str.count("\n")+1;
    for(int i = 0; i<count; ++i)
    {
        if(str.section("\n",i,i).trimmed() == "SKIP")
        {
            sumStr += "SKIP\n";
        }
        else
        {
            sumStr += tr("\'%1\'\n").arg(str.section("\n",i,i).trimmed());
        }
    }

    QString sumType;

    if(radio_md5sums->isChecked())
        sumType = "md5sums";
    if(radio_sha1sums->isChecked())
        sumType = "sha1sums";
    if(radio_sha256sums->isChecked())
        sumType = "sha256sums";
    if(radio_sha512sums->isChecked())
        sumType = "sha512sums";

    pkgBuf.append(tr("%1=(%2)\n\n").arg(sumType).arg(sumStr));

    // Package()
    //head
    pkgBuf.append("package(){\n");

    if(!le_archPath->text().isEmpty())  // deb archive
        if(le_archPath->text().mid(le_archPath->text().lastIndexOf("."),-1) == ".deb")
        {
            pkgBuf.append("\t cd ${srcdir}\n");
            pkgBuf.append("\t tar -xJvf data.tar.xz -C \"${pkgdir}\"\n\n");
        }

    // body
    count = 0;
    QString packageStr = le_preview->toPlainText();

    count = packageStr.count("\n")+1;
    for(int i=0; i<count; ++i)
    {
        pkgBuf.append(tr("\t %1\n").arg(packageStr.section("\n",i,i)));
    }

    //end
    pkgBuf.append("}");

    le_savePreview->setText(pkgBuf.data());

}

void PKGBUILDASSISTANT::checkFile()
{
    ui->statusbar->showMessage("Checking PKGBUILD...");

    /* /usr/bin/ppp */
    QString currentDirName = le_savePath->text().mid(0, le_savePath->text().lastIndexOf('/'));
    QDir currentDir(currentDirName);
    qDebug()<<"currentDirName:"<<currentDirName;

    if(!currentDir.exists("PKGBUILD"))
    {
        QMessageBox::critical(this,"Check Error","Can't find PKGBUILD!");
        return;
    }

    // Run namcap
    QProcess namcap;
    namcap.setWorkingDirectory(currentDirName);
    namcap.start("namcap -m PKGBUILD");

    namcap.waitForFinished();

    qDebug()<<"namcap";
    QByteArray retBuf = namcap.readAll();
    QString ret(retBuf);
    qDebug()<<retBuf.data();

    // Loop Output
    int errorNum = 0; //count errors
    int warningNum = 0; //count warnings

    int countReturn = ret.count('\n');
    qDebug()<<"countReturn:"<<countReturn;
    for(int i=0; i<countReturn; ++i)
    {
        QString currentStr = ret.section("\n",i,i);
        qDebug()<<"currentStr:"<<currentStr;
        QString type = currentStr.section(" ",2,2);
        qDebug()<<"type:"<<type;
        if(type=="E:")
        {
            errorNum++;

            continue;
        }
        if(type=="W:")
        {
            warningNum++;

            continue;
        }
    }


    ui->statusbar->showMessage(tr("PKGBUILD checked. There are %1 ERRORS and %2 WARNINGS.").arg(errorNum).arg(warningNum));

    QMessageBox::information(this, "PKGBUID Checked", tr("PKGBUILD checked.\nThere are %1 ERRORS and %2 WARNINGS:\n%3").arg(errorNum).arg(warningNum).arg(ret));

}

void PKGBUILDASSISTANT::readStandardOutput()
{
    QString outStr = QString::fromLocal8Bit(makepkg->readAllStandardOutput());
    qDebug()<<"output: "<<outStr;
}

void PKGBUILDASSISTANT::readStandardError()
{
    QString outStr = QString::fromLocal8Bit(makepkg->readAllStandardError());
    erroList<<outStr;
    qDebug()<<"error: "<<outStr;
}

void PKGBUILDASSISTANT::buildFile()
{
    le_checkLog->append("Start makepkg...");

    QString currentDirName = le_savePath->text().mid(0, le_savePath->text().lastIndexOf('/'));
    QDir currentDir(currentDirName);
    qDebug()<<"currentDirName:"<<currentDirName;

    if(!currentDir.exists("PKGBUILD"))
    {
        QMessageBox::critical(this,"Build Error","Can't find PKGBUILD!");

        le_checkLog->append("Build error, can't find PKGBUILD!");

        le_checkLog->append("Stop build.");

        isBuildSuccess = false;

        return;
    }

    if(currentDir.exists(tr("%1-%2-%3-%4.pkg.tar.xz").arg(le_pkgname->text().trimmed()).arg(le_pkgver->text().trimmed()).arg(le_pkgrel->text().trimmed()).arg(le_arch->text().trimmed())))
    {
        currentDir.remove(tr("%1-%2-%3-%4.pkg.tar.xz").arg(le_pkgname->text().trimmed()).arg(le_pkgver->text().trimmed()).arg(le_pkgrel->text().trimmed()).arg(le_arch->text().trimmed()));
    }

    ui->statusbar->showMessage("Start build PKGBUILD...");

    makepkg = new QProcess(this);

    makepkg->setWorkingDirectory(currentDirName);


    connect(makepkg, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),[=](int exitCode, QProcess::ExitStatus exitStatus)
    {
        Q_UNUSED(exitCode);
        Q_UNUSED(exitStatus);
        qDebug()<<"finished";

        qDebug()<<tr("%1-%2-%3-%4.pkg.tar.xz").arg(le_pkgname->text().trimmed()).arg(le_pkgver->text().trimmed()).arg(le_pkgrel->text().trimmed()).arg(le_arch->text().trimmed());
        if(currentDir.exists(tr("%1-%2-%3-%4.pkg.tar.xz").arg(le_pkgname->text().trimmed()).arg(le_pkgver->text().trimmed()).arg(le_pkgrel->text().trimmed()).arg(le_arch->text().trimmed())))
        {
            le_checkLog->append("Build Success!");

            ui->statusbar->showMessage("Build PKGBUILD success",1000);

            isBuildSuccess = true;

        }
        else
        {
            le_checkLog->append("Build Error!");
            le_checkLog->append(tr("Last Error:\n%1%2").arg(erroList.at(erroList.size()-1)).arg(erroList.at(erroList.size()-2)));

            ui->statusbar->showMessage("Build Error",1000);

            isBuildSuccess = false;

        }
        //qDebug()<<erroList;
    });
    connect(makepkg,&QProcess::readyReadStandardOutput,this, &PKGBUILDASSISTANT::readStandardOutput);
    connect(makepkg,&QProcess::readyReadStandardError, this, &PKGBUILDASSISTANT::readStandardError);

    makepkg->start("makepkg -f");

    //makepkg->waitForFinished(60000);

    //qDebug()<<"error:"<<makepkg->readAllStandardError().data();
    //qDebug()<<"output:"<<makepkg->readAllStandardOutput().data();

}

void PKGBUILDASSISTANT::checkPkg()
{
    le_checkLog->append("Start build check..");

    if(!isBuildSuccess)
    {
        QMessageBox::critical(this, "Error", "Please build PKGBUILD at first.");

        return;
    }

    ui->statusbar->showMessage("Start check build...");

    QString currentDirName = le_savePath->text().mid(0, le_savePath->text().lastIndexOf('/'));
    QDir currentDir(currentDirName);
    qDebug()<<"currentDirName:"<<currentDirName;

    // Run namcap
    QProcess namcap;
    namcap.setWorkingDirectory(currentDirName);
    namcap.start(tr("namcap -m %1-%2-%3-%4.pkg.tar.xz").arg(le_pkgname->text().trimmed()).arg(le_pkgver->text().trimmed()).arg(le_pkgrel->text().trimmed()).arg(le_arch->text().trimmed()));

    namcap.waitForFinished();

    qDebug()<<"namcap";
    QByteArray retBuf = namcap.readAll();
    QString ret(retBuf);
    qDebug()<<retBuf.data();

    ui->statusbar->showMessage("Build checked finished.",2000);

    le_checkLog->append("Build checked finished.\n");

    le_checkLog->append(ret);

    // Loop Output
    QStringList satisfiedDependcyList;

    int errorNum = 0; //count errors
    int warningNum = 0; //count warnings

    int countReturn = ret.count('\n');
    qDebug()<<"countReturn:"<<countReturn;
    for(int i=0; i<countReturn; ++i)
    {
        QString currentStr = ret.section("\n",i,i);
        qDebug()<<"currentStr:"<<currentStr;
        QString type = currentStr.section(" ",1,1);
        qDebug()<<"type:"<<type;
        if(type=="E:")
        {
            errorNum++;
        }
        if(type=="W:")
        {
            warningNum++;
        }

        qDebug()<<currentStr.section(" ",2,2);
        if(currentStr.section(" ",2,2)=="dependency-already-satisfied")
        {
            satisfiedDependcyList<<currentStr.section(" ",3,3).trimmed();
        }
    }

    qDebug()<<satisfiedDependcyList.size();
    // Remove Satisfied Depends
    if(!satisfiedDependcyList.isEmpty())
    {
        QString satisfiedDepencyStr;
        for(int i=0; i<satisfiedDependcyList.size(); ++i)
        {
            satisfiedDepencyStr+=satisfiedDependcyList.at(i)+"\n";
        }

        int ret = QMessageBox::question(this,"Confirming",tr("There are %1 packages that are already satisfied, do you want to remove them from depends?\nThese packages:%2").arg(satisfiedDependcyList.size()).arg(satisfiedDepencyStr.trimmed()));

        if(ret == QMessageBox::Yes)
        {
            QString oldDependsStr = le_depends->toPlainText();

            for(int i=0; i<satisfiedDependcyList.size(); ++i)
            {
                oldDependsStr.remove(satisfiedDependcyList.at(i));
            }

            QString newDependsStr;

            int returnCount = 0;
            returnCount = oldDependsStr.count("\n");
            qDebug()<<returnCount;
            for(int i=0; i<returnCount; ++i)
            {
                QString currentStr = oldDependsStr.section("\n",i,i);
                qDebug()<<currentStr;
                if(currentStr=="")
                {
                    continue;
                }
                else
                {
                    newDependsStr+=currentStr+"\n";
                }
            }

            newDependsStr = newDependsStr.trimmed();

            qDebug()<<oldDependsStr;
            qDebug()<<newDependsStr;

            le_depends->setText(newDependsStr);
        }
    }

    ui->statusbar->showMessage(tr("Build checked. There are %1 ERRORS and %2 WARNINGS.").arg(errorNum).arg(warningNum));
}
