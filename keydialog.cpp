#include "keydialog.h"
#include "ui_keydialog.h"
#include"settings.h"

KeyDialog::KeyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeyDialog)
{
    ui->setupUi(this);
    updateKey();
}

void KeyDialog::updateKey()
{
    ui->key_next->setKeySequence(Settings::sets.key_next);
    ui->key_prev->setKeySequence(Settings::sets.key_prev);
    ui->key_stop->setKeySequence(Settings::sets.key_stop);
    ui->key_play->setKeySequence(Settings::sets.key_play);
}

KeyDialog::~KeyDialog()
{
    delete ui;
}

void KeyDialog::on_buttonBox_2_accepted()
{
    Settings::sets.key_next = ui->key_next->keySequence();
    Settings::sets.key_prev = ui->key_prev->keySequence();
    Settings::sets.key_stop = ui->key_stop->keySequence();
    Settings::sets.key_play = ui->key_play->keySequence();
    accept();
}


void KeyDialog::on_buttonBox_2_rejected()
{
    reject();
}


void KeyDialog::on_pushButton_clicked()
{
    Settings::sets.key_next=Settings::sets.key_prev=Settings::sets.key_stop=Settings::sets.key_play=QKeySequence::fromString("");
    updateKey();
}

