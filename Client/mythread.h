#ifndef MYTHREAD
#define MYTHREAD

#include <QThread>
#include <QTimer>
#include "../Client/IClient.h"
#include "mainwindow.h"

class MyThread : public QObject
{
     Q_OBJECT
signals:
    void workFinished();
public:
    explicit MyThread(IGui *gui, std::string const& username);
    void    setThread(QThread* thread);
public slots:
    void run();
protected:
    IGui *gui;
    std::string username;
    QThread *thread;
};

#endif // MYTHREAD

