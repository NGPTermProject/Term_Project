namespace PLAYER
{
	enum P_State {
		IDLE,
		MOVE,
		JUMP,
		FALL,
		ATTACK,
		DEAD
	};
}

namespace MONSTER
{
	enum M_State {
		PIG,
		PLANT
	};
}

namespace OBSTACLE {
	enum M_Obstacle {
		BLADE,
		LONG_UP,
		LONG,
		MIDDLE_UP,
		MIDDLE,
		SHORT
	};
}
namespace MAP {
	enum M_Map {
		BUTTON,
		PLAT,
		LONG,
	};
}

static PLAYER::P_State p_state;
static MONSTER::M_State m_state;
static MAP::M_Map map_state;