#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include<QKeySequence>
class Settings : public QObject
{
    Q_OBJECT
public:
    static struct SetData
    {
        //qint32 volume;
        QString path;
        QKeySequence key_next,key_prev,key_stop,key_play;
        friend QDataStream &operator<<(QDataStream &stream,const SetData &data)
        {
            stream<<data.path;
            stream<<data.key_next;
            stream<<data.key_prev;
            stream<<data.key_stop;
            stream<<data.key_play;
            //stream<<(qint32)data.volume;
            return stream;
        }
        friend QDataStream &operator>>(QDataStream &stream, SetData &data)
        {
            stream>>data.path;
            stream>>data.key_next;
            stream>>data.key_prev;
            stream>>data.key_stop;
            stream>>data.key_play;
            return stream;
        }
    }sets;
    explicit Settings(QObject *parent = nullptr);

signals:

};

#endif // SETTINGS_H
