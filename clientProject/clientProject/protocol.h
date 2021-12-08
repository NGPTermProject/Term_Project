#pragma once

const short SERVER_PORT = 9000;



#pragma pack (push, 1)


struct cs_send_player_id {
	short id;
};

struct cs_send_player {
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
	float x = 0;
	float y = 0;

};

struct cs_obstacle {
	float x;
	float y; 
	bool isColl;
};

struct sc_button {
	bool isPush[2];
};

struct cs_bullet {
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
struct sc_put_object {

	float x, y;
	bool isClick;
	bool isPush[2];
	int AttackMonsterId;
	bool clear = false;
	int Stage;
};

struct cs_login_button {
	bool buttonCheck[2];
	int buttonid;
	int id;
};

struct cs_login_info {
	int buttonid;
	int id;
};


struct cs_start_game {
	bool gamestart;
	int stage;
};


#pragma pack(pop)

