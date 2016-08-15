#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include "configuration.h"
#include "reportrepository.h"
#include "pauserepository.h"
#include "absrepository.h"
#include <report.h>


class Thread : public QThread {

public:
    Thread(QObject *parent);

private:
    void run();
    ReportRepository *repositoryreport;
    PauseRepository *repositorypause;
    AbsRepository *repositoryabs;
    QObject *obj2save;
};

#endif // THREAD_H
