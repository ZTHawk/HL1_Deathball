#include "player_data.hpp"
#include "utilfunctions.hpp"
#include "ctf_mod.h"

FLAG_PLAYER player_data[33];

void FLAG_PLAYER::init( edict_t *player )
{
	ID = ENTINDEX(player);
	pEntity = player;
	
	old_team = 0;
	
	myFlag = NULL;
	enemyFlag = NULL;
	
	name[0] = 0;
	ingame = true;
}

void FLAG_PLAYER::reset( int in_game )
{
	myFlag = NULL;
	enemyFlag = NULL;
	
	old_team = 0;
	
	if ( in_game == 0 )
	{
		pEntity = NULL;
		ID = 0;
		ingame = false;
		name[0] = 0;	// reset
	}
}

void FLAG_PLAYER::think( )
{
	if ( enemyFlag == NULL )
		return;
	
	if ( old_team != pEntity->v.team )
	{
		drop();
		old_team = pEntity->v.team;
		
		return;
	}
	
	if ( UTIL_isAlive(pEntity) == false )
	{
		drop();
		
		return;
	}
	
	if ( myFlag == NULL )
	{
		if ( FlagA.pEntity->v.team == pEntity->v.team )
			myFlag = &FlagA;
		else
			myFlag = &FlagB;
		
		UTIL_ServerPrint("myflag was NULL / is %p / A = %p / B = %p \n", myFlag, FlagA, FlagB);
		return;
	}
	
	// check capture position
	if ( pEntity->v.origin.x < myFlag->minX
		|| pEntity->v.origin.x > myFlag->maxX
		|| pEntity->v.origin.y < myFlag->minY
		|| pEntity->v.origin.y > myFlag->maxY
		|| pEntity->v.origin.z < myFlag->minZ
		|| pEntity->v.origin.z > myFlag->maxZ )
		return;
	
	FLAG_ENT *dummyFlag = enemyFlag;
	drop();
	
	UTIL_sound_team(pEntity->v.team, FLAG_TEAM_CAPTURED, FLAG_ENEMY_CAPTURED, pEntity, FLAG_CAPTURED);
	dummyFlag->captured();
	*(myFlag->myTeamPoints) += 1;
}

void FLAG_PLAYER::drop( )
{
	enemyFlag->carierID = 0;
	
	DROP_TO_FLOOR(enemyFlag->pEntity);
	enemyFlag->pEntity->v.origin.z += FLAG_Z_ORIGIN_ADDER;
	
	enemyFlag->pEntity->v.sequence = 1;
	
	enemyFlag->idleTime = gpGlobals->time + FLAG_IDLE_TIME;
	
	enemyFlag = NULL;
}


