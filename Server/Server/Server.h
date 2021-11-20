#pragma once
#include <WS2tcpip.h>
#include <vector>
#pragma comment(lib, "ws2_32")

class Server
{
private:
	std::vector<SOCKET> Wait_Queue;
	bool				StartFlag;
	HANDLE				PThread;

public:
	Server();
	~Server();
	void err_display(int err_num);
	void err_quit(const char* msg);
	int recvn(SOCKET s, char* buf, int len, int flags);
	void LoginServer();


	void LobbyServer();
	DWORD WINAPI InitServer();
	static DWORD WINAPI ProcessThread(LPVOID arg);

	void InGame();

	void UpdateStatus();
	void UpdateCollision();



};

