/*************************************************************************
 *
 *    DESCRIPTION :    
 *
 *    FILE 	 : utils.h
 *
 *    AUTHOR     : Anton Ivanov
 *
 *    $Revision: 1.2 $
 *
 *    $Id: utils.h,v 1.2 2004/04/09 06:35:05 anton Exp $
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

/* shared/utils.h
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

/* this file contains definitions for miscellaneous utilities, including
   some numeric processing, error processing, string handling, and random numbers */

#ifndef _UTILS_
#define _UTILS_

#include <math.h>
#include <fstream>
#include "types.h"

using namespace std;

void dump_core(char*);

void my_error(const char*);
void my_error(const char*, int p1);
void my_error(const char*, int p1, int p2);
void my_error(const char*, int p1, int p2, int p3);
void my_error(const char*, int p1, int p2, int p3, int p4);
void my_error(const char*, int p1, int p2, int p3, int p4, int p5);
void my_error(const char*, int p1, int p2, int p3, int p4, int p5, int p6);
void my_error(const char*,float);
void my_error(const char*,float,float);
void my_error(const char*,float,float,float);
void my_error(const char*,float,float,float,float);
void my_error(const char*,float,int);
void my_error(const char*, char* p1);
void my_error(const char*, char* p1, char* p2);
void my_error(const char*, char* p1, char* p2, char* p3);


int closest_int( float x );

typedef float Value;
typedef float AngleRad;
typedef float AngleDeg;

inline AngleDeg Rad2Deg(AngleRad x) { return x * 180 / M_PI; }
inline AngleRad Deg2Rad(AngleDeg x) { return x * M_PI / 180; }

/* needed? */
/* inline float cos(AngleRad x) { return cos((float) x); } */
/* inline float sin(AngleRad x) { return sin((float) x); } */
/* inline float tan(AngleRad x) { return tan((float) x); } */

inline float Cos(AngleDeg x) { return cos(Deg2Rad(x)); }
inline float Sin(AngleDeg x) { return sin(Deg2Rad(x)); }
inline float Tan(AngleDeg x) { return tan(Deg2Rad(x)); }
inline AngleDeg ACos(float x)           { return ((x) >= 1 ? 0 : ((x) <= -1 ? 180 : (Rad2Deg(acos(x))))); }
inline AngleDeg ASin(float x)           { return ((x) >= 1 ? 90 : ((x) <= -1 ? -90 : (Rad2Deg(asin(x))))); }
inline AngleDeg ATan(float x)           { return (Rad2Deg(atan(x))); }
inline AngleDeg ATan2(float x, float y) { return ((x == 0 && y == 0) ? 0 : (Rad2Deg(atan2(x,y)))); } 

void NormalizeAngleDeg(int*);
void NormalizeAngleDeg(AngleDeg*);
void NormalizeAngleRad(AngleRad*);
AngleDeg GetNormalizeAngleDeg(AngleDeg);
float GetDistance(float *x, float *y, float *a, float *b);

#define FLOAT_EPS .001

#define Mod(a,b) (a - (b)*(int)((a)/(b)))
#define Sign(x) ((x) >= 0 ? 1 : -1)
#define Min(x,y) ((x) < (y) ? (x) : (y))
#define Max(x,y) ((x) > (y) ? (x) : (y))
#define MinMax(min, x, max) Min(Max((min),(x)), (max))
inline int Sqr(int x){ return x*x; }
inline float Sqr(float x) { return x*x; }
inline  float Exp(float x, int y) { float a = 1; for (int i=0; i<y; i++) a*=x; return a; }
inline float SumInfGeomSeries(float first_term, float r)
{ return first_term / (1 - r); }
inline float SolveForSumInfGeomSeries(float r, float sum)
{ return sum * (1 - r); }
float SumGeomSeries(float first_term, float r, int n);
/* returns -1 on error */
float SolveForSumGeomSeries(float first_term, float r, float sum);
#define signf(x) ( ((x) > 0.0) ? 1.0 : -1.0) 
float Round(float x, int p=0);
inline int RoundToInt(float x) 
{
  return (int)Round(x,0); 
} 

inline float WeightedSum (float v1, float w1, float v2, float w2)
{
  return (w1*v1 + w2*v2) / (w1+w2);
}


Bool skip_white_space(istream& infile) ;
/* advances to the specifed character, but only on the same line */
Bool advance_to_on_line(istream& infile, char c);
/* path goes in, dir goes out */
/* returns a pathname with a trailing / */
void get_leading_directory(const char* path, char* dir); 

/* puts the number after the colon in the int, returns FALSE on failure */
Bool GetIntFromLine(char* line, int* pval); 
/* puts the number after the colon in the float, returns FALSE on failure */
Bool GetFloatFromLine(char* line, float* pval); 
Bool GetDoubleFromLine(char* line, double* pval); 
/* puts the number after the colon in the int, returns FALSE on failure */
Bool GetIntOffLine(char** pline, int* pval); 
/* puts the number after the colon in the float, returns FALSE on failure */
Bool GetDoubleOffLine(char** pline, double* pval); 


extern double get_double(char **str_ptr);
extern double get_double(char *str);
extern float  get_float (char **str_ptr);
extern float  get_float (char *str);
extern int    get_int   (char **str_ptr);
extern int    get_int   (char *str);
extern void   get_word  (char **str_ptr);
extern void   get_next_word (char **str_ptr);
extern void   get_token  (char **str_ptr);
extern void   advance_to(char c, char **str_ptr);
extern void   advance_past_space(char **str_ptr);

extern int put_int(char *str, int num);
extern int put_float(char *str, float fnum, int precision);

extern void BubbleSort  (int length, int *elements, float *keys);
extern int  BinarySearch(int length, float *elements, float key);
extern void StrReplace  (char *str, char oldchar, char newchar);

extern int   int_random(int n);
extern float range_random(float lo, float hi);
extern int   very_random_int(int n);

extern float weighted_avg(float val1, float val2, float w1, float w2);

extern void GetStampedName( char *name, char *outputName );


#endif
