#include "configuration.h"
#include <QSettings>

Configuration::Configuration(QObject *parent) : QObject(parent)
{
    QSettings settings("TimePlayer.ini", QSettings::IniFormat);

    driver = settings.value("DATABASE/driver").toString();
    host = settings.value("DATABASE/host").toString();
    port = settings.value("DATABASE/port").toInt();
    dbname = settings.value("DATABASE/database").toString();
    user = settings.value("DATABASE/user").toString();
    pwd = settings.value("DATABASE/pwd").toString();

    driverrh = settings.value("DATABASERH/driver").toString();
    hostrh = settings.value("DATABASERH/host").toString();
    dbnamerh = settings.value("DATABASERH/database").toString();
    userrh = settings.value("DATABASERH/user").toString();
    pwdrh = settings.value("DATABASERH/pwd").toString();    

    heures = 8;

    if(driver!= NULL)
        naxoodb = QSqlDatabase::addDatabase(driver,dbname);

    if(host!=NULL)
        naxoodb.setHostName(this->host);

    if(port!=NULL&& port>0)
        naxoodb.setPort(this->port);
    else
        naxoodb.setPort(3306);

    if(dbname!= NULL)
        naxoodb.setDatabaseName(this->dbname);

    if(user!= NULL)
        naxoodb.setUserName(this->user);

    if(pwd!= NULL)
        naxoodb.setPassword(this->pwd);

    if(driverrh!= NULL)
        sirhdb = QSqlDatabase::addDatabase(driverrh,dbnamerh);

    if(hostrh!=NULL)
        sirhdb.setHostName(this->hostrh);

    if(dbnamerh!= NULL)
        sirhdb.setDatabaseName(this->dbnamerh);

    if(userrh!= NULL)
        sirhdb.setUserName(this->userrh);

    if(pwd!= NULL)
        sirhdb.setPassword(this->pwdrh);    

    bool ok = naxoodb.open();
    if(!ok){
        qCritical() << naxoodb.lastError().text();
    }    

    ok = sirhdb.open();
    if(!ok){
        qCritical() << sirhdb.lastError().text();
    }

    QString name = getenv("USERNAME");
    QSqlQuery selectuser(sirhdb);
    selectuser.prepare("SELECT * FROM HRMAEC WHERE EMPSIGN=:3g");
    selectuser.bindValue(":3g",name);
    selectuser.exec();

    while(selectuser.next()){
        currentuser = CurrentUser::Instance();
        currentuser->setId(selectuser.value("EMPKEY").toInt());
        currentuser->setLastName(selectuser.value("ACEEMPN").toString());
        currentuser->setFirstName(selectuser.value("ACEEMPFN").toString());
        currentuser->setTaux(selectuser.value("AECPARTP").toInt());
        currentuser->setUserMail(selectuser.value("AECEMAIL").toString());

        //Employee &emp = RepositoryEmployee::Instance()->findByEmpSign(selectuser.value("EMPKEY").toString());

        QSqlQuery selectrh(naxoodb);
        selectrh.prepare("SELECT * FROM rh_employee WHERE empkey=:empkey");
        selectrh.bindValue(":empkey",selectuser.value("EMPKEY").toString());
        selectrh.exec();

        if(selectrh.next())
            currentuser->setIsSimplify(true);
        else
            currentuser->setIsSimplify(false);

        //qDebug() << "is : " << currentuser->isSimplify();

        QSqlQuery selectou(sirhdb);
        selectou.prepare("SELECT ORGPNTOU FROM HRMORG WHERE ORSID = 1 and ORGCHDOU in (SELECT B.OUNKEY FROM HRMASN A, HRMOUN B, HRMAEC D  WHERE A.EMPKEY =:empkey AND A.OUNKEY = B.OUNKEY AND A.EMPKEY = D.EMPKEY and A.CONNUMB = D.CONNUMB) and ORGUNTIL>= GETDATE()");
        selectou.bindValue(":empkey",currentuser->getId());
        selectou.exec();

        if(selectou.next()){           

            QSqlQuery selectemail(sirhdb);
            selectemail.prepare("SELECT AECEMAIL FROM HRMASN A, HRMOUN B, HRMAEC D , HRMAST E WHERE A.OUNKEY = B.OUNKEY AND A.EMPKEY = D.EMPKEY and A.CONNUMB = D.CONNUMB  and E.astID = A.ASTID and ASTLEVEL=1 and A.OUNKEY=:ou");
            selectemail.bindValue(":ou",selectou.value("ORGPNTOU").toInt());
            selectemail.exec();

            if(selectemail.next()){
                currentuser->setManagerMail(selectemail.value("AECEMAIL").toString());
                currentuser->setManagerMail("tgovin@naxoo.ch");
            }

            selectemail.clear();
            selectemail.finish();
        }

        selectou.clear();
        selectou.finish();

    }

    selectuser.clear();
    selectuser.finish();
    sirhdb.close();
    //sirhdb.removeDatabase(dbnamerh);

}
