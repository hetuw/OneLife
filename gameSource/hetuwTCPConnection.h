#ifndef HETUWTCPCONNECTION_H
#define HETUWTCPCONNECTION_H

#include <string>
#include <vector>

#include "hetuwmod.h"

class TCPConnection {

public:

	std::string ip;
	int port;

	enum statusType { OFFLINE, CONNECTING, ONLINE };
	statusType status = OFFLINE;

	void (*onReceivedMessage)(std::string);
	void (*onStatusChanged)(statusType);

	int socket = -1;
	bool keepConnected = false;
	std::vector<std::string> sendBuffer;
	char charEnd = (char)4;
	std::string logTag = "hetuw";
	bool verbose = false;
	HetuwMod::IntervalTimed intervalSendRead = HetuwMod::IntervalTimed(0.3);
	bool bSendFirstMessage = false;

	double timeConnectingSince = 0;
	double waitTimeBeforeReadingFirstMsgs = 3; // in seconds
	double timeLastMessage = 0; // time of last sent or received message
	double timeOfflineSince = 1000;
	double waitTimeBeforeReconnect = 3; // change this to change how fast the client reconnects after it loses connection

	void init(std::string serverIp, int serverPort, void (*inOnReceivedMessage)(std::string), void (*inOnStatusChanged)(statusType) = NULL);
	void connect();
	void disconnect();
	void reconnect();
	void send(std::string message);
	void step();

private:

	void connectC();
	void disconnectC();
	void readAllMessagesFromServer();
	std::string readFromServer();
	void sendAllBufferedMessages();
	bool sendC(std::string str);
	void updateStatus(statusType inStatus);

};

#endif
