#include "ball_data.hpp"
#include <algorithm>

float vars_float[FLOAT_END] =
{
	1.0,
	1.0,
	9999.0,
	-300.0,
	300.0,
	10.0,
};
int vars_int[INT_END] =
{
	100,
	400,
	300,
};
bool vars_bool[BOOL_END] =
{
	true,
	true,
	true,
	true,
	false,
};

int ball_count = 0;
map<edict_t*, Ball*> ballEntMap;
vector<Ball*> ballList;

Ball::Ball( )
{
	ent = CREATE_NAMED_ENTITY(ALLOC_STRING("info_target"));
	ballEntMap[ent] = this;
	ballList.push_back(this);
	thinkDone = false;
	modelname = NULL;
	
	++ball_count;
}

Ball::~Ball( )
{
	REMOVE_ENTITY(ent);
	vector<Ball*>::iterator it = std::find(ballList.begin(), ballList.end(), this);
	if ( it != ballList.end() )
		ballList.erase(it);
	map<edict_t*, Ball*>::iterator itMap = ballEntMap.begin();
	while ( itMap != ballEntMap.end() )
	{
		if ( itMap->second != this )
		{
			++itMap;
			continue;
		}
		ballEntMap.erase(itMap);
		break;
	}
	delete[] modelname;
	--ball_count;
}

edict_t* Ball::getEnt( )
{
	return ent;
}

void Ball::Init( edict_t *player , int speed , const char *name , int type )
{
	if ( vars_int[BALL_LIMIT] <= ball_count )
		return;
	
	if ( speed == 0 )
		speed = BASE_SPEED;
	
	MAKE_VECTORS(player->v.v_angle);
	Vector vVector = gpGlobals->v_forward * speed;
	
	gpGamedllFuncs->dllapi_table->pfnSpawn(ent);
	
	ent->v.classname = ALLOC_STRING("deathball");
	if ( name != NULL
		&& strlen(name) > 1 )
	{
		modelname = new char[strlen(name) + 1];
		strcpy(modelname, name);
		modelname[strlen(name)] = '\0';
		ent->v.model = ALLOC_STRING(name);
		SET_MODEL(ent, name);
	}else
	{
		ent->v.model = ALLOC_STRING(BALL_MODEL);
		SET_MODEL(ent, BALL_MODEL);
	}
	
	ent->v.origin = player->v.origin + Vector(0, 0, 70);
	ent->v.movetype = MOVETYPE_BOUNCEMISSILE;
	ent->v.velocity = vVector;
	
	ent->v.health = vars_float[BALL_HP];
	ent->v.max_health = ent->v.health;
	//ent->v.solid = SOLID_TRIGGER;
	ent->v.solid = SOLID_BBOX;
	ent->v.team = player->v.team;
	
	ent->v.mins = Vector(-16, -16, -16);
	ent->v.maxs = Vector(16, 16, 16);
	SET_SIZE(ent, ent->v.mins, ent->v.maxs);
	
	Vector vAngle = Vector(0, 0, 0);
	VEC_TO_ANGLES(vVector, vAngle);
	ent->v.angles = vAngle;
	
	ent->v.nextthink = gpGlobals->time + BASE_THINK_TIME;
	
	ent->ENT_TYPE = type;
	
	if ( vars_bool[CAN_KILL] == 1 )
		ent->v.takedamage = 1.0;
}

