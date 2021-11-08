#pragma once

const short SERVER_PORT = 4000;

const int  MAX_NAME_SIZE = 20;
const int  MAX_USER = 10;

const char CS_PACKET_LOGIN = 1;
const char CS_PACKET_MOVE = 2;
const char CS_MOUSE_INPUT = 3;
const char CS_CLOSE_ANIMATION = 4;
const char CS_PUT_BUTTON = 5; 

const char SC_PACKET_LOGIN_OK = 1;
const char SC_PACKET_MOVE = 2;
const char SC_PACKET_PUT_OBJECT = 3;
const char SC_PACKET_REMOVE_OBJECT = 4;
const char SC_SET_ANIMATION = 5;
const char SC_CLEAR_STAGE = 6;
const char SC_PLAYER_COLLISION = 7;


#pragma pack (push, 1)
struct cs_packet_login {
	unsigned char size;
	char	packet_type;
	char	name[MAX_NAME_SIZE];
};

struct sc_login_ok {
	unsigned char size;
	char	packet_type;
	int		id;
	short	x, y;
};


//cs
struct cs_move {
	unsigned char size;
	char packet_type;
	int type; //왼,오 점프 
};

struct cs_mouse_input{
	unsigned char size;
	char packet_type;
	float x,y;
};

struct cs_close_animation{		//애니메이션 동작이 끝났음을 알림. 
	unsigned char size;
	char packet_type;
	bool isDone;
};

struct cs_put_button{		//플레이어가 버튼을 누르고 있음을 알림. 
	unsigned char size;
	char packet_type;
	bool isPush;
};

//sc

struct sc_move {
	unsigned char size;
	char packet_type;
	float x ,y;
	int dir;
};


struct sc_put_object {
	unsigned char size;
	char packet_type;
	float x,y;
	int type;		//무엇이 들어왔는지 알리기 위함, Bullet 중 Bullet의 종류가 다를것이고 Map설치가 될수도있음. 
};


struct sc_remove_object {
	unsigned char size;
	char packet_type;
	int type;
	int index;
};

struct sc_set_animation{
	unsigned char size;
	char packet_type;
	int type;			//애니메이션 타입 변경 
	int animCount = 0;		//애니메이션 진행 카운트 초기화 
	int maxCount;	//애니메이션의 최대 카운트 변경
 
};

struct sc_clear_stage{	//스테이지 변경
	unsigned char size;
	char packet_type;
	int stage;			 
}; 

struct sc_player_collision{	//몬스터에 의한 충돌
	unsigned char size;
	char packet_type;
	
	float x,y; 				//플레이어 초기 위치값.
	
};




#pragma pack(pop)
