/*************************************************************************
 *
 *    DESCRIPTION :    
 *
 *    FILE 	 : client.h
 *
 *    AUTHOR     : Anton Ivanov
 *
 *    $Revision: 1.3 $
 *
 *    $Id: client.h,v 1.3 2004/04/19 08:01:38 anton Exp $
 *
 ************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your opfion) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/* -*- Mode: C++ -*- */

/* coach/client.h
 * CMUnited99 (code for on-line/off-line coach)
 * Patrick Riley <pfr+@cs.cmu.edu>
 * Computer Science Department
 * Carnegie Mellon University
 * Copyright (C) 1999 Patrick Riley
 *
 * CMUnited-99 was created by Peter Stone, Patrick Riley, and Manuela Veloso
 *
 * You may copy and distribute this program freely as long as you retain this notice.
 * If you make any changes or have any comments we would appreciate a message.
 */

/* this file contains defintions for the main program
   for the on-line/off-line coach */

#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "Memory.h"

extern Memory *Mem;

#define NOANG 400

Bool      wait_for_signals(sigset_t *);
sigset_t  init_handler();
void      sigio_handler(); 
void      sigalrm_handler();
void AnalyzeLogLoop();
void ConnectToServerLoop();

void send_initialize_message();
int parse_initialize_message(char* mess) ;//return 0 - error; 1 - ok; 2 - must wait
void ChangePlayerType(Unum num,int player_type);
void ChangeMode(Pmode pm);
void CheckBall();
void Ear(Bool OnOff);
void Eye(Bool OnOff);
void Look();
void MoveBall(Vector pos);
inline void MoveBall(float X, float Y)
{ MoveBall(Vector(X,Y)); }
void MovePlayer(TeamSide side, Unum num, Vector pos, float ang = NOANG);
inline void MovePlayer(TeamSide side, Unum num, float X, float Y, float ang = NOANG)
{ MovePlayer(side, num, Vector(X,Y), ang); }
void Say(char* mess);
void FreeformSay(char* mess);
  
void SelectHetroPlayers();
vector<int> OldSelectPlayerTypes(bool strong);
vector<int> NewSelelctPlayerTypes(bool strong);

void CheckOppponentPlayerTypes();
int CorrectForMaxAcc(int num,float player_decay,const set<Unum>& close_types);
set<Unum> GetCloseTypes(float decay);
bool IsDecayClose(float decay);
int FindClosestPlayerDecayType(float decay);
void SendOpponentTypes();

void AnalizeTypesFromCin();

void disconnect();

#endif
