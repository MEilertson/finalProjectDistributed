#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "Client.h"
#include <stdio.h> 
#include <arpa/inet.h> 
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <cstdlib> 
#include <unistd.h> 
#include <string>
#include <queue>
#include <mutex>
#include <chrono>
#include <boost/multiprecision/cpp_int.hpp>
#include <atomic>
#include <list>
#include <thread>
#include "config.h"
#include "DivFinderSP.h"

// The amount to read in before we send a packet
const unsigned int stdin_bufsize = 50;
const unsigned int socket_bufsize = 100;

class TCPClient : public Client
{
public:
   TCPClient();
   ~TCPClient();
	bool sendData(std::string data);
	std::string receiveData();
	void receivingThread();
	void sendingThread();
	std::string sanitizeUserInput(const std::string& s);

   virtual void connectTo(const char *ip_addr, unsigned short port) override;
   virtual void handleConnection() override;

   virtual void closeConn() override;

   protected:
   	std::queue<std::string> receivedMessages;
	std::queue<std::string> sendMessages;
	bool connClosed = false;
	bool connectionBroke = false;
	std::mutex mtx1;

private:
	 sockaddr_in sockaddr;
	 int sockfd;
	 struct sockaddr_in server;
     bool clientTestMode = false; // used to test setting client IP address to static ip addr
	 std::chrono::system_clock::time_point lastTimeHeartBeatReceived = std::chrono::system_clock::now();;

};

using namespace boost::multiprecision;

class Slave : public TCPClient
{
public:
	Slave():TCPClient() {}
	void factorNumber(LARGEINT n);
	void handleConnection();
	void handleMessage(std::string msg);
	LARGEINT strtoLARGE(std::string str_num);

private:
	int client_ID;
	int slave_ID;
	LARGEINT num_to_factor;
	DivFinderSP* slave_div;
	std::atomic<bool> cancel_op;
	std::thread div_thread;
	std::list<LARGEINT> prime_factors;
};


#endif
