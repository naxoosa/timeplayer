#include "absrepository.h"

AbsRepository::AbsRepository(QObject *parent) : QObject(parent)
{
    db = DatabaseConfig::Instance()->getSIRHDb();
}

QList<Absence *> AbsRepository::findByUserId(int userid){
    QList<Absence *> list;
    db.open();
    QSqlQuery select(db);    
    select.prepare("SELECT * FROM HRMABZ WHERE EMPKEY=:userid AND ABTKEY IN (11,42) AND YEAR(ABZDATF)=2016");
    select.bindValue(":userid",userid);
    select.exec();
    while(select.next()){
        QDateTime start = select.value(9).toDateTime();
        QDateTime end = select.value(12).toDateTime();
        qDebug()<< start.toString() << "-" << end.toString();
        while(start<=end){
            if(start.date().dayOfWeek()!=6 && start.date().dayOfWeek()!=7){
                Absence *abs = new Absence();
                abs->setId(select.value(8).toInt());
                abs->setUserId(select.value(6).toInt());
                abs->setAbsDate(start);
                list.append(abs);
            }
            start = start.addDays(1);
        }
    }
    select.clear();
    select.finish();
    db.close();
    return list;
}

QList<Absence *> AbsRepository::findByUserIdMonthYear(int userid,int month, int year){
    QList<Absence *> list;    
    db.open();
    QSqlQuery select(db);    
    select.prepare("SELECT * FROM HRMABZ WHERE EMPKEY=:userid AND ABTKEY IN (11,42) AND MONTH(ABZDATF)=:month AND YEAR(ABZDATF)=:year");
    select.bindValue(":userid",userid);
    select.bindValue(":month",month);
    select.bindValue(":year",year);
    select.exec();
    while(select.next()){
        QDateTime start = select.value(9).toDateTime();
        QDateTime end = select.value(12).toDateTime();
        qDebug()<< start.toString() << "-" << end.toString();
        while(start<=end){
            if(start.date().dayOfWeek()!=6 && start.date().dayOfWeek()!=7){
                Absence *abs = new Absence();
                abs->setId(select.value(8).toInt());
                abs->setUserId(select.value(6).toInt());
                abs->setAbsType(select.value(37).toInt());
                abs->setAbsDate(start);
                list.append(abs);
            }
            start = start.addDays(1);
        }
    }

    select.clear();
    select.finish();
    db.close();
    return list;
}
