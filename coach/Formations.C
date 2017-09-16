/*************************************************************************
 *
 *    DESCRIPTION :    
 *
 *    FILE 	 : Formations.C
 *
 *    AUTHOR     : Anton Ivanov
 *
 *    $Revision: 1.4 $
 *
 *    $Id: Formations.C,v 1.4 2004/06/26 08:43:35 anton Exp $
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


#include "Formations.h"
#include "client.h"

#include <sstream>
#include <string>
#include <fstream>
using namespace std;
///////////////////////
Formations::bp *Formations::begin_pos;
int Formations::num_of_begin_pos;
string Formations::PlayerName;//name of player
FormationData Formations::formations[Formations::num_of_formations];

//для добавления новой формации надо:
// 1. Создать новое имя в перечеслении Ftype (файл Formations.h)
// 2. Увеличить значение переменной num_of_formations на 1 (файл Formations.h)
// 3. Добавить новую запись в структуру form_str (файл Formations.C)

Formations::Formation_Str Formations::form_str[Formations::num_of_formations]={
  {"[433",FT_433},
  {"[433_offense",FT_433_OFFENSE},
  {"[523_defense",FT_523_DEFENSE}
};
/////////////////////////////////////////////////////////////////////////
bool FormationLine::LoadData(istream& file){
  char c;
  file>>c>>from_x>>c>>base>>c>>attr>>c;
  return true;
}
//////////////////////////////////////////////////////////////////////////
FormationData::FormationData(){
  for(int i=0;i<11;i++)
    line_x[i]=line_y[i]=0;
  formation=FT_None;
}
//////////////////////////////////////////////////////////////////////////
FormationData::~FormationData(){
  for(int i=0;i<11;i++){
    if(line_x[i]) delete []line_x[i];
    if(line_y[i]) delete []line_y[i];
  }
}
//////////////////////////////////////////////////////////////////////////
bool FormationData::LoadData(ifstream& file,Ftype formation,int num_fracture_x,int num_fracture_y){
  this->formation=formation;
  num_x_lines=num_fracture_x+1;
  num_y_lines=num_fracture_y+1;
  string str;
  char val;
  int num;
  int i=0;
  while(i<11){
    getline(file,str);
    if(str[0]=='#'||str.size()<=1)
      continue;
    istringstream istr(str);
    istr>>num;
    if(num<1||num>11){
      cout<<"Error player number "<<num<<endl;

      return false;
    }
    int player=num-1;//set number as index
    line_x[player]=new FormationLine[num_x_lines];
    line_y[player]=new FormationLine[num_y_lines];
   
    istr>>val;
    types[player]=conv_char_to_ptype(val);
    istr>>val;
    sides[player]=conv_char_to_pside(val);
    for(int j=0;j<num_x_lines;j++)
      if(!line_x[player][j].LoadData(istr))
	return false;
    for(int j=0;j<num_y_lines;j++)
      if(!line_y[player][j].LoadData(istr))
	return false;
    istr>>min_x[player];
    istr>>max_x[player];
    i++;
  }
  for(i=0;i<10;i++)
    for(int j=0;j<10;j++)
      file>>activity_digraph[i][j];
  return true;
}
/////////////////////////////////////////////////////////////////////////
Ptype FormationData::conv_char_to_ptype(char c) const{
  Ptype p;
  switch(c){
  case 'G': p=PT_Goaltender;
    break;
  case 'D': p=PT_Defender;
    break;
  case 'M': p=PT_Midfielder;
    break;
  case 'F': p=PT_Forward;
    break;
  case 'S': p=PT_Sweeper;
    break;
  default:
    cerr<<"Not right format of player role: "<<c<<endl;
    exit(1);
  }
  return p;
}
///////////////////////////////////////////////////
Pside FormationData::conv_char_to_pside(char c) const{
  Pside s;
  switch(c){
  case 'C': s=PS_Center;
    break;
  case 'L': s=PS_Left;
    break;
  case 'R': s=PS_Right;
    break;
  default:
    cerr<<"Not right format of player side: "<<c<<endl;
    exit(1);
  }
  return s;
}
/////////////////////////////////////////////////////

Formations::~Formations(){

  if(begin_pos)
    delete []begin_pos;
}
/////////////////////////////////////////////////////////////////////////

Formations::Formations(){
  begin_pos=0;
  for(int i=0;i<11;i++)
    updateHomePos[i]=-1;
  InitBDPMatrix();
  //initialize current formation
  SetCurrentFormation(FT_433_OFFENSE);
  important_action=false;
}
////////////////////////////////////////////////////
Ftype Formations::GetFormationByString(string str){
  for(int i=0;i<num_of_formations;i++){
    if(str==form_str[i].name){//find formation
      return form_str[i].type;
    }
  }
  return FT_None;
}
////////////////////////////////////////////////////
void Formations::LoadConfigFile(){
  string str;
  ifstream file("./formations.conf");
  if(!file){

    cerr<<"Not find file \""<<"./formations.conf"<<"\" !!!"<<endl;
    exit(1);
  }
  Ftype formation;
  int fracture_x,fracture_y;
  while(file>>str){
    if(str[0]=='#'){
      getline(file,str);
      continue;
    }
  
    if(str=="[Players]"){
      int i;
      int step=0;
      while(step<11){
	step++;
	file>>i;
	getline(file,str);//= name
      }
    }else
  	
      if(str=="[BeginPos]"){
	file>>num_of_begin_pos;
	begin_pos=new bp[num_of_begin_pos];
	for(int j=0;j<num_of_begin_pos;j++)
	  for(int i=0;i<11;i++){
	    file>>begin_pos[j][i][0];
	    file>>begin_pos[j][i][1];
	  }
      }else{
	formation=GetFormationByString(str);
	if(formation!=FT_None){
	  file.ignore(100,'=');
	  file>>fracture_x;
	  if(fracture_x<0||fracture_x>10){
	    cout<<"Error in loading formation ["<<form_str[formation].name<<"]; wrong fracture_x:"<<fracture_x<<endl;
	    exit(1);
	  }
	  file.ignore(100,'=');
	  file>>fracture_y>>str;
	  if(fracture_y<0||fracture_y>10){
	    cout<<"Error in loading formation ["<<form_str[formation].name<<"]; wrong fracture_y:"<<fracture_y<<endl;
	    exit(1);
	  }
	  if(!formations[formation].LoadData(file,formation,fracture_x,fracture_y)){
            cout<<"Error in loading formation ["<<form_str[formation].name<<"]"<<endl;
            exit(1);
	  }else{
	    cout<<"Loading formation "<<form_str[formation].name<<"] ok..."<<endl;
	  }
	}else
	  if(str=="[EndFile]")
	    break;
      }//and else
  }
}
///////////////////////////////////////////////////
void Formations::SetCurrentFormation(Ftype fm){
  if (fm<=FT_None||fm>num_of_formations)
    cerr<<"Wrong formation "<<fm;
  else
    current_formation=int(fm);
}
///////////////////////////////////////////////////////////
void Formations::InitBDPMatrix(){
  float width=68.0/NUM_WIDTH_BLOCKS;
  float height=105.0/NUM_HEIGHT_BLOCKS;
  for(int i=0;i<NUM_HEIGHT_BLOCKS;i++)
    for(int j=0;j<NUM_WIDTH_BLOCKS;j++)
      BDPMatrix[i][j]=Vector(height/2+i*height-105.0/2,width/2+j*width-68.0/2);
}
/////////////////////////////////////////////////////////////
Vector Formations::GetBDP(const Vector& ball){
  float width=Mem->SP_pitch_width/NUM_WIDTH_BLOCKS;
  float height=Mem->SP_pitch_length/NUM_HEIGHT_BLOCKS;
  int j=static_cast<int>((ball.y+Mem->SP_pitch_width/2)/width);
  if(j==NUM_WIDTH_BLOCKS) j--;
  int i=static_cast<int>((ball.x+Mem->SP_pitch_length/2)/height);
  if(i==NUM_HEIGHT_BLOCKS) i--;
  //  Mem->LogAction6(10,"GetBDP:was (%.2f,%.2f), new (%.2f,%.2f)",ball.x,ball.y,BDPMatrix[i][j].x,BDPMatrix[i][j].y);
  return BDPMatrix[i][j];
}
///////////////////////////////////////////////////////////////
Unum Formations::GetPlayerNumber(Iterator& iter,int PT_mask,int PS_mask) const{
  for(Iterator index=iter;index<11;index++)
    if(PT_mask&formations[current_formation].types[index] && PS_mask&formations[current_formation].sides[index]){
      iter=index+1;
      return static_cast<Unum> (iter);
    }
  return Unum_Unknown;
}
