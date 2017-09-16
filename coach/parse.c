/*************************************************************************
 *
 *    DESCRIPTION :    
 *
 *    FILE 	 : parse.C
 *
 *    AUTHOR     : Anton Ivanov
 *
 *    $Revision: 1.2 $
 *
 *    $Id: parse.C,v 1.2 2004/04/09 06:35:06 anton Exp $
 *
 ************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/* -*- Mode: C++ -*- */
/* shared/parse.C
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

/* shared/parse.C contains implementation for the parsing routines.
   These routines parse messages passed to the coach/trainer, not to clients
   The information is put into a PositionInfo structure */

#include <string>
#include <sstream>
#include "utils.h"
#include "parse.h"

/* parsing routines; defined in parse.C */

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

SenseType Parse (char *SensoryInfo, PositionInfo* MemPos)
{
  SenseType sense_type;
  int     time;

  if (MemPos->MP_save_log&&MemPos->Initialized)
    MemPos->LogFile << SensoryInfo << endl << endl;

  /* NOTE TO SELF: are we handling looks and oks correctly? */
  
  switch ( SensoryInfo[1] ){
  case 'c':
    if(SensoryInfo[2]=='h'){//opponent change type of his player
      SensoryInfo+=19;		// (change_player_type
      sense_type=ST_ChangeTheirPlayerType;
    }else{    
      sense_type=ST_ClangVersion;
    }
    break;			
  case 's':
    if(SensoryInfo[3]=='e'){
      sense_type = ST_See;
      time = get_int(&SensoryInfo);         /* %d    */
    }else{//server param
      sense_type=ST_ServerParam;
    }
    break; 
  case 'h':
    /* this advances SensoryInfo */
    /* this is wrong right now becuase coach messages come as online_coach_<side> */
    time = get_int(&SensoryInfo);         /* %d    */
    if ( SensoryInfo[1] == 'r' ){   /* Referee or Coach message */
      SensoryInfo += 9;             /* " referee " */
      if (strncmp(SensoryInfo, "training", 8) == 0)
	sense_type = ST_HearTrainer;
      else
	sense_type = ST_HearReferee;
    } else if (strncmp(SensoryInfo, "online_coach_", 13) == 0) {
      sense_type = ST_HearCoach;
    } else {
      /* This is a player hear message */
      sense_type = ST_HearPlayer;
    }
    
    break;
  case 'e':
    my_error("Error message: %s", SensoryInfo);
    sense_type = ST_Error;
    break;
  case 'w':
    my_error("Warning message: %s",SensoryInfo);
    sense_type=ST_Warning;
    break;
  case 'o':
    if (SensoryInfo[4] == 'l') {
      /* look message */
      SensoryInfo += 9; /* "(ok look " */
      sense_type = ST_See;
    }else
      if(SensoryInfo[4]=='c'){
	SensoryInfo+=22;//"(ok change_player_type"
	sense_type= ST_ChangeMyPlayerType;
      }			
      else {
	//printf("Got ok message: %s\n", SensoryInfo);    
	sense_type = ST_Ok;
      }
    break;
  case 'p':
    if(SensoryInfo[8]=='p'){
      sense_type=ST_PlayerParam;
    }else{
      sense_type=ST_PlayerType;
    }
    break;
  default: 
    my_error("Sent an illegal message"); 
    return ST_None;
  }


  switch ( sense_type ){
  case ST_See:   
    MemPos->BeginNewCycle(time);
    Parse_Sight(SensoryInfo, MemPos); 
    break;
  case ST_HearPlayer:  
    /* we may want to handle player's sounds sometime */
    break;
  case ST_HearReferee:
    Parse_Referee_Sound(SensoryInfo, MemPos);  
    break;
  case ST_ServerParam:
    MemPos->Parse_Server_Param_message(SensoryInfo);
    break;
  case ST_PlayerParam:
    MemPos->Parse_Player_Param_message(SensoryInfo);
    break;
  case ST_PlayerType:
    MemPos->Parse_Player_Type_message(SensoryInfo);
    break;
  case ST_ClangVersion:
    Parse_Clang_Version(SensoryInfo,MemPos);
    break;
  case ST_ChangeMyPlayerType:
    Parse_Change_My_Player_Type(SensoryInfo,MemPos);
    break;
  case ST_ChangeTheirPlayerType:
    Parse_Change_Their_Player_Type(SensoryInfo,MemPos);
    break;
  case ST_HearCoach:
  case ST_HearTrainer:
    break; 
  case ST_Error:
  case ST_Warning:
  case ST_Ok:
  case ST_None:
    break;
  default:
    my_error("Parse: sense_type is bad value: %d around time %d",
	     sense_type, MemPos->GetTime());
    break;
  }

  return sense_type;
}