void Ball::Think( )
{
	thinkDone = true;
	if ( ent->v.velocity == Vector(0, 0, 0)
		|| ent->v.oldorigin == ent->v.origin )
	{
		static float rand_a = (float)vars_int[BALL_SPEED_MAX];
		static float rand_b = -(float)vars_int[BALL_SPEED_MAX];
		ent->v.velocity = Vector(RANDOM_FLOAT(rand_a, rand_b), RANDOM_FLOAT(rand_a, rand_b), RANDOM_FLOAT(rand_a, rand_b));
		
		UTIL_unstuck(ent);
	}
	
	// correct angle after bounce
	Vector vAngle = Vector(0, 0, 0);
	VEC_TO_ANGLES(ent->v.velocity, vAngle);
	ent->v.angles = vAngle;
	
	ent->v.oldorigin = ent->v.origin;
	
	// glow part
	ent->v.rendermode = kRenderNormal;
	ent->v.renderfx = kRenderFxGlowShell;
	float rand2_a = 0.0;
	float rand2_b = 255.0;
	ent->v.rendercolor = Vector(RANDOM_FLOAT(rand2_a, rand2_b), RANDOM_FLOAT(rand2_a, rand2_b), RANDOM_FLOAT(rand2_a, rand2_b));
	ent->v.renderamt = RANDOM_FLOAT(rand2_a, rand2_b);
	
	ent->v.nextthink = gpGlobals->time + BASE_THINK_TIME;
	
	switch ( ent->ENT_TYPE )
	{
		case BALL_NORMAL:
		{
			think_normal();
			break;
		}
		case BALL_CRAZY:
		{
			think_crazy();
			break;
		}
		case BALL_DEATH:
		{
			think_death();
			break;
		}
		case BALL_PSYCHO:
		{
			think_psycho();
			break;
		}
	}
}

void Ball::think_normal( )
{
	
}

void Ball::think_crazy( )
{
	SET_MODEL(ent, modellist[RANDOM_LONG(0, MODELLIST_LEN_0)]);
	
	ent->v.nextthink = gpGlobals->time + RANDOM_FLOAT(vars_float[CRAZY_TIME_MIN], vars_float[CRAZY_TIME_MAX]);
}

void Ball::think_death( )
{
	
}

void Ball::think_psycho( )
{
	ent->v.velocity = Vector(RANDOM_FLOAT(vars_float[PSYCHO_VEL_MIN], vars_float[PSYCHO_VEL_MAX]), RANDOM_FLOAT(vars_float[PSYCHO_VEL_MIN], vars_float[PSYCHO_VEL_MAX]), RANDOM_FLOAT(vars_float[PSYCHO_VEL_MIN], vars_float[PSYCHO_VEL_MAX]));
	static Vector vAngle = Vector(0, 0, 0);
	VEC_TO_ANGLES(ent->v.velocity, vAngle);
	ent->v.angles = vAngle;
}

void Ball::Touch( edict_t *other )
{
	if ( thinkDone == false )
		return;
	
	if ( other->ENT_TYPE >= BALL_NORMAL )
		return;
	
	int otherID = ENTINDEX(other);
	if ( otherID > gpGlobals->maxClients
		|| otherID <= 0 )
		return;
	
	switch ( ent->ENT_TYPE )
	{
		case BALL_NORMAL:
		{
			touch_normal(other);
			break;
		}
		case BALL_CRAZY:
		{
			touch_crazy(other);
			break;
		}
		case BALL_DEATH:
		{
			touch_death(other);
			break;
		}
		case BALL_PSYCHO:
		{
			touch_psycho(other);
			break;
		}
	}
	
	if ( vars_bool[ent->ENT_TYPE - BALL_NORMAL] == false )
	{
		return;
	}
	
	if ( gpGlobals->time <= ent->SPLIT_DELAY )
	{
		ent->v.nextthink = gpGlobals->time + FAST_THINK_TIME;
		return;
	}
	
	ent->SPLIT_DELAY = gpGlobals->time + DELAY_BEFORE_NEXT_SPLIT;
	Ball *newBall = new Ball();
	newBall->Init(other, RANDOM_LONG(vars_int[BALL_SPEED_MIN], vars_int[BALL_SPEED_MAX]), modelname, ent->ENT_TYPE);
}

void Ball::touch_normal( edict_t *other )
{
	
}

void Ball::touch_crazy( edict_t *other )
{
	
}

void Ball::touch_death( edict_t *other )
{
	UTIL_killPlayer(other);
}

void Ball::touch_psycho( edict_t *other )
{
	
}
