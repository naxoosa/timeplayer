#ifndef ABSREPOSITORY_H
#define ABSREPOSITORY_H

#include <QObject>
#include <assert.h>
#include <QtSql>
#include "singleton.h"
#include "absence.h"
#include "configuration.h"


class AbsRepository :public QObject
{
    Q_OBJECT
public:
    explicit AbsRepository(QObject *parent = 0);
    QList<Absence *> findByUserId(int userid);
    QList<Absence *> findByUserIdMonthYear(int userid,int month, int year);

private:
    QSqlDatabase db;
};

typedef Singleton<AbsRepository> RepositoryAbs;

#endif // ABSREPOSITORY_H
