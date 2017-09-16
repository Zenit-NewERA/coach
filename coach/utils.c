/*************************************************************************
 *
 *    DESCRIPTION :    
 *
 *    FILE 	 : utils.C
 *
 *    AUTHOR     : Anton Ivanov
 *
 *    $Revision: 1.2 $
 *
 *    $Id: utils.C,v 1.2 2004/04/09 06:35:06 anton Exp $
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

/* shared/utils.C
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

/* this file contains implementation for miscellaneous utilities, including
   some numeric processing, error processing, string handling, and random numbers */

#include <fstream>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include "utils.h"
#include "MemOption.h"
#include <sstream>

using namespace std;

void dump_core(char *msg){
  fprintf(stderr,"dumping core");
  msg[1000000]=0;
  my_error("Core didn't dump");
}

void my_error(const char *msg){
  fprintf(stderr,"%s ERROR: %s\n", PROGRAM_NAME, msg);
  //msg[1000000]=0; /* to segfault */
  /*  int tmp;
      scanf("%d",&tmp); */
}

void my_error(const char *msg, int p1)
{
  char str[100];
  sprintf(str, msg, p1);
  fprintf(stderr,"%s ERROR: %s\n", PROGRAM_NAME, str);
}

void my_error(const char *msg, int p1, int p2)
{
  char str[100];
  sprintf(str, msg, p1, p2);
  fprintf(stderr,"%s ERROR: %s\n", PROGRAM_NAME, str);
}

void my_error(const char *msg, int p1, int p2, int p3)
{
  char str[100];
  sprintf(str, msg, p1, p2, p3);
  fprintf(stderr,"%s ERROR: %s\n", PROGRAM_NAME, str);
}

void my_error(const char *msg, int p1, int p2, int p3, int p4)
{
  char str[100];
  sprintf(str, msg, p1, p2, p3, p4);
  fprintf(stderr,"%s ERROR: %s\n", PROGRAM_NAME, str);
}

void my_error(const char *msg, int p1, int p2, int p3, int p4, int p5)
{
  char str[100];
  sprintf(str, msg, p1, p2, p3, p4, p5);
  fprintf(stderr,"%s ERROR: %s\n", PROGRAM_NAME, str);
}

void my_error(const char *msg, int p1, int p2, int p3, int p4, int p5, int p6)
{
  char str[100];
  sprintf(str, msg, p1, p2, p3, p4, p5, p6);
  fprintf(stderr,"%s ERROR: %s\n", PROGRAM_NAME, str);
}

void my_error(const char *msg, char* p1)
{
  char str[100];
  sprintf(str, msg, p1);
  fprintf(stderr,"%s ERROR: %s\n", PROGRAM_NAME, str);
}

void my_error(const char* msg, char* p1, char* p2)
{
  char str[100];
  sprintf(str, msg, p1, p2);
  fprintf(stderr,"%s ERROR: %s\n", PROGRAM_NAME, str);
}

void my_error(const char* msg, char* p1, char* p2, char* p3)
{
  char str[100];
  sprintf(str, msg, p1, p2, p3);
  fprintf(stderr,"%s ERROR: %s\n", PROGRAM_NAME, str);
}
void my_error(const char* msg, float param) {
  char outstring[100];
  sprintf(outstring,msg,param);
  my_error(outstring);
}

void my_error(const char* msg, float param1, float param2) {
  char outstring[100];
  sprintf(outstring,msg,param1,param2);
  my_error(outstring);
}

void my_error(const char* msg, float param1, float param2, float param3) {
  char outstring[100];
  sprintf(outstring,msg,param1,param2,param3);
  my_error(outstring);
}

void my_error(const char *msg, float param1, float param2, float param3, float param4) {
  char outstring[100];
  sprintf(outstring,msg,param1,param2,param3,param4);
  my_error(outstring);
}

void my_error(const char *msg, float param1, int param2) {
  char outstring[100];
  sprintf(outstring,msg,param1,param2);
  my_error(outstring);
}




int closest_int( float x ){
  if ( x < 0 ) x -= 0.5;
  else         x += 0.5;
  return (int) x;
}

void NormalizeAngleDeg(int *ang){
  if (fabs(float(*ang)) > 5000)
    my_error("Huge angle passed to NormalizeAngleDeg");
  while (*ang > 180) *ang-=360;
  while (*ang < -180) *ang+=360;
}

void NormalizeAngleDeg(AngleDeg *ang){
  if (fabs(*ang) > 5000)
    my_error("Huge angle passed to NormalizeAngleDeg");
  while (*ang > 180) *ang-=360;
  while (*ang < -180) *ang+=360;
}

void NormalizeAngleRad(AngleRad *ang){
  while (*ang > M_PI) *ang-=2*M_PI;
  while (*ang < -M_PI) *ang+=2*M_PI;
}

