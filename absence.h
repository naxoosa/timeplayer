#ifndef ABSENCE_H
#define ABSENCE_H

#include <QObject>
#include <QtSql>

class Absence : public QObject
{
    Q_OBJECT
public:
    explicit Absence(QObject *parent = 0);
    int getId(){return id;}
    void setId(int id){this->id=id;}
    int getUserId(){return userid;}
    void setUserId(int userid){this->userid=userid;}
    int getAbsType(){return type;}
    void setAbsType(int type){this->type=type;}
    QDateTime getAbsDate(){return absday;}
    void setAbsDate(QDateTime absday){this->absday=absday;}

signals:

public slots:
private:
    int id = 0;
    int userid;
    int type;
    QDateTime absday;
};

#endif // ABSENCE_H
