/*************************************************************************
 *
 *    DESCRIPTION :    
 *
 *    FILE 	 : MemPosition.C
 *
 *    AUTHOR     : Anton Ivanov
 *
 *    $Revision: 1.2 $
 *
 *    $Id: MemPosition.C,v 1.2 2004/04/09 06:35:06 anton Exp $
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
/* shared/MemPosition.C
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

/* shared/MemPosition.C contains implementation for the common parts of the
   memory model */

#include "Memory.h"

/***************************** PlayerInfo *******************************/
PlayerInfo::PlayerInfo()
{
  Reset();
}

void PlayerInfo::Reset()
{
  side     = TS_NoValue;
  num      = Unum_Unknown;
  pos      = Vector(0,0);
  vel      = Vector(0,0);
  body_ang = 0.0;
  neck_ang = 0.0;
}


/***************************** BallInfo *******************************/
BallInfo::BallInfo()
{
  Reset();
}

void BallInfo::Reset()
{
  pos = Vector(0,0);
  vel = Vector(0,0);
}

/***************************** CycleInfo *******************************/
int CycleInfo::players_per_side = 0;

CycleInfo::CycleInfo()
{
  pPrev = pNext = NULL;
  players = new PlayerInfo[2*players_per_side];

  ResetData();
}

CycleInfo::~CycleInfo()
{
  delete [] players;
}

void CycleInfo::ResetData()
{
  time = -1;
  for (int i=0; i<2*players_per_side; i++)
    players[i].Reset();
  ball.Reset();
}

PlayerInfo* CycleInfo::GetPlayer(TeamSide side, Unum num)
{
  if (side != TS_Left && side != TS_Right) {
    my_error("Tried to get player with bad side: %d", side);
    return NULL;
  }
  if (num < 1 || num > players_per_side) {
    my_error("Tried to get player with bad nubmer: %d", num);
    return NULL;
  }
  return  &players[(side*players_per_side) + (num-1)]; 
}



/***************************** PositionInfo *******************************/
PositionInfo::PositionInfo()
{
  pLatestCycle = NULL;
  ShutDown = FALSE;
  Initialized=FALSE;
  last_send_freeform=0;
  
  playMode = PM_Before_Kick_Off;
  numCycles = 0;
	
  OurGoalieNum=TheirGoalieNum=Unum_Unknown;

  CycleInfo::players_per_side = SP_team_size;
  player_type=new int[SP_team_size];
  opp_player_type=new int[SP_team_size];
  av_opp_player_decay=new float[SP_team_size];
  num_av_opp_player_decay=new int[SP_team_size];
  max_opp_acc=new float[SP_team_size];
  start_opp_analyze=new Time[SP_team_size];

  min_clang_ver=new int[SP_team_size];
  max_clang_ver=new int[SP_team_size];

  for(int i=0;i<SP_team_size;i++){
    player_type[i]=0;
    opp_player_type[i]=0;
    min_clang_ver[i]=max_clang_ver[i]=0;
  }
  can_send_types=false;

}

Bool PositionInfo::Initialize()
{
  pCycleInfo pHead, pTail, pCI;

  pHead = pTail = NULL;

  for (int i=0; i < MP_cycles_to_store; i++) {
    pCI = new CycleInfo;
    if (pCI == NULL) {
      my_error("Initialize failed?");
      return FALSE;
    }
    pCI->pNext = NULL;
    if (pHead == NULL) {
      pHead = pTail = pCI;
      pCI->pPrev = NULL;
    } else {
      pTail->pNext = pCI;
      pCI->pPrev = pTail;
      pTail = pCI;
    }
  }

  /* now connect the head an tail so that we have a circle */
  pHead->pPrev = pTail;
  pTail->pNext = pHead;

  pLatestCycle = pHead;

  if (MP_save_log) {
    LogFile.open("save.log");
    if (!LogFile)
      my_error("could not open log file");
  }
  
  DropBallTime = 0;
  Initialized=TRUE;
  return TRUE;
}

PositionInfo::~PositionInfo()
{
  /* clean up - be careful because it's a cyclic array */
  pCycleInfo pTemp;
  for (int i=0; i < MP_cycles_to_store; i++) {
    pTemp = pLatestCycle->pNext;
    delete pLatestCycle;
    pLatestCycle = pTemp;
  }
  delete [] player_type;
  LogFile.close();
}


