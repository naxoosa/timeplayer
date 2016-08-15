#include "applicationsignal.h"
#include <QLabel>
#include <QProgressBar>

ApplicationSignal::ApplicationSignal(QObject *parent) : QObject(parent)
{
    this->engine = (QQmlApplicationEngine*)parent;
    this->parent = engine->rootObjects().first();
    repositoryreport = RepositoryReport::Instance();
    repositorypause = RepositoryPause::Instance();
    repositoryabs = RepositoryAbs::Instance();
}

void ApplicationSignal::sendmessage(QVariant object){    
    QJSValue msg = object.value<QJSValue>();
    QString action = msg.property("action").toString();
    if(action=="startreport"){
        qDebug()<<"startreport";
        if(report==NULL){
            report = new Report();
            report->setStartDate(QDateTime::currentDateTime());
            report->setPauses(new QList<Pause*>());
            report->setUserId(DatabaseConfig::Instance()->getCurrentUser()->getId());
        }        
        reporttimer = new QTimer(this);
        connect(reporttimer,SIGNAL(timeout()),this,SLOT(updateReportTimer()));
        reporttimer->start(100);
        thread = new Thread(report);
        thread->start();
    }
    else if(action=="endreport"){
        if(report!=NULL){
            qDebug()<<"endreport";
            report->setEndDate(QDateTime::currentDateTime());
            reporttimer->stop();
            thread = new Thread(report);
            thread->start();
        }
        else {

        }
    }
    else if(action.startsWith("startpause")){
        if(report!=NULL){
            qDebug()<<"startpause";
            Pause *pause = new Pause();
            pause->setStartTime(QDateTime::currentDateTime());
            pause->setReportId(report->getId());
            if(action.endsWith("cafe"))
                pause->setTypeId(1);
            else
                pause->setTypeId(2);             
            report->getPauses()->append(pause);            
            CustomProgressBar *reportbar = parent->findChild<CustomProgressBar*>("playprogressbar");
            QQmlComponent pausecomponent(engine, QUrl(QStringLiteral("qrc:/items/progressbar.qml")));
            CustomProgressBar *pobj = qobject_cast<CustomProgressBar *>(pausecomponent.create());
            QQmlApplicationEngine::setObjectOwnership(pause, QQmlApplicationEngine::JavaScriptOwnership);
            pobj->setWidth(319);
            pobj->setHeight(319);
            if(pause->getTypeId()==2)
                pobj->setColor(QColor("#d07f58"));
            else
                pobj->setColor(QColor("#ce8d1b"));
            pobj->setStartAngle(reportbar->getStartAngle()+reportbar->getSpanAngle());
            pobj->setVisible(true);
            pobj->setParentItem(reportbar);
            pobj->setEchelle(reportbar->getEchelle());
            pausesobject.append(pobj);           
            QObject *rect = parent->findChild<QObject*>("grpprogressbar");
            QObject *clock = parent->findChild<QObject*>("clock");
            QObject *pausebar = parent->findChild<QObject*>("progressbar");
            QObject *endmin = parent->findChild<QObject*>("endmin");
            pausebar->setProperty("maximumValue",100);
            rect->setProperty("visible",true);
            clock->setProperty("visible",false);
            if(pause->getTypeId()==2)
            reporttimer->stop();
            pausetimer = new QTimer(this);
            if(pause->getTypeId()==1){
                endmin->setProperty("text",QString::number(CAFE)+" Min");
                connect(pausetimer,SIGNAL(timeout()),this,SLOT(updatePauseTimerCafe()));
            }
            else{
                endmin->setProperty("text",QString::number(REPAS)+" Min");
                connect(pausetimer,SIGNAL(timeout()),this,SLOT(updatePauseTimerRepas()));
            }
            pausetimer->start(100);
            thread = new Thread(pause);
            thread->start();            
        }
        else {

        }
    }
    else if(action.startsWith("endpause")){
        if(report!=NULL){
            qDebug()<<"endpause";
            qDebug()<<report->getPauses()->size();
            Pause *pause = report->getPauses()->last();
            pause->setEndTime(QDateTime::currentDateTime());
            pause->setDuration((QDateTime::currentDateTime().toMSecsSinceEpoch()-pause->getStartTime().toMSecsSinceEpoch())/1000);
            QObject *rect = parent->findChild<QObject*>("grpprogressbar");
            QObject *clock = parent->findChild<QObject*>("clock");
            rect->setProperty("visible",false);
            clock->setProperty("visible",true);
            pausetimer->stop();
            if(pause->getTypeId()==2){
                CustomProgressBar *reportbar = parent->findChild<CustomProgressBar*>("playprogressbar");
                reportbar->setEchelle(reportbar->getEchelle()+pause->getDuration()/3600);                
                for(int i=0;i<pausesobject.size();i++){
                    CustomProgressBar *p = pausesobject.at(i);
                    qreal startangle = (pause->getStartTime().toMSecsSinceEpoch()-report->getStartDate().toMSecsSinceEpoch())/reportbar->getEchelle()/36/1000;
                    qreal spanangle = pause->getDuration()*1000/reportbar->getEchelle()/36/1000;
                    p->setStartAngle(startangle);
                    p->setSpanAngle(spanangle);
                    p->setEchelle(reportbar->getEchelle()+pause->getDuration()/3600);
                }
                reporttimer->start();
                QObject *endmin = parent->findChild<QObject*>("endmin");
                endmin->setProperty("text",QString::number(CAFE)+" Min");
                QObject *currentvalue = parent->findChild<QObject*>("currentvalue");
                currentvalue->setProperty("text","");
                REPAS=30;
            }
            thread = new Thread(pause);
            thread->start();
        }
        else {

        }
    }
    else if(action.startsWith("addpause")){
        QDateTime startdate = msg.property("startdate").toDateTime();
        QDateTime enddate = msg.property("enddate").toDateTime();
        int typepause = msg.property("typepause").toInt();

        qDebug()<<startdate<<"-"<<enddate<<"-"<<typepause;

        Pause *pause = new Pause();
        pause->setStartTime(startdate);
        pause->setEndTime(enddate);
        pause->setReportId(report->getId());
        pause->setTypeId(typepause);
        pause->setDuration((enddate.toMSecsSinceEpoch()-startdate.toMSecsSinceEpoch())/1000);
        report->getPauses()->append(pause);
        CustomProgressBar *reportbar = parent->findChild<CustomProgressBar*>("playprogressbar");
        if(pause->getTypeId()==2){
            qDebug()<<"Ok";
            reportbar->setEchelle(reportbar->getEchelle()+pause->getDuration()/3600);
            for(int i=0;i<pausesobject.size();i++){
                qDebug()<<"Pause "<<i;
                CustomProgressBar *p = pausesobject.at(i);
                qreal startangle = (report->getPauses()->at(i)->getStartTime().toMSecsSinceEpoch()-report->getStartDate().toMSecsSinceEpoch())/reportbar->getEchelle()/36/1000;
                qreal spanangle = report->getPauses()->at(i)->getDuration()*1000/reportbar->getEchelle()/36/1000;
                p->setStartAngle(startangle);
                p->setSpanAngle(spanangle);
                p->setEchelle(reportbar->getEchelle());
                //p->update();
            }
        }
        QQmlComponent pausecomponent(engine, QUrl(QStringLiteral("qrc:/items/progressbar.qml")));
        CustomProgressBar *pobj = qobject_cast<CustomProgressBar *>(pausecomponent.create());
        QQmlApplicationEngine::setObjectOwnership(pause, QQmlApplicationEngine::JavaScriptOwnership);
        pobj->setWidth(319);
        pobj->setHeight(319);
        if(pause->getTypeId()==2)
            pobj->setColor(QColor("#d07f58"));
        else
            pobj->setColor(QColor("#ce8d1b"));
        qreal startangle = (startdate.toMSecsSinceEpoch() - report->getStartDate().toMSecsSinceEpoch())*100.0/reportbar->getEchelle()/3600000.0;
        qreal spanangle = (enddate.toMSecsSinceEpoch() - startdate.toMSecsSinceEpoch())*100.0/reportbar->getEchelle()/3600000.0;
        pobj->setStartAngle(startangle);
        pobj->setSpanAngle(spanangle);
        pobj->setVisible(true);
        pobj->setParentItem(reportbar);
        pobj->setEchelle(reportbar->getEchelle());
        pausesobject.append(pobj);
        thread = new Thread(pause);
        thread->start();
    }
    else {

    }
}

