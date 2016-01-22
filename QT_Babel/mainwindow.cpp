#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <iostream>
#include <QWidget>
#include <QDir>
#include <QDebug>
#include <QCoreApplication>
#include <thread>
#include "../Client/HandleConnection.h"

#ifdef _WIN32
# include <direct.h>
# include "../Client/Connection_Win.h"
#else
# include <iostream>
# include "../Client/Connection_Lin.h"
#endif

void MainWindow::getDirPath()
{
    char buf[4096];

    memset (buf, 0, 4096);
    #ifdef _WIN32
        _getcwd(buf, 4096);
    #else
        getcwd(buf, 4096);
    #endif
    std::string fullPath(buf);
    unsigned int pos = fullPath.rfind("cpp_babel");
    curPath = fullPath.substr(0, pos + sizeof("cpp_babel"));
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _isHost(false)
{
    getDirPath();
    //QWidget::setWindowTitle(QString("Babel"));
     this->microOn = true;
     ui->setupUi(this);
     setFixedSize(600, 600);
     ui->chekUser->setEchoMode(QLineEdit::Password);
     std::string pathPhone = curPath;
     pathPhone += "QT_Babel/Images/phone.png";

     QPixmap pixmap3(pathPhone.c_str());
     QIcon icon(pixmap3);

     this->ui->call_button->setIconSize(QSize(60,60));
     this->ui->call_button->setIcon(icon);
     std::string pathPlus = curPath;
     pathPlus += "QT_Babel/Images/plus.png";
     QPixmap pixmap5(pathPlus.c_str());
     QIcon icon2(pixmap5);
     this->ui->plus_buton->setIconSize(QSize(30,30));
     this->ui->plus_buton->setIcon(icon2);

     std::string pathMicro = curPath;
     pathMicro += "QT_Babel/Images/micro.png";
     QPixmap pixmap6(pathMicro.c_str());
     QIcon icon3(pixmap6);
      this->ui->micro_button->setIconSize(QSize(40,40));
     this->ui->micro_button->setIcon(icon3);

     //QPixmap pixmap7("./Images/phoneuncall.png");
     std::string pathPhoneInCall = curPath;
     pathPhoneInCall += "QT_Babel/Images/phoneuncall.png";
     QIcon icon4(pathPhoneInCall.c_str());
     this->ui->racroch_button->setIconSize(QSize(60,60));
     this->ui->racroch_button->setIcon(icon4);

     std::string pathGens = curPath;
     pathGens += "QT_Babel/Images/gens.png";
     QPixmap pixmap8(pathGens.c_str());

     this->ui->userss->setPixmap(pixmap8);

     std::string pathBackGround = curPath;
     pathBackGround += "QT_Babel/Images/backgrond.png";

     QPixmap pixmap9(pathBackGround.c_str());
     this->ui->label_back->setPixmap(pixmap9);

     QPixmap pixmap10(pathBackGround.c_str());
     this->ui->label_back2->setPixmap(pixmap10);

     QPixmap pixmap11(pathBackGround.c_str());
     this->ui->label_back3->setPixmap(pixmap11);
    
     QPixmap pixmap12(pathBackGround.c_str());
     this->ui->background->setPixmap(pixmap12);

     std::string pathTitle = curPath;
     pathTitle += "QT_Babel/Images/babel_white.png";

     QPixmap pixmap16(pathBackGround.c_str());
     this->ui->label_ground->setPixmap(pixmap16);

     QPixmap pixmap17(pathTitle.c_str());
     this->ui->name_babel->setPixmap(pixmap17.scaled(150,150,Qt::KeepAspectRatio));

     QPixmap pixmap18(pathTitle.c_str());
     this->ui->label_7->setPixmap(pixmap18.scaled(150,150,Qt::KeepAspectRatio));

     QPixmap pixmap19(pathTitle.c_str());
     this->ui->label_10->setPixmap(pixmap19.scaled(150,150,Qt::KeepAspectRatio));

     QPixmap pixmap20(pathTitle.c_str());
     this->ui->label_13->setPixmap(pixmap20.scaled(150,150,Qt::KeepAspectRatio));

     QPixmap pixmap21(pathTitle.c_str());
     this->ui->name_logo->setPixmap(pixmap21.scaled(150,150,Qt::KeepAspectRatio));


     std::string pathSalut = curPath;
     pathSalut += "QT_Babel/Images/salut.png";
     QPixmap pixmap13(pathSalut.c_str());
     this->ui->label_11->setPixmap(pixmap13);

     std::string pathGrobold = curPath;
     pathGrobold += "QT_Babel/Font/GROBOLD.ttf";


      QFont f(pathGrobold.c_str());
      this->ui->name_babel->setFont(f);

    connect(ui->login, SIGNAL( clicked()), this, SLOT(userLogin()));
    connect(ui->button_disonnects, SIGNAL(clicked()), this, SLOT(disconnectUser()));
    connect(ui->discobutton, SIGNAL(clicked()), this, SLOT(disconnectUser()));
    connect(ui->call_button, SIGNAL(clicked()), this, SLOT(callUser()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(acceptCall()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(refuseCall()));
    connect(ui->racroch_button, SIGNAL(clicked()), this, SLOT(refuseCall()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(quitGUI()));
    connect(ui->micro_button, SIGNAL(clicked()), this, SLOT(microphoneState()));
    connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(refuseCall()));
	connect(this, SIGNAL(connectSignal()), this, SLOT(onConnectSignal()));
	connect(this, SIGNAL(disconnectSignal()), this, SLOT(onDisconnectSignal()));
	connect(this, SIGNAL(rcvCallSignal()), this, SLOT(onRcvCallSignal()));
	connect(this, SIGNAL(askCallAsHost()), this, SLOT(onAskCallAsHost()));
    connect(this, SIGNAL(askCallAsClient()), this, SLOT(onAskCallAsClient()));
	connect(this, SIGNAL(callEstablished()), this, SLOT(onCallEstablished()));
	connect(this, SIGNAL(endCallSignal()), this, SLOT(onEndCallSignal()));
}

void MainWindow::onConnectSignal()
{
	this->ui->stackedWidget->setCurrentIndex(2);
	setFixedSize(600, 600);
	ui->login_name->setText("Welcome " + ui->checkpass->text());
}

void MainWindow::onDisconnectSignal()
{
	this->ui->stackedWidget->setCurrentIndex(0);
	this->ui->listWidget->clear();
    #ifdef _WIN32
        closesocket(this->client->getFd());
    #else
        ::close(this->client->getFd());
    #endif
	setFixedSize(600, 600);
}

void MainWindow::onRcvCallSignal()
{
	this->ui->stackedWidget->setCurrentIndex(5);
	setFixedSize(600, 600);

//        this->musicPlayer = new QMediaPlayer;
//        musicPlayer->setVolume(100);
//        std::string pathIphone = curPath;
//        pathIphone += "QT_Babel/Images/iphone.mp3";
//       musicPlayer->setMedia(( QUrl::fromLocalFile( pathIphone.c_str() )));
//       musicPlayer->play();
}

void MainWindow::onAskCallAsHost()
{
    std::cout << "music" << std::endl;
    this->musicPlayer = new QMediaPlayer;
    musicPlayer->setVolume(100);
    std::string pathIphone = curPath;
    pathIphone += "QT_Babel/Images/phoneremix.mp3";
   musicPlayer->setMedia(( QUrl::fromLocalFile( pathIphone.c_str() )));
   musicPlayer->play();

   this->ui->stackedWidget->setCurrentIndex(1);
	setFixedSize(600, 600);
}

void MainWindow::onAskCallAsClient()
{
    std::cout << "music" << std::endl;
    this->musicPlayer = new QMediaPlayer;
    musicPlayer->setVolume(100);
    std::string pathIphone = curPath;
    pathIphone += "QT_Babel/Images/phoneremix.mp3";
   musicPlayer->setMedia(( QUrl::fromLocalFile( pathIphone.c_str() )));
   musicPlayer->play();

   this->ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::onCallEstablished()
{
    if (musicPlayer)
        musicPlayer->stop();
	this->ui->stackedWidget->setCurrentIndex(3);
	setFixedSize(780, 780);
}

void MainWindow::onEndCallSignal()
{
    if (musicPlayer)
        musicPlayer->stop();
	this->ui->stackedWidget->setCurrentIndex(2);
	setFixedSize(600, 600);
}

std::string getIpAddr()
{
#ifdef _WIN32
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
		return "";
    char ac[80];
	if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR)
		return ("");
	struct hostent *phe = gethostbyname(ac);
	if (phe == 0)
		return ("");
	for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
		struct in_addr addr;
		memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
		return inet_ntoa(addr);
	}
    return ("127.0.0.1");
#else
	int tmpFd;
	struct ifreq ifr;

	tmpFd = socket(AF_INET, SOCK_DGRAM, 0);
	ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, "en0", IFNAMSIZ - 1);
	ioctl(tmpFd, SIOCGIFADDR, &ifr);
	close(tmpFd);
	std::string ip(inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

	return (ip);
#endif
}

void MainWindow::setClient(IClient *cli)
{
    client = cli;
}

void   MainWindow::quitThread()
{
    std::cout << "quit" << std::endl;
    thread->quit();
}

void    MainWindow::userLogin()
{
    if(ui->checkpass->text().isEmpty() || ui->chekUser->text().isEmpty()){
        QMessageBox msgBox;
        msgBox.setWindowFlags(Qt::FramelessWindowHint);
        msgBox.setText("Enter an username and a valid password.");
        msgBox.exec();
        }
    else if(ui->chekUser->text().compare("epitech42") == 0)
        {
        MyThread *newThread = new MyThread(this, ui->checkpass->text().toStdString());
        thread = new QThread();

        connect(this, SIGNAL(running()), newThread, SLOT(run()));
        connect(newThread, SIGNAL(workFinished()), this, SLOT(quitThread()) );
        newThread->moveToThread(thread);
        thread->start();
        emit running();
    }
    else
    {
        QMessageBox msgBox2;
        msgBox2.setWindowFlags(Qt::FramelessWindowHint);
        msgBox2.setText("Enter an username and a valid password.");
        msgBox2.exec();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void    MainWindow::disconnectUser()
{
//	emit disconnectSignal();
    exit(0);
}

std::string    MainWindow::getCurrentTAction()
{
   return (currentTAction);
}

void    MainWindow::setCurrentTAction(std::string action)
{
    currentTAction = action;
}

void MainWindow::callUser()
{
    QListWidgetItem *tmp =  this->ui->listWidget->currentItem();
    if (tmp != NULL)
    {
    QString name = tmp->text();
      if (name.isEmpty() == false)
     {
//          this->musicPlayer = new QMediaPlayer;
//          musicPlayer->setVolume(100);
//          std::string pathAttente =  curPath;
//          pathAttente += "QT_Babel/Images/attente.mp3";
//         musicPlayer->setMedia(( QUrl::fromLocalFile( pathAttente.c_str() )));
//         musicPlayer->play();
          callerName = name.toStdString();
         std::string toCall = name.toStdString();
          _hc->askCall(toCall);
          _isHost = true;
     }
  }
}

void MainWindow::refuseCall()
{
    std::cout << "stop music" << std::endl;
  //  musicPlayer->stop();
    _hc->endCall(callerName);
}

void    MainWindow::quitGUI()
{
    exit(0);
}

void MainWindow::setListitems(std::list<std::string> &list)
{
    for (std::list<std::string>::iterator it = list.begin(); it != list.end(); ++it)
    {
        QString s((*it).c_str());
        this->ui->listWidget->addItem(s);
    }
}

void    MainWindow::microphoneState()
{
    this->microOn = !this->microOn;
    if (this->microOn == false)
    {
        std::cout << "red" << std::endl;
        this->ui->micro_button->setStyleSheet("background-color : #B40404; border-style : solid; border-width : 1px; border-radius : 40px; border-color : #298A08; max-width :80px; max-height:80px; min-width : 80px; min-height : 80px;");
    }
     else
    {
        std::cout << "green" << std::endl;
        this->ui->micro_button->setStyleSheet("background-color : #298A08; border-style : solid; border-width : 1px; border-radius : 40px; border-color : #298A08; max-width : 80px; max-height:80px; min-width : 80px; min-height : 80px;");

    }
}

void    MainWindow::acceptCall()
{
    this->ui->stackedWidget->setCurrentIndex(3);
    setFixedSize(780, 780);
    ui->nameofuser->setText(ui->checkpass->text());
    std::string tmp("Incall with ");
    tmp += callerName;
    ui->name_collegue->setText(tmp.c_str());
    _hc->co->connectionHost(comPort, comIp);
}

void MainWindow::receiveCall(std::string &name)
{
	QString s(name.c_str());
	QListWidgetItem *a = new QListWidgetItem(s);
	this->ui->listWidget->setCurrentItem(a);
	emit rcvCallSignal();
}

void MainWindow::onConnect(Packet *packet)
{
	emit connectSignal();
    std::cout << "onConnect" << std::endl;
/*    this->ui->stackedWidget->setCurrentIndex(2);
    setFixedSize(600, 600);
    ui->login_name->setText("Welcome " + ui->checkpass->text());*/
}

void MainWindow::onDisconnnect(Packet *packet)
{

}

void MainWindow::onAskContact(Packet *packet)
{

}

void MainWindow::onAddContact(Packet *packet)
{
    QString s(packet->getIdentity()->getUsername());
    this->ui->listWidget->addItem(s);
    std::cout << "adding contact" << std::endl;
}

void MainWindow::onDelContact(Packet *packet)
{
    int i;
    std::cout << "deleting item" << std::endl;
    QString s(packet->getIdentity()->getUsername());
    for(i = 0; i < this->ui->listWidget->count(); ++i)
    {
        QListWidgetItem* item = this->ui->listWidget->item(i);
		if (item->text() == s)
		{
			if (item->text().toStdString() == callerName)
				onEndCall(NULL);
			break;
		}
    }
    this->ui->listWidget->takeItem(i);
}

void MainWindow::onAskCall(Packet *packet)
{
    if (!_isHost)
    {
        std::cout << "asking call at port: " << packet->getIdentity()->getPort() << std::endl;
        comIp = packet->getIdentity()->getIp();
        comPort = packet->getIdentity()->getPort();
        callerName = std::string(packet->getIdentity()->getUsername());
		emit askCallAsClient();
//        this->ui->stackedWidget->setCurrentIndex(4);
    }
    else
    {
        _hc->co->create_server(4545);

        std::string name = packet->getIdentity()->getUsername();
		emit askCallAsHost();
		ui->name_wait->setText(ui->checkpass->text());
      std::string tmp("Awaiting for ");
       tmp += name;
       ui->name_wait->setText(tmp.c_str());
    }
}

void MainWindow::onRcvCall(Packet *packet)
{

}

void MainWindow::onCallEst(Packet *packet)
{
	emit callEstablished();
}

void MainWindow::onCallFailed(Packet *packet)
{

}

void MainWindow::onEndCall(Packet *packet)
{    
	emit endCallSignal();
//    ui->login_name->setText("Welcome " + ui->checkpass->text());
    callerName.clear();
    _isHost = false;
    client->resetClientFd();
    std::cout << ""
                 " music" << std::endl;
    std::cout << "ending calll" << std::endl;
}
