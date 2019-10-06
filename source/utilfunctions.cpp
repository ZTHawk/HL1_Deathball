//this file exists so that you can paste functions that you need from util.cpp in the actual SDK
//as well as your new custom helper functions you declared in utilfunctions.h

//REMEMBER: to keep your DLL size down, remove functions you don't need from this file


#include "plugin.hpp"
#include <sdk_util.h>	//pre-defined function definitions saves us a lot of work
#include "utilfunctions.hpp"
#include "ball_data.hpp"

DLL_GLOBAL const Vector g_vecZero = Vector(0,0,0);

//=========================================================
// UTIL_LogPrintf - Prints a logged message to console.
// Preceded by LOG: ( timestamp ) < message >
//=========================================================
void UTIL_LogPrintf( char *fmt , ... )
{
	va_list argptr;
	static char string[1024];
	
	va_start(argptr, fmt);
	vsnprintf(string, sizeof(string), fmt, argptr);
	va_end(argptr);

	// Print to server console
	ALERT(at_logged, "%s", string);
}

// same as above but to server console
void UTIL_ServerPrint( const char *fmt , ... )
{
	va_list argptr;
	static char pszMessage[1024];
	
	// concatenate all the arguments in one string
	va_start(argptr, fmt);
	vsnprintf(pszMessage, sizeof(pszMessage), fmt, argptr);
	va_end(argptr);
	
	SERVER_PRINT(pszMessage);
}

bool UTIL_isAlive( edict_t *pEntity )
{
	return ( pEntity->v.deadflag == DEAD_NO
		&& pEntity->v.health > 0 );
}

void UTIL_killPlayer( edict_t *pEntity )
{
	pEntity->v.takedamage = DAMAGE_AIM;
	
	edict_t *entity = CREATE_NAMED_ENTITY(ALLOC_STRING("trigger_hurt"));
	
	KeyValueData kvd;
	kvd.szClassName = (char*)STRING(entity->v.classname);
	kvd.szKeyName = "classname";
	kvd.szValue = "trigger_hurt";
	kvd.fHandled = 0;
	MDLL_KeyValue(entity, &kvd);
	
	char dmg_str[32];
	sprintf(dmg_str, "%f", vars_float[DEATH_DMG]);
	KeyValueData kvd1;
	kvd1.szClassName = (char*)STRING(entity->v.classname);
	kvd1.szKeyName = "dmg";
	kvd1.szValue = dmg_str;
	kvd1.fHandled = 0;
	MDLL_KeyValue(entity, &kvd1);
	
	KeyValueData kvd2;
	kvd2.szClassName = (char*)STRING(entity->v.classname);
	kvd2.szKeyName = "damagetype";
	kvd2.szValue = "1";
	kvd2.fHandled = 0;
	MDLL_KeyValue(entity, &kvd2);
	
	KeyValueData kvd3;
	kvd3.szClassName = (char*)STRING(entity->v.classname);
	kvd3.szKeyName = "origin";
	kvd3.szValue = "8192 8192 8192";
	kvd3.fHandled = 0;
	MDLL_KeyValue(entity, &kvd3);
	
	MDLL_Spawn(entity);
	
	entity->v.classname = ALLOC_STRING("trigger_hurt");
	
	MDLL_Touch(entity, pEntity);
	REMOVE_ENTITY(entity);
}

bool UTIL_check_trace_hull_stuck( edict_t *pEntity , Vector origin )
{
	TraceResult ptr;
	TRACE_HULL(origin, origin, false, HULL_HEAD, pEntity, &ptr);
	
	return ( ptr.fStartSolid || ptr.fAllSolid || !ptr.fInOpen );
}

void UTIL_unstuck( edict_t *pEntity )
{
	Vector new_origin = Vector(0.0, 0.0, 0.0);
	
	float dist = 32.0;
	while( dist < 1000.0 )	// 1000 is just incase, should never get anywhere near that
	{
		for ( byte i = 0; i < 128; ++i )
		{
			new_origin.x = RANDOM_FLOAT(pEntity->v.origin.x - dist, pEntity->v.origin.x + dist);
			new_origin.y = RANDOM_FLOAT(pEntity->v.origin.y - dist, pEntity->v.origin.y + dist);
			new_origin.z = RANDOM_FLOAT(pEntity->v.origin.z - dist, pEntity->v.origin.z + dist);
			
			if ( UTIL_check_trace_hull_stuck(pEntity, new_origin) == TRACEHULL_STUCK )
				continue;
			
			SET_ORIGIN(pEntity, new_origin);
			
			return;
		}
		dist += 32.0;
	}
}

void UTIL_remove_Balls( )
{
	edict_t *temp = NULL;
	temp = FIND_ENTITY_BY_STRING(temp, "classname", "deathball");
	while ( FNullEnt(temp) == false )
	{
		map<edict_t*, Ball*>::iterator itBall = ballEntMap.find(temp);
		if ( itBall != ballEntMap.end() )
			delete itBall->second;
		temp = FIND_ENTITY_BY_STRING(temp, "classname", "deathball");
	}
	
	ball_count = 0;
}

void UTIL_count_Balls( )
{
	int cur_balls = 0;
	edict_t *temp = NULL;
	temp = FIND_ENTITY_BY_STRING(temp, "classname", "deathball");
	while ( FNullEnt(temp) == false )
	{
		++cur_balls;
		temp = FIND_ENTITY_BY_STRING(temp, "classname", "deathball");
	}
	
	ball_count = cur_balls;
}


















unsigned short FixedUnsigned16( float value, float scale )
{
	int output;

	output = (int)(value * scale);
	if ( output < 0 )
		output = 0;
	if ( output > 0xFFFF )
		output = 0xFFFF;

	return (unsigned short)output;
}

short FixedSigned16( float value, float scale )
{
	int output;

	output = (int)(value * scale);

	if ( output > 32767 )
		output = 32767;

	if ( output < -32768 )
		output = -32768;

	return (short)output;
}