#ifndef MANAGECOMMAND_HH_
# define MANAGECOMMAND_HH_

# include <map>
# include "Packet.h"

class Client;

class ManageCommand
{
public:
	ManageCommand();
	~ManageCommand();
	void	whichInstruct(Identity& id, Client&);
private:
	void onConnect(Client&, Identity&);
	void onDisconnect(Client&, Identity&);
	void onAskContact(Client&, Identity&);
	void onAddContact(Client&, Identity&);
	void onDelContact(Client&, Identity&);
	void onAskCall(Client&, Identity&);
	void onRcvCall(Client&, Identity&);
	void onCallEst(Client&, Identity&);
	void onCallFailed(Client&, Identity&);
	void onEndCall(Client&, Identity&);
	void onOk(Client&, Identity&);
	void onKo(Client&, Identity&);
	std::map<Instruct, void (ManageCommand::*)(Client&, Identity&)> commands;
	std::string name;
};

#include "Client.hh"

#endif