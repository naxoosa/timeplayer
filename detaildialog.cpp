#include "detaildialog.h"
#include "ui_detaildialog.h"

DetailDialog::DetailDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DetailDialog)
{
    ui->setupUi(this);

    repositoryreport = RepositoryReport::Instance();
    repositorypause = RepositoryPause::Instance();
    repositoryabs = RepositoryAbs::Instance();
    repositoryemployee = RepositoryEmployee::Instance();

    QList<Report*> reports = repositoryreport->findByUserId(DatabaseConfig::Instance()->getCurrentUser()->getId());
    QDate dt = QDate::currentDate();

    QList<Absence *> listabs = repositoryabs->findByUserIdMonthYear(DatabaseConfig::Instance()->getCurrentUser()->getId(),dt.month(),dt.year());

    calendar = findChild<QCalendarWidget*>("calendarWidget");
    connect(calendar,SIGNAL(currentPageChanged(int,int)), this, SLOT(currentPageChanged(int,int)));
    connect(calendar,SIGNAL(clicked(QDate)), this, SLOT(clicked(QDate)));    
    QTextCharFormat format = calendar->weekdayTextFormat(Qt::Monday);
    QList<Report*> dataList = reports;
    if(!dataList.isEmpty()){
        qDebug() << dataList.size();
        for(int i=0;i<dataList.size();i++){
            Report *r = (Report*)dataList.at(i);
            QList<Pause*> *listmodel = repositorypause->findByReportId(r->getId());
            if(r->getEndDate().isNull()){
                format.setBackground(QBrush(Qt::red, Qt::SolidPattern));
            }
            else{
                double delta = r->getEndDate().toMSecsSinceEpoch()-r->getStartDate().toMSecsSinceEpoch();
                bool is30=false;
                for(int i=0;i< listmodel->size();i++)
                {
                    if(listmodel->at(i)->getTypeId()==2){
                        delta-=listmodel->at(i)->getDuration()*1000;
                        is30=true;
                    }
                }

                if(delta>= 8*3600000*0.99 && delta<=8*3600000*1.01){
                    if(&repositoryemployee->findByEmpKey(""+r->getUserId())!= NULL)
                        format.setBackground(QBrush(Qt::green, Qt::SolidPattern));
                    else{
                        if(listmodel->size()==3 && is30){
                            format.setBackground(QBrush(Qt::green, Qt::SolidPattern));
                        }
                        else{
                            format.setBackground(QBrush(Qt::yellow, Qt::SolidPattern));
                        }
                    }
                }
                else{
                    format.setBackground(QBrush(Qt::red, Qt::SolidPattern));
                }
            }

            calendar->setDateTextFormat(r->getStartDate().date(), format);
        }
    }

    if(!listabs.isEmpty()){
        qDebug() << listabs.size();
        for(int i=0;i<listabs.size();i++){
            Absence *abs = (Absence*)listabs.at(i);
            if(abs->getAbsType()==11)
                format.setBackground(QBrush(Qt::blue, Qt::SolidPattern));
            else
                format.setBackground(QBrush("#bfdbbf", Qt::SolidPattern));

            calendar->setDateTextFormat(abs->getAbsDate().date(), format);
        }
    }

    QStandardItemModel *model = new QStandardItemModel(1,2,this);
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Début de la pause")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Fin de la pause")));

    tableview = findChild<QTableView*>("pauseview");
    tableview->setModel(model);

}

void DetailDialog::currentPageChanged(int year, int month){
    QList<Absence *> listabs = repositoryabs->findByUserIdMonthYear(DatabaseConfig::Instance()->getCurrentUser()->getId(),month,year);

    QTextCharFormat format = calendar->weekdayTextFormat(Qt::Saturday);

    if(!listabs.isEmpty()){
        qDebug() << listabs.size();
        for(int i=0;i<listabs.size();i++){
            Absence *abs = (Absence*)listabs.at(i);
            if(abs->getAbsType()==11)
                format.setBackground(QBrush(Qt::blue, Qt::SolidPattern));
            else
                format.setBackground(QBrush("#bfdbbf", Qt::SolidPattern));

            calendar->setDateTextFormat(abs->getAbsDate().date(), format);
        }
    }

}

void DetailDialog::clicked(const QDate & date){
    Report *r = repositoryreport->findByUserIdAndDate(DatabaseConfig::Instance()->getCurrentUser()->getId(),QDateTime(date));

    if(r!=NULL){
    QLabel *startreport = findChild<QLabel*>("startreport");
    QLabel *endreport = findChild<QLabel*>("endreport");

    if(startreport!=NULL&&!r->getStartDate().isNull())
        startreport->setText(r->getStartDate().time().toString(("hh:mm")));
    else
        startreport->setText("--:--");
    if(endreport!=NULL&&!r->getEndDate().isNull())
        endreport->setText(r->getEndDate().time().toString(("hh:mm")));
    else
        endreport->setText("--:--");

    }

    QStandardItemModel *model = new QStandardItemModel(0,2,this);
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Début de la pause")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Fin de la pause")));
    if(r!=NULL){
        QList<Pause*> *listmodel = repositorypause->findByReportId(r->getId());

        model = new QStandardItemModel(listmodel->size(),2,this);
        model->setHorizontalHeaderItem(0, new QStandardItem(QString("Début de la pause")));
        model->setHorizontalHeaderItem(1, new QStandardItem(QString("Fin de la pause")));

        for(int i=0;i<listmodel->size();i++){
            Pause *p = listmodel->at(i);
            QStandardItem *firstRowCol1 = new QStandardItem(p->getStartTime().time().toString("hh:mm"));
            model->setItem(i,0,firstRowCol1);
            QStandardItem *firstRowCol2 = new QStandardItem(p->getEndTime().time().toString("hh:mm"));
            model->setItem(i,1,firstRowCol2);
        }
    }


    tableview->setModel(model);
}


DetailDialog::~DetailDialog()
{
    delete ui;
}

