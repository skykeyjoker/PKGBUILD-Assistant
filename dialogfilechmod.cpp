#include "dialogfilechmod.h"

DialogFileChmod::DialogFileChmod(QWidget *parent, int mode) : QDialog(parent), _mode(mode)
{
    setWindowIcon(QIcon(":/ico.png"));

    setFixedSize(QSize(600,150));

    mainLay = new QVBoxLayout(this);
    le_fileName = new QLineEdit;
    le_mod = new QLineEdit;
    combox = new QComboBox;
    check_allFiles = new QCheckBox("Include All Subfolders and Files");

    QHBoxLayout *lay_edit = new QHBoxLayout;
    QHBoxLayout *lay_mod = new QHBoxLayout;
    QHBoxLayout *lay_btns = new QHBoxLayout;

    mainLay->addWidget(check_allFiles);
    mainLay->addLayout(lay_edit);
    mainLay->addLayout(lay_mod);
    mainLay->addLayout(lay_btns);

    check_allFiles->setChecked(true);

    //file
    if(_mode == 0)
    {
        setWindowTitle("Chmod File");
        check_allFiles->setVisible(false);
    }

    //dir
    if(_mode == 1)
    {
        setWindowTitle("Chmod Dir");
        check_allFiles->setVisible(true);
    }


    lay_edit->addWidget(new QLabel("Path:"));
    lay_edit->addWidget(combox);
    lay_edit->addWidget(le_fileName);

    lay_mod->addWidget(new QLabel("Authorization:"));
    lay_mod->addWidget(le_mod);
    lay_mod->addStretch(3);

    combox->addItem("${srcdir}");
    combox->addItem("${pkgdir}");
    combox->setCurrentIndex(1);

    QPushButton *btn_cancel = new QPushButton("Cancel");
    QPushButton *btn_apply = new QPushButton("Apply");

    lay_btns->addStretch();
    lay_btns->addWidget(btn_cancel);
    lay_btns->addStretch();
    lay_btns->addWidget(btn_apply);
    lay_btns->addStretch();

    le_fileName->setText("/");


    connect(btn_cancel, &QPushButton::clicked, [=](){
        int ret = QMessageBox::question(this, "Confirming", "Are you sure to exit?");

        if(ret == QMessageBox::Yes)
        {
            close();
        }
    });

    connect(btn_apply, &QPushButton::clicked, [=](){
        if(le_fileName->text()=="/" || le_mod->text().isEmpty())
        {
            QMessageBox::critical(this, "Error", "Please input the path and authorization of the target file!");

            return;
        }

        if(_mode == 0)
        {
            emit signalAddOperation(tr("chmod 0%1 %2").arg(le_mod->text()).arg(combox->currentText()+le_fileName->text()));
        }

        if(_mode == 1)
        {
            if(check_allFiles->isChecked())
                emit signalAddOperation(tr("chmod -R 0%1 %2").arg(le_mod->text()).arg(combox->currentText()+le_fileName->text()));
            else
                emit signalAddOperation(tr("chmod 0%1 %2").arg(le_mod->text()).arg(combox->currentText()+le_fileName->text()));
        }
    });


}
