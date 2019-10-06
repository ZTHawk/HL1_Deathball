//This file contains utility functions that don't serve any specific purpose but are invaluable nevertheless

#include "plugin.hpp"
#include <sdk_util.h>	//pre-defined function definitions saves us a lot of work

cvar_t *CVAR_ctf_active;

//=================================================================
//the grand old initialization function
//=================================================================
void plugin_init(void)
{
	UTIL_LogPrintf( "[" PLUGIN_NAME "] ================LOADING===============\n");
	UTIL_LogPrintf( "[" PLUGIN_NAME "] By " PLUGIN_AUTHOR " - Version: " PLUGIN_VERSION " - Date: " PLUGIN_DATE " \n");
	
	//Automatically register a cvar that makes the plugin version publically available
	static cvar_t plugin_version_register = { PLUGIN_CVAR, PLUGIN_VERSION, FCVAR_SERVER | FCVAR_SPONLY | FCVAR_PRINTABLEONLY, 0, NULL };
	CVAR_REGISTER(&plugin_version_register);
}


//=================================================================
//the grand old quit function
//=================================================================
void plugin_quit(void)
{
	UTIL_LogPrintf( "[" PLUGIN_NAME "] ===============UNLOADING==============\n");
	UTIL_LogPrintf( "[" PLUGIN_NAME "] By " PLUGIN_AUTHOR " - Version: " PLUGIN_VERSION " - Date: " PLUGIN_DATE " \n");
}