AngleDeg GetNormalizeAngleDeg(AngleDeg ang){
  if (fabs(ang) > 5000)
    my_error("Huge angle passed to GetNormalizeAngleDeg");
  while (ang > 180) ang-=360;
  while (ang < -180) ang+=360;
  return ang;
}

float GetDistance(float *x, float *y, float *a, float *b){
  return sqrt((*x-*a)*(*x-*a) + (*y-*b)*(*y-*b));
}

float weighted_avg(float val1, float val2, float w1, float w2){
  return (val1*w1 + val2*w2)/(w1+w2);
}

float SumGeomSeries(float first_term, float r, int n)
{
  return first_term * (Exp(r, n) - 1) / (r - 1);
}

float SolveForSumGeomSeries(float first_term, float r, float sum)
{
  if (r < 0)
    my_error("SolveForSumGeomSeries: can't take r < 0");
  float temp = sum * (r-1) / first_term + 1;
  if (temp <= 0)
    return -1.0;
  return log(temp) / log(r);
}

float Round(float x, int p)
{
  float s = signf(x);
  x = fabs(x);
  double m = pow(10.0, p);
  //printf("m: %f\n", m);
  double f = fmod(double(x),m);
  //printf("f: %f\n", f);
  //printf("pow: %f\n", .5*m);
  if (f >= .5*m) {
    //printf("Going up");
    x += (m-f);
  } else {
    //printf("Going down");
    x -= f;
  }
  return s*x;
} 



Bool skip_white_space(istream& infile)
{
  char c;

  while(1) {
    infile.get(c);
    if (!infile) return FALSE;
    if (!isspace(c)) {
      infile.putback(c);
      return TRUE; 
    } 
  } 
} 

/* advances to the specifed character, but only on the same line */
Bool advance_to_on_line(istream& infile, char match_c)
{
  char c;

  while(1) {
    infile.get(c);
    if (!infile) return FALSE;
    if (c == match_c) {
      infile.putback(c);
      return TRUE; 
    }
    if (c == '\n') {
      return FALSE; 
    } 
  } 
}



/* returns a pathname with a trailing / */
void get_leading_directory(const char* path, char* dir)
{
  char *pc;
  pc = strrchr(path, '/');
  if (pc == NULL) {
    /* there are no slashes in the pathname */
    strcpy(dir, "./");
  } else {
    strncpy(dir, path, (pc - path) + 1 );
    dir[(pc-path) + 1] = 0; 
  } 
} 

/* puts the number after the colon in the float, returns FALSE on failure */
Bool GetIntOffLine(char** ppc, int* pval)
{
  *ppc = strchr(*ppc, ':');
  if (*ppc == NULL) {
    my_error("Observe (GetNumFromLine): malformed count line");
    return FALSE;
  } else {
    (*ppc)++;
    *pval = get_int(ppc);
  }
  return TRUE;
}

/* puts the number after the colon in the float, returns FALSE on failure */
Bool GetDoubleOffLine(char** ppc, double* pval)
{
  *ppc = strchr(*ppc, ':');
  if (*ppc == NULL) {
    my_error("Observe (GetNumFromLine): malformed count line");
    return FALSE;
  } else {
    (*ppc)++;
    *pval = (float)get_double(ppc);
  }
  return TRUE;
}

/* puts the number after the colon in the int, returns FALSE on failure */
Bool GetIntFromLine(char* line, int* pval)
{
  char** ppc = &line;
  return GetIntOffLine(ppc, pval);
}

/* puts the number after the colon in the float, returns FALSE on failure */
Bool GetFloatFromLine(char* line, float* pval)
{
  char** ppc = &line;
  double d;
  Bool ret = GetDoubleOffLine(ppc, &d);
  *pval = (float)d;
  return ret;
}

Bool GetDoubleFromLine(char* line, double* pval)
{
  char** ppc = &line;
  return GetDoubleOffLine(ppc, pval);

}


/****************************************************************************/
/* These routines are to save time instead of using sscanf or atof, etc.    */
/* When passing **str_ptr, the pointer is advanced past the number          */
/* When passing  *str    , the pointer remains where it was before          */
/****************************************************************************/

double get_double(char **str_ptr){

  double d_frac, result;
  int  m_flag, d_flag;

  d_frac = 1.0;
  result = 0.0;
  m_flag = d_flag = 0;

  /* Advance to the beginning of the number */
  while( !isdigit(**str_ptr) && **str_ptr!='-' && **str_ptr!='.')
    (*str_ptr)++;

  /* Process the number bit by bit */
  while((**str_ptr!=')') &&
	(**str_ptr!='\t') &&
	(**str_ptr!=' ') &&
	(**str_ptr!='\n') &&
	(**str_ptr!= 0 ) &&
	(**str_ptr!=',')){
    if (**str_ptr=='.')
      d_flag=1;
    else if (**str_ptr=='-')
      m_flag=1;
    else if (d_flag){
      d_frac *= 10.0;
      result+=(double)(**str_ptr-'0')/d_frac;
    }
    else
      result=result*10.0+(double)(**str_ptr-'0');
    (*str_ptr)++;
  }
  if (m_flag)
    result=-result;

  //printf("%.1f\n",result);

  return result;
}

