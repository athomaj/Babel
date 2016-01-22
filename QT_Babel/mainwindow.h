#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtMultimedia/QMediaPlayer>
#include "IGui.h"
#include "../Client/HandleConnection.h"

class PortAudio;
class HandleConnection;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public IGui
{
	Q_OBJECT
signals:
	void connectSignal();
	void disconnectSignal();
	void rcvCallSignal();
	void askCallAsHost();
	void askCallAsClient();
	void callEstablished();
	void endCallSignal();
    void running();
public:
    explicit MainWindow(QWidget *parent = 0);
   virtual void setListitems(std::list<std::string> &list);
   virtual std::string getCurrentTAction();
   virtual void    setCurrentTAction(std::string);
   virtual void receiveCall(std::string &name);
   virtual ~MainWindow();

    virtual void onConnect(Packet *);
    virtual void onDisconnnect(Packet *);
    virtual void onAskContact(Packet *);
    virtual void onAddContact(Packet *);
    virtual void onDelContact(Packet *);
    virtual void onAskCall(Packet *);
    virtual void onRcvCall(Packet *);
    virtual void onCallEst(Packet *);
    virtual void onCallFailed(Packet *);
    virtual void onEndCall(Packet *);
    virtual void setClient(IClient *);
    void getDirPath();

    HandleConnection *_hc;

protected slots:
	void onConnectSignal();
	void onDisconnectSignal();
	void onRcvCallSignal();
	void onAskCallAsHost();
	void onAskCallAsClient();
	void onCallEstablished();
	void onEndCallSignal();
    void   quitThread();

	virtual void  userLogin();
    virtual void  disconnectUser();
    //void waitingUser();
   virtual void callUser();
   virtual void refuseCall();
   virtual void quitGUI();
  virtual  void microphoneState();
   virtual void acceptCall();


private:
    Ui::MainWindow *ui;
    std::string currentTAction;
    bool    microOn;
    QMediaPlayer* musicPlayer;
    bool   _isHost;
    std::string comIp;
    int comPort;
    std::string callerName;
    IClient* client;
    std::string curPath;
    QThread * thread;
};

#include "../Client/mythread.h"

#include "../Client/portAudio.hh"

#endif // MAINWINDOW_H
