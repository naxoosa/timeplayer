#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QObject>
#include <QtSql>
#include "singleton.h"
#include "user.h"
#include "employeerepository.h"

class Configuration : public QObject
{
    Q_OBJECT
public:
    explicit Configuration(QObject *parent = 0);
    QString getDriver(){return driver;}
    QString getHost(){return host;}
    QString getDbName(){return dbname;}
    QString getUser(){return  user;}
    QString getPassword(){return pwd;}
    QString getDriverRH(){return driverrh;}
    QString getHostRH(){return hostrh;}
    QString getDbNameRH(){return dbnamerh;}
    QString getUserRH(){return userrh;}
    QString getPasswordRH(){return pwdrh;}
    int getHeures(){return heures;}
    QSqlDatabase getNaxooPlayerDb(){return naxoodb;}
    QSqlDatabase getSIRHDb(){return sirhdb;}
    User *getCurrentUser(){return currentuser;}


signals:

public slots:

private:
    QString driver;
    QString host;
    int port;
    QString dbname;
    QString user;
    QString pwd;
    QString driverrh;
    QString hostrh;
    QString dbnamerh;
    QString userrh;
    QString pwdrh;
    int heures;
    QSqlDatabase naxoodb;
    QSqlDatabase sirhdb;
    User *currentuser;
};

typedef Singleton<Configuration> DatabaseConfig;


#endif // CONFIGURATION_H
