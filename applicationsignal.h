#ifndef APPLICATIONSIGNAL_H
#define APPLICATIONSIGNAL_H

#include <QObject>
#include <QDebug>
#include <QQmlApplicationEngine>
#include <messageobject.h>
#include <QJSValue>
#include <items/CustomProgressBar.h>
#include <QTimer>
#include "thread.h"
#include "detaildialog.h"
/*#include "pausedataobject.h"
#include "detaildialog.h"
#include "bulldialog.h"
#include "mailsenderssl.h"
#include "mailsender.h"*/

class ApplicationSignal: public QObject
{
    Q_OBJECT
public:
    explicit ApplicationSignal(QObject *parent = 0);
    QQmlApplicationEngine *engine;
    QObject *parent;
    QTimer *reporttimer;
    QTimer *pausetimer;
    Thread *thread;
    Report *report = NULL;
    QList<CustomProgressBar *> pausesobject;
    void createReport();

public slots:
    void sendmessage(QVariant object);
    void updateReportTimer();
    void updatePauseTimerCafe();
    void updatePauseTimerRepas();
    void opendetail(QVariant val);

private:
    ReportRepository *repositoryreport;
    PauseRepository *repositorypause;
    AbsRepository *repositoryabs;    
    int CAFE = 15;
    int REPAS = 30;
};

#endif // APPLICATIONSIGNAL_H
