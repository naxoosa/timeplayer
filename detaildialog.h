#ifndef DETAILDIALOG_H
#define DETAILDIALOG_H

#include <QDialog>
#include <QCalendarWidget>
#include <QTableView>
#include <QTextCharFormat>
#include <QStandardItemModel>
#include "configuration.h"
#include "reportrepository.h"
#include "pauserepository.h"
#include "absrepository.h"
#include "employeerepository.h"

namespace Ui {
class DetailDialog;
}

class DetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DetailDialog(QWidget *parent = 0);
    ~DetailDialog();

public slots:
    void currentPageChanged(int year, int month);
    void clicked(const QDate &date);

private:
    Ui::DetailDialog *ui;
    ReportRepository *repositoryreport;
    PauseRepository *repositorypause;
    AbsRepository *repositoryabs;
    EmployeeRepository * repositoryemployee;
    QCalendarWidget *calendar;
    QTableView *tableview;
};

#endif // DETAILDIALOG_H
