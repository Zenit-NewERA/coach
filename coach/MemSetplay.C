/*************************************************************************
 *
 *    DESCRIPTION :    
 *
 *    FILE 	 : MemSetplay.C
 *
 *    AUTHOR     : Anton Ivanov
 *
 *    $Revision: 1.2 $
 *
 *    $Id: MemSetplay.C,v 1.2 2004/04/09 06:35:06 anton Exp $
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

/* coach/MemSetplay.C
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

/* this file contains implementation for the on-line coaching of set-plays */

//#include <iostream.h>
#include "MemSetplay.h"
#include "client.h"

Bool SetplayInfo::Initialize()
{
  return TRUE;
}
  
void SetplayInfo::SetplaySightHandler(void)
{
  Vector ballPos = Mem->GetBall()->pos;
  Pmode PlayMode = Mem->GetPlayMode();

  if ( PlayMode != PM_Play_On && Mem->GetPrevPlayMode(1) != PlayMode ){
    switch ( PlayMode ){
    case PM_OffSide_Left:
    case PM_OffSide_Right:
    case PM_Free_Kick_Left:
    case PM_Free_Kick_Right:
    case PM_Kick_In_Left:
    case PM_Kick_In_Right:
      if ( MySide == TS_Right ) ballPos = -ballPos; /* flip coords for right team */
      break;
    default:
      break;
    }
  }
}

void SetplayInfo::SetplayPlayerSoundHandler(void)
{
  //cout << "Setplay sound handler: " << CP_team_name << endl;
}
