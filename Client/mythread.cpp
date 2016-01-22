#include "mythread.h"

MyThread::MyThread(IGui *gui, std::string const& username) :
    gui(gui), username(username)
{
//    QTimer *timer = new QTimer(this);
//    timer->start(100);
}

void    MyThread::setThread(QThread* thread)
{
    thread = thread;
}

void MyThread::run()
{
    IClient *co = NULL;
     HandleConnection hc;
#ifdef _WIN32
     co = new Connection_Win(username);
     std::cout << "* Running in Windows env *" << std::endl;
	 dynamic_cast<MainWindow *>(gui)->_hc = &hc;
#else
     co = new Connection_Lin(username);
     std::cout << "* Running in UNIX env *" << std::endl;
     dynamic_cast<MainWindow *>(gui)->_hc = &hc;
#endif

     if (co->connection(4242, "10.20.86.166") == true)
             std::cout << "Worked" << std::endl;
     else
             std::cout << "blop" << std::endl;

     PortAudio 	audio(co);
      audio.initialize();
      audio.initInputDevice();
      audio.initOutputDevice();
    co->setAudio(&audio);
    gui->setClient(co);
     hc.handle(co, gui);
}
