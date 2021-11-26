//#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
//#pragma once
//#include <WS2tcpip.h>
//#pragma comment(lib, "ws2_32")
//#pragma comment(lib,"winmm")
//#include <vector>
//#include "Object.h"
//using namespace std;
//
//class Server
//{
//public:
//
//	 vector<Map> m_map;
//	 vector<Map> m_static_map;
//	 Map m_button[2];
//	 vector<Player> p;
//	 vector<Monster> m_monster;
//	 vector<Bullet> vec_bullet;
//	 vector<Obstacle> m_obstacle;
//
//	 HANDLE hThread;
//
//	 int Client_Count = -1;
//	Server();
//	~Server();
//	
//public:
//	void err_display(int err_num);
//	void err_quit(const char* msg);
//	int recvn(SOCKET s, char* buf, int len, int flags);
//	int InitServer();
//
//	void LoginServer();
//
//
//	void LobbyServer();
//
//	void InGame();
//
//	void UpdateStatus();
//	void UpdateCollision();
//	
//	//DWORD WINAPI Client_Thread(LPVOID arg);
//
//	// 충돌 박스 
//
//};
//
