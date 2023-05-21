#ifndef KEYDIALOG_H
#define KEYDIALOG_H

#include <QDialog>

namespace Ui {
class KeyDialog;
}

class KeyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KeyDialog(QWidget *parent = nullptr);
    ~KeyDialog();

private slots:
    void on_buttonBox_2_accepted();

    void on_buttonBox_2_rejected();

    void on_pushButton_clicked();

private:
    void updateKey();
    Ui::KeyDialog *ui;
};

#endif // KEYDIALOG_H
