
#include <string>
#include <vector>

#include "hetuwTCPConnection.h"
#include "LivingLifePage.h"
#include "hetuwmod.h"

void TCPConnection::init(std::string serverIp, int serverPort, void (*inOnReceivedMessage)(std::string), void (*inOnStatusChanged)(statusType)) {
	ip = serverIp;
	port = serverPort;
	onReceivedMessage = inOnReceivedMessage;
	onStatusChanged = inOnStatusChanged;
	if (verbose) printf("%s init TCPConnection ip: %s port: %d\n", logTag.c_str(), ip.c_str(), port);
}
void TCPConnection::connect() {
	if (verbose) printf("%s set keepConnected to true\n", logTag.c_str());
	keepConnected = true;
}
void TCPConnection::disconnect() {
	if (verbose) printf("%s set keepConnected to false\n", logTag.c_str());
	keepConnected = false;
}
void TCPConnection::send(std::string message) {
	if (verbose) printf("%s push to send buffer: %s\n", logTag.c_str(), message.c_str());
	sendBuffer.push_back(message);
}
void TCPConnection::step() {
	if (!keepConnected) {
		if (status == OFFLINE) return;
		disconnectC();
		return;
	}
	if (status == OFFLINE) {
		if (HetuwMod::curStepTime-timeOfflineSince < waitTimeBeforeReconnect) return;
		connectC();
		return;
	}

	if (!intervalSendRead.step()) return;
	if (bSendFirstMessage) {
		sendC("KA");
		return;
	}
	readAllMessagesFromServer();
	sendAllBufferedMessages();
}

// --------------- private ---------------

void TCPConnection::connectC() {
	if (verbose) printf("%s connect - openSocketConnection\n", logTag.c_str());
	socket = openSocketConnection(ip.c_str(), port);
	updateStatus(CONNECTING);
}
void TCPConnection::disconnectC() {
	if (socket == -1) return;
	if (verbose) printf("%s disconnect - closeSocket\n", logTag.c_str());
	closeSocket(socket);
	socket = -1;
	updateStatus(OFFLINE);
}
void TCPConnection::readAllMessagesFromServer() {
	std::string msg = readFromServer();
	if (msg.length() <= 0) return;
	std::vector<std::string> messages = HetuwMod::splitStrXTimes(msg, charEnd, 99999);
	for (auto const& str: messages) {
		if (verbose) printf("%s onReceivedMessage msg: %s\n", logTag.c_str(), str.c_str());
		onReceivedMessage(str);
	}
}

std::string TCPConnection::readFromServer() {
	std::string result = "";
	int bufferSize = 512;
	unsigned char buffer[bufferSize+1];
	int numRead = readFromSocket(socket, buffer, bufferSize);
	while (numRead > 0) {
		char* charBuffer = reinterpret_cast<char*>(buffer);
		charBuffer[numRead] = 0;
		//printf("Phex buffer[%d]: %s\n", numRead, charBuffer);
		result += std::string(charBuffer, numRead);
		numRead = readFromSocket(socket, buffer, bufferSize);
	}
	if (numRead == -1) {
		if (verbose) printf("%s failed to read message from server\n", logTag.c_str());
		if (status != CONNECTING) disconnectC();
	} else if (result.length() > 0) {
		if (verbose) printf("%s received server msg: %s\n", logTag.c_str(), result.c_str());
		updateStatus(ONLINE);
		timeLastMessage = HetuwMod::curStepTime;
	}
	return result;
}
void TCPConnection::sendAllBufferedMessages() {
	int deleteCount = 0;
	for(unsigned k=0; k<sendBuffer.size(); k++) {
		if (!sendC(sendBuffer[k])) break;
		deleteCount++;
	}
	for (int i=0; i<deleteCount; i++) {
		sendBuffer.erase(sendBuffer.begin());
	}
}
bool TCPConnection::sendC(std::string str) {
	str += charEnd;
	int len = str.length();
	if (verbose) printf("%s send message: %s\n", logTag.c_str(), str.c_str());
	int numSent = sendToSocket(socket, (unsigned char*)str.c_str(), str.length());
	if (len != numSent) {
		if (status == CONNECTING) return false;
		if (verbose) printf("%s failed to send message, tried to sent: %d, but sent: %d\n", logTag.c_str(), len, numSent);
		disconnectC();
		return false;
	}
	timeLastMessage = HetuwMod::curStepTime;
	updateStatus(ONLINE);
	return true;
}
void TCPConnection::updateStatus(statusType inStatus) {
	if (status == inStatus) return;
	if (verbose) printf("%s update status %d -> %d\n", logTag.c_str(), status, inStatus);
	status = inStatus;

	if (status == CONNECTING) bSendFirstMessage = true;
	else if (status == ONLINE) bSendFirstMessage = false;

	if (status == OFFLINE) timeOfflineSince = HetuwMod::curStepTime;

	if (onStatusChanged) onStatusChanged(status);
}
