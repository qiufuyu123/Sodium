#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget>
#include<QHotkey>
#include<QtMultimedia/QMediaPlayer>
QT_BEGIN_NAMESPACE
namespace Ui { class MainForm; }
QT_END_NAMESPACE

class MainForm : public QWidget
{
    Q_OBJECT

public:
    MainForm(QWidget *parent = nullptr);
    ~MainForm();

private slots:
    void on_pushButton_clicked();

    void on_listView_doubleClicked(const QModelIndex &index);

    void on_progress_valueChanged(int value);


    void on_sliderVolume_valueChanged(int value);

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_buttonClose_clicked();

private:
    bool m_alreay_stopped = false;
    QString setPlayTime(uint64_t t);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void timerEvent(QTimerEvent *event);
    void durationChanged(uint64_t dur);
    void playtimeChanged(uint64_t time);
    void playStateChanged(QMediaPlayer::State s);
    void onHumanChanged(int v);
    void switchToMusic();
    void showKeySeq();
    void updateSets();
    void updatePath();
    Ui::MainForm *ui;

    QPoint m_beginPos,m_leftTopPos,m_cur_pos;
    QHotkey *hotkey_next,*hotkey_prev,*hotkey_stop,*hotkey_play;
};
#endif // MAINFORM_H
