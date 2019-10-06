//function declarations for intercepted functions, any function that we want metamod to call needs
//to be declared in this header

#ifndef _HOOKEDFUNCTIONS_H_
#define _HOOKEDFUNCTIONS_H_

#include <extdll.h>

#include <string.h> //to prevent linux errors while processing meta_api.h
#include <meta_api.h>

void Think( edict_t *pEntity );
void Touch( edict_t *pentTouched , edict_t *pentOther );
void ServerDeactivate_Post( void );
void ServerActivate_Post( edict_t *pEdictList , int edictCount , int clientMax );
void ClientCommand( edict_t *pEntity );
void StartFrame( );

#endif