void PositionInfo::BeginNewCycle(Time time)
{
  pLatestCycle = pLatestCycle->pNext;
  pLatestCycle->ResetData();
  pLatestCycle->time = time;
  pLatestCycle->playMode = playMode;
  numCycles++;
}

void PositionInfo::SetBallInfo(Vector pos, Vector vel)
{
  pLatestCycle->ball.pos = pos;
  pLatestCycle->ball.vel = vel;
}

void PositionInfo::SetPlayerInfo(char* team_name, Unum num,
				 Vector pos, Vector vel,
				 float body_ang, float face_ang,bool goalie)
{
  if (strcasecmp(team_name, MP_left_team_name) == 0)
    SetPlayerInfo(TS_Left, num, pos, vel, body_ang, face_ang,goalie);
  else if (strcasecmp(team_name, MP_right_team_name) == 0)
    SetPlayerInfo(TS_Right, num, pos, vel, body_ang, face_ang,goalie);
  else if (MP_left_team_name[0]==0) {
    strcpy(MP_left_team_name, team_name);
    SetPlayerInfo(TS_Left, num, pos, vel, body_ang, face_ang,goalie);
  } else if (MP_right_team_name[0]==0) {
    strcpy(MP_right_team_name, team_name);
    SetPlayerInfo(TS_Right, num, pos, vel, body_ang, face_ang,goalie);
  } else
    my_error("Told to set player for team that does not exist!");
}

void PositionInfo::SetPlayerInfo(TeamSide side, Unum num,
				 Vector pos, Vector vel,
				 float body_ang, float neck_ang,bool goalie)
{
  PlayerInfo* pPI = pLatestCycle->GetPlayer(side, num);

  pPI->side = side;
  pPI->num = num;
  pPI->pos = pos;
  pPI->vel = vel;
  pPI->body_ang = body_ang;
  pPI->neck_ang = neck_ang;
  pPI->goalie=goalie;
}

Time PositionInfo::GetTime()
{
  if (pLatestCycle == NULL)
    return (Time)0; /* we haven't gotten any info yet */
  else
    return pLatestCycle->time;
}

Pmode PositionInfo::GetPlayMode()
{
  if (pLatestCycle == NULL)
    /* not right if we connect mid game,
       but it	shoudl be right most of the time */
    return PM_Before_Kick_Off; 
  else
    return pLatestCycle->playMode;
}
BallInfo* PositionInfo::GetBall()
{
  if (pLatestCycle == NULL)
    return NULL;
  else
    return &pLatestCycle->ball;
}


PlayerInfo* PositionInfo::GetPlayer(int num)
{
  if (num < 0 || num >= SP_team_size*2) {
    my_error("Trying to get a player out of range: %d", num);
    return NULL;
  }

  if (pLatestCycle == NULL)
    return NULL;
  
  pPlayerInfo pPI = &(pLatestCycle->players[num]);
  if (pPI->side == TS_NoValue)
    return NULL; /* not a valid player */
  else
    return pPI;
}

PlayerInfo* PositionInfo::GetPlayer(TeamSide side, Unum num)
{
  if (pLatestCycle == NULL)
    return NULL;
  
  pPlayerInfo pPI = pLatestCycle->GetPlayer(side, num);
  if (pPI->side == TS_NoValue)
    return NULL; /* not a valid player */
  else
    return pPI;
}

pCycleInfo PositionInfo::GetBackInfo(int cycles_back)
{
  if (cycles_back >= numCycles)
    return NULL; /* don't have that many cycles */
  
  if (cycles_back >= MP_cycles_to_store) {
    my_error("Tried to get cycles to far in the past: %d >= %d",
	     cycles_back, MP_cycles_to_store);
    return NULL;
  }

  if (pLatestCycle == NULL)
    return NULL;
  
  pCycleInfo pCI = pLatestCycle;
  for (int i=0; i < cycles_back; i++)
    pCI = pCI->pPrev;

  return pCI;
}


Pmode PositionInfo::GetPrevPlayMode(int cycles_back)
{
  pCycleInfo pCI = GetBackInfo(cycles_back);
  if (pCI == NULL)
    return PM_No_Mode;
  else
    return pCI->playMode;

}


BallInfo*   PositionInfo::GetPrevBall(int cycles_back)
{
  pCycleInfo pCI = GetBackInfo(cycles_back);
  if (pCI == NULL)
    return NULL;
  else
    return &pCI->ball;
}

