/*************************************************************************
 *
 *    DESCRIPTION :    
 *
 *    FILE 	 : data.h
 *
 *    AUTHOR     : Anton Ivanov
 *
 *    $Revision: 1.2 $
 *
 *    $Id: data.h,v 1.2 2004/04/09 06:35:06 anton Exp $
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
/* shared/data.h
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

/* Classes dealing with statistical stuff from data points */
/* This may not actually be used in the released code, but it's useful for
   collecting statistics */


#ifndef _DATA_H_
#define _DATA_H_

#include <stdio.h>
#include <fstream>
#include "types.h"

using namespace std;

class SingleDataSummary {

public:
  SingleDataSummary();

  void Reset();
  
  void AddPoint(double x);
  double GetMean();
  double GetVariance();
  double GetStdDev();
  int GetNumPoints();

  void CombineWith(SingleDataSummary* sds);

  void SetPrecision(int prec) { precision = prec; }
  
      
  int  GetPrecision() { return precision; }
  
  void WriteInfoToFile(ostream& output_file, int WithNumPoints = FALSE);
  void WriteCompactInfoToFile(ostream& output_file, int WithNumPoints = FALSE);

  /* if num_points is given, it should NOT be in the file */
  Bool ReadInfoFromFile(istream& infile, int num_points = -1);
  Bool ReadCompactInfoFromFile(istream& infile, int num_points = -1);
  
private:
  int n;
  double sum;
  double sum_squares;

  int precision;
};

#endif
