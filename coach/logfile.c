/*************************************************************************
 *
 *    DESCRIPTION :    
 *
 *    FILE 	 : logfile.C
 *
 *    AUTHOR     : Anton Ivanov
 *
 *    $Revision: 1.2 $
 *
 *    $Id: logfile.C,v 1.2 2004/04/09 06:35:06 anton Exp $
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

/* coach/logfile.C
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

/* This file contains the implementation for the processing of a Soccer Server logfile
   Note that this code is partially taken from the logplayer application of the
   Soccer Server System. Here is a copy of the liciense for Soccer Server

   License of Soccer Server 

   Conditions for using Soccer Server: 

   1.This version of Soccer Server is still under developing and may have serious problem. Neither Electrotechnical Laboratory (ETL) nor development staffs of
   Soccer Server provides warranty. Use the software at your own risk. 
   2.Copyright of all program code and documentation included in source or binary package of Soccer Server belongs to ETL. 
   3.You can use Soccer Server for research and/or education purpose only, commercial use is not allowed. 
   4.If you want to re-distribute Soccer Server to someone, you should inform us of destination of re-distribution to the address noda@etl.go.jp. via e-mail. 

*/

#ifdef SunOS4
#include <sys/types.h>
#endif
#include <netinet/in.h>

#include "types.h"
#include "utils.h"
#include "logfile.h"
#include "MemPosition.h"
#include "parse.h"

#ifdef SunOS4
#define SEEK_SET 0
#endif

#ifdef DEBUG_OUTPUT
#define DebugLog(x) 
#else
#define DebugLog(x) 
#endif


FILE* OpenLogFileForRead(char* in_file, RecVersion* pVer)
{
  char localbuf[6];
  FILE* infp;

  // check if an input file is specified
  if (in_file[0] == '\0') {
    my_error("No logfile specified");
    return NULL;
  }

  // open the file
  if ((infp = fopen(in_file, "rb")) == NULL) {
    my_error("Can't open log file %s", in_file);
    return NULL;
  }

  // default is old version
  *pVer = REC_OLD_VERSION;
  // read version information
  if (fread(localbuf, 4, 1, infp) == 0)
    {
      // could not read from file
      fclose(infp);
      my_error("Can't open log file %s", in_file);
      return NULL;
    }

  // check version
  if ((localbuf[0] == 'U') && (localbuf[1] == 'L') && (localbuf[2] == 'G'))
    *pVer = (RecVersion)localbuf[3];
  else
    // no version information, so it is an old logfile
    // (or no logfile at all)
    fseek(infp, 0, SEEK_SET);

  return infp;
} 

Bool ReadLog(FILE* infp, dispinfo_t* pBuf, RecVersion ver)
{
  short		len;
  long		pos;

  pos  = ftell(infp);

  switch (ver) {
  case REC_OLD_VERSION:
    // it is a logfile of the previous version
    if (fread((char *) (pBuf), sizeof(dispinfo_t), 1, infp) == 0)
      return FALSE;
    DebugLog(cout << "read a ver1 record" << endl);
    return TRUE;

  case REC_VERSION_2:
    // new version 
    // read the type of next msg
    if (fread((char *)(&pBuf->mode), sizeof(pBuf->mode), 1, infp) == 0)
      return FALSE;

    switch (ntohs(pBuf->mode)) {
    case SHOW_MODE:
      // read a show struct
      if (fread((char *)(&pBuf->body.show),
		sizeof (pBuf->body.show), 1, infp) == 0)
	return FALSE;
      DebugLog(cout << "read a ver2 show_mode" << endl);
      return TRUE;

    case MSG_MODE:
      // read the board info
      if (fread((char *)(&pBuf->body.msg.board),
		sizeof(pBuf->body.msg.board), 1, infp) == 0)
	return FALSE;

      // read the string length
      if (fread((char *)(&len), sizeof(len), 1, infp) == 0)
	return FALSE;

      if (len > 2048) {
	my_error("logfile message length too large %d", len);
	return FALSE;
      }
      
      // read the message
      if (fread(pBuf->body.msg.message, len, 1, infp) == 0)
	return FALSE;
      DebugLog(cout << "read a ver2 msg_mode" << endl);
      return TRUE;
    default:
      my_error("Bad mode in logfile: %d", ntohs(pBuf->mode));
      return FALSE;
    } /* switch on mode */
  case REC_NONE:
  default:
    my_error("Bad version for logfile: %d", ntohs(pBuf->mode));
    return FALSE;
  } /* switch on version */

  my_error("How did I get here?");
  return FALSE;
}


