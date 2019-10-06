//here should be the new versions of functions we get called with our hooks

#include "hookedfunctions.hpp"
#include "plugin.hpp"
#include "ball_data.hpp"
#include <sdk_util.h>	//useful almost everywhere

float counter_time = 0.0;

void CreateBall( edict_t *player , int speed , const char *name , int type );

void ServerActivate_Post( edict_t *pEdictList , int edictCount , int clientMax )
{
	PRECACHE_MODEL(BALL_MODEL);
	//REG_SVR_COMMAND("deathball", ClientCommand2);
	
	RETURN_META(MRES_IGNORED);
}

void Think( edict_t *pEntity )
{
	if ( pEntity->ENT_TYPE >= BALL_NORMAL )
		ballEntMap[pEntity]->Think();
	
	//if ( pEntity->ENT_TYPE == ENT_TYPE_BALL )
	//	UTIL_think(pEntity);
	
	//if ( FStrEq(STRING(pEntity->v.classname), "deathball") )
	//	UTIL_think(pEntity);
	
	RETURN_META(MRES_IGNORED);
}

void Touch( edict_t *pentTouched , edict_t *pentOther )
{
	if ( pentTouched->ENT_TYPE >= BALL_NORMAL )
		ballEntMap[pentTouched]->Touch(pentOther);
	else if ( pentTouched->ENT_TYPE >= BALL_NORMAL )
		ballEntMap[pentOther]->Touch(pentTouched);
	
	//if ( FStrEq(STRING(pentTouched->v.classname), "deathball") )
	//	UTIL_touch(pentTouched , pentOther);
	
	
	RETURN_META(MRES_IGNORED);
}

void ServerDeactivate_Post( void )
{
	UTIL_remove_Balls();
	
	RETURN_META(MRES_IGNORED);
}

void StartFrame( )
{
	if ( counter_time > gpGlobals->time )
		RETURN_META(MRES_IGNORED);
	
	counter_time = gpGlobals->time + BALL_COUNT_TIME;
	
	UTIL_count_Balls();
	
	RETURN_META(MRES_IGNORED);
}

void ClientCommand( edict_t *pEntity )
{
	const char* szUserCommand = CMD_ARGV(0);
	if( szUserCommand[0] == '\0' )
		RETURN_META(MRES_IGNORED);
	
	int arg_num = CMD_ARGC();
	int ID = ENTINDEX(pEntity);
	if ( FStrEq(szUserCommand, "deathball") )
	{
		if ( arg_num == 1 )
		{
			UTIL_ServerPrint("Available Commands:\n"
				"Basic\n"
				"....................\n"
				"config\n"
				"remove_all\n"
				"count_balls\n"
				"--------------------\n"
				"Creation:\n"
				"....................\n"
				"normal <opt:speed> <opt:model>\n"
				"crazy <opt:speed> <opt:model>\n"
				"death <opt:speed> <opt:model>\n"
				"psycho <opt:speed> <opt:model>\n"
				);
		}else if ( FStrEq(CMD_ARGV(1), "config") )
		{
			if ( arg_num == 2 )
			{
				UTIL_ServerPrint("Configs:\n"
					"Float:\n"
					"....................\n"
					"crazy_time_min\n"
					"crazy_time_max\n"
					"death_dmg\n"
					"psycho_vel_min\n"
					"psycho_vel_max\n"
					"--------------------\n"
					"Integer\n"
					"....................\n"
					"ball_speed_min\n"
					"ball_speed_max\n"
					"ball_limit\n"
					"hp (only new are affected)\n"
					"--------------------\n"
					"Boolean:\n"
					"....................\n"
					"split_normal\n"
					"split_crazy\n"
					"split_death\n"
					"split_psycho\n"
					"can_kill (only new are affected)\n"
					"--------------------\n"
					);
			}
			CONFIG_FLOAT(CRAZY_TIME_MIN)
			CONFIG_FLOAT(CRAZY_TIME_MAX)
			CONFIG_FLOAT(DEATH_DMG)
			CONFIG_FLOAT(PSYCHO_VEL_MIN)
			CONFIG_FLOAT(PSYCHO_VEL_MAX)
			CONFIG_FLOAT(BALL_HP)
			
			CONFIG_INT(BALL_SPEED_MIN)
			CONFIG_INT(BALL_SPEED_MAX)
			CONFIG_INT(BALL_LIMIT)
			
			CONFIG_BOOL(SPLIT_NORMAL)
			CONFIG_BOOL(SPLIT_CRAZY)
			CONFIG_BOOL(SPLIT_DEATH)
			CONFIG_BOOL(SPLIT_PSYCHO)
			CONFIG_BOOL(CAN_KILL)
		}else if ( FStrEq(CMD_ARGV(1), "remove_all") )
		{
			UTIL_remove_Balls();
		}else if ( FStrEq(CMD_ARGV(1), "count_balls") )
		{
			UTIL_ServerPrint("There are currently %d DeathBalls\n", ball_count);
		}else if ( FStrEq(CMD_ARGV(1), "normal") )
		{
			CreateBall(pEntity, atoi(CMD_ARGV(2)), CMD_ARGV(3), BALL_NORMAL);
		}else if ( FStrEq(CMD_ARGV(1), "crazy") )
		{
			CreateBall(pEntity, atoi(CMD_ARGV(2)), CMD_ARGV(3), BALL_CRAZY);
		}else if ( FStrEq(CMD_ARGV(1), "death") )
		{
			CreateBall(pEntity, atoi(CMD_ARGV(2)), CMD_ARGV(3), BALL_DEATH);
		}else if ( FStrEq(CMD_ARGV(1), "psycho") )
		{
			CreateBall(pEntity, atoi(CMD_ARGV(2)), CMD_ARGV(3), BALL_PSYCHO);
		}
		
		RETURN_META(MRES_SUPERCEDE);
	}
	
	RETURN_META(MRES_IGNORED);
}

void CreateBall( edict_t *player , int speed , const char *name , int type )
{
	Ball *newBall = new Ball();
	newBall->Init(player, speed, name, type);
}