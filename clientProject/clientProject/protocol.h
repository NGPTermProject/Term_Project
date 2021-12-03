#pragma once

const short SERVER_PORT = 9000;

const int  MAX_NAME_SIZE = 20;

#pragma pack (push, 1)
//cs


struct cs_send_keyinfo {
	short id;
	bool left = false;
	bool right = false;
	bool jump = false;
	bool isClick = false;
	float x = 0;
	float y = 0;
};

//sc

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


struct sc_obstacle {
	float x;
	float y; 
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
	float x, y;
	int type;		//무엇이 들어왔는지 알리기 위함, Bullet 중 Bullet의 종류가 다를것이고 Map설치가 될수도있음. 
	bool isClick;
	bool isPush[2];
	int AttackMonsterId;
	bool clear = false;
	int Stage;
};



struct sc_start_game {
	bool gamestart;
	int stage;
};


#pragma pack(pop)