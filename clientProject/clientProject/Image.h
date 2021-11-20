#include <atlimage.h>

// Player
static CImage player_idle;
static CImage player_move;
static CImage player_jump;
static CImage player_fall;
static CImage player_dead;

// BackGround
static CImage img_bg;

// Monster
static CImage monster;
static CImage monster_idle;
static CImage monster_dead;

// Wall
static CImage img_wall;


// Width and Height
static int img_bg_width;
static int img_bg_height;

static CImage img_ButtonUp;
static CImage img_ButtonDown;
static CImage img_Bullet;
static CImage img_Bullet_Coll;
static CImage img_Bomb;
static CImage img_Bomb_Coll;

static CImage img_Bomb_Monster_Attack;
static CImage img_Bomb_Monster_Idle;

static CImage img_Plant_Monster_Idle;
static CImage img_Plant_Monster_Attack;


static CImage img_Bullet_Monster_Attack;
static CImage img_Bullet_Monster_Idle;


static CImage img_wall_Long;
static CImage img_wall_Middle;

static CImage img_Login;

static CImage img_cutting_blade;

static CImage img_Obs_960_96_Up;
static CImage img_Obs_960_96;
static CImage img_Obs_544_96_Up;
static CImage img_Obs_544_96;
static CImage img_Obs_320_96;

void LoadImage();
void LoadImage()
{
	//id입력창
	img_Login.Load("white.png");
	//버튼
	img_ButtonUp.Load("button_up.png");
	img_ButtonDown.Load("button_down.png");

	//총알
	img_Bullet.Load("bullet_move.png");
	img_Bullet_Coll.Load("bullet_coll.png");
	img_Bomb.Load("bomb.png");
	img_Bomb_Coll.Load("bomb_coll.png");


	//배경
	img_bg.Load("background_grass.png");
	img_bg_width = img_bg.GetWidth();
	img_bg_height = img_bg.GetHeight();

	//발판
	img_wall.Load("block_grass.png");

	//몬스터
	img_Bomb_Monster_Attack.Load("monster_bomb1.png");
	img_Bomb_Monster_Idle.Load("monster_bomb_idle.png");
	img_Plant_Monster_Attack.Load("plant_attack.png");
	img_Plant_Monster_Idle.Load("plant_idle.png");



	//플레이어
	player_idle.Load("frog_idle.png");
	player_move.Load("frog_move.png");
	player_jump.Load("frog_jump.png");
	player_fall.Load("frog_fall.png");

	//장애물
	img_cutting_blade.Load("cutting_blade.png");
	img_Obs_960_96_Up.Load("Obs960x96_Up.png");
	img_Obs_960_96.Load("Obs960x96.png");
	img_Obs_544_96_Up.Load("Obs544x96_Up.png");
	img_Obs_544_96.Load("Obs544x96.png");
	img_Obs_320_96.Load("Obs544x96_Up.png");
}
