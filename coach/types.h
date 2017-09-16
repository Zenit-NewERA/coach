/*************************************************************************
 *
 *    DESCRIPTION :    
 *
 *    FILE 	 : types.h
 *
 *    AUTHOR     : Anton Ivanov
 *
 *    $Revision: 1.2 $
 *
 *    $Id: types.h,v 1.2 2004/04/09 06:35:06 anton Exp $
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

/* shared/types.h
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

/* shared/types.h contains definitions for common types */

#ifndef _TYPES_H_
#define _TYPES_H_
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <ctype.h>
#include <signal.h>
#include <sys/time.h>
#include <vector>
#include <fstream>
#include <string>
#include <list>
#include <algorithm>
#include <functional>

enum Bool{
  FALSE = 0,
  TRUE = 1
};

#define num_TeamSide 5
enum TeamSide {
  TS_Left = 0,
  TS_Right = 1,
  TS_Both = 2,
  TS_Neither = 3,
  TS_NoValue = 4
};

#define TEAMSIDE_STRINGS \
   {"Left", "Right", "Both", "Neither", "NoValue" }

typedef int Unum;
#define Unum_Unknown 0

typedef int Time;

#define NULLCHAR '\000'

#define MAXNAMELEN 30

#define MAX_PLAYERS 30

enum SenseType{
  ST_None,
  ST_See,
  ST_HearPlayer,
  ST_HearReferee,
  ST_HearTrainer,
  ST_HearCoach,
  ST_Error,
  ST_Warning,
  ST_ServerParam,
  ST_PlayerParam,
  ST_PlayerType,
  ST_ClangVersion,
  ST_ChangeMyPlayerType,
  ST_ChangeTheirPlayerType,
  ST_Ok
};

typedef enum _PlayMode {
  PM_No_Mode,
  PM_Before_Kick_Off,
  PM_Time_Over,
  PM_Play_On,
  PM_Kick_Off_Left,
  PM_Kick_Off_Right,
  PM_Kick_In_Left,
  PM_Kick_In_Right,
  PM_Free_Kick_Left,
  PM_Free_Kick_Right,
  PM_Corner_Kick_Left,
  PM_Corner_Kick_Right,
  PM_Goal_Kick_Left,
  PM_Goal_Kick_Right,
  PM_After_Goal_Left,
  PM_After_Goal_Right,
  PM_Drop_Ball,
  PM_OffSide_Left,
  PM_OffSide_Right,
  PM_MAX 
} Pmode ;
/*enum Pmode{
  PM_No_Mode,
  PM_Before_Kick_Off,
  PM_Left_Kick_Off,
  PM_Right_Kick_Off,
  PM_Left_Kick_In,
  PM_Right_Kick_In,
  PM_Left_Corner_Kick,
  PM_Right_Corner_Kick,
  PM_Left_Goal_Kick,
  PM_Right_Goal_Kick,
  PM_Left_Free_Kick,
  PM_Right_Free_Kick,
  PM_Drop_Ball,
  PM_Left_Offside_Kick,
  PM_Right_Offside_Kick,
  PM_Play_On,
  PM_Half_Time,
  PM_Time_Up,
  PM_Extended_Time
  };
*/

#define PLAYMODE_STRINGS {"",\
   "before_kick_off",\
   "time_over",\
   "play_on",\
   "kick_off_l",\
   "kick_off_r",\
   "kick_in_l",\
   "kick_in_r",\
   "free_kick_l",\
   "free_kick_r",\
   "corner_kick_l",\
   "corner_kick_r",\
   "goal_kick_l",\
   "goal_kick_r",\
   "goal_l",\
   "goal_r",\
   "drop_ball",\
   "offside_l",\
   "offside_r",\
   ""\
}

typedef enum TURNDIR 
  { TURN_NONE = 0,
    TURN_CW = -1,
    TURN_CCW = 1,
    TURN_CLOSEST = 10,
    TURN_AVOID = 11 /* avoid any opponents */
  } TurnDir;

#define MAX_OPTION_LENGTH 50
enum TrainerValueType {
  TVT_OnOff,
  TVT_Int,
  TVT_Float
}; /* Bool and string types are not sent by trainer */

enum Ptype{
  PT_None=0,
  PT_Goaltender=1<<0,
  PT_Sweeper=1<<1,
  PT_Defender=1<<2,
  PT_Midfielder=1<<3,
  PT_Forward=1<<4,
  PT_All=PT_Goaltender|PT_Sweeper|PT_Defender|PT_Midfielder|PT_Forward
};

enum Pside{
  PS_None=0,
  PS_Left=1<<0,
  PS_Center=1<<1,
  PS_Right=1<<2,
  PS_All=PS_Left|PS_Center|PS_Right
};

/* these are taken from the server params.h file */
#define MAX_PLAYER	11
#define	NO_INFO		0
#define	SHOW_MODE	1
#define	MSG_MODE	2
#define DRAW_MODE	3
#define	BLANK_MODE	4

#define	MSG_BOARD	1
#define	LOG_BOARD	2

#define SHOWINFO_SCALE		16.0

/* these are taken from the server types.h file */
#define	COLOR_NAME_MAX	64

#define LEFT	1
#define NEUTRAL	0
#define RIGHT 	-1

typedef struct {
  short	enable ;
  short	side ;
  short	unum ;
  short	angle ;
  short	x ;
  short	y ;
} pos_t ;

typedef struct {
  char	name[16] ;
  short	score ;
} team_t ;

typedef struct {
  char	pmode ;
  team_t	team[2] ;
  pos_t	pos[MAX_PLAYER * 2 + 1] ;
  short	time ;
} showinfo_t ;

typedef	struct {
  short	board ;
  char	message[2048] ;
} msginfo_t ;

typedef struct {
  short	x ;
  short	y ;
  char	color[COLOR_NAME_MAX] ;
} pointinfo_t ;

typedef struct {
  short	x ;
  short	y ;
  short	r ;
  char	color[COLOR_NAME_MAX] ;
} circleinfo_t ;
	
typedef struct {
  short	x1 ;
  short	y1 ;
  short	x2 ;
  short	y2 ;
  char	color[COLOR_NAME_MAX] ;
} lineinfo_t ;

typedef struct {
  short mode ;
  union {
    pointinfo_t		pinfo ;
    circleinfo_t	cinfo ;
    lineinfo_t		linfo ;
  } object ;
} drawinfo_t ;

typedef struct {
  short	mode ;
  union {
    showinfo_t	show ;
    msginfo_t	msg ;
    drawinfo_t	draw ;
  } body ;
} dispinfo_t ;


#endif
