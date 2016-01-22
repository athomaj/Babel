#ifndef _IGUI_H_
# define _IGUI_H_

#include <iostream>
#include <list>
#include "../Client/Packet.h"

class IClient;

class	IGui
 {
  public:
    virtual ~IGui() {};
    virtual void  userLogin() = 0;
    virtual void  disconnectUser() = 0;
    virtual void callUser() = 0;
    virtual void refuseCall() = 0;
    virtual void quitGUI() = 0;
    virtual void microphoneState() = 0;
    virtual void acceptCall() = 0;
    virtual void setListitems(std::list<std::string> &list) = 0;
    virtual std::string getCurrentTAction() = 0;
    virtual void    setCurrentTAction(std::string) = 0;
    virtual void receiveCall(std::string &name) = 0;

    virtual void onConnect(Packet *) = 0;
    virtual void onDisconnnect(Packet *) = 0;
    virtual void onAskContact(Packet *) = 0;
    virtual void onAddContact(Packet *) = 0;
    virtual void onDelContact(Packet *) = 0;
    virtual void onAskCall(Packet *) = 0;
    virtual void onRcvCall(Packet *) = 0;
    virtual void onCallEst(Packet *) = 0;
    virtual void onCallFailed(Packet *) = 0;
    virtual void onEndCall(Packet *) = 0;
    virtual void setClient(IClient *) = 0;

};

#include "../Client/IClient.h"

#endif
