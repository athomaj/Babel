#include "ManagePacket.hh"

ManagePacket::ManagePacket(Packet *pckt, actFptr act) :
	request(pckt), response(NULL), toCall(act), sendTimer(time(NULL)), sent(false)
{

}

ManagePacket::~ManagePacket()
{

}

Packet* ManagePacket::getPacket()
{
	return (this->request);
}

Packet* ManagePacket::getResponse()
{
	return (this->response);
}

bool	ManagePacket::isResponseOk() const
{
    Identity* id = response->getIdentity();
    if (id && id->getInstruct() == OK)
        return (true);
    return (false);
}

void ManagePacket::addResponse(Packet *pck)
{
      // if (pck == NULL)
      //   std::cout << "NULL" << std::endl;
	response = pck;
}

void ManagePacket::execTAction(IGui *gui)
{
    (gui->*toCall)(request);
}

bool	ManagePacket::getSent() const
{
	return (sent);
}

void	ManagePacket::setSent(bool const s)
{
	sent = s;
}
