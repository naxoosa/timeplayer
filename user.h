#ifndef USER_H
#define USER_H

#include <QObject>
#include "singleton.h"

class User : public QObject
{
    Q_OBJECT
public:
    explicit User(QObject *parent = 0);
    int getId(){ return id; }
    void setId(int id){this->id=id;}
    QString getUserName(){ return username; }
    void setUserName(QString username){this->username=username;}
    QString getLastName(){ return lastname; }
    void setLastName(QString lastname){this->lastname=lastname;}
    QString getFirstName(){ return firstname; }
    void setFirstName(QString firstname){this->firstname=firstname;}
    int getTaux(){ return taux; }
    void setTaux(int taux){this->taux=taux;}
    int getReportId(){ return reportid; }
    void setReportId(int reportid){this->reportid=reportid;}
    QString getUserMail(){ return email; }
    void setUserMail(QString email){this->email=email;}
    QString getManagerMail(){ return emailmanager; }
    void setManagerMail(QString emailmanager){this->emailmanager=emailmanager;}
    bool isSimplify() { return issimplify;}
    void setIsSimplify(bool issimplify) {this->issimplify = issimplify;}

signals:

public slots:

private:
    int id;
    QString username;
    QString lastname;
    QString firstname;
    int taux;
    int reportid;
    QString email;
    QString emailmanager;
    bool issimplify;
};

typedef Singleton<User> CurrentUser;

#endif // USER_H
