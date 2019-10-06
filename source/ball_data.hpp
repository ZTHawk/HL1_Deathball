#ifndef _BALL_DATA_H
#define _BALL_DATA_H

#include "plugin.hpp"
#include <vector>
#include <map>

using namespace std;

enum
{
	BALL_NORMAL = 9000,
	BALL_CRAZY,
	BALL_DEATH,
	BALL_PSYCHO,
};

enum
{
	FLOAT_START = 0,
	CRAZY_TIME_MIN = 0,
	CRAZY_TIME_MAX,
	DEATH_DMG,
	PSYCHO_VEL_MIN,
	PSYCHO_VEL_MAX,
	BALL_HP,
	FLOAT_END,
	
	INT_START = FLOAT_END,
	BALL_SPEED_MIN = 0,
	BALL_SPEED_MAX,
	BALL_LIMIT,
	INT_END,
	
	BOOL_START = FLOAT_END + INT_END,
	SPLIT_NORMAL = 0,
	SPLIT_CRAZY,
	SPLIT_DEATH,
	SPLIT_PSYCHO,
	CAN_KILL,
	BOOL_END,
};

extern float vars_float[FLOAT_END];
extern int vars_int[INT_END];
extern bool vars_bool[BOOL_END];

static const char *vars_names[] =
{
	"crazy_time_min",
	"crazy_time_max",
	"death_dmg",
	"psycho_vel_min",
	"psycho_vel_max",
	"hp",
	
	"ball_speed_min",
	"ball_speed_max",
	"ball_limit",
	
	"split_normal",
	"split_crazy",
	"split_death",
	"split_psycho",
	"can_kill",
};

#define DELAY_BEFORE_NEXT_SPLIT		0.1f
#define BASE_THINK_TIME			0.3f
#define FAST_THINK_TIME			0.1f

#define BASE_SPEED				200

#define MODELLIST_LEN_0		10
static const char *modellist[MODELLIST_LEN_0 + 1] =
{
	"models/player/alien1/alien1.mdl",
	"models/player/alien2/alien2.mdl",
	"models/player/alien3/alien3.mdl",
	"models/player/alien4/alien4.mdl",
	"models/player/alien5/alien5.mdl",
	"models/player/soldier/soldier.mdl",
	"models/player/gestate/gestate.mdl",
	"models/player/heavy/heavy.mdl",
	BALL_MODEL,
	"models/hive.mdl",
	"models/b_armory.mdl"
};

#define CONFIG_FLOAT(var) \
	else if ( FStrEq(CMD_ARGV(2), vars_names[FLOAT_START + var]) ) \
	{ \
		if ( arg_num == 3 ) \
			UTIL_ServerPrint("%s = %f\n", vars_names[FLOAT_START + var], vars_float[var]); \
		else \
			vars_float[var] = atof(CMD_ARGV(3)); \
	}

#define CONFIG_INT(var) \
	else if ( FStrEq(CMD_ARGV(2), vars_names[INT_START + var]) ) \
	{ \
		if ( arg_num == 3 ) \
			UTIL_ServerPrint("%s = %d\n", vars_names[INT_START + var], vars_int[var]); \
		else \
			vars_int[var] = atoi(CMD_ARGV(3)); \
	}

#define CONFIG_BOOL(var) \
	else if ( FStrEq(CMD_ARGV(2), vars_names[BOOL_START + var]) ) \
	{ \
		if ( arg_num == 3 ) \
			UTIL_ServerPrint("%s = %d\n", vars_names[BOOL_START + var], vars_bool[var]); \
		else \
			vars_bool[var] = atoi(CMD_ARGV(3)) > 0 ? true : false; \
	}

class Ball
{
public:
	Ball( );
	~Ball( );
	void Think( );
	void Touch( edict_t *other );
	edict_t* getEnt( );
	void Init( edict_t *player , int speed , const char *name , int type );
	
private:
	edict_t *ent;
	bool thinkDone;
	char *modelname;
	
	void think_normal( );
	void think_crazy( );
	void think_death( );
	void think_psycho( );
	void touch_normal( edict_t *other );
	void touch_crazy( edict_t *other );
	void touch_death( edict_t *other );
	void touch_psycho( edict_t *other );
};

extern int ball_count;
extern map<edict_t*, Ball*> ballEntMap;
extern vector<Ball*> ballList;

#endif
