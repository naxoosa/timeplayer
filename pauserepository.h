#ifndef PAUSEREPOSITORY_H
#define PAUSEREPOSITORY_H

#include <QObject>
#include <assert.h>
#include <QtSql>
#include "singleton.h"
#include "pause.h"
#include "configuration.h"

class PauseRepository : public QObject
{
    Q_OBJECT
public:
    explicit PauseRepository(QObject *parent = 0);
    Pause *findById(int id);
    QList<Pause *> *findByReportId(int reportid);
    Pause *findLastPause(int reportid);
    void create(Pause *pause);
    void remove(Pause *pause);
    int save(Pause *pause);
    bool stopDb = true;

signals:

public slots:

private:
    QSqlDatabase db;
};

typedef Singleton<PauseRepository> RepositoryPause;

#endif // PAUSEREPOSITORY_H
