#include "Action.hh"

TAction::TAction()
{
    actions[CONNECTION] = &IGui::onConnect;
    actions[DECONNECTION] = &IGui::onDisconnnect;
    actions[ASKCONTACT] = &IGui::onAskContact;
    actions[ADDCONTACT] = &IGui::onAddContact;
    actions[DELCONTACT] = &IGui::onDelContact;
    actions[ASKCALL] = &IGui::onAskCall;
    actions[RCVCALL] = &IGui::onRcvCall;
    actions[CALLESTABLISHED] = &IGui::onCallEst;
    actions[CALLFAILED] = &IGui::onCallFailed;
    actions[ENDCALL] = &IGui::onEndCall;
}

TAction::~TAction()
{
	
}

actFptr TAction::getAction(Instruct const inst)
{
	std::map<Instruct, actFptr>::iterator it;
	it = actions.find(inst);
	if (it != actions.end())
		return (it->second);
	return (NULL);
}

void TAction::onAskContact(IGui *gui)
{
    std::cout << "asking contacts" << std::endl;
}

void TAction::onConnect(IGui *_gui)
{
}

void TAction::onDisconnnect(IGui *gui)
{
	std::cout << "deco" << std::endl;

}

void TAction::onAddContact(IGui *gui)
{
	std::cout << "addcont" << std::endl;

}

void TAction::onDelContact(IGui *gui)
{
	std::cout << "delcont" << std::endl;

}

void TAction::onRcvCall(IGui *gui)
{
	std::cout << "rcvcall" << std::endl;

}

void TAction::onCallEst(IGui *gui)
{
	std::cout << "callest" << std::endl;

}

void TAction::onCallFailed(IGui *gui)
{
	std::cout << "callfailed" << std::endl;

}

void TAction::onEndCall(IGui *gui)
{
	std::cout << "endcall" << std::endl;

}
