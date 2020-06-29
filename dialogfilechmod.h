#ifndef DIALOGFILECHMOD_H
#define DIALOGFILECHMOD_H

#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QMessageBox>

class DialogFileChmod : public QDialog
{
    Q_OBJECT
public:
    explicit DialogFileChmod(QWidget *parent = nullptr, int mode =0);

    QVBoxLayout *mainLay;

    QComboBox *combox;

    QLineEdit *le_fileName;

    QLineEdit *le_mod;


private:
    int _mode;

signals:

    void signalAddOperation(QString operation);

};

#endif // DIALOGFILECHMOD_H
