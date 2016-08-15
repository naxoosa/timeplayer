#include "reportrepository.h"

ReportRepository::ReportRepository(QObject *parent) : QObject(parent)
{
    db = DatabaseConfig::Instance()->getNaxooPlayerDb();
    repositorypause = RepositoryPause::Instance();
}

Report &ReportRepository::findById(int id){
    Report *report = NULL;
    if(!db.isOpen())
        db.open();
    QSqlQuery select(db);    
    select.prepare("SELECT * FROM rh_report_user WHERE id=:id");
    select.bindValue(":id",id);
    select.exec();
    if(select.next()){
        report = new Report();
        report->setId(select.value(0).toInt());
        report->setUserId(select.value(1).toInt());
        report->setStartDate(select.value(2).toDateTime());
        report->setEndDate(select.value(3).toDateTime());
        report->setReportUserCol(select.value(4).toString());
    }

    select.clear();
    select.finish();
    db.close();

    return *report;
}

QList<Report*> ReportRepository::findByUserId(int userid){
    QList<Report*> reports;
    if(!db.isOpen())
        db.open();
    QSqlQuery select(db);
    select.prepare("SELECT * FROM rh_report_user WHERE userid=:userid");
    select.bindValue(":userid",userid);
    select.exec();
    while(select.next()){
        Report *report = new Report();
        report->setId(select.value(0).toInt());
        report->setUserId(select.value(1).toInt());
        report->setStartDate(select.value(2).toDateTime());
        report->setEndDate(select.value(3).toDateTime());
        report->setReportUserCol(select.value(4).toString());
        reports.append(report);
    }

    select.clear();
    select.finish();
    db.close();

    return reports;
}

Report *ReportRepository::findByUserIdAndDate(int userid, QDateTime date){
    Report *report = NULL;
    if(!db.isOpen())
        db.open();
    QSqlQuery select(db);    
    select.prepare("SELECT * FROM rh_report_user WHERE userid=:userid and DATE(startdate)=:startdate");
    select.bindValue(":userid",userid);
    select.bindValue(":startdate",date.date());
    select.exec();
    if(select.next()){
        report = new Report();
        report->setId(select.value(0).toInt());
        report->setUserId(select.value(1).toInt());
        report->setStartDate(select.value(2).toDateTime());
        report->setEndDate(select.value(3).toDateTime());
        report->setReportUserCol(select.value(4).toString());
    }    

    select.clear();
    select.finish();
    db.close();
    return report;
}

int ReportRepository::save(Report *report){
    assert(report!=NULL);
    if(!db.isOpen())
        db.open();
    QSqlQuery save(db);
    if(report->getId()==0){
        save.prepare("INSERT INTO rh_report_user (userid,startdate) VALUES(:userid,:startdate)");
        save.bindValue(":userid",report->getUserId());
        save.bindValue(":startdate",report->getStartDate());
    }
    else {        
        save.prepare("UPDATE rh_report_user SET userid=:userid, startdate=:startdate, enddate=:enddate, rh_report_usercol=:reportuser WHERE id=:id");
        save.bindValue(":id",report->getId());
        save.bindValue(":userid",report->getUserId());
        save.bindValue(":startdate",report->getStartDate());
        save.bindValue(":enddate",report->getEndDate());
        save.bindValue(":reportuser",report->getReportUserCol());        
    }   

    repositorypause->stopDb = false;
    for(int i=0;i<report->getPauses()->size();i++){
        Pause * pause = report->getPauses()->at(i);
        repositorypause->save(pause);
    }    
    repositorypause->stopDb = true;

    if(!save.exec()){
        qDebug() << save.lastError().text();
    }    

    int id = save.lastInsertId().toInt();
    save.clear();
    save.finish();
    db.close();
    return id;
}

