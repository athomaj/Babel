#ifndef TACTION_HH_
# define TACTION_HH_

# include <iostream>
# include <map>
# include "Identity.hpp"

class IGui;
class Packet;


typedef void (IGui::*actFptr)(Packet *);

class TAction
{
public:
    TAction();
    ~TAction();
    actFptr getAction(Instruct const);
    static void onAskContact(IGui *);
    static void onConnect(IGui *);
    static void onDisconnnect(IGui *);
    static void onAddContact(IGui *);
    static void onDelContact(IGui *);
    static void onRcvCall(IGui *);
    static void onCallEst(IGui *);
    static void onCallFailed(IGui *);
    static void onEndCall(IGui *);
private:
	std::map<Instruct, actFptr> actions;
};

# include "mainwindow.h"


#endif
