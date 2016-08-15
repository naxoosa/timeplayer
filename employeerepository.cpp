#include "employeerepository.h"

EmployeeRepository::EmployeeRepository(QObject *parent) : QObject(parent)
{
    db = DatabaseConfig::Instance()->getNaxooPlayerDb();
}

Employee &EmployeeRepository::findById(int id){
    Employee *employee = NULL;
    if(!db.isOpen())
    db.open();
    QSqlQuery select(db);
    select.prepare("SELECT * FROM rh_employee WHERE id=:id");
    select.bindValue(":id",id);
    select.exec();
    if(select.next()){
        employee = new Employee();
        employee->setId(select.value(0).toInt());
        employee->setEmpSign(select.value(1).toString());
    }    
    select.clear();
    select.finish();
    db.close();
    return *employee;
}

Employee &EmployeeRepository::findByEmpSign(QString empsign){
    Employee *employee = NULL;
    if(!db.isOpen())
    db.open();
    QSqlQuery select(db);
    select.prepare("SELECT * FROM rh_employee WHERE empsign=:empsign");
    select.bindValue(":empsign",empsign);
    select.exec();
    if(select.next()){
        employee = new Employee();
        employee->setId(select.value(0).toInt());
        employee->setEmpSign(select.value(1).toString());
    }

    select.clear();
    select.finish();
    db.close();
    return *employee;
}

Employee &EmployeeRepository::findByEmpKey(QString empkey){
    Employee *employee = NULL;
    if(!db.isOpen())
    db.open();
    QSqlQuery select(db);
    select.prepare("SELECT * FROM rh_employee WHERE empkey=:empkey");
    select.bindValue(":empsign",empkey);
    select.exec();
    if(select.next()){
        employee = new Employee();
        employee->setId(select.value(0).toInt());
        employee->setEmpSign(select.value(1).toString());
    }

    select.clear();
    select.finish();
    db.close();
    return *employee;
}
