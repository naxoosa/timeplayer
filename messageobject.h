#ifndef MESSAGEOBJECT_H
#define MESSAGEOBJECT_H

#include <QObject>

class MessageObject : public QObject
{
    Q_OBJECT
public:
    explicit MessageObject(QObject *parent = 0);
    void setType(QString type);
    QString getType();
    void setAction(QString action);
    QString getAction();

signals:

public slots:

private:
    QString type;
    QString action;
};

#endif // MESSAGEOBJECT_H
