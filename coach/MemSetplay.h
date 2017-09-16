/*************************************************************************
 *
 *    DESCRIPTION :    
 *
 *    FILE 	 : MemSetplay.h
 *
 *    AUTHOR     : Anton Ivanov
 *
 *    $Revision: 1.2 $
 *
 *    $Id: MemSetplay.h,v 1.2 2004/04/09 06:35:06 anton Exp $
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

/* coach/MemSetplay.h
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

/* this file contains definitions for the on-line coaching of set-plays */

#ifndef _SETPLAY_H_
#define _SETPLAY_H_

//#include <ostream>
#include "Formations.h"
using namespace std;

class SetplayInfo : public Formations
{
public:

  Bool Initialize();
  
  void SetplaySightHandler(void);
  void SetplayPlayerSoundHandler(void);
private:

};


#endif
