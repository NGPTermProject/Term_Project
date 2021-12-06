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

void InitGameObject();

void err_display(int err_num);
void err_quit(const char* msg);
int recvn(SOCKET s, char* buf, int len, int flags);
DWORD WINAPI Client_Thread(LPVOID arg);
vector<Map> m_map;
vector<Map> m_static_map;
Map m_button[2];
vector<Player> player;
vector<Monster> m_monster;
<<<<<<< HEAD
vector<Bullet> vec_bullet;
<<<<<<< HEAD
=======
=======
Bullet m_bullet[20];
>>>>>>> RemotTest2

>>>>>>> parent of c122221 (push_bullet)
vector<Obstacle> m_obstacle;
vector<SOCKET> matching_queue;

HANDLE hThread;

int Client_Count = -1;
sc_send_player_id p_id;
sc_send_player sc_p[2];
sc_obstacle sc_obs[2];
<<<<<<< HEAD

sc_recv_keyinfo keyinfo;
=======
sc_bullet bullet[20];
cs_send_keyinfo keyinfo;
>>>>>>> RemotTest2
CRITICAL_SECTION cs;
short client_id;
bool c_left[2];
bool c_right[2];
<<<<<<< HEAD
sc_put_object put[2];
sc_button sc_b;
<<<<<<< Updated upstream
=======


void send_login_ok(SOCKET s, short id)
{
	sc_login_ok packet;
	packet.size = sizeof(packet);
	packet.packet_type = SC_PACKET_LOGIN_OK;
	packet.id = id;

	send(s, reinterpret_cast<char*>(&packet), sizeof(packet), 0);

}

>>>>>>> Stashed changes
int main()
{

	//server.InitServer();
	player.push_back(Player(200, 600, 0));
	player.push_back(Player(400, 600, 1));
	m_static_map.push_back(Map(MAP::BUTTON, 48, 344));
	m_static_map.push_back(Map(MAP::BUTTON, 1392, 344));
	m_static_map.push_back(Map(MAP::PLAT, 48, 374));
	m_static_map.push_back(Map(MAP::PLAT, 1392, 374));
	m_static_map.push_back(Map(MAP::PLAT, 48, 566));
	m_static_map.push_back(Map(MAP::PLAT, 432, 214));
	m_static_map.push_back(Map(MAP::PLAT, 1008, 214));
=======
sc_update put[2];

int MapSize[3] = { 7,15,0 };
int MonsterSize[3] = { 4,9,0 };
int ObstacleSize[3] = { 2,7,0 };

int FirstMapSize = 7;
int SecondMapSize = FirstMapSize + 8;
int FirstMonsterSize = 4;
int SecondMonsterSize = FirstMonsterSize + 5;
int FirstObstacleSize = 2;
int SecondObstacleSize = FirstObstacleSize + 5;
>>>>>>> RemotTest2

int MapStartSize = 0;
int MapEndSize = FirstMapSize;

int MonsterStartSize = 0;
int MonsterEndSize = FirstMonsterSize;

int ObstacleStartSzie = 0;
int ObstacleEndSize = FirstObstacleSize;

<<<<<<< HEAD
	m_monster.push_back(Monster(MONSTER::PLANT, 1392, 780));
	m_monster.push_back(Monster(MONSTER::PIG, 48, 536));
	m_monster.push_back(Monster(MONSTER::PIG, 432, 184));
	m_monster.push_back(Monster(MONSTER::PIG, 1008, 184));
=======
sc_start_game stage_game_info;
bool GameStart;
>>>>>>> RemotTest2

int Current_Stage = 1;
int Next_Stage = 1;

int main()
{
	InitGameObject();
	MapSize[2] = m_static_map.size();
	MonsterSize[2] = m_monster.size();
	ObstacleSize[2] = m_obstacle.size();

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
<<<<<<< HEAD
	SOCKET client_sock;

=======

	SOCKET clientSock;
>>>>>>> RemotTest2
	SOCKADDR_IN clientaddr;
	int addrlen;

	bool flag = TRUE;

	while (1)
	{
		//accept()
		addrlen = sizeof(clientaddr);
		clientSock = accept(server_socket, (SOCKADDR*)&clientaddr, &addrlen);
		setsockopt(clientSock, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag));

		if (clientSock == INVALID_SOCKET) {
			//err_display("accept()");
			std::cout << "!!!" << std::endl;
		}
<<<<<<< HEAD
		printf("Connected Client IP : %s\n", inet_ntoa(clientaddr.sin_addr));

<<<<<<< Updated upstream

		Client_Count++;
=======
		matching_queue.push_back(client_sock);

		cs_packet_login packet;
		recvn(client_sock, reinterpret_cast<char*>(&packet),sizeof(packet), 0);
		cout <<"클라이언트에서 보낸 패킷 : " << static_cast<int>(packet.packet_type) << endl;
>>>>>>> Stashed changes

		if (packet.packet_type == CS_PACKET_LOGIN) {
			Client_Count++;
			if (matching_queue.size() == 2) {
				for (int i = 0; i < 2; ++i) {
					send_login_ok(matching_queue[i], Client_Count);

					cout << "recvn login packet" << endl;

					hThread = CreateThread(NULL, 0, Client_Thread, (LPVOID)client_sock, 0, NULL);//HandleClient 쓰레드 실행, clientSock을 매개변수로 전달
					if (hThread == NULL) { closesocket(client_sock); }
				}
			}
			else {
				for (int i = 0; i < matching_queue.size(); ++i)
					send_login_ok(matching_queue[i], Client_Count);
			}
		}
	
		

	}
	DeleteCriticalSection(&cs);

=======
		//   printf("Connected Client IP : %s\n", inet_ntoa(clientaddr.sin_addr));
		Client_Count++;
		hThread = CreateThread(NULL, 0, Client_Thread, (LPVOID)clientSock, 0, NULL);//HandleClient 쓰레드 실행, clientSock을 매개변수로 전달
	}
	DeleteCriticalSection(&cs);
