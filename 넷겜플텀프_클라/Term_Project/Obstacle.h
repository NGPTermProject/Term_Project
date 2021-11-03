#define OBS_GARO_COUNT 20
#define OBS_SERO_COUNT 36


// 가로 장애물 2개
struct Obs_Garo {
	int pos_x;
	int pos_y;
	int rand_num;
};

// 세로 장애물 3개
struct Obs_Sero {
	int pos_x;
	int pos_y;
	int rand_num;
};

static Obs_Garo obs_garo[OBS_GARO_COUNT];
static Obs_Sero obs_sero[OBS_SERO_COUNT];
static int garo_speed1 = 8;
static int garo_speed2 = 10;

void InitObstacle()
{
	for (int i = 0; i < OBS_GARO_COUNT; ++i) {
		obs_garo[i].pos_x = (i / 2 + 1) * 1000 - 50;

		if (i % 2 == 0)
			obs_garo[i].rand_num = 100;
		else if (i % 1 == 1)
			obs_garo[i].rand_num = 400;
	}

	for (int i = 0; i < OBS_SERO_COUNT; ++i) {
		obs_sero[i].pos_x = (i / 3 * 1000) + (i % 3) * 200;
	}
}