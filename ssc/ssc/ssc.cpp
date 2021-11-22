//#include "Server.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma once
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32")
#pragma comment(lib,"winmm")
#include <vector>
#include "Object.h"
#include "protocol.h"
#include <iostream>
using namespace std;


//Server server;

void err_display(int err_num);
void err_quit(const char* msg);
int recvn(SOCKET s, char* buf, int len, int flags);
DWORD WINAPI Client_Thread(LPVOID arg);
vector<Map> m_map;
vector<Map> m_static_map;
Map m_button[2];
vector<Player> player;
vector<Monster> m_monster;
vector<Bullet> vec_bullet;
vector<Obstacle> m_obstacle;

HANDLE hThread;

int Client_Count = -1;
sc_send_player_id p_id;
sc_send_player sc_p[2];
sc_recv_keyinfo keyinfo;
CRITICAL_SECTION cs;
sc_put_object put;
short client_id;
bool c_left[2];
bool c_right[2];
int main()
{
	//server.InitServer();
	player.push_back(Player(200, 600, 0));
	player.push_back(Player(400 , 600, 1));
	m_static_map.push_back(Map(MAP::BUTTON, 200, 120));
	m_static_map.push_back(Map(MAP::BUTTON, 1100, 120));
	m_static_map.push_back(Map(MAP::PLAT, 200, 150));
	m_static_map.push_back(Map(MAP::PLAT, 1100, 150));
	m_static_map.push_back(Map(MAP::PLAT, 500, 150));


	m_obstacle.push_back(Obstacle(OBSTACLE::BLADE, 100, 500));
	m_obstacle.push_back(Obstacle(OBSTACLE::BLADE, 300, 500));

	m_monster.push_back(Monster(MONSTER::PLANT, 200, 100));
	m_monster.push_back(Monster(MONSTER::PIG, 500, 100));



	WSADATA wsa;
	SOCKADDR_IN sock_addr;
	if (WSAStartup(MAKEWORD(2, 2), &wsa))
		return -1;

	SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(SERVER_PORT);
	sock_addr.sin_addr.s_addr = INADDR_ANY;

	int ret = bind(server_socket, reinterpret_cast<sockaddr*>(&sock_addr), sizeof(sock_addr));
	if (SOCKET_ERROR == ret)
	{
		std::cout << "bind Error\n";
		int err_num = WSAGetLastError();
		err_display(err_num);
	}
	ret = listen(server_socket, SOMAXCONN);
	if (SOCKET_ERROR == ret) {
		std::cout << "listen Error\n";
		int err_num = WSAGetLastError();
		err_display(err_num);
	}

	InitializeCriticalSection(&cs);

	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;


	while (1)
	{
		//accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(server_socket, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			//err_display("accept()");
			std::cout << "!!!" << std::endl;
		}
		printf("Connected Client IP : %s\n", inet_ntoa(clientaddr.sin_addr));

	
		Client_Count++;

		hThread = CreateThread(NULL, 0, Client_Thread, (LPVOID)client_sock, 0, NULL);//HandleClient 쓰레드 실행, clientSock을 매개변수로 전달

	}
	DeleteCriticalSection(&cs);
	
	closesocket(server_socket);
	
	WSACleanup();
	
	return 0;
}