PlayerInfo* PositionInfo::GetPrevPlayer(int cycles_back, TeamSide side, Unum num)
{
  pCycleInfo pCI = GetBackInfo(cycles_back);
  if (pCI == NULL)
    return NULL;
  else {
    pPlayerInfo pPI = pCI->GetPlayer(side, num);
    if (pPI->side == TS_NoValue)
      return NULL; /* not a valid player */
    else
      return pPI;
  }
}

PlayerInfo* PositionInfo::GetPrevPlayer(int cycles_back, int num)
{
  if (num < 0 || num >= SP_team_size*2) {
    my_error("Trying to get a player out of range: %d", num);
    return NULL;
  }

  pCycleInfo pCI = GetBackInfo(cycles_back);
  if (pCI == NULL)
    return NULL;
  else {
    pPlayerInfo pPI = &pCI->players[num];
    if (pPI->side == TS_NoValue)
      return NULL; /* not a valid player */
    else
      return pPI;
  }
}


/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/



/*********************************************************************************/

int PositionInfo::SortPlayersByDistanceToPoint(TeamSide side,
					       Vector point, Unum* players)
{
  int result = 0; /*Number of players sorted */

  /* Make aux array big enough */
  int num = (( side == TS_Both) ? SP_team_size*2 : SP_team_size); 
			
  float *vals;
  vals = new float[num];
   
  TeamSide team;
  if (side == TS_Left || TS_Both) {
    team = TS_Left;
    for (int i=1; i<=SP_team_size; i++){
      if ( GetPlayer(team, i) ){
	players[result]=i;
	vals[result]   =GetPlayer(team, i)->pos.dist(point);  
	result++;
      }
    }
  }
  if (side == TS_Right || TS_Both) {
    team = TS_Right;
    for (int i=1; i<=SP_team_size; i++){
      if ( GetPlayer(team, i) ){
	players[result]=i;
	vals[result]   =GetPlayer(team, i)->pos.dist(point);  
	result++;
      }
    }
  }
    
  /* Now should have all values in question in vals, with uniform number in 
     corresponding position of players ( +TEAM_SIZE for their team if 
     side == 'b'):  Just sort em */

  BubbleSort(result,players,vals);
  delete vals;
  return result;

}
//////////////////////////////////////////////////////////////////////
Unum PositionInfo::ClosestPlayerToPlayer(TeamSide side,Unum num,TeamSide cl_side)
{
  if(GetPlayer(side,num)==0)
    return Unum_Unknown;
  Vector pos=GetPlayer(side,num)->pos;
  float min_dist=100.0f;
  Unum cl_player=Unum_Unknown;
  float dist;
  
  for(int i=1;i<=SP_team_size;i++){
    if(GetPlayer(cl_side,i)==0)
      continue;
    if(side==cl_side&&i==num)
      continue;
    if((dist=(pos-GetPlayer(cl_side,i)->pos).mod())<min_dist){
      min_dist=dist;
      cl_player=i;
    }
  }
  return cl_player;
}
//////////////////////////////////////////////////////////////////////
void PositionInfo::OpponentChangeType(Unum opp_number)
{
  if(MP_save_log)
    LogFile<<"Type of opponent "<<opp_number<<" was changed"<<endl;
  av_opp_player_decay[opp_number-1]=0.0f;
  num_av_opp_player_decay[opp_number-1]=0;
  opp_player_type[opp_number-1]=-1;
  opp_with_unknown_pt.insert(opp_number);
  max_opp_acc[opp_number-1]=0.0f;
  start_opp_analyze[opp_number-1]=GetTime();
}
//////////////////////////////////////////////////////////////////////
bool PositionInfo::CanFreeformSend()
{
  if(GetPlayMode()!=PM_Play_On)
    return true;
  //в принципе должно работать, но почемуто во время игры не посылается!
  /*
  int i=1;
  while(1){
    if(GetTime()<last_send_freeform+SP_freeform_wait_period*i)
      return false;
    if(GetTime()>last_send_freeform+SP_freeform_wait_period*i&&
       GetTime()<last_send_freeform+SP_freeform_wait_period*i+SP_freeform_send_period)
      return true;
    i++;
  }
  my_error("Error in CanFreeformSend");
  */
  return false;
}
//////////////////////////////////////////////////////////////////////