/****************************************************************************/

void Parse_Sight(char *SightInfo, PositionInfo* MemPos)
{
  float X, Y;
  float velX, velY;
  float bodyAng, neckAng;
  char  teamname[MAX_TEAMNAME_LEN];
  bool goalie;
	
  TeamSide player_side;
  Unum player_number;

  while (*SightInfo != ')'){

    player_number = Unum_Unknown;
    player_side = TS_NoValue;
    goalie=false;
		
    get_word(&SightInfo);            /* " ((" */

    if ( *SightInfo=='b') {
      /* this is the ball*/
      X = get_float(&SightInfo);
      Y = get_float(&SightInfo);
      velX = get_float(&SightInfo);
      velY = get_float(&SightInfo);
      MemPos->SetBallInfo(Vector(X,Y), Vector(velX,velY));

      advance_to(')', &SightInfo); /* end of ObjInfo */
      SightInfo++;
    } else if (*SightInfo=='p') {
      SightInfo+=3;                          /* "p "" */

      /* copy team name */
      int i = 0;
      while ( *SightInfo != ' ' && *SightInfo != ')' && *SightInfo!='\"') {
	teamname[i++] = *SightInfo;
	SightInfo++; 
      }
      SightInfo++;//close "
      teamname[i]=0;
      if ( *SightInfo!= ' ' )
	my_error("No player number");
      player_number = get_int(&SightInfo);
      //set opponent teammname
      if(MemPos->TheirTeamname.size()==0&&strncmp(teamname,MemPos->CP_team_name,10))
	MemPos->TheirTeamname=teamname;

      if(*SightInfo==' '){//it is goalie
	goalie=true;
	if(!strncmp(teamname,MemPos->CP_team_name,10))
	  MemPos->OurGoalieNum=player_number;
	else
	  MemPos->TheirGoalieNum=player_number;
	SightInfo+=8;  //" goalie)"
      }
      X = get_float(&SightInfo);
      Y = get_float(&SightInfo);
      velX = get_float(&SightInfo);
      velY = get_float(&SightInfo);
      bodyAng = get_float(&SightInfo);
      neckAng = get_float(&SightInfo);

      MemPos->SetPlayerInfo(teamname, player_number, Vector(X, Y),
			    Vector(velX, velY), bodyAng, neckAng,goalie);
      
      advance_to(')', &SightInfo); /* end of ObjInfo */
      SightInfo++;
    } else {
      /* we need to skip this */
      advance_to(')', &SightInfo); /* end of ObjName */
      SightInfo++;
      advance_to(')', &SightInfo); /* end of ObjInfo */
      SightInfo++;
    }
  } /* while SightInfo */
}



/****************************************************************************************/
/****************************************************************************************/
/****************************************************************************************/