/* Get the number, but don't advance pointer */

double get_double(char *str){
  char **str_ptr = &str;
  return get_double(str_ptr);
}

/****************************************************************************/

float get_float(char **str_ptr){

  /* Advance to the beginning of the number */
  while( !isdigit(**str_ptr) && **str_ptr!='-' && **str_ptr!='.')
    (*str_ptr)++;
  istringstream ist(*str_ptr);
  float res;
  ist>>res;
  while(**str_ptr!=' '&&**str_ptr!='\t'&&**str_ptr!='\n'&&**str_ptr!=')'&&*str_ptr!=0)
    (*str_ptr)++;
  return res;

//   float d_frac, result;
//   int  m_flag, d_flag;

//   d_frac = 1.0;
//   result = 0.0;
//   m_flag = d_flag = 0;

//   /* Advance to the beginning of the number */
//   while( !isdigit(**str_ptr) && **str_ptr!='-' && **str_ptr!='.')
//     (*str_ptr)++;

//   /* Process the number bit by bit */
//   while((**str_ptr!=')') &&
// 	(**str_ptr!=' ') &&
// 	(**str_ptr!='\t') &&
// 	(**str_ptr!='\n') &&
// 	(**str_ptr!= 0 ) &&
// 	(**str_ptr!=',')){
//     if(**str_ptr=='e')//AI: hack
//       return 0.0f;
//     if (**str_ptr=='.')
//       d_flag=1;
//     else if (**str_ptr=='-')
//       m_flag=1;
//     else if (d_flag){
//       d_frac *= 10.0;
//       result+=(float)(**str_ptr-'0')/d_frac;
//     }
//     else
//       result=result*10.0+(float)(**str_ptr-'0');
//     (*str_ptr)++;
//   }
//   if (m_flag)
//     result=-result;

//   //printf("%.1f\n",result);

//   return result;
}

/* Get the number, but don't advance pointer */

float get_float(char *str){
  /* Advance to the beginning of the number */
  
  char **str_ptr = &str;
  return get_float(str_ptr);
}

/****************************************************************************/

int get_int(char **str_ptr){

  int result;
  int m_flag;

  result = 0;
  m_flag = 0;

  /* Advance to the beginning of the number */
  while( !isdigit(**str_ptr) && **str_ptr!='-')
    (*str_ptr)++;

  /* Process the number bit by bit */
  while((**str_ptr!=')') && (**str_ptr!=' ') && (**str_ptr!='\t') && (**str_ptr!='\n') && (**str_ptr!=',')){
    if (**str_ptr=='-')
      m_flag=1;
    else
      result=result*10+(int)(**str_ptr-'0');
    (*str_ptr)++;
  }
  if (m_flag)
    result=-result;

  return result;
}

int get_int(char *str){
  char **str_ptr = &str;
  return get_int(str_ptr);
}

/****************************************************************************/

void get_word(char **str_ptr){
  while ( !isalpha(**str_ptr) ) (*str_ptr)++;
}

/****************************************************************************/

void get_next_word(char **str_ptr){
  while ( isalpha(**str_ptr) ) (*str_ptr)++;
  get_word(str_ptr);
}

/****************************************************************************/

void advance_to(char c, char **str_ptr){
  while ( **str_ptr != c ) (*str_ptr)++;
}
/****************************************************************************/

void get_token (char **str_ptr) {
  advance_past_space(str_ptr);
  while ( (*str_ptr) && !isspace(**str_ptr)) (*str_ptr)++;
}
/****************************************************************************/

void   advance_past_space(char **str_ptr) {
  while ( (*str_ptr) && isspace(**str_ptr)) (*str_ptr)++;
}


/****************************************************************************/
/* These routines are to save time instead of using sprintf or atof, etc.   */
/* *str should point to the END of the string where the number is going     */
/* return the length of the number placed in                                */
/****************************************************************************/

int put_float(char *str, float fnum, int precision){
  int m_flag = 0, length = 0;
  int num, old_num;

  for (int i=0; i<precision; i++)
    fnum *= 10;

  num = closest_int(fnum);  /* round off the rest */

  if ( precision == 0 ) 
    return put_int(str,num);

  if ( num < 0 ){
    m_flag = 1;
    num = -num;
  }

  old_num = num;
  while ( num > 0 || length < precision ){
    num /= 10;
    *str = '0' + old_num - num*10;
    old_num = num;
    str--;
    length++;
    if ( length == precision ){
      *str = '.';
      str--;
      length++;
      if ( num == 0 ){
	*str = '0';
	str--;
	length++;
	break;
      }
    }
  }

  if ( m_flag ){
    *str = '-';
    length++;
  }

  return length;
}