void ApplicationSignal::updateReportTimer(){
    CustomProgressBar *reportbar = parent->findChild<CustomProgressBar*>("playprogressbar");
    QObject *clock = parent->findChild<QObject*>("clock");
    if(reportbar!=NULL){
        if(reportbar->getSpanAngle()<100){
            reportbar->setStartAngle(0);
            reportbar->setColor(QColor("#00b7ee")); //#da4c1b"));
            QDateTime fin = report->getStartDate();            
            fin = fin.addSecs(reportbar->getEchelle()*3600);
            int restant = (fin.toMSecsSinceEpoch() - QDateTime::currentDateTime().toMSecsSinceEpoch())/1000;
            qreal spanangle = (QDateTime::currentDateTime().toMSecsSinceEpoch() - report->getStartDate().toMSecsSinceEpoch())*100.0/reportbar->getEchelle()/3600000.0;
            reportbar->setSpanAngle(spanangle);
            reportbar->update();
            if(clock!=NULL){
                int hours = floor(restant/3600);
                int minutes = floor((restant%3600)/60);
                int secondes = floor(restant%60);
                if(restant<=0){
                    hours = 0;
                    minutes = 0;
                    secondes = 0;
                }
                QString shours = (hours<10?"0"+QString::number(hours):QString::number(hours));
                QString sminutes = (minutes<10?"0"+QString::number(minutes):QString::number(minutes));
                QString sseconds = (secondes<10?"0"+QString::number(secondes):QString::number(secondes));
                QString heures = shours+":"+sminutes;
                if(restant<60)
                    heures = sseconds;
                clock->setProperty("text",heures);
            }
        }
        else{
            QObject *btnright = parent->findChild<QObject*>("btnright");
            QVariant returnedValue;
            QVariant msg = 1;
            QMetaObject::invokeMethod(btnright, "sendStop",Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, msg));
            QJSEngine jsEngine;
            QJSValue messg = jsEngine.newObject();
            messg.setProperty("action","endreport");
            emit sendmessage(QVariant::fromValue(messg));
            reporttimer->stop();
        }
    }
}

