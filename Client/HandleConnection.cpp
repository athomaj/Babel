//
// HandleConnection.cpp for  in /home/burdai_t/rendu/cpp_babel/Client
// 
// Made by Timothy Burdairon
// Login   <burdai_t@epitech.net>
// 
// Started on  Mon Nov  2 11:05:40 2015 Timothy Burdairon
// Last update Wed Nov  4 11:21:42 2015 Timothy Burdairon
//

#include "HandleConnection.h"
#include <string.h>

HandleConnection::HandleConnection()
{
  // ptr.insert(std::make_pair("001\n", &HandleConnection::connection));
  // ptr.insert(std::make_pair("002\n", &HandleConnection::deconnection));
  // ptr.insert(std::make_pair("003\n", &HandleConnection::contactList));
  // ptr.insert(std::make_pair("004\n", &HandleConnection::addList));
  // ptr.insert(std::make_pair("005\n", &HandleConnection::delContact));
  // ptr.insert(std::make_pair("006\n", &HandleConnection::askCall));
  // ptr.insert(std::make_pair("007\n", &HandleConnection::rcvCall));
  // ptr.insert(std::make_pair("008\n", &HandleConnection::callEstablished));
  // ptr.insert(std::make_pair("009\n", &HandleConnection::callFailed));
  // ptr.insert(std::make_pair("018\n", &HandleConnection::callEstablished));
  // ptr.insert(std::make_pair("019\n", &HandleConnection::callFailed));
  // ptr.insert(std::make_pair("010\n", &HandleConnection::endCall));
  // ptr.insert(std::make_pair("011\n", &HandleConnection::rcvEndCall));
  // ptr.insert(std::make_pair("212\n", &HandleConnection::commandOK));
  // ptr.insert(std::make_pair("213\n", &HandleConnection::commandKO));
}

HandleConnection::~HandleConnection()
{

}

// void			HandleConnection::connection(std::string &name) //send
// {
//   std::stringstream	flux;
//   std::string		buf;

//   name = name;
//   flux << "001";
//   flux << myName;
//   flux >> buf;
//   Packet *packet = new Packet(buf);
//   co->write_to_server(packet);
//   std::cout << "Connection" << std::endl;
// }

// void            HandleConnection::deconnection(std::string &str) //send
// {
//   std::stringstream	flux;
//   std::string		buf;

//   str = str;
//   flux << "002";
//   flux << myName;
//   flux >> buf;
//   Packet *packet = new Packet(buf);
//   co->write_to_server(packet);
//   std::cout << "deconnection" << " " << str << std::endl;
// }

// void            HandleConnection::contactList(std::string &str) //rcv
// {
//   std::istringstream iss(str);

//   while (iss)
//     {
//       std::string sub;
//       iss >> sub;
//       contacts.push_back(sub);
//     }
//   std::cout << "contactList"  << " " << str << std::endl;
// }

// void            HandleConnection::addList(std::string &str) // rcv
// {
//   std::cout << "addList"  << " " << str << std::endl;
//   clientsInCall.push_back(str);
//   inCall = true;
// }

// void            HandleConnection::delContact(std::string &str) //rcv
// {
//   for (std::list<std::string>::iterator it = clientsInCall.begin(); it != clientsInCall.begin(); ++it)
//     {
//       if ((*it).compare(0,str.size(), str) == 0)
// 	{
// 	  clientsInCall.erase(it);
// 	  if (clientsInCall.empty() == true)
// 	    inCall = false;
// 	  return ;
// 	}
//     }
// }

 void			HandleConnection::askCall(std::string &name) // send packet()
 {
    Identity id(name, ASKCALL);
    Packet *t = new Packet(id);
	std::cout << "co addr" << (void *)co << std::endl;
    co->write_to_server(t, ASKCALL);
	std::cout << "asking call" << std::endl;
 }

// void            HandleConnection::rcvCall(std::string &name) // rcv packet
// {
//   if (inCall == false)
//     std::cout << "rcvCall"  << " " << name << std::endl;
// }

// void            HandleConnection::callEstablished(std::string &str) // rcv
// {
//   if (inCall == false)
//     {
//       std::cout << "callEstablished"  << " " << str << std::endl;
//       inCall = true;
//     }
// }

// void            HandleConnection::callFailed(std::string &str) // rcv
// {
//   if (inCall == false)
//     {
//       std::cout << "callFailed"  << " " << str << std::endl;
//       inCall = false;
//     }
// }

// void            HandleConnection::sendCallEstablished(std::string &str) // send
// {
//   str = str;
//   if (inCall == false)
//     {
//       std::string	tmp("018");
//       Packet *packet = new Packet(tmp);
//       co->write_to_server(packet);
//       std::cout << "sendcallEstablished"  << " " << str << std::endl;
//     }
//   else
//     commandOK(str);
// }

// void            HandleConnection::sendCallFailed(std::string &str) // send
// {
//   str = str;
//   if (inCall == false)
//     {
//       std::string	tmp("019");
//       Packet *packet = new Packet(tmp);
//       co->write_to_server(packet);
//       std::cout << "sendcallFailed"  << " " << str << std::endl;
//     }
//   else
//     commandKO(str);
// }

 void            HandleConnection::endCall(std::string &str) // send
 {
     Identity id(str, ENDCALL);
     Packet *t = new Packet(id);
     co->write_to_server(t, ENDCALL);
     co->resetClientFd();

     std::cout << "endCall"  << " " << str << std::endl;

     //inCall.erase(it);
//   inCall = false;
  // clientsInCall.clear();
 }

// void            HandleConnection::rcvEndCall(std::string &str) // send
// {
//   std::cout << "rcvendCall"  << " " << str << std::endl;
//   inCall = false;
//   clientsInCall.clear();
// }

// void            HandleConnection::commandOK(std::string &str) // send
// {
//   str = str;
//   std::string	tmp("212");
//   Packet *packet = new Packet(tmp);
//   co->write_to_server(packet);
//   //std::cout << "rcvendCall"  << " " << str << std::endl;
// }

// void            HandleConnection::commandKO(std::string &str) // send
// {
//   str = str;
//   std::string	tmp("213");
//   Packet *packet = new Packet(tmp);
//   co->write_to_server(packet);
//   //std::cout << "rcvendCall"  << " " << str << std::endl;
// }

// void		HandleConnection::handlePacket(std::list<Packet *> lst)
// {
//   std::string	str;

//   if (lst.empty() == false)
//     {
//       std::list<Packet*>::iterator curentpacket = lst.begin();
//       for (std::map<std::string, ptrf>::iterator it = ptr.begin(); it !=  ptr.end(); it++)
// 	{
// 	  str = *(*curentpacket)->getString();
// 	  if (strncmp(str.c_str(), (*it).first.c_str(), 4) == 0)
// 	  //if ((*it).first.compare(0, 4, str.c_str(), 4) == 0)
// 	    {
// 	      std::string tmp(str.c_str() + 4);
// 	      (this->*((*it).second))(tmp);
// 	      lst.erase(curentpacket);
// 	      this->co->setPacket(lst);
// 	      return ;
// 	    }
// 	}
//       lst.erase(curentpacket);
//       this->co->setPacket(lst);
//       return ;
//     }
// }

void		HandleConnection::handle(IClient *_co, IGui *gui)
{
  std::list<Packet *> lst;

  this->co = _co;
  while (1)
    {
      if (co->handle_connection() == -1)
          return ;
      co->setGui(gui);
      // handlePacket(co->getPacket());
    }
}
