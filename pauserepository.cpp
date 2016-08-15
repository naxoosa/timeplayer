#include "pauserepository.h"

PauseRepository::PauseRepository(QObject *parent) : QObject(parent)
{
    db = DatabaseConfig::Instance()->getNaxooPlayerDb();
}

Pause *PauseRepository::findById(int id){
    Pause *pause = NULL;
    if(!db.isOpen())
        db.open();
    QSqlQuery select(db);
    select.prepare("SELECT * FROM rh_pause_report WHERE id=:id");
    select.bindValue(":id",id);
    select.exec();
    if(select.next()){
        pause = new Pause();
        pause->setId(select.value(0).toInt());
        pause->setStartTime(select.value(1).toDateTime());
        pause->setEndTime(select.value(2).toDateTime());
        pause->setDuration(select.value(3).toInt());
        pause->setTypeId(select.value(4).toInt());
        pause->setReportId(select.value(5).toInt());
    }

    select.clear();
    select.finish();
    db.close();
    return pause;
}

QList<Pause *> *PauseRepository::findByReportId(int reportid){
    QList<Pause *> *list = new QList<Pause *>();
    if(!db.isOpen())
        db.open();
    QSqlQuery select(db);
    select.prepare("SELECT * FROM rh_pause_report WHERE reportid=:reportid");
    select.bindValue(":reportid",reportid);
    select.exec();
    while(select.next()){
        Pause *pause = new Pause();
        pause->setId(select.value(0).toInt());
        pause->setStartTime(select.value(1).toDateTime());
        pause->setEndTime(select.value(2).toDateTime());
        pause->setDuration(select.value(3).toInt());
        pause->setTypeId(select.value(4).toInt());
        pause->setReportId(select.value(5).toInt());
        list->append(pause);
    }

    select.clear();
    select.finish();
    db.close();
    return list;
}

Pause *PauseRepository::findLastPause(int reportid){
    Pause *pause = NULL;
    if(!db.isOpen())
        db.open();
    QSqlQuery select(db);
    select.prepare("SELECT * FROM rh_pause_report WHERE endtime is null and reportid=:reportid");
    select.bindValue(":reportid",reportid);
    select.exec();
    if(select.next()){
        pause = new Pause();
        pause->setId(select.value(0).toInt());
        pause->setStartTime(select.value(1).toDateTime());
        pause->setEndTime(select.value(2).toDateTime());
        pause->setDuration(select.value(3).toInt());
        pause->setTypeId(select.value(4).toInt());
        pause->setReportId(select.value(5).toInt());
    }

    select.clear();
    select.finish();
    db.close();
    return pause;
}

void PauseRepository::remove(Pause *pause){
    assert(&pause!=NULL);
    if(!db.isOpen())
        db.open();
    QSqlQuery remove(db);
    if(pause->getId()!=0){
        remove.prepare("DELETE FROM rh_pause_report WHERE id=:id");
        remove.bindValue(":id",pause->getId());

        if(!remove.exec()){
            qDebug() << remove.lastError().text();
        }
    }
    remove.clear();
    remove.finish();
    db.close();
}

int PauseRepository::save(Pause *pause){
    assert(pause!=NULL);
    if(!db.isOpen())
        db.open();
    QSqlQuery save(db);    
    if(pause->getId()==0){
        save.prepare("INSERT INTO rh_pause_report (starttime,endtime,duration,typeid,reportid) VALUES(:date,:endtime,:duration,:type,:reportid)");
        save.bindValue(":date",pause->getStartTime());
        save.bindValue(":endtime",pause->getEndTime());
        if(!pause->getEndTime().isNull())
            save.bindValue(":duration",(pause->getEndTime().toMSecsSinceEpoch()-pause->getStartTime().toMSecsSinceEpoch())/1000);
        save.bindValue(":type",pause->getTypeId());
        save.bindValue(":reportid",pause->getReportId());
    }
    else {        
        save.prepare("UPDATE rh_pause_report SET starttime=:startime, endtime=:endtime, duration=:duration, typeid=:typeid, reportid=:reportid WHERE id=:id");
        save.bindValue(":id",pause->getId());
        save.bindValue(":startime",pause->getStartTime());
        save.bindValue(":endtime",pause->getEndTime());
        save.bindValue(":duration",pause->getDuration());
        save.bindValue(":typeid",pause->getTypeId());
        save.bindValue(":reportid",pause->getReportId());
    }

    if(!save.exec()){
        qDebug() << save.lastError().text();
    }    

    int id = save.lastInsertId().toInt();
    save.clear();
    save.finish();
    if(stopDb)
        db.close();
    return id;
}

