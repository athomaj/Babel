#include "ManageCommand.hh"

ManageCommand::ManageCommand() :
	name("Server")
{
	commands[CONNECTION] = &ManageCommand::onConnect;
	commands[DECONNECTION] = &ManageCommand::onDisconnect;
	commands[ASKCONTACT] = &ManageCommand::onAskContact;
	commands[ADDCONTACT] = &ManageCommand::onAddContact;
	commands[DELCONTACT] = &ManageCommand::onDelContact;
	commands[ASKCALL] = &ManageCommand::onAskCall;
	commands[RCVCALL] = &ManageCommand::onRcvCall;
	commands[CALLESTABLISHED] = &ManageCommand::onCallEst;
	commands[CALLFAILED] = &ManageCommand::onCallFailed;
	commands[ENDCALL] = &ManageCommand::onEndCall;
	commands[OK] = &ManageCommand::onOk;
	commands[KO] = &ManageCommand::onKo;
}

void ManageCommand::onConnect(Client& client, Identity& id)
{
	std::cout << "New client trying to connect" << std::endl;
    std::vector<Client *> clients = client.getServer()->getClients();

	for (unsigned int i=0; i < clients.size(); ++i)
	{
		if (clients[i]->getUsername() == id.getUsername())
		{
			Identity resp(name, KO);
			Packet *t = new Packet(resp);
			client.addPacketToSend(t);
			return ;
		}
	}
	Identity resp(name, OK);
    Packet *t = new Packet(resp);
	client.addPacketToSend(t);

	client.setIp(id.getIp());
	client.setPort(id.getPort());
	client.setUsername(id.getUsername());

	Identity id2(id.getUsername(), ADDCONTACT);
    t = new Packet(id2);
    for (unsigned int i=0; i < clients.size(); ++i)
    	if (clients[i]->getUsername() != client.getUsername())
			clients[i]->addPacketToSend(t);

	Identity* newId;
    for (unsigned int i=0; i < clients.size(); ++i)
    {
    	if (clients[i]->getUsername() != client.getUsername())
    	{
			newId = new Identity(clients[i]->getUsername(), ADDCONTACT);
		    t = new Packet(*newId);
		    client.addPacketToSend(t);
		    delete newId;
    	}
    }
}

void ManageCommand::onAskContact(Client& client, Identity& id)
{
	Identity resp(name, ADDCONTACT);
    Packet *t = new Packet(resp);
	client.addPacketToSend(t);
	std::cout << "asking contacts" << std::endl;
}

void ManageCommand::onDisconnect(Client& client, Identity& id)
{

}

void ManageCommand::onAddContact(Client& client, Identity& id)
{

}

void ManageCommand::onDelContact(Client& client, Identity& id)
{

}

void ManageCommand::onAskCall(Client& client, Identity& id)
{
    std::vector<Client *> clients = client.getServer()->getClients();
    for (unsigned int i=0; i < clients.size(); ++i)
    	if (clients[i]->getUsername() == id.getUsername())
    	{
			Identity resp(client.getUsername(), client.getIp(), client.getPort(), ASKCALL);
		    Packet *t = new Packet(resp);
			clients[i]->addPacketToSend(t);

			Identity resp2(name, OK);
		    t = new Packet(resp2);
			client.addPacketToSend(t);
    	}
}

void ManageCommand::onRcvCall(Client& client, Identity& id)
{

}

void ManageCommand::onCallEst(Client& client, Identity& id)
{

}

void ManageCommand::onCallFailed(Client& client, Identity& id)
{

}

void ManageCommand::onEndCall(Client& client, Identity& id)
{
	Packet* t;
    std::vector<Client *> clients = client.getServer()->getClients();
	std::string tmp(id.getUsername());

	Identity resp2(name, OK);
    t = new Packet(resp2);
	client.addPacketToSend(t);
    for (unsigned int i=0; i < clients.size(); ++i)
    {
    	std::cout << clients[i]->getUsername() << "::" << id.getUsername() << std::endl;
    	if (clients[i]->getUsername().compare(tmp) == 0)
    	{
    		std::cout << "name: " << tmp << std::endl;
			Identity* newId = new Identity(client.getUsername(), ENDCALL);
		    t = new Packet(*newId);
		    clients[i]->addPacketToSend(t);
		    delete newId;
    	}
    }
	std::cout << "onendcall" << std::endl;
}

void ManageCommand::onOk(Client& client, Identity& id)
{

}

void ManageCommand::onKo(Client& client, Identity& id)
{

}


ManageCommand::~ManageCommand()
{

}

void	ManageCommand::whichInstruct(Identity& id, Client& client)
{
	std::map<Instruct, void (ManageCommand::*)(Client&, Identity&)>::iterator it;

	std::cout << id.getInstruct() << std::endl;
	it = commands.find(id.getInstruct());
	if (it != commands.end())
	{
		(this->*(it->second))(client, id);
	}
}