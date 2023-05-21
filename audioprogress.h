#ifndef AUDIOPROGRESS_H
#define AUDIOPROGRESS_H

#include <QSlider>
#include <QObject>

class AudioProgress : public QSlider
{
    Q_OBJECT

public:
    AudioProgress(QWidget *parent);
private:
    void mousePressEvent(QMouseEvent *ev);
signals:
    void humanChanged(int val);
};

#endif // AUDIOPROGRESS_H
