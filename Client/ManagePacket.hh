#ifndef MANAGEPACKET_HH_
# define MANAGEPACKET_HH_

# include "Action.hh"
# include "Packet.h"

class ManagePacket
{
public:
	ManagePacket(Packet *, actFptr);
	~ManagePacket();
	Packet* getPacket();
	Packet* getResponse();
	void addResponse(Packet *);
    void execTAction(IGui *);
	bool isResponseOk() const;
	bool	getSent() const;
	void	setSent(bool const);

private:
	Packet*	request;
	Packet*	response;
	actFptr toCall;
	time_t	sendTimer;
	bool 	sent;
};


#endif
