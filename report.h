#ifndef REPORT_H
#define REPORT_H

#include <QObject>
#include <QtSql>
#include <QList>
#include <pause.h>

class Report : public QObject
{
    Q_OBJECT
public:
    explicit Report(QObject *parent = 0);
    int getId(){return id;}
    void setId(int id){this->id=id;}
    int getUserId(){return userid;}
    void setUserId(int userid){this->userid=userid;}
    QDateTime getStartDate(){return startdate;}
    void setStartDate(QDateTime startdate){this->startdate=startdate;}
    QDateTime getEndDate(){return enddate;}
    void setEndDate(QDateTime enddate){this->enddate=enddate;}
    QString getReportUserCol(){return rh_report_usercol;}
    void setReportUserCol(QString rh_report_usercol){this->rh_report_usercol=rh_report_usercol;}
    QList<Pause*> *getPauses(){return pauses;}
    void setPauses(QList<Pause*> *pause){this->pauses=pause;}

signals:

public slots:

private:
    int id = 0;
    int userid;
    QDateTime startdate;
    QDateTime enddate;
    int durationtotal;
    QString rh_report_usercol;
    QList<Pause*> *pauses;

};

#endif // REPORT_H
