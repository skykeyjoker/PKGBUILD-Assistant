#include "dialogfilecopy.h"

DialogFileCopy::DialogFileCopy(QWidget *parent, int mode) : QDialog(parent), _mode(mode)
{
    setWindowIcon(QIcon(":/ico.png"));

    setFixedSize(QSize(600,200));

    mainLay = new QVBoxLayout(this);

    QHBoxLayout *hlay_source = new QHBoxLayout;
    QHBoxLayout *hlay_destination = new QHBoxLayout;
    QHBoxLayout *hlay_btns = new QHBoxLayout;

    check_binary = new QCheckBox("可执行文件");

    mainLay->addWidget(check_binary);
    mainLay->addLayout(hlay_source);
    mainLay->addLayout(hlay_destination);
    mainLay->addLayout(hlay_btns);

    combox_source = new QComboBox;
    combox_destination = new QComboBox;
    le_source = new QLineEdit;
    le_destination = new QLineEdit;

    QPushButton *btn_cancel = new QPushButton("取消");
    QPushButton *btn_apply = new QPushButton("确定");

    hlay_source->addWidget(new QLabel("源路径:"));
    hlay_source->addWidget(combox_source);
    hlay_source->addWidget(le_source);


    hlay_destination->addWidget(new QLabel("目标路径:"));
    hlay_destination->addWidget(combox_destination);
    hlay_destination->addWidget(le_destination);


    hlay_btns->addStretch();
    hlay_btns->addWidget(btn_cancel);
    hlay_btns->addStretch();
    hlay_btns->addWidget(btn_apply);
    hlay_btns->addStretch();

    combox_source->addItem("${srcdir}");
    combox_source->addItem("${pkgdir}");
    combox_source->setCurrentIndex(0);

    combox_destination->addItem("${srcdir}");
    combox_destination->addItem("${pkgdir}");
    combox_destination->setCurrentIndex(1);

    le_source->setText("/");
    le_destination->setText("/");

    check_binary->setChecked(false);

    if(_mode==0)  //文件
    {
        setWindowTitle("复制文件");

        check_binary->setCheckable(true);

    }

    if(_mode==1) //文件夹
    {
        setWindowTitle("复制文件夹");

        check_binary->setCheckable(false);
    }


    connect(btn_cancel, &QPushButton::clicked, [=](){
        int ret = QMessageBox::question(this, "确认退出", "确认退出文件拷贝吗？");

        if(ret == QMessageBox::Yes)
        {
            close();
        }
    });

    connect(btn_apply, &QPushButton::clicked, [=](){
        if(le_source->text()=="/" || le_destination->text()=="/")
        {
            QMessageBox::critical(this, "错误！", "请填写源路径和目标路径！");

            return;
        }

        if(_mode == 0)
        {

            if(check_binary->isChecked())
            {
                emit signalAddOperation(tr("install -Dm755 %1 %2").arg(combox_source->currentText()+le_source->text()).arg(combox_destination->currentText()+le_destination->text()));
            }
            else
            {
                emit signalAddOperation(tr("install -Dm644 %1 %2").arg(combox_source->currentText()+le_source->text()).arg(combox_destination->currentText()+le_destination->text()));
            }
        }

        if(_mode == 1)
        {

            if(le_source->text().at(le_source->text().length()-1)!='/')
            {
                le_source->setText(le_source->text()+"/");
            }

            emit signalAddOperation(tr("install -Dm644 %1* %2").arg(combox_source->currentText()+le_source->text()).arg(combox_destination->currentText()+le_destination->text()));
        }
    });

}

