/*************************************************************************
 *
 *    DESCRIPTION :    
 *
 *    FILE 	 : logfile.h
 *
 *    AUTHOR     : Anton Ivanov
 *
 *    $Revision: 1.2 $
 *
 *    $Id: logfile.h,v 1.2 2004/04/09 06:35:06 anton Exp $
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

/* coach/logfile.h
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

/* This file contains definitions for the processing of a Soccer Server logfile
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

#ifndef LOGFILES_H_
#define LOGFILES_H_

#include <stdio.h>
#include "MemPosition.h"
#include "types.h"

enum RecVersion {
  REC_NONE = 0,
  REC_OLD_VERSION = 1,
  REC_VERSION_2 = 2
};

FILE* OpenLogFileForRead(char* in_file, RecVersion* pVer);
Bool ReadLog(FILE* infp, dispinfo_t* pBuf, RecVersion ver);
int IncorporateDispInfo(dispinfo_t* pBuf, PositionInfo* pMemPos);


#endif