void Parse_Referee_Sound(char *msg, PositionInfo* MemPos)
{
  switch( msg[0] ){
  case 'p': MemPos->SetPlayMode(PM_Play_On);           /* play_on */
    MemPos->last_send_freeform=MemPos->GetTime();
    break;
  case 'k': 
    if ( msg[5] == 'i' ){                                  /* kick_in */
      if ( msg[8] == 'l' )
	MemPos->SetPlayMode(PM_Kick_In_Left);
      else if ( msg[8] == 'r' )
	MemPos->SetPlayMode(PM_Kick_In_Right);
      else 
	my_error("kick_in_?");
    }
    else if ( msg[5] == 'o' ){                            /* kick_off */
      if ( msg[9] == 'l' )
	MemPos->SetPlayMode(PM_Kick_Off_Left);
      else if ( msg[9] == 'r' )
	MemPos->SetPlayMode(PM_Kick_Off_Right);
      else 
	my_error("kick_off_?");
    }
    else
      my_error("referee k..?");
    break;
  case 'g': 
    if ( msg[5] == 'k' ){                                 /* goal_kick */
      if ( msg[10] == 'l' )
	MemPos->SetPlayMode(PM_Goal_Kick_Left);
      else if ( msg[10] == 'r' )
	MemPos->SetPlayMode(PM_Goal_Kick_Right);
      else 
	my_error("goal_kick_?");
    }
    else if ( msg[5] == 'e' ){                          /* goalie_catch_ball */
      if ( msg[18] == 'l' )
	MemPos->SetPlayMode(PM_Free_Kick_Left);
      else if ( msg[18] == 'r' )
	MemPos->SetPlayMode(PM_Free_Kick_Right);
      else
	my_error("goalie_catch_ball_?");
    }
    /* NOTE TO SELF: should we keep track of score */
    else if ( msg[5] == 'l' ){                    /* goal */
      MemPos->SetPlayMode(PM_After_Goal_Left);
    }
    else if ( msg[5] == 'r' ){
      MemPos->SetPlayMode(PM_After_Goal_Right);
    }
    else 
      my_error("referee g..?");
    break;
  case 'c':                                               /* corner_kick */
    if ( msg[12] == 'l' )
      MemPos->SetPlayMode(PM_Corner_Kick_Left);
    else if ( msg[12] == 'r' )
      MemPos->SetPlayMode(PM_Corner_Kick_Right);
    else 
      my_error("corner_kick_?");
    break;
  case 'd':                                               /* drop_ball */    
    MemPos->SetPlayMode(PM_Drop_Ball); 
    MemPos->DropBallTime = MemPos->GetTime();
    break;        
  case 'o':                                               /* offside */    
    if ( msg[8] == 'l' )
      MemPos->SetPlayMode(PM_OffSide_Left);
    else if ( msg[8] == 'r' )
      MemPos->SetPlayMode(PM_OffSide_Right);
    else 
      my_error("offside_?");
    break;
  case 'f':
    if ( msg[5] == 'k' ){                                 /* free_kick */
      if ( msg[10] == 'l' )
	MemPos->SetPlayMode(PM_Free_Kick_Left);
      else if ( msg[10] == 'r' )
	MemPos->SetPlayMode(PM_Free_Kick_Right);
      else 
	my_error("free_kick_?");
    }
    else if ( msg[5] == 'l' )                     /* foul */
      ;
    else if ( msg[5] == 'r' )
      ;
    else 
      my_error("referee f..?");
    break;
  case 'h':                                               /* half_time */
    MemPos->SetPlayMode(PM_Before_Kick_Off);  /* play_mode to before_kick_off        */
    break;             
  case 'b': MemPos->SetPlayMode(PM_Before_Kick_Off); break;       /* before_kick_off */
  case 't': 
    if ( msg[5] == 'u' ){                             /* time_up */
      MemPos->SetPlayMode(PM_Time_Over); 
    }
    else if ( msg[5] == 'o' ) {                            /* time_over */
    }
    else if ( msg[5] == 'e' ){                            /* time_extended */
      MemPos->SetPlayMode(PM_Before_Kick_Off);
    }
    else 
      my_error("referee t..?");
    break;
  default: my_error("Referee msg ????");
  }
}
////////////////////////////////////////////////////////////////////////
void Parse_Clang_Version(char* str,PositionInfo* MemPos){
  static bool first=true;
  int tm;
  istringstream ist(str);
  string temp;
  ist>>temp;//"(clang"
  ist>>temp;
  if(temp=="(ver"){
    ist>>temp;
    if(temp=="(p"){
      ist>>temp;
      if(temp.find(MemPos->CP_team_name)){
	ist>>tm;
	ist>>temp;//")" or "goalie)"
	ist>>MemPos->min_clang_ver[tm-1];
	ist>>MemPos->max_clang_ver[tm-1];
      }else{      
	if(first==true){
	  cout<<"Opponent use clang"<<endl;
	  first=false;
	}
      }
    }
  }      
}
/////////////////////////////////////////////////////////////////////////
void Parse_Change_My_Player_Type(char* PTInfo,PositionInfo* MemPos){
  Unum player_number = get_int(&PTInfo);
  int player_type=get_int(&PTInfo);
  MemPos->player_type[player_number]=player_type;
}
//////////////////////////////////////////////////////////////////////
void Parse_Change_Their_Player_Type(char* SensoryInfo,PositionInfo* MemPos)
{
  Unum opp_number=get_int(&SensoryInfo);
  MemPos->OpponentChangeType(opp_number);
}
