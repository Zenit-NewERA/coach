/*************************************************************************
 *
 *    DESCRIPTION :    
 *
 *    FILE 	 : parse.h
 *
 *    AUTHOR     : Anton Ivanov
 *
 *    $Revision: 1.2 $
 *
 *    $Id: parse.h,v 1.2 2004/04/09 06:35:06 anton Exp $
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
/* shared/parse.h
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

/* shared/parse.h contains definitions for the parsing routines.
   These routines parse messages passed to the coach/trainer, not to clients
   The information is put into a PositionInfo structure */

#ifndef _PARSE_H_
#define _PARSE_H_

#include "MemPosition.h"

SenseType Parse (char *SensoryInfo, PositionInfo* MemPos);
void Parse_Clang_Version(char* SensoryInfo,PositionInfo* MemPos);
SenseType Parse(char* SensoryInput, PositionInfo* MemPos);
void Parse_Sight(char *SightInfo, PositionInfo* MemPos);
void Parse_Referee_Sound(char *RefereeSound, PositionInfo* MemPos);
void Parse_Change_My_Player_Type(char* SensoryInfo,PositionInfo* MemPos);
void Parse_Change_Their_Player_Type(char* SensoryInfo,PositionInfo* MemPos);

#endif
