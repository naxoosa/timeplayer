#ifndef SINGLEAPPLICATION_H
#define SINGLEAPPLICATION_H


#include <QApplication>
#include <QSharedMemory>
#include <QStringList>

class SingleApplication : public QApplication
{
        Q_OBJECT
public:
        SingleApplication(int &argc, char *argv[], const QString uniqueKey);

        bool alreadyExists() const{
            return bAlreadyExists;
        }
        bool isMasterApp() const{
            return !alreadyExists();
        }

        bool sendMessage(const QString &message);

public slots:
        void checkForMessage();
        void test();

signals:
        void messageAvailable(const QStringList& messages);

private:
        bool bAlreadyExists;
        QSharedMemory sharedMemory;
};

#endif // SINGLEAPPLICATION_H
