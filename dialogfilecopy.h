#ifndef DIALOGFILECOPY_H
#define DIALOGFILECOPY_H

#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QCheckBox>
#include <QMessageBox>

class DialogFileCopy : public QDialog
{
    Q_OBJECT
public:
    explicit DialogFileCopy(QWidget *parent = nullptr, int mode = 0);

    QVBoxLayout *mainLay;

    QComboBox *combox_source;
    QComboBox *combox_destination;
    QCheckBox *check_binary;

    QLineEdit *le_source;
    QLineEdit *le_destination;

private:
    int _mode;
signals:
    void signalAddOperation(QString operation);

};

#endif // DIALOGFILECOPY_H
