#ifndef REPORTREPOSITORY_H
#define REPORTREPOSITORY_H

#include <QObject>
#include <assert.h>
#include <QtSql>
#include "singleton.h"
#include "report.h"
#include "pause.h"
#include "pauserepository.h"
#include "configuration.h"

class ReportRepository : public QObject
{
    Q_OBJECT
public:
    explicit ReportRepository(QObject *parent = 0);
    Report &findById(int id);
    QList<Report *> ReportRepository::findByUserId(int userid);
    Report *findByUserIdAndDate(int userid, QDateTime date);
    void create(Report &report);
    int save(Report *report);    

signals:

public slots:

private:
    QSqlDatabase db;
    PauseRepository *repositorypause;
};

typedef Singleton<ReportRepository> RepositoryReport;

#endif // REPORTREPOSITORY_H