/****************************************************************************/

int put_int(char *str, int num){

  int m_flag = 0, length = 0;
  int old_num;

  if ( num == 0 ){
    *str = '0';
    return 1;
  }

  if ( num < 0 ){
    m_flag = 1;
    num = -num;
  }

  old_num = num;
  while ( num > 0 ){
    num /= 10;
    *str = '0' + old_num - num*10;
    old_num = num;
    str--;
    length++;
  }

  if ( m_flag ){
    *str = '-';
    length++;
  }

  return length;
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

void BubbleSort(int length, int *elements, float *keys){
  
  /*  for (int a=0; a<length; a++){
      printf("%d--%.1f ",elements[a], keys[a]);
      }
      printf("\n");*/

  /* Sort the elements in increasing order according to the keys */
  float keytemp;
  int eltemp;
  for (int i=0; i<length; i++){
    for (int j=i+1; j<length; j++){
      if ( keys[j] < keys[i] ){
	keytemp = keys[i];
	keys[i] = keys[j];
	keys[j] = keytemp;
	eltemp = elements[i];
	elements[i] = elements[j];
	elements[j] = eltemp;
      }
    }
  }
  /*  for (int a=0; a<length; a++){
      printf("%d--%.1f ",elements[a], keys[a]);
      }
      printf("\n");*/
}

/****************************************************************************/

int BinarySearch(int length, float *elements, float key){
  
  /* Assume the list is already sorted in increasing order */
  int lbound = 0, ubound = length;
  
  for ( int index = length/2; ubound-lbound > 0; index = lbound+(ubound-lbound)/2 ){
    /* printf("%d ",index); */
    if ( elements[index] == key ){
      lbound = ubound = index;
    }
    else if ( elements[index] < key ){
      lbound = index+1;
    }
    else {
      ubound = index-1;
    }
  }
 
  int toReturn = Max(ubound,lbound);
  if (elements[toReturn] < key) toReturn++;  /* Guarantees >= key */

  return toReturn;
}

/****************************************************************************/ 

/* replace all occurrences in a string */
void StrReplace(char *str, char oldchar, char newchar){
  int i=0;
#if 0
  int numReplaced;
#endif
  int strLength = strlen(str);
  while ( i++ < strLength ){
    if ( str[i] == oldchar ){
      str[i] = newchar;
#if 0
      numReplaced++;
#endif
    }
    if ( i==1000 ) 
      my_error("String of length >1000?");
  }
#if 0
  printf("***Replaced %d %c's in string of length %d (%d): %s***\n",
	 numReplaced,oldchar,strlen(str),i,str);
#endif
}

/****************************************************************************/
/***************************   random stuff    ******************************/
/****************************************************************************/
/* From Andrew's C package                                                  */

int int_random(int n)
{
  static int FirstTime = TRUE;
  
  if ( FirstTime ){
    /* initialize the random number seed. */
    timeval tp;
    gettimeofday( &tp, NULL );
    srandom( (unsigned int) tp.tv_usec );
    FirstTime = FALSE;
  }

  if ( n > 2 )
    return( random() % n );
  else if ( n == 2 )
    return( ( (random() % 112) >= 56 ) ? 0 : 1 );
  else if ( n == 1 )
    return(0);
  else
    {
      printf("int_random(%d) ?\n",n);
      my_error("You called int_random(<=0)");
      return(0);
    }
}

float range_random(float lo, float hi)
{
  int x1 = int_random(10000);
  int x2 = int_random(10000);
  float r = (((float) x1) + 10000.0 * ((float) x2))/(10000.0 * 10000.0);
  return( lo + (hi - lo) * r );
}

int very_random_int(int n)
{
  int result = (int) range_random(0.0,(float)n);  /* rounds down */
  if ( result == n ) result = n-1;
  return(result);
}

void GetStampedName( char *name, char *outputName ){
  char date[100],weekday[10],month[10],temp[10];
  int  day,hour,min,sec,year;
  FILE *dateFile;
  
  //system("date > ../date.log");        /* Put the date in a file */ /* done by a player */

  dateFile = fopen("../date.log","r");
  fscanf(dateFile,"%[^\n]",date);         /* Scan it in             */
  fclose(dateFile);
  
  sscanf(date,"%s %s %d %d:%d:%d %s %d",
	 weekday,month,&day,&hour,&min,&sec,temp,&year);
  sprintf(name,"%s-%s%d-%d:%d:%d.dat",outputName,month,day,hour,min,sec);
}