/* returns a pointer to a static pos_t */
pos_t* ConvertPosTByteOrder(pos_t& in)
{
  static pos_t out;
  out.enable = ntohs(in.enable);
  out.side = ntohs(in.side);
  out.unum = ntohs(in.unum);
  out.angle = ntohs(in.angle);
  out.x = ntohs(in.x);
  out.y = ntohs(in.y);

  return &out;
}

/* returns the mode of the read information */
int IncorporateDispInfo(dispinfo_t* pBuf, PositionInfo* pMemPos)
{
  switch (ntohs(pBuf->mode)) {
  case SHOW_MODE: 
    {
      pMemPos->SetPlayMode((Pmode)ntohs(pBuf->body.show.pmode));
      pMemPos->BeginNewCycle((Time)ntohs(pBuf->body.show.time));

      /* fill in team names if needed */
      if (pMemPos->MP_left_team_name[0] == 0) 
	strcpy(pMemPos->MP_left_team_name,
	       pBuf->body.show.team[0].name);
      if (pMemPos->MP_right_team_name[0] == 0) 
	strcpy(pMemPos->MP_right_team_name,
	       pBuf->body.show.team[1].name);

      pos_t* pPos;
      /* set the ball position */
      BallInfo* pPastBI = pMemPos->GetPrevBall(1); /* 1 cycle back */
      pPos = ConvertPosTByteOrder(pBuf->body.show.pos[0]);
      if (pPastBI == NULL) {
	/* there is no previous cycle!, so vel is 0 */
	pMemPos->SetBallInfo(Vector(pPos->x/SHOWINFO_SCALE,
				    pPos->y/SHOWINFO_SCALE),
			     Vector(0,0));
      } else {
	Vector pos(pPos->x/SHOWINFO_SCALE,
		   pPos->y/SHOWINFO_SCALE);
	pMemPos->SetBallInfo(pos, (pos - pPastBI->pos)*pMemPos->SP_ball_decay);
      }
    
      /* set the player positions */
      for (int i=1; i<MAX_PLAYER*2+1; i++) {
	pPos = ConvertPosTByteOrder(pBuf->body.show.pos[i]);
	if (pPos->side == NEUTRAL)
	  continue; /* player doesn't exist */

	TeamSide side = (pPos->side == LEFT)?TS_Left:TS_Right;

	PlayerInfo* pPastPI = pMemPos->GetPrevPlayer(1, side, pPos->unum);

	/* face angle is not recorded in the log files, so just set it to 0 */
	if (pPastPI == NULL) {
	  /* there is no previous cycle!, so vel is 0 */
	  pMemPos->SetPlayerInfo(side, pPos->unum,
				 Vector(pPos->x/SHOWINFO_SCALE,
					pPos->y/SHOWINFO_SCALE),
				 Vector(0,0),
				 pPos->angle, 0.0,false);//AI:for goalie must change if we want it to use   
	} else {
	  Vector pos(pPos->x/SHOWINFO_SCALE, pPos->y/SHOWINFO_SCALE);
	  pMemPos->SetPlayerInfo(side, pPos->unum,
				 pos,
				 (pos - pPastBI->pos)*pMemPos->SP_player_decay,
				 pPos->angle, 0.0,false);//AI:for goalie must change if we want it to use  
	}
      } /* player loop */
      
    }
    
    break;
  case MSG_MODE:
    /* eventually we should handle sounds */
    /*if (ntohs(pBuf->body.msg.board) == MSG_BOARD) */
    break;
  default:
    my_error("Bad mode in pBuf: %d", ntohs(pBuf->mode));
    return NO_INFO;
  }

  return ntohs(pBuf->mode);
}
