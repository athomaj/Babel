
/*
** HandleConnection.h for  in /home/burdai_t/rendu/cpp_babel/Client
** 
** Made by Timothy Burdairon
** Login   <burdai_t@epitech.net>
** 
** Started on  Mon Nov  2 11:03:36 2015 Timothy Burdairon
** Last update Wed Nov  4 11:22:00 2015 Timothy Burdairon
*/


#ifndef HANDLECONNECTION_H_
#define HANDLECONNECTION_H_

#include "IGui.h"
#include "IClient.h"
#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <map>
#include "Identity.hpp"

class HandleConnection;
class IClient;

typedef void(HandleConnection::*ptrf)(std::string &);
typedef std::map<std::string, ptrf> ptrFunc;
class HandleConnection
{
 public:
  HandleConnection();
  ~HandleConnection();
  // void		connection(std::string &);
  // void		deconnection(std::string &);
  // void		contactList(std::string &);
  // void				addList(std::string &);
  // void				delContact(std::string &);
   void				askCall(std::string &);
  // void				rcvCall(std::string &);
  // void				callEstablished(std::string &);
  // void				callFailed(std::string &);
  // void				sendCallEstablished(std::string &);
  // void				sendCallFailed(std::string &);
   void				endCall(std::string &);
  // void				rcvEndCall(std::string &);
  // void				commandOK(std::string &);
  // void				commandKO(std::string &);
  // ptrFunc			ptr;
  void				handle(IClient *, IGui *);
  // void				handlePacket(std::list<Packet *>);
  IClient			*co;
 private:
  // std::string			myName;
  // std::list<std::string>	contacts;
  // std::list<std::string>	clientsInCall;
};



#endif