void ApplicationSignal::updatePauseTimerCafe(){
    if(pausesobject.size()>0){
        CustomProgressBar *pause = pausesobject.last();
        Pause *currentpause= report->getPauses()->last();        
        QDateTime debutpause = currentpause->getStartTime();
        qreal spanangle = (QDateTime::currentDateTime().toMSecsSinceEpoch() - debutpause.toMSecsSinceEpoch())*100.0/pause->getEchelle()/3600000.0;
        pause->setSpanAngle(spanangle);
        QObject *pausebar = parent->findChild<QObject*>("progressbar");
        qreal spanpourc = (QDateTime::currentDateTime().toMSecsSinceEpoch() - debutpause.toMSecsSinceEpoch())*100.0/CAFE/60000.0;
        pausebar->setProperty("value",spanpourc);
        if((QDateTime::currentDateTime().toMSecsSinceEpoch() - debutpause.toMSecsSinceEpoch())>=CAFE*60*1000 ){
            QObject *btnright = parent->findChild<QObject*>("btncenterm");
            QVariant returnedValue;
            QVariant msg = 1;
            QMetaObject::invokeMethod(btnright, "sendClick",Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, msg));
            QJSEngine jsEngine;
            QJSValue messg = jsEngine.newObject();
            messg.setProperty("action","endpausecafe");
            emit sendmessage(QVariant::fromValue(messg));
            pausetimer->stop();
        }

    }
    else {
        pausetimer->stop();
    }
}

void ApplicationSignal::updatePauseTimerRepas(){    
    if(pausesobject.size()>0){        
        CustomProgressBar *pause = pausesobject.last();
        Pause *currentpause= report->getPauses()->last();        
        QDateTime debutpause = currentpause->getStartTime();       
        qreal spanangle = (QDateTime::currentDateTime().toMSecsSinceEpoch() - debutpause.toMSecsSinceEpoch())*100.0/pause->getEchelle()/3600000.0;
        pause->setSpanAngle(spanangle);
        QObject *pausebar = parent->findChild<QObject*>("progressbar");
        qreal spanpourc = (QDateTime::currentDateTime().toMSecsSinceEpoch() - debutpause.toMSecsSinceEpoch())*100.0/REPAS/60000.0;
        pausebar->setProperty("value",spanpourc);
        if((QDateTime::currentDateTime().toMSecsSinceEpoch() - debutpause.toMSecsSinceEpoch())>=REPAS*60*1000 ){
            QObject *btncenter = parent->findChild<QObject*>("btncenter");
            btncenter->setProperty("enabled",true);
            REPAS+=15;
            QObject *endmin = parent->findChild<QObject*>("endmin");
            endmin->setProperty("text",QString::number(REPAS)+" Min");
            QObject *currentvalue = parent->findChild<QObject*>("currentvalue");
            currentvalue->setProperty("text",QString::number(spanpourc*60/100)+" Min");
        }
    }
    else {
        pausetimer->stop();
    }
}

void ApplicationSignal::createReport(){

}

void ApplicationSignal::opendetail(QVariant val){
    QList<Report*> dataList;
    QList<Absence*> dataAbsList;


    DetailDialog *dialog = new DetailDialog();
    //BullDialog *dialog = new BullDialog();
    //qDebug() << "count1 : " << reports.size();
    //dialog->setData(dataList);
    dialog->exec();
}
