#ifndef EMPLOYEEREPOSITORY_H
#define EMPLOYEEREPOSITORY_H

#include <QObject>
#include <assert.h>
#include <QtSql>
#include "singleton.h"
#include "employee.h"
#include "configuration.h"

class EmployeeRepository: public QObject
{
    Q_OBJECT
public:
    explicit EmployeeRepository(QObject *parent = 0);
    Employee &findById(int id);
    Employee &findByEmpSign(QString empsign);
    Employee &findByEmpKey(QString empkey);
    void create(Employee &employee);
    int save(Employee &employee);

signals:

public slots:

private:
    QSqlDatabase db;
};

typedef Singleton<EmployeeRepository> RepositoryEmployee;

#endif // EMPLOYEEREPOSITORY_H
