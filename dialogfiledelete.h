#ifndef DIALOGFILEDELETE_H
#define DIALOGFILEDELETE_H

#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QMessageBox>

class DialogFileDelete : public QDialog
{
    Q_OBJECT
public:
    explicit DialogFileDelete(QWidget *parent = nullptr, int mode = 0);

    QVBoxLayout *mainLay;

    QComboBox *combox;

    QLineEdit *le_fileName;

private:
    int _mode;
signals:
    void signalAddOperation(QString operation);
};

#endif // DIALOGFILEDELETE_H
