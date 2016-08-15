#include "thread.h"

Thread::Thread(QObject *parent)
{
    this->obj2save = parent;
    repositoryreport = RepositoryReport::Instance();
    repositorypause = RepositoryPause::Instance();
    repositoryabs = RepositoryAbs::Instance();
}

void Thread::run()
{
    if(qobject_cast<Report*>(this->obj2save)){
        //qDebug()<<"C'est un report";
        Report *report = (Report *) this->obj2save;        
        int id = repositoryreport->save(report);        
        if(report->getId()==0)
        report->setId(id);
    }
    else if(qobject_cast<Pause*>(this->obj2save)){
        //qDebug()<<"C'est une pause";
        Pause *pause = (Pause *)this->obj2save;
        int id = repositorypause->save(pause);
        if(pause->getId()==0)
        pause->setId(id);
    }
    else if(qobject_cast<Report*>(this->obj2save)){
        qDebug()<<"C'est un report";
    }
    else {
        qDebug()<<"C'est rien";
    }
}