DWORD WINAPI Client_Thread(LPVOID arg)
{
	SOCKET clientSock = (SOCKET)arg;

	int retval;

	// hero.id 송신
	//p_id.id = 
	p_id.id = Client_Count;
	cout << Client_Count << endl;
	send(clientSock, (char*)&p_id, sizeof(sc_send_player_id), 0);


	int StartTime = GetTickCount64();
	int deltaTime;

	while (1) {
		int NowTime = GetTickCount64();

		deltaTime = NowTime - StartTime;
		if (deltaTime >= 10) {
			recvn(clientSock, (char*)&keyinfo, sizeof(keyinfo), 0);
			//EnterCriticalSection(&cs);
			client_id = keyinfo.id;
			//cout << keyinfo.id << endl;

			put.isClick = keyinfo.isClick;
			put.x = keyinfo.x;
			put.y = keyinfo.y;

			if (client_id == keyinfo.id) {
				c_left[client_id] = keyinfo.left;
				c_right[client_id] = keyinfo.right;
			}
			//LeaveCriticalSection(&cs);
			if (keyinfo.jump == true) {
				player[client_id].Jump();
			}

			if (c_left[client_id] == true) {
				if (player[client_id].getVely() == 0)
					player[client_id].SwitchState(PLAYER::MOVE);
				player[client_id].setDir(32);
				player[client_id].move(-(300 * 0.016f));
			}

			if (c_right[client_id] == true) {
				if (player[client_id].getVely() == 0)
					player[client_id].SwitchState(PLAYER::MOVE);
				player[client_id].setDir(0);
				player[client_id].move((300 * 0.016f));
			}

			player[client_id].UpdateGravity();
			
			
			//EnterCriticalSection(&cs);
			//cout << "?" << endl;

			//for(int i = 0; i < m_obstacle.size(); ++i)
			//	m_obstacle[i].Move();

			//for (int i = 0; i < vec_bullet.size(); ++i) {
			//	vec_bullet[i].Update();
			//	if (player[Client_Count].CollsionByObstacle(vec_bullet[i]) && vec_bullet[i].getisColl() != true) {
			//		vec_bullet[i].setisColl(true);
			//		m_map.clear();
			//	}
			//}

			if (player[client_id].getVely() > 0 || player[client_id].getisRanding()) {
				if (player[client_id].getVely() > 0) player[client_id].SwitchState(PLAYER::FALL);
				int check = 0;
				int b_check = 0;

				////서버 코드에서 버튼 state가 둘다 1 일때 다음 스테이지.

				//for (int i = 0; i < 2; ++i)
				//	if (m_button[i].getState() == 1)
				//	{
				//		b_check++;
				//	}
				//if (b_check == 2)
				//	m_button[0].x = 200;
				//else
				//	b_check = 0;


				for (int i = 0; i < m_map.size(); ++i) {
					if (player[client_id].getVely() > 600) player[client_id].setCollisonHelperY(8);
					else player[client_id].setCollisonHelperY(0);

					if (player[client_id].FallingCollsionOtherObject(m_map[i]))
					{
						player[client_id].setPlayerRanding(m_map[i].y - 32);
						check++;
					}
				}

				for (int i = 0; i < 2; ++i) {
					// 플레이어 버른 누름
					if (player[client_id].FallingCollsionOtherObject(m_static_map[i]))
					{
						player[client_id].setPlayerRanding(m_static_map[i].y - 16);
						m_static_map[i].setState(true);
						check++;
					}
					// 안누름
					else {
						m_static_map[i].setState(false);
					}
				}


				for (int i = 2; i < m_static_map.size(); ++i) {
					if (player[client_id].getVely() > 600) player[client_id].setCollisonHelperY(8);
					else player[client_id].setCollisonHelperY(0);

					if (player[client_id].FallingCollsionOtherObject(m_static_map[i]))
					{
						player[client_id].setPlayerRanding(m_static_map[i].y - 32);
						check++;
					}
				}

				if (check == 0) {
					player[client_id].setGravity();
				}
				else check = 0;
			}
			// 땅 착지     
			if (player[client_id].getPos().y > 780) {
				player[client_id].setPlayerRanding(780);
			}


			
			//LeaveCriticalSection(&cs);

			//sc_p.anim = player[Client_Count].getAnim();
			//sc_p.imageCount = player[Client_Count].getImageCount();
			
			sc_p[client_id].id = client_id;	
			sc_p[client_id].state = player[client_id].getState();
			sc_p[client_id].x = player[client_id].getPos().x;
			sc_p[client_id].y = player[client_id].getPos().y;
			sc_p[client_id].dir = player[client_id].getDir();
			sc_p[client_id].jumpCount = player[client_id].getJumpCount();
			


			send(clientSock, (char*)&sc_p, sizeof(sc_p), 0);

			send(clientSock, (char*)&put, sizeof(put), 0);
			//if (keyinfo.isClick) {
			//	sc_put_object put;
			//	put.x = keyinfo.x;
			//	put.y = keyinfo.y;
			//	send(clientSock, (char*)&put, sizeof(put), 0);
			//}
			//c_left[client_id] = false;
			//c_right[client_id] = false;

		}
		StartTime = NowTime;
	}
	return 0;
}
void err_display(int err_num)
{
	std::wcout.imbue(std::locale("Korean"));
	WCHAR* lpmsgbuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_num,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpmsgbuf, 0, 0

	);
	std::wcout << lpmsgbuf << std::endl;
	while (true);
	LocalFree(lpmsgbuf);
}

void err_quit(const char* msg)
{
	LPVOID lpmsgbuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpmsgbuf, 0, NULL
	);
	MessageBox(NULL, (LPCTSTR)lpmsgbuf,(LPCWSTR)msg, MB_ICONERROR);
	LocalFree(lpmsgbuf);
	exit(-1);
}

int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR) {
			return SOCKET_ERROR;
		}
		else if (received == 0)
			break;

		left -= received;
		ptr += received;
	}

	return (len - left);

}