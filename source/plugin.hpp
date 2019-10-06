
#ifndef _PLUGIN_H_
#define _PLUGIN_H_

// disable "XXX was declared deprecated" warnings
#pragma warning(disable:4996)

//=================================================================
//Plugin Information
//=================================================================
#define PLUGIN_DATE			"March 27, 2008"
#define PLUGIN_NAME			"Deathball"
#define PLUGIN_AUTHOR		"White Panther"
#define PLUGIN_EMAIL		""
#define PLUGIN_URL			""
#define PLUGIN_LOGTAG		"DEATHBALL"
#define PLUGIN_CVAR		"mm_deathball"
#define PLUGIN_VERSION		"0.4"
#define PLUGIN_LOADABLE		PT_CHANGELEVEL
#define PLUGIN_UNLOADABLE	PT_ANYPAUSE


#define PLUGIN_VERS_DWORD	0,4,0,0
#define PLUGIN_COMMENTS		"Deathball"
#define PLUGIN_DESC			"Deathball"
#define PLUGIN_FILENAME		PLUGIN_LOGTAG ".DLL"
#define PLUGIN_INTERNAL		PLUGIN_LOGTAG
#define PLUGIN_COPYRIGHT		"GNU Public License"


#ifndef VERS_PLUGIN_H		//so that we don't confuse the resource compiler
#pragma warning(disable: 4005)
#include <extdll.h>		//always

#include <string.h> //to prevent linux errors while processing meta_api.h
#include <meta_api.h>
#include "utilfunctions.hpp"

#define DEBUG_MODE		0	// set this to 1 to enable debug mode

#define BALL_MODEL		"models/testsphere.mdl"

#define ENT_TYPE		v.iuser3
#define SPLIT_DELAY		v.fuser1
#define BALL_COUNT_TIME		5.0

//=================================================================
//helper functions
//=================================================================
void plugin_init(void);
void plugin_quit(void);

//simplify the process of registering a user message without stepping on anyone else's code unless we have to
inline void SAFE_USER_MSG( int &gmsgvar , const char *szMsgName , const int &iSize )
{
	if( gmsgvar == 0 ) 
	{
		//attempt to get the msg from the game that's running
		gmsgvar = GET_USER_MSG_ID( &Plugin_info, szMsgName, NULL);
		
		//and if that fails, register it ourselves
		if( gmsgvar == 0 )
			gmsgvar = REG_USER_MSG(szMsgName, iSize);
	}
}




#endif // #ifndef VERS_PLUGIN_H
#endif // #ifndef _PLUGIN_H_

