#include <QApplication>
#include <QQmlApplicationEngine>
#include <applicationsignal.h>
#include <QAction>
#include <QMenu>
#include <QSystemTrayIcon>
#include <window.h>
#include "items/CustomProgressBar.h"
#include "singleapplication.h"
#include <QMessageBox>

int main(int argc, char *argv[])
{
    //QApplication app(argc, argv);
    SingleApplication  app(argc, argv, "TimePlayer");

    if(app.alreadyExists()){
        qDebug() << "Deja ouvert";
        return 0;
    }

    // Initialisation du curseur;
    qmlRegisterType<CustomProgressBar>("CustomProgressBar", 1, 0, "CustomProgressBar");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    ApplicationSignal appsignal(&engine);

    QObject *root = engine.rootObjects().first();    

    if(DatabaseConfig::Instance()->getCurrentUser()!=NULL){
        qDebug()<<DatabaseConfig::Instance()->getCurrentUser()->getId();
        QObject *player = root->findChild<QObject*>("player");
        //player->setProperty("parentplayer",QVariant::fromValue(root));
        if(DatabaseConfig::Instance()->getCurrentUser()->isSimplify()){            
            player->setProperty("issimplify",true);
            qDebug()<<"isSimp";
        }

        QDateTime now = QDateTime::currentDateTime();
        Report *report = RepositoryReport::Instance()->findByUserIdAndDate(DatabaseConfig::Instance()->getCurrentUser()->getId(),now);

        if(report!=NULL && !report->getEndDate().isNull()){
            QMessageBox *messageBox = new QMessageBox();
            messageBox->setIcon(QMessageBox::Critical);
            messageBox->setText("Il existe déjà un report pour aujourd'hui.");
            messageBox->setInformativeText("Veuillez contacter le service RH !");
            messageBox->exec();
            return 0;
        }
        else{
            CustomProgressBar *reportbar = root->findChild<CustomProgressBar*>("playprogressbar");
            QObject *clock = root->findChild<QObject*>("clock");
            if(report!=NULL){                
                QDateTime startreport = report->getStartDate();
                // On définit la playprogressbar
                DatabaseConfig::Instance()->getCurrentUser()->setReportId(report->getId());
                double echelle = 8.0;                
                QList<Pause *> *list = RepositoryPause::Instance()->findByReportId(report->getId());                
                report->setPauses(list);

                for(int i=0;i<list->size();i++){
                    Pause *p = list->at(i);
                    if(p->getTypeId()==2)
                        echelle+=(p->getDuration()/3600);
                }

                for(int i=0;i<list->size();i++){
                    Pause *p = list->at(i);
                    QDateTime startpause = p->getStartTime();
                    qreal startangle = (startpause.toMSecsSinceEpoch()-startreport.toMSecsSinceEpoch())/echelle/36/1000;
                    qreal spanangle = p->getDuration()*1000/echelle/36/1000;
                    QQmlComponent pausecomponent(&engine, QUrl(QStringLiteral("qrc:/items/progressbar.qml")));
                    CustomProgressBar *pobj = qobject_cast<CustomProgressBar *>(pausecomponent.create());
                    QQmlApplicationEngine::setObjectOwnership(p, QQmlApplicationEngine::JavaScriptOwnership);
                    pobj->setWidth(319);
                    pobj->setHeight(319);
                    if(p->getTypeId()==2)
                        pobj->setColor(QColor("#d07f58"));
                    else
                        pobj->setColor(QColor("#ce8d1b"));
                    pobj->setStartAngle(startangle);
                    pobj->setSpanAngle(spanangle);
                    pobj->setVisible(true);
                    pobj->setParentItem(reportbar);
                    pobj->setEchelle(reportbar->getEchelle());
                    appsignal.pausesobject.append(pobj);
                }

                int restant = (startreport.addSecs(echelle*3600).toMSecsSinceEpoch() - now.toMSecsSinceEpoch())/1000;

                double delta = (now.toMSecsSinceEpoch()-startreport.toMSecsSinceEpoch())/echelle/36/1000;

                reportbar->setColor(QColor("#00b7ee"));
                reportbar->setStartAngle(0);
                reportbar->setSpanAngle(delta);
                reportbar->setEchelle(echelle);

                // On définit le compteur
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

                appsignal.report = report;

                QObject *btncenter = root->findChild<QObject*>("btncenterm");
                QVariant returnedValue;
                QVariant msg = 1;
                QMetaObject::invokeMethod(btncenter, "sendClick",Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, msg));
                QJSEngine jsEngine;
                QJSValue messg = jsEngine.newObject();
                messg.setProperty("action","startreport");
                emit appsignal.sendmessage(QVariant::fromValue(messg));
            }


        }
    }
    else {
        QMessageBox *messageBox = new QMessageBox();
        messageBox->setIcon(QMessageBox::Critical);
        messageBox->setText("Vous n'êtes pas connu dans le Système.");
        messageBox->setInformativeText("Veuillez contacter le service RH !");
        messageBox->exec();
        return 0;
    }

    /* Définition du systray, incompatible pour le moment avec QML

    Window window;
    window.show();

    QAction *minimizeAction = new QAction(QObject::tr("Mi&nimize"), root);
    root->connect(minimizeAction, SIGNAL(triggered()), root, SLOT(hide()));
    QAction *maximizeAction = new QAction(QObject::tr("Ma&ximize"), root);
    root->connect(maximizeAction, SIGNAL(triggered()), root, SLOT(showMaximized()));
    QAction *restoreAction = new QAction(QObject::tr("&Restore"), root);
    root->connect(restoreAction, SIGNAL(triggered()), root, SLOT(showNormal()));
    QAction *quitAction = new QAction(QObject::tr("&Quit"), root);
    root->connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    QMenu *trayIconMenu = new QMenu();
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(root);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(QIcon(":/contents/Clock22x22.png"));
    trayIcon->show();
    */

    QObject::connect(root, SIGNAL(sendmessage(QVariant)),
                     &appsignal, SLOT(sendmessage(QVariant)));
    QObject::connect(root, SIGNAL(opendetail(QVariant)),
                     &appsignal, SLOT(opendetail(QVariant)));

    return app.exec();
}
