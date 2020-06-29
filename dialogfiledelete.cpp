#include "dialogfiledelete.h"

DialogFileDelete::DialogFileDelete(QWidget *parent, int mode) : QDialog(parent), _mode(mode)
{
    setWindowIcon(QIcon(":/ico.png"));

    setFixedSize(QSize(600,100));

    mainLay = new QVBoxLayout(this);
    le_fileName = new QLineEdit;
    combox = new QComboBox;

    QHBoxLayout *lay_edit = new QHBoxLayout;
    QHBoxLayout *lay_btns = new QHBoxLayout;

    mainLay->addLayout(lay_edit);
    mainLay->addLayout(lay_btns);

    lay_edit->addWidget(new QLabel("目标路径:"));
    lay_edit->addWidget(combox);
    lay_edit->addWidget(le_fileName);

    combox->addItem("${srcdir}");
    combox->addItem("${pkgdir}");
    combox->setCurrentIndex(1);

    QPushButton *btn_cancel = new QPushButton("取消");
    QPushButton *btn_apply = new QPushButton("确定");

    lay_btns->addStretch();
    lay_btns->addWidget(btn_cancel);
    lay_btns->addStretch();
    lay_btns->addWidget(btn_apply);
    lay_btns->addStretch();

    le_fileName->setText("/");

    //file
    if(_mode == 0)
    {
        setWindowTitle("删除文件");
    }

    //dir
    if(_mode == 1)
    {
        setWindowTitle("删除文件夹");
    }

    connect(btn_cancel, &QPushButton::clicked, [=](){
        int ret = QMessageBox::question(this, "确认退出", "确认退出文件删除吗？");

        if(ret == QMessageBox::Yes)
        {
            close();
        }
    });

    connect(btn_apply, &QPushButton::clicked, [=](){
        if(le_fileName->text()=="/")
        {
            QMessageBox::critical(this, "错误！", "请填写目标路径！");

            return;
        }

        if(_mode == 0)
        {
            emit signalAddOperation(tr("rm -f %1").arg(combox->currentText()+le_fileName->text()));
        }

        if(_mode == 1)
        {
            emit signalAddOperation(tr("rm -rf %1").arg(combox->currentText()+le_fileName->text()));
        }
    });
}
