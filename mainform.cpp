#include "mainform.h"
#include "ui_mainform.h"
#include<QFile>
#include<QTextStream>
#include<QMouseEvent>
#include<QTabBar>
#include<QTimer>
#include<QTimerEvent>
#include<QFileDialog>
#include <QStringListModel>
#include<QtMultimedia/QMediaPlayer>
#include<QHotkey>

#include"keydialog.h"
#include"settings.h"

int m_timer_id = 0;
int m_cur_idx = 0;
QTimer *m_hid_timer = nullptr;
QStringListModel *m_modelitem;
QStringList m_full_path;
QMediaPlayer* m_player = nullptr;
QFile setting_file("userset.txt");
QStringList file_list;
MainForm::MainForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainForm)
{
    ui->setupUi(this);
    m_timer_id = startTimer(500);
    m_player = new QMediaPlayer(this);
    connect(m_player,&QMediaPlayer::durationChanged,this,&MainForm::durationChanged);
    connect(m_player,&QMediaPlayer::positionChanged,this,&MainForm::playtimeChanged);
    connect(m_player,&QMediaPlayer::stateChanged,this,&MainForm::playStateChanged);
    connect(ui->progress,&AudioProgress::humanChanged,this,&MainForm::onHumanChanged);
    QFile f(":qdarkstyle/dark/darkstyle.qss");

    if (!f.exists())   {
        printf("Unable to set stylesheet, file not found\n");
    }
    else   {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        this->setStyleSheet(ts.readAll());
    }
    // Load dark-style skin

    setAttribute(Qt::WA_TranslucentBackground,true); //设置背景透明
    this->setWindowFlags(Qt::FramelessWindowHint);
//    ui->moveLabel->setStyleSheet("border-width: 1px 1px 1px 1px;\
//            border-style: solid;\
//            border-color: rgb(130,130,130);\
//            border-radius:6px;\
//    background-color:#c0c0c0");
    ui->progress->setStyleSheet(R"(
    QSlider::groove:horizontal {
        border: none;
        height: 2px;
        background-color: lightgray;
    }

    QSlider::handle:horizontal {
        background-color: #2f77cb;
        width: 8px;
        margin: -9px 0px -9px 0px;
        border-radius: 4px;
    }
#
    QSlider::sub-page:horizontal {
        background-color: #4898ec;
    }
    )");
    ui->buttonClose->setStyleSheet(R"(
    QPushButton {
        border-radius:15px;
    }
    )");
    setWindowFlag(Qt::WindowStaysOnTopHint,true);
    setMouseTracking(true);
    if(setting_file.exists())
    {
        setting_file.open(QIODevice::ReadOnly);
        QDataStream in(&setting_file);
        in>>Settings::sets;
        setting_file.close();
    }else
    {
        updateSets();
    }

    hotkey_next =new QHotkey(Settings::sets.key_next,true,this);
    hotkey_prev=new QHotkey(Settings::sets.key_prev,true,this);
    hotkey_stop=new QHotkey(Settings::sets.key_stop,true,this);
    hotkey_play=new QHotkey(Settings::sets.key_play,true,this);

    QObject::connect(hotkey_next, &QHotkey::activated, this, [&](){
            m_cur_idx++;
            if(m_cur_idx>=m_full_path.size())
                m_cur_idx=0;
            switchToMusic();
        });
    QObject::connect(hotkey_prev, &QHotkey::activated, this, [&](){
            m_cur_idx--;
            if(m_cur_idx<0)
                m_cur_idx=0;
            switchToMusic();
        });
    QObject::connect(hotkey_stop, &QHotkey::activated, this, [&](){
            m_player->stop();
        });
    QObject::connect(hotkey_play, &QHotkey::activated, this, [&](){
            m_player->play();
        });
    updatePath();
}

void MainForm::updateSets()
{
    setting_file.open(QIODevice::WriteOnly);
    QDataStream out(&setting_file);
    out<<Settings::sets;
    setting_file.close();
}

void MainForm::timerEvent(QTimerEvent *event)
{
    if(event->timerId()==m_timer_id)
    {
        QRect r= this->geometry();
        QPoint p =QCursor::pos();
        if(r.contains(p))
        {
            ui->tabWidget->tabBar()->show();
        }else
        {
            ui->tabWidget->tabBar()->hide();
        }
    }
}

void MainForm::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_beginPos = event->globalPos();
        m_leftTopPos = pos();//也可以使用geometry().topLeft()
    }
    QWidget::mousePressEvent(event);
}

