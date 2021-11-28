#include <atlimage.h>

// Player
static CImage player_idle;
static CImage player_move;
static CImage player_jump;
static CImage player_fall;
static CImage player_dead;

static CImage player2_idle;
static CImage player2_move;
static CImage player2_jump;
static CImage player2_fall;
static CImage player2_dead;

// BackGround
static CImage img_bg;

// Monster
static CImage monster;
static CImage monster_idle;
static CImage monster_dead;

// Wall
static CImage img_wall;
static CImage img_longwall;


// Width and Height
static int img_bg_width;
static int img_bg_height;


static int img_Loginbg_width;
static int img_Loginbg_height;

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
static CImage img_LoginBG;

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
	img_LoginBG.Load(L"LoginBG.png");
	img_Loginbg_width = img_LoginBG.GetWidth();
	img_Loginbg_height = img_LoginBG.GetHeight();
	img_Login.Load(L"Login.png");

	//버튼
	img_ButtonUp.Load(L"button_up.png");
	img_ButtonDown.Load(L"button_down.png");

	//총알
	img_Bullet.Load(L"bullet_move.png");
	img_Bullet_Coll.Load(L"bullet_coll.png");
	img_Bomb.Load(L"bomb.png");
	img_Bomb_Coll.Load(L"bomb_coll.png");


	//배경
	img_bg.Load(L"background_grass.png");
	img_bg_width = img_bg.GetWidth();
	img_bg_height = img_bg.GetHeight();

	//발판
	img_wall.Load(L"block_grass.png");
	img_longwall.Load(L"grass_ground.png");

	//몬스터
	img_Bomb_Monster_Attack.Load(L"monster_bomb1.png");
	img_Bomb_Monster_Idle.Load(L"monster_bomb_idle.png");
	img_Plant_Monster_Attack.Load(L"plant_attack.png");
	img_Plant_Monster_Idle.Load(L"plant_idle.png");



	//플레이어
	player_idle.Load(L"frog_idle.png");
	player_move.Load(L"frog_move.png");
	player_jump.Load(L"frog_jump.png");
	player_fall.Load(L"frog_fall.png");

	player2_idle.Load(L"frog_idle2.png");
	player2_move.Load(L"frog_move2.png");
	player2_jump.Load(L"frog_jump2.png");
	player2_fall.Load(L"frog_fall2.png");


	//장애물
	img_cutting_blade.Load(L"cutting_blade.png");
	img_Obs_960_96_Up.Load(L"Obs960x96_Up.png");
	img_Obs_960_96.Load(L"Obs960x96.png");
	img_Obs_544_96_Up.Load(L"Obs544x96_Up.png");
	img_Obs_544_96.Load(L"Obs544x96.png");
	img_Obs_320_96.Load(L"Obs544x96_Up.png");
}
