#ifndef PAUSE_H
#define PAUSE_H

#include <QObject>
#include <QtSql>

class Pause : public QObject
{
    Q_OBJECT
public:
    explicit Pause(QObject *parent = 0);
    int getId(){return id;}
    void setId(int id){this->id=id;}
    QDateTime getStartTime() const {return starttime;}
    void setStartTime(QDateTime starttime){this->starttime=starttime;}
    QDateTime getEndTime() const {return endtime;}
    void setEndTime(QDateTime endtime){this->endtime=endtime;}
    double getDuration(){return duration;}
    void setDuration(double duration){this->duration=duration;}
    int getTypeId(){return typid;}
    void setTypeId(int typid){this->typid=typid;}
    int getReportId(){return reportid;}
    void setReportId(int reportid){this->reportid=reportid;}

signals:

public slots:

private:
    int id = 0;
    QDateTime starttime;
    QDateTime endtime;
    double duration;
    int typid;
    int reportid;

};

#endif // PAUSE_H
