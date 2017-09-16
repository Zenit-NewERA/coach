/*************************************************************************
 *
 *    DESCRIPTION :    
 *
 *    FILE 	 : MemPosition.h
 *
 *    AUTHOR     : Anton Ivanov
 *
 *    $Revision: 1.2 $
 *
 *    $Id: MemPosition.h,v 1.2 2004/04/09 06:35:06 anton Exp $
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
/* shared/MemPosition.h
 * CMUnited99 (shared code for on-line coach, off-line coach, and off-line trainer)
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

/* shared/MemPosition.h contains the specification for the common parts of the
   memory model */

/* The Position Info class maintains information on the current state of all the
   players and the ball. It stores MP_cycles_to_store cycle history. For example, if
   MP_cycles_to_store is 2, then PositionInfo contains information on the current cycle
   and the previous cycle */

#ifndef _MEMPOSITION_H_
#define _MEMPOSITION_H_


#include "MemOption.h"
#include "geometry.h"
#include <set>

class PlayerInfo 
{
public:
  PlayerInfo();
  void Reset();
  
  TeamSide side;
  Unum     num;
  Vector   pos;
  Vector   vel;
  float    body_ang;
  float    neck_ang; /* relative to body angle */
  bool 		 goalie;
};
typedef PlayerInfo* pPlayerInfo;


struct BallInfo
{
public:
  BallInfo();
  void Reset();
  
  Vector pos;
  Vector vel;
};
typedef BallInfo* pBallInfo;


struct CycleInfo 
{
public:
  CycleInfo();
  ~CycleInfo();
  void ResetData();

  PlayerInfo* GetPlayer(TeamSide side, Unum num);

  static int players_per_side;
  
  CycleInfo* pPrev;
  CycleInfo* pNext;

  Time        time;
  Pmode       playMode;
  PlayerInfo* players;
  BallInfo    ball;
};
typedef CycleInfo* pCycleInfo;


/* the Memory will have a circular linked list of CycleInfo to automatically
   store some previous cycles */

class PositionInfo : public OptionInfo {
private:
  pCycleInfo pLatestCycle;
  Pmode      playMode;

  int numCycles;

  
  pCycleInfo GetBackInfo(int cycles_back);
  
public:
  PositionInfo();
  ~PositionInfo();
  Bool Initialize();

  Bool ShutDown;
  Bool Initialized;
  TeamSide MySide;
  TeamSide TheirSide;
  ofstream   LogFile;
  Unum OurGoalieNum;
  Unum TheirGoalieNum;
  string TheirTeamname;//our teamname in CP_team_name
  
  int* player_type;
  int* opp_player_type;
  set<Unum> opp_with_unknown_pt;
  float* av_opp_player_decay;
  int*   num_av_opp_player_decay;
  float* max_opp_acc;
  Time* start_opp_analyze;
  bool can_send_types;
  
  Time last_send_freeform;
  bool CanFreeformSend();

  int* min_clang_ver;
  int* max_clang_ver;

  void OpponentChangeType(Unum opp_number);
  
  void SetPlayMode(Pmode pm)
  { playMode = pm; }
  void BeginNewCycle(Time time);
  void SetBallInfo(Vector pos, Vector vel);
  void SetPlayerInfo(char* team_name, Unum num,
		     Vector pos, Vector vel,
		     float body_ang, float face_ang,bool goalie);
  void SetPlayerInfo(TeamSide side, Unum num,
		     Vector pos, Vector vel,
		     float body_ang, float neck_ang,bool goalie);

  
  Time        GetTime();
  Time        DropBallTime;
  /* so this is a little strange. The playMode in PositionInfo is used to
     keep track of the referee messages, but then when we get a sight, it's
     recorded onto the CycleInfo. All access functions go to the Cycle
     Info */  
  Pmode       GetPlayMode();
  BallInfo*   GetBall();
  PlayerInfo* GetPlayer(int num);
  PlayerInfo* GetPlayer(TeamSide side, Unum num);

  Pmode       GetPrevPlayMode(int cycles_back);
  BallInfo*   GetPrevBall(int cycles_back);
  PlayerInfo* GetPrevPlayer(int cycles_back, TeamSide side, Unum num);
  PlayerInfo* GetPrevPlayer(int cycles_back, int num);

  /* these are somewhat higher level functions */
  int SortPlayersByDistanceToPoint(TeamSide side, Vector point, Unum* players);
  Unum ClosestPlayerToPlayer(TeamSide side,Unum num,TeamSide cl_side);


};


#endif
