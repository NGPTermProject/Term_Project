#include "Server.h"
#include <iostream>
#include "protocol.h"

Server::Server()
{
	GameThread = CreateThread(NULL, 0, GameThread, NULL, 0, NULL);
	StartFlag = false;
}

Server::~Server()
{

}

void Server::err_display(int err_num)
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

void Server::err_quit(const char* msg)
{
	LPVOID lpmsgbuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpmsgbuf, 0, NULL
	);
	MessageBox(NULL, (LPCTSTR)lpmsgbuf, (LPCSTR)msg, MB_ICONERROR);
	LocalFree(lpmsgbuf);
	exit(-1);
}

int Server::recvn(SOCKET s, char* buf, int len, int flags)
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




DWORD WINAPI Server::InitServer()
{
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


	SOCKET c_socket;
	SOCKADDR_IN c_addr;
	int c_addrlen;

	while (1) {
		// accept
		c_addrlen = sizeof(c_addr);
		c_socket = accept(server_socket, reinterpret_cast<sockaddr*>(&c_addr), &c_addrlen);
		if (c_socket == INVALID_SOCKET) {
			std::cout << "accept error\n";
			int err_num = WSAGetLastError();
			err_display(err_num);
		}

		// 대기 큐에 플레이어 저장
		Wait_Queue.push_back(c_socket);

		// 로그인 완료.



		char ip_addr[100];
		inet_ntop(AF_INET, &c_addr.sin_addr, ip_addr, 100);

		// 접속한 클라이언트 정보 출력
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			ip_addr, ntohs(c_addr.sin_port));
		


		if (Wait_Queue.size() == 2) {
			// 게임 시작
			StartFlag = true;

			for (int i = 0; i < Wait_Queue.size(); ++i) {
				int ret = send(Wait_Queue[i], (char*)&StartFlag, sizeof(StartFlag), 0);
				if (ret == SOCKET_ERROR) {
					std::cout << "size == 2 error \n";
					int err_num = WSAGetLastError();
					err_display(err_num);
				}


				PlayerThread = CreateThread(NULL, 0, ProcessThread, (LPVOID)Wait_Queue[i], 0, NULL);
				if (PlayerThread == NULL) closesocket(c_socket);


			}

			std::cout << "Exit Lobby Thread\n";
			ExitThread(0);
		}
		else {
			for (int i = 0; i < Wait_Queue.size(); ++i) {
				ret = send(Wait_Queue[i], (char*)&StartFlag, sizeof(StartFlag), 0);
				if (ret == SOCKET_ERROR) {
					std::cout << "size =! 2 error \n";
					int err_num = WSAGetLastError();
					err_display(err_num);
				}

			}
		}

	}

}

DWORD WINAPI Server::ProcessThread(LPVOID arg)
{
	std::cout << "ProcesThread Running\n";

	SOCKET c_socket = (SOCKET)arg;
	SOCKADDR_IN c_addr;

	int ret;
	int c_addrlen;

	//클라이언트 정보 얻기.
	c_addrlen = sizeof(c_addr);
	getpeername(c_socket, (SOCKADDR*)&c_addr, &c_addrlen);

	//클라이언트와 데이터 통신

	return 0;
}

void Server::LoginServer()
{
}

void Server::LobbyServer()
{

}

void Server::UpdateStatus()
{

}

void Server::UpdateCollision()
{

}

void Server::InGame()
{
	while (true) {
		UpdateStatus();
		UpdateCollision();
	}


}
