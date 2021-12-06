#pragma once

const short SERVER_PORT = 9000;

#pragma pack (push, 1)
<<<<<<< HEAD
struct cs_packet_login {
	unsigned char size;
	char	packet_type;
};

struct sc_login_ok {
	unsigned char size;
	char	packet_type;
	short		id;
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
=======
>>>>>>> RemotTest2

struct sc_send_player_id {
	short id;
};

struct sc_send_player {
	short id;
	int state;

	float x;
	float y;

	int jumpCount;
	int dir;

};

struct cs_send_keyinfo {
	short id;
	bool left = false;
	bool right = false;
	bool jump = false;
	bool isClick = false;
	float x;
	float y;
};

struct sc_obstacle {
	float x;
	float y;
	bool isColl = false;
};

struct sc_bullet {
	float x;
	float y;
	bool isColl = false;
	int type = 0;
	int imageCount;
	int	imageSizeX;
	int	imageSizeY;
	int anim;
	bool isStart = false;
};

struct sc_update {
	unsigned char size;
	char packet_type;
	float x, y;
	int type;		//무엇이 들어왔는지 알리기 위함, Bullet 중 Bullet의 종류가 다를것이고 Map설치가 될수도있음. 
	bool isClick;
	bool isPush[2];
	int AttackMonsterId = -1;
	bool clear = false;
	int Current_Stage;
};

struct sc_start_game {
	bool gamestart;
	int stage;
};
#pragma pack(pop)
