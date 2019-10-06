#ifndef _PLAYER_DATA_H
#define _PLAYER_DATA_H

#include "plugin.h"
#include <vector>
#include "flag_data.h"

using namespace std;

class FLAG_PLAYER
{
	public:
	int ID;
	edict_t *pEntity;
	
	int old_team;
	
	FLAG_ENT *myFlag;
	FLAG_ENT *enemyFlag;
	
	bool ingame;
	char name[33];
	
	void init( edict_t *pEntity );
	void reset( int in_game = 0 );
	
	void think( );
	
	void drop( );
};

extern FLAG_PLAYER player_data[33];

#endif