void MainForm::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        QPoint endPos = event->globalPos();
        QPoint offPos = endPos - m_beginPos;
        QPoint pos = m_leftTopPos + offPos;
        if(!ui->sliderVolume->hasFocus())
            move(pos);
    }
    m_cur_pos = event->pos();

    QWidget::mouseMoveEvent(event);
}
void MainForm::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
}
MainForm::~MainForm()
{
    delete ui;
}

QString MainForm::setPlayTime(uint64_t time)
{
    int h,m,s;
    time /= 1000;  //获得的时间是以毫秒为单位的
    h = time/3600;
    m = (time-h*3600)/60;
    s = time-h*3600-m*60;
    return QString("%1:%2:%3").arg(h).arg(m).arg(s);
}

void MainForm::switchToMusic()
{
    QString path = m_full_path.at(m_cur_idx);
    m_player->stop();
    m_player->setMedia(QUrl::fromLocalFile(path));
    m_player->play();
    ui->labelMusicname->setText(ui->listView->model()->data(ui->listView->model()->index(m_cur_idx,0)).toString());
}

void MainForm::playStateChanged(QMediaPlayer::State s)
{
//    if(s == QMediaPlayer::StoppedState)
//    {
//        if(m_alreay_stopped)
//            return;
//        m_cur_idx ++;
//        if(m_cur_idx>=m_full_path.size())
//            m_cur_idx=0;
//        switchToMusic();
//        m_alreay_stopped = true;
//    }else if(s == QMediaPlayer::PlayingState)
//    {
//        m_alreay_stopped = false;
//    }
}

void MainForm::durationChanged(uint64_t dur)
{
    ui->labelDuration->setText(setPlayTime(dur));
}

void MainForm::playtimeChanged(uint64_t time)
{
    ui->labelPlaytime->setText(setPlayTime(time));
    if(!m_player->duration())
        return;
    uint64_t dura = m_player->duration();
    ui->progress->setValue(time*100/dura);
    if(dura == time)
    {
        m_cur_idx++;
        if(m_cur_idx >= m_full_path.size())
            m_cur_idx = 0;
        switchToMusic();
    }
}

void MainForm::showKeySeq()
{
    //ui->textKeyseq->setText(QString::fromLocal8Bit("下一首: ")+Settings::sets.key_next.toString()+QString::fromLocal8Bit(",上一首: ")+Settings::sets.key_prev.toString()
    //                        +QString::fromLocal8Bit("\n暂停: ")+Settings::sets.key_stop.toString()+QString::fromLocal8Bit(",继续: ")+Settings::sets.key_play.toString());
}

void MainForm::updatePath()
{
    ui->linePath->setText(Settings::sets.path);

    QDir dir(Settings::sets.path);
    file_list.clear();
    m_full_path.clear();
        QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Dirs);
        foreach (auto fileInfo, fileInfoList) {
            if(fileInfo.isFile())
            {
                file_list.append(fileInfo.fileName());
                m_full_path.append(fileInfo.absoluteFilePath());
            }
        }
    if(!m_modelitem)
        m_modelitem = new QStringListModel(file_list);
    else
        m_modelitem->setStringList(file_list);
    ui->listView->setModel(m_modelitem);
}

void MainForm::on_pushButton_clicked()
{

    Settings::sets.path = QFileDialog::getExistingDirectory(this,"Open A Directroy");
    updatePath();
}


void MainForm::on_listView_doubleClicked(const QModelIndex &index)
{
    m_cur_idx = index.row();
    switchToMusic();

}

void MainForm::onHumanChanged(int v)
{
    m_player->setPosition(v/100.0f*m_player->duration());
}

void MainForm::on_progress_valueChanged(int value)
{

}


void MainForm::on_sliderVolume_valueChanged(int value)
{
    //Settings::sets.volume = value;
    m_player->setVolume(value);
}


void MainForm::on_pushButton_3_clicked()
{
    m_player->pause();
}


void MainForm::on_pushButton_4_clicked()
{
    m_player->play();
}


void MainForm::on_pushButton_2_clicked()
{
    KeyDialog d(this);
    hotkey_next->setRegistered(false);
    hotkey_prev->setRegistered(false);
    hotkey_stop->setRegistered(false);
    hotkey_play->setRegistered(false);
    if(d.exec() == QDialog::Accepted)
    {
        updateSets();
    }
    hotkey_next->setRegistered(true);
    hotkey_prev->setRegistered(true);
    hotkey_stop->setRegistered(true);
    hotkey_play->setRegistered(true);
}


void MainForm::on_buttonClose_clicked()
{
    qApp->exit();
}

