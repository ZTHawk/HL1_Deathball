//======================================================================
// Forward declarations for functions that deal directly with the engine
//======================================================================

#ifndef _UTILFUNCTIONS_H_
#define _UTILFUNCTIONS_H_

#define ABSOLUTE_VALUE_EASY(x) (((x) < 0) ? (-(x)) : (x)) //very useful for gpGlobals->time comparisons

#define HULL_HEAD			1
#define TRACEHULL_STUCK			true
#define MOVETYPE_BOUNCEMISSILE          11          // Bounce w/o gravity

bool UTIL_isAlive( edict_t *pEntity );

void UTIL_LogPrintf( char *fmt , ... );
void UTIL_ServerPrint( const char *fmt , ... );

void UTIL_remove_Balls( );
void UTIL_count_Balls( );

void UTIL_unstuck( edict_t *pEntity );
bool UTIL_check_trace_hull_stuck( edict_t *pEntity , Vector origin );
void UTIL_killPlayer( edict_t *pEntity );

#endif

