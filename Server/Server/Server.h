#pragma once
#include <WS2tcpip.h>


class Server
{
public:
	Server();
	~Server();

public:
	void err_display(int err_num);
	void err_quit(const char* msg);
	int recvn(SOCKET s, char* buf, int len, int flags);
	int InitServer();

	void LoginServer();


	void LobbyServer();

	void InGame();

	void UpdateStatus();
	void UpdateCollision();



};

