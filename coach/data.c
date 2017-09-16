/*************************************************************************
 *
 *    DESCRIPTION :    
 *
 *    FILE 	 : data.C
 *
 *    AUTHOR     : Anton Ivanov
 *
 *    $Revision: 1.2 $
 *
 *    $Id: data.C,v 1.2 2004/04/09 06:35:06 anton Exp $
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
/* shared/data.C
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


#include <iomanip>
#include <math.h>
#include "data.h"
#include "utils.h"
using namespace std;

SingleDataSummary::SingleDataSummary()
{
  Reset();
  precision = 2;
}

void SingleDataSummary::Reset() 
{
  n = 0;
  sum = 0.0;
  sum_squares = 0.0;
}

void SingleDataSummary::AddPoint(double x)
{
  n++;
  sum += x;
  sum_squares += x*x;
}

double SingleDataSummary::GetMean()
{
  if (n==0)
    return 0.0;
  else
    return sum/n;
}
				 
double SingleDataSummary::GetVariance()
{
  if (n==1 || n==0)
    return 0;
  else
    return ((double)n*sum_squares - sum*sum) / ((double)n * (n-1));
}


double SingleDataSummary::GetStdDev()
{
  if (Round(GetVariance(), -5) < 0) {
    my_error("How is variance negative: %f", (float)GetVariance());
  }  
  return (sqrt(Round(GetVariance(),-5)));
}

int SingleDataSummary::GetNumPoints()
{
  return n;
}

void SingleDataSummary::CombineWith(SingleDataSummary* sds)
{
  n += sds->n;
  sum += sds->sum;
  sum_squares += sds->sum_squares;
}




void SingleDataSummary::WriteInfoToFile(ostream& output_file,
					int WithNumPoints)
{
  if (WithNumPoints)
    output_file << "NumPoints: " << GetNumPoints() << "\t";
  output_file << setprecision(precision) << "Mean: " << GetMean()
	      << setprecision(precision) << "\tVariance: " << GetVariance()
	      << setprecision(precision) << "\tStdDev: " << GetStdDev()
	      << endl;
}

void SingleDataSummary::WriteCompactInfoToFile(ostream& output_file,
					       int WithNumPoints)
{
  if (WithNumPoints)
    output_file << GetNumPoints() << "\t";
  output_file << setprecision(precision) << GetMean() << "\t"
	      << setprecision(precision) << GetVariance() << "\t"
	      << setprecision(precision) << GetStdDev();
}


Bool SingleDataSummary::ReadInfoFromFile(istream& infile, int num_points)
{
  char line[100], *pc;
  infile.getline(line, 100);
  if (!infile)
    return FALSE;
  
  pc = line;
  
  if (num_points == -1)
    if (!GetIntOffLine(&pc, &num_points))
      return FALSE;

  n = num_points;
  double mean, var, stdev;
  if (!GetDoubleOffLine(&pc, &mean) ||
      !GetDoubleOffLine(&pc, &var) ||
      !GetDoubleOffLine(&pc, &stdev))
    return FALSE;

  sum = mean * n;
  sum_squares = (var * n * (n-1) + sum*sum) / n;

  return TRUE;
}

Bool SingleDataSummary::ReadCompactInfoFromFile(istream& infile, int num_points)
{
  char line[100], *pc;
  infile.getline(line, 100);
  if (!infile)
    return FALSE;

  pc = line;

  if (num_points == -1)
    num_points = get_int(&pc);

  n = num_points;
  double mean, var, stdev;
  mean = get_double(&pc);
  var  = get_double(&pc);
  stdev = get_double(&pc);

  sum = mean * n;
  sum_squares = (var * n * (n-1) + sum*sum) / n;

  /*printf("line: %s, I got: %d %g %g %g, %g %g %g\n", line,
    n, mean, var, stdev, 
    GetMean(), GetVariance(), GetStdDev());*/
  
  return TRUE;
}