>>>>>>> RemotTest2
	closesocket(server_socket);

	WSACleanup();

	return 0;
}


DWORD WINAPI Client_Thread(LPVOID arg)
{
	SOCKET clientSock = (SOCKET)arg;

	int retval;

	int loginClient = 0;
	loginClient = Client_Count;
	p_id.id = Client_Count;

	while (1) {
		if (Client_Count == 1) {
			if (p_id.id == 0) {
				p_id.id = 1;
				send(clientSock, (char*)&p_id, sizeof(sc_send_player_id), 0);
				break;
			}
			else {
				send(clientSock, (char*)&p_id, sizeof(sc_send_player_id), 0);
				break;
			}
		}
		else
			send(clientSock, (char*)&p_id, sizeof(sc_send_player_id), 0);
	}

<<<<<<< HEAD
	int StartTime = GetTickCount64();
	int deltaTime;

	//timeGetTime();
	while (1) {
		int NowTime = GetTickCount64();

		deltaTime = NowTime - StartTime;

		if (deltaTime >= 10) {
=======
	//호스트인경우
	if (loginClient == 0) {
		recvn(clientSock, (char*)&p_id, sizeof(sc_send_player_id), 0);
		if (p_id.id == 3) {
			stage_game_info.gamestart = true;
			stage_game_info.stage = 1;
		}
	}
	//시작 대기
	while (1) {
		if (stage_game_info.gamestart) {
			break;
		}
	}

	send(clientSock, (char*)&stage_game_info, sizeof(sc_start_game), 0);

	while (1) {

		int StartTime = (int)GetTickCount64();
		while ((GetTickCount64() - StartTime) <= 10) {}
		{
>>>>>>> RemotTest2
			recvn(clientSock, (char*)&keyinfo, sizeof(keyinfo), 0);
			if (keyinfo.isClick) {
				m_map.push_back(Map(MAP::PLAT, keyinfo.x, keyinfo.y));
				for (int i = 0; i < 2; ++i) {
					put[i].isClick = true;
					put[i].x = keyinfo.x;
					put[i].y = keyinfo.y;
				}
			}

			EnterCriticalSection(&cs);
			client_id = keyinfo.id;
			if (client_id == keyinfo.id) {
				c_left[client_id] = keyinfo.left;
				c_right[client_id] = keyinfo.right;
			}
			LeaveCriticalSection(&cs);

			player[client_id].UpdateGravity();

			//EnterCriticalSection(&cs);
			//cout << "?" << endl;

			//for(int i = 0; i < m_obstacle.size(); ++i)
			//	m_obstacle[i].Move();

			//if (keyinfo.jump) {
			//	for (int i = 0; i < 2; ++i)
			//		m_monster[i].Attack();
			//}

			//for (int i = 0; i < vec_bullet.size(); ++i) {
			//	vec_bullet[i].Update();
			//	if (player[Client_Count].CollsionByObstacle(vec_bullet[i]) && vec_bullet[i].getisColl() != true) {
			//		vec_bullet[i].setisColl(true);
			//		m_map.clear();
			//	}
			//}
			//플레이어 점프 상태에서 충돌 처리 부분
			if (player[client_id].getVely() > 0 || player[client_id].getisRanding()) {
				if (player[client_id].getVely() > 0) player[client_id].SwitchState(PLAYER::FALL);
				int check = 0;

				for (int i = 0; i < m_map.size(); ++i) {
					if (player[client_id].getVely() > 600) player[client_id].setCollisonHelperY(8);
					else player[client_id].setCollisonHelperY(0);

					if (player[client_id].FallingCollsionOtherObject(m_map[i]))
					{
						player[client_id].setPlayerRanding(m_map[i].y - 32);
						check++;
					}
				}

				for (int i = MapStartSize; i < MapStartSize + 2; ++i) {
					// 플레이어 버른 누름
					if (player[client_id].FallingCollsionOtherObject(m_static_map[i]))
					{
						player[client_id].setPlayerRanding(m_static_map[i].y - 16);
						m_static_map[i].setState(true);

						if (put[client_id].isPush[i % 2] == put[(client_id + 1) % 2].isPush[i % 2]) {
							EnterCriticalSection(&cs);
							put[client_id].isPush[i % 2] = true;
							put[(client_id + 1) % 2].isPush[i % 2] = true;
							LeaveCriticalSection(&cs);
							//다음스테이지
							if (put[client_id].isPush[(i + 1) % 2] || put[(client_id + 1) % 2].isPush[(i + 1) % 2]) {
								if (Current_Stage == Next_Stage && m_map.size() != 0) {
									Next_Stage++;
									cout << "Stage:" << Next_Stage << endl;
								}
							}
						}
						check++;
					}
					else if (!player[(client_id + 1) % 2].FallingCollsionOtherObject(m_static_map[i])) {
						EnterCriticalSection(&cs);
						put[client_id].isPush[i % 2] = false;
						put[(client_id + 1) % 2].isPush[i % 2] = false;
						LeaveCriticalSection(&cs);
					}
				}


				for (int i = MapStartSize + 2; i < MapEndSize; ++i) {
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

			// --키입력------------------------------------------///
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
			///-------------------------------------------------///
<<<<<<< HEAD
			for (int i = 0; i < m_obstacle.size(); ++i) {
=======

			for (int i = ObstacleStartSzie; i < ObstacleEndSize; ++i) {
>>>>>>> RemotTest2
				if (m_obstacle[i].type == OBSTACLE::BLADE) {
					EnterCriticalSection(&cs);
					m_obstacle[i].Move();
					sc_obs[i % 2].x = m_obstacle[i].getPosX();
					sc_obs[i % 2].y = m_obstacle[i].getPosY();
					LeaveCriticalSection(&cs);
				}
				if (player[client_id].CollsionByObstacle(m_obstacle[i])) {
					EnterCriticalSection(&cs);
					put[client_id].clear = true;
					put[(client_id + 1) % 2].clear = true;
					m_map.clear();
					if (Current_Stage == 3) {
						player[0].setStartLine(356, 270);
						player[1].setStartLine(452, 270);
					}
					else {
						player[0].setStartLine(200, 600);
						player[1].setStartLine(400, 600);
					}
					LeaveCriticalSection(&cs);
				}
			}
<<<<<<< HEAD
<<<<<<< HEAD

=======
			for (int i = 0; i < m_monster.size(); ++i) {
=======
			for (int i = MonsterStartSize; i < MonsterEndSize; ++i) {
>>>>>>> RemotTest2
				EnterCriticalSection(&cs);
				m_monster[i].Update();
				LeaveCriticalSection(&cs);

				if (m_monster[i].getisAttack()) {
					EnterCriticalSection(&cs);
					put[0].AttackMonsterId = i;
					put[1].AttackMonsterId = i;
					m_bullet[i].InitBullet(m_monster[i]);
					m_monster[i].setisAttack(false);
					LeaveCriticalSection(&cs);

				}
			}

			for (int i = MonsterStartSize; i < MonsterEndSize; ++i) {
				//충돌 후 터지는 애니메이션 재생
				if (m_bullet[i].isColl && m_bullet[i].anim >= m_bullet[i].anim_max) {
					m_bullet[i].DeadAnimation(m_monster[i]);
				}
				//플레이어와 충돌
				if (player[client_id].CollsionByObstacle(m_bullet[i]) && m_bullet[i].isColl == false && m_bullet[i].isStart) {
					put[client_id].clear = true;
					put[(client_id + 1) % 2].clear = true;
					m_map.clear();
					m_bullet[i].setisColl(true);
					if (Current_Stage == 3) {
						player[0].setStartLine(356, 270);
						player[1].setStartLine(452, 270);
					}
					else {
						player[0].setStartLine(200, 600);
						player[1].setStartLine(400, 600);
					}
				}
				//총알 생성 후 이동 & 애니메이션
				if (m_bullet[i].isStart) {
					EnterCriticalSection(&cs);
					m_bullet[i].animation();
					m_bullet[i].Update();
					LeaveCriticalSection(&cs);
				}

				// 클라이언트에 전달하기 위한 패킷에 총알 정보 전달.
				EnterCriticalSection(&cs);
				m_bullet[i].SetBulletInfo(bullet[i]);
				LeaveCriticalSection(&cs);
			}

			//클라이언트에 전달하기 위한 패킷에 플레이어 정보 전달.
			player[client_id].setPlayerInfo(sc_p[client_id]);

<<<<<<< HEAD
			//for (int i = 0; i < vec_bullet.size(); ++i) {
			//	if (vec_bullet[i].isColl && vec_bullet[i].anim == 0) {
			//		EnterCriticalSection(&cs);
			//		vec_bullet.erase(vec_bullet.begin() + i);
			//		LeaveCriticalSection(&cs);

			//	}
			//}
			//EnterCriticalSection(&cs);
			//put[0].bulletsize = vec_bullet.size();
			//put[1].bulletsize = vec_bullet.size();
			//LeaveCriticalSection(&cs);
			//for (int i = 0; i < vec_bullet.size(); ++i) {			

			//	//if (player[Client_Count].CollsionByObstacle(vec_bullet[i]) && vec_bullet[i].getisColl() != true) {
			//	if (vec_bullet[i].getisColl()) {
			//		EnterCriticalSection(&cs);
			//		
			//		bullet[i].isColl = true;
			//		//vec_bullet.erase(vec_bullet.begin() + i);
			//		
			//		LeaveCriticalSection(&cs);				
			//	}

			//	EnterCriticalSection(&cs);

			//	vec_bullet[i].animation();
			//	vec_bullet[i].Update();

			//	bullet[i].x = vec_bullet[i].x;
			//	bullet[i].y = vec_bullet[i].y;
			//	bullet[i].type = vec_bullet[i].type;
			//	bullet[i].imageCount = vec_bullet[i].imageCount;
			//	bullet[i].imageSizeX = vec_bullet[i].imageSizeX;
			//	bullet[i].imageSizeY = vec_bullet[i].imageSizeY;
			//	bullet[i].anim = vec_bullet[i].anim;

			//	LeaveCriticalSection(&cs);
			//	//	vec_bullet[i].setisColl(true);
			//	//	m_map.clear();
			//	//}

			//}

			//if (vec_bullet[i].isColl && vec_bullet[i].anim == 0)
			//	vec_bullet.erase(vec_bullet.begin() + i);
>>>>>>> parent of c122221 (push_bullet)


			sc_p[client_id].id = client_id;
			sc_p[client_id].state = player[client_id].getState();
			sc_p[client_id].x = player[client_id].getPos().x;
			sc_p[client_id].y = player[client_id].getPos().y;
			sc_p[client_id].dir = player[client_id].getDir();
			sc_p[client_id].jumpCount = player[client_id].getJumpCount();


			//int b_check = 0;
			//for (int i = 0; i < 2; ++i) {
			//	if (put[client_id].isPush[i]) b_check++;
			//	if (put[(client_id + 1) % 2].isPush[i]) b_check++;

			//}
			//if (b_check == 4) {
			//	cout << "Next Stage" << endl;
			//}
			//else {
			//	b_check = 0;
			//	cout << b_check << endl;
			//}
=======
			if (Current_Stage != Next_Stage) {
				EnterCriticalSection(&cs);
				Current_Stage++;
				MapStartSize = MapSize[Current_Stage - 2];
				MapEndSize = MapSize[Current_Stage - 1];

				MonsterStartSize = MonsterSize[Current_Stage - 2];
				MonsterEndSize = MonsterSize[Current_Stage - 1];
>>>>>>> RemotTest2

				ObstacleStartSzie = ObstacleSize[Current_Stage - 2];
				ObstacleEndSize = ObstacleSize[Current_Stage - 1];
				m_map.clear();
				for (int i = 0; i < 2; ++i) {
					put[client_id].isPush[i] = false;
					put[(client_id + 1) % 2].isPush[i] = false;
				}
				LeaveCriticalSection(&cs);
			}

			put[client_id].Current_Stage = Current_Stage;
			send(clientSock, (char*)&sc_p, sizeof(sc_p), 0);
			send(clientSock, (char*)&sc_obs, sizeof(sc_obs), 0);
			send(clientSock, (char*)&put[client_id], sizeof(put[client_id]), 0);

			put[client_id].isClick = false;
<<<<<<< HEAD

		}
		StartTime = NowTime;
=======
			put[client_id].AttackMonsterId = -1;
			put[client_id].clear = false;
		}
>>>>>>> RemotTest2
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
	MessageBox(NULL, (LPCTSTR)lpmsgbuf, (LPCWSTR)msg, MB_ICONERROR);
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

void InitGameObject()
{
	player.push_back(Player(200, 600, 0));
	player.push_back(Player(400, 600, 1));

	m_static_map.push_back(Map(MAP::BUTTON, 48, 344));
	m_static_map.push_back(Map(MAP::BUTTON, 1392, 344));
	m_static_map.push_back(Map(MAP::PLAT, 48, 374));
	m_static_map.push_back(Map(MAP::PLAT, 1392, 374));
	m_static_map.push_back(Map(MAP::PLAT, 48, 566));
	m_static_map.push_back(Map(MAP::PLAT, 432, 214));
	m_static_map.push_back(Map(MAP::PLAT, 1008, 214));
	//first

	m_static_map.push_back(Map(MAP::BUTTON, 48, 312));
	m_static_map.push_back(Map(MAP::BUTTON, 432, 312));
	m_static_map.push_back(Map(MAP::PLAT, 500, 550));
	m_static_map.push_back(Map(MAP::PLAT, 288, 150));
	m_static_map.push_back(Map(MAP::PLAT, 432, 342));
	m_static_map.push_back(Map(MAP::PLAT, 1392, 510));
	m_static_map.push_back(Map(MAP::PLAT, 1392, 210));
	m_static_map.push_back(Map(MAP::PLAT, 250, 342));
	//second

	m_static_map.push_back(Map(MAP::BUTTON, 1392, 24));
	m_static_map.push_back(Map(MAP::BUTTON, 1296, 664));
	m_static_map.push_back(Map(MAP::PLAT, 480, 374));
	m_static_map.push_back(Map(MAP::PLAT, 576, 374));
	m_static_map.push_back(Map(MAP::PLAT, 384, 374));
	m_static_map.push_back(Map(MAP::PLAT, 48, 598));
	m_static_map.push_back(Map(MAP::PLAT, 336, 502));
	m_static_map.push_back(Map(MAP::PLAT, 624, 758));
	m_static_map.push_back(Map(MAP::PLAT, 816, 310));
	m_static_map.push_back(Map(MAP::PLAT, 1080, 726));
	m_static_map.push_back(Map(MAP::PLAT, 1200, 726));
	m_static_map.push_back(Map(MAP::PLAT, 1296, 694));
	m_static_map.push_back(Map(MAP::PLAT, 1104, 54));
	m_static_map.push_back(Map(MAP::PLAT, 1392, 54));
	m_static_map.push_back(Map(MAP::PLAT, 1392, 434));


	m_monster.push_back(Monster(MONSTER::PLANT, 1392, 775, 300));
	m_monster.push_back(Monster(MONSTER::RPLANT, 48, 531, 300));
	m_monster.push_back(Monster(MONSTER::PIG, 432, 184, 200));
	m_monster.push_back(Monster(MONSTER::PIG, 1008, 184, 200));
	//first

	m_monster.push_back(Monster(MONSTER::RPLANT, 250, 307, 5000));
	m_monster.push_back(Monster(MONSTER::PLANT, 1392, 180, 5000));
	m_monster.push_back(Monster(MONSTER::PLANT, 1392, 480, 5000));
	m_monster.push_back(Monster(MONSTER::PLANT, 1392, 780, 5000));
	m_monster.push_back(Monster(MONSTER::PIG, 500, 520, 5000));
	//second

	m_monster.push_back(Monster(MONSTER::RPLANT, 48, 563, 5000));
	m_monster.push_back(Monster(MONSTER::TREE, 1392, 404, 5000));
	m_monster.push_back(Monster(MONSTER::RPLANT, 336, 467, 5000));
	m_monster.push_back(Monster(MONSTER::PIG, 624, 728, 5000));
	m_monster.push_back(Monster(MONSTER::PIG, 816, 280, 5000));
	m_monster.push_back(Monster(MONSTER::PIG, 1080, 696, 5000));
	m_monster.push_back(Monster(MONSTER::PIG, 1200, 696, 5000));



	m_obstacle.push_back(Obstacle(OBSTACLE::BLADE, 100, 500));
	m_obstacle.push_back(Obstacle(OBSTACLE::BLADE, 300, 500));
	//first

	m_obstacle.push_back(Obstacle(OBSTACLE::BLADE, 790, 600));
	m_obstacle.push_back(Obstacle(OBSTACLE::BLADE, 288, 150));

	m_obstacle.push_back(Obstacle(OBSTACLE::MIDDLE_UP, 912, 406));
	m_obstacle.push_back(Obstacle(OBSTACLE::SHORT, 144, 280));
	m_obstacle.push_back(Obstacle(OBSTACLE::LONG, 432, 402));
	//second

	m_obstacle.push_back(Obstacle(OBSTACLE::BLADE, 48, 374));
	m_obstacle.push_back(Obstacle(OBSTACLE::BLADE, 1296, 654));

	m_obstacle.push_back(Obstacle(OBSTACLE::LONG, 300, 800));
	m_obstacle.push_back(Obstacle(OBSTACLE::LONG, 1100, 800));
	m_obstacle.push_back(Obstacle(OBSTACLE::LONG_UP, 576, -100));
}