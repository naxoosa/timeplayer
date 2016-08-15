#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QObject>


class Employee: public QObject
{
    Q_OBJECT
public:
    explicit Employee(QObject *parent = 0);
    int getId(){return id;}
    void setId(int id){this->id=id;}
    QString getEmpSign(){return empsign;}
    void setEmpSign(QString empsign){this->empsign=empsign;}
    QString getEmpKey(){return empkey;}
    void setEmpKey(QString empkey){this->empkey=empkey;}

signals:

public slots:

private:
    int id = 0;
    QString empsign;
    QString empkey;
};

#endif // EMPLOYEE_H
