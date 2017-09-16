/*************************************************************************
 *
 *    DESCRIPTION :    
 *
 *    FILE 	 : client.C
 *
 *    AUTHOR     : Anton Ivanov
 *
 *    $Revision: 1.4 $
 *
 *    $Id: client.C,v 1.4 2004/06/26 08:43:35 anton Exp $
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

/* coach/client.C
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

/* this file contains the main program for the on-line/off-line coach */

#include "client.h"
#include "types.h"
#include "netif.h"
#include "Memory.h"
#include "parse.h"
#include "utils.h"
#include "logfile.h"
#include <sstream>



/* Global variables -- don't want to reallocate buffers each time */

/* these are for network IO */     
sigset_t sigiomask, sigalrmask;

Memory *Mem;

char     recvbuf[MAXMESG];	
char     sendbuf[MAXMESG];	

Socket sock;

int      max_alrs_since_io = 10;
int      alrsigs_since_iosig=0;



/****************************************************************************/
int main(int argc, char *argv[])
{    
  Mem = new Memory();

  if ( Mem == NULL ){
    my_error("couldn't allocate Mem");
    exit(0);
  }
  Mem->GetOptions(argc,argv);
  if(Mem->MP_analize_types_from_cin==1){
    Mem->Initialize();
    AnalizeTypesFromCin();
    return 0;
  }
  
  //if want analyze Logs add if statement here
  ConnectToServerLoop();
  
  printf("Shutting down coach\n");

  return 0;
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

void ConnectToServerLoop() 
{
  //this is an online coach
  sock = init_connection(Mem->SP_host,Mem->SP_olcoach_port);
  
  if(sock.socketfd == -1) {
    cerr << "Can't open connection for trainer" << endl;
    abort();
  }

  if (!Mem->Initialize()) {
    my_error("Memory failed to initialize");
    return;
  }

  send_initialize_message();
  int res=2;
  while(res==2){	
    if ( wait_message(recvbuf, &sock) == 0 )
      my_error("wait_message failed");
    res=parse_initialize_message(recvbuf);//return 1 if ok
    if (!res) {
      my_error("Could not parse initialize message");
      return;
    }
  }

  printf("Coach started\n");

  max_alrs_since_io =
    int(((float)Mem->CP_max_cycles_since_io) * Mem->SP_send_vi_step / Mem->CP_alarm_interval + 1.0);
  
  sigset_t sigfullmask = init_handler();	      

  Eye(TRUE);

  while ( !Mem->ShutDown == TRUE && wait_for_signals(&sigfullmask) )
    ;

  if (sock.socketfd != -1) close_connection(&sock);

  printf("Coach shutting down\n");
}

/****************************************************************************/

void send_initialize_message()
{
  sprintf(sendbuf, "(init %s (version %d))", Mem->CP_team_name,(int)Mem->SP_version);

  if(send_message(sendbuf, &sock) == -1) abort(); 
}

/****************************************************************************/

int parse_initialize_message(char* mess)//return 0 - error; 1 - ok; 2 - must wait 
{
  if (Mem->MP_save_log && Mem->Initialized)
    Mem->LogFile<<mess<<endl<<endl; 
  if ( !(strncmp(recvbuf,"(init",4)) ) {
    /* It's an init msg */
    /* I don't have to do anything */
    if(mess[6]=='l'){
      Mem->MySide=TS_Left;
      Mem->TheirSide=TS_Right;
      cout<<"My sight is left"<<endl;
    }else
      if(mess[6]=='r'){
	Mem->MySide=TS_Right;
	Mem->TheirSide=TS_Left;
	cout<<"My sight is right"<<endl;
      }else
	my_error("Wrong param in parse_initialize_message: %s",mess);		
  } else {
    my_error("Didn't get an init message: '%s'",mess);
    Parse(mess, Mem);
    if(Mem->GetPlayMode()!=PM_Before_Kick_Off){
      my_error("Game start but i still have not init massage!!!");
      return 0;
    }
    return 2;
  }
  return 1;
}


/****************************************************************************/


/* set time interval between the sensor receiving and command sending */ 
inline void set_timer() {
  struct itimerval itv;
  itv.it_interval.tv_sec = 0;
  itv.it_interval.tv_usec = Mem->CP_alarm_interval * 1000;
  itv.it_value.tv_sec = 0;
  itv.it_value.tv_usec = Mem->CP_alarm_interval * 1000;
  setitimer(ITIMER_REAL, &itv, NULL);
}

inline void set_timer(int usec) {
  struct itimerval itv;
  itv.it_interval.tv_sec = 0;
  itv.it_interval.tv_usec = Mem->CP_alarm_interval * 1000;
  itv.it_value.tv_sec = 0;
  itv.it_value.tv_usec = usec;
  setitimer(ITIMER_REAL, &itv, NULL);
}

/****************************************************************************/

sigset_t init_handler() { 
  sigemptyset(&sigiomask);
  sigaddset(&sigiomask, SIGIO);
  
  struct sigaction sigact;
  sigact.sa_flags = 0;
  sigact.sa_mask = sigiomask;

#ifdef Solaris
  sigact.sa_handler = (void (*)(int))sigalrm_handler; 
#else
  sigact.sa_handler = (void (*)(int))sigalrm_handler; 
#endif

  sigaction(SIGALRM, &sigact, NULL);
  sigact.sa_mask = sigalrmask;

#ifdef Solaris
  sigact.sa_handler = (void (*)(int))sigio_handler; 
#else
  sigact.sa_handler = (void (*)(int))sigio_handler; 
#endif

  sigaction(SIGIO, &sigact, NULL);
  set_timer();
  sigprocmask(SIG_UNBLOCK, &sigiomask, NULL);
  sigprocmask(SIG_UNBLOCK, &sigalrmask, NULL);

  sigset_t sigsetmask;
  sigprocmask(SIG_BLOCK, NULL, &sigsetmask);   /* Get's the currently unblocked signals */
  return sigsetmask;   
}


/****************************************************************************/

/* suspend the process until one of the signals comes through */
/* could check for situation to kill client, return FALSE     */
/* i.e. too many actions with no sensory input coming in      */
Bool wait_for_signals(sigset_t *mask){
  sigsuspend(mask);
  return TRUE;
}


/****************************************************************************/

/* SIGIO handler: receive and parse messages from server */
void sigio_handler() {
  sigprocmask(SIG_BLOCK, &sigalrmask, NULL);  
  int counter = 0;
  while (receive_message(recvbuf, &sock) == 1) {
    SenseType st = Parse(recvbuf, Mem);
    switch (st) {
    case ST_None:
      my_error("Could not incorporate sense");
      break;
    case ST_See:
      SelectHetroPlayers();
      CheckOppponentPlayerTypes();
      if (Mem->CP_coach_setplays) {
	Mem->SetplaySightHandler();
      }
      if(Mem->can_send_types&&Mem->CanFreeformSend()){
	Mem->last_send_freeform=Mem->GetTime();
	Mem->can_send_types=false;
	SendOpponentTypes();
      }
      break;
    case ST_HearPlayer:
      /* INSERT CODE HERE! */
      if (Mem->CP_coach_setplays) {
	Mem->SetplayPlayerSoundHandler();
      }
      break;
    case ST_HearReferee:
      /* the info is already incorporated... */
      break;
    case ST_HearCoach:
    case ST_HearTrainer:
      break;
    case ST_ServerParam:
    case ST_PlayerParam:
    case ST_PlayerType:
      break;
    case ST_ChangeMyPlayerType:
    case ST_ChangeTheirPlayerType:
    case ST_ClangVersion:
      break;
    case ST_Ok:
    case ST_Error:
    case ST_Warning:
      break;      
    default:      
      my_error("Illegal value of SenseType: %d", st);
      break;
    }
    
    counter++;
  }

  sigprocmask(SIG_UNBLOCK, &sigalrmask, NULL);  

  alrsigs_since_iosig = 0;
}
//~/////////////////////////////////////////////////////////////////////////////
//~/////////////////////////////////////////////////////////////////////////////

void LogPlayerTypes(){
  if(!Mem->MP_save_log)
    return;
  Mem->LogFile.width(20);
  Mem->LogFile<<"Types:";
  for(int i=0;i<Mem->SP_player_types;i++){
    Mem->LogFile.width(10);
    Mem->LogFile<<i;
  }
  Mem->LogFile<<endl;
  Mem->LogFile.width(20);
  Mem->LogFile<<"player_speed_max";
  for(int i=0;i<Mem->SP_player_types;i++){
    Mem->LogFile.width(10);
    Mem->LogFile<<Mem->SP_hetro_player_type[i].GetPlayerSpeedMax();
  }
  Mem->LogFile<<endl;
  Mem->LogFile.width(20);
  Mem->LogFile<<"stamina_inc_max";
  for(int i=0;i<Mem->SP_player_types;i++){
    Mem->LogFile.width(10);
    Mem->LogFile<<Mem->SP_hetro_player_type[i].GetStaminaIncMax();
  }
  Mem->LogFile<<endl;
  Mem->LogFile.width(20);
  Mem->LogFile<<"player_decay";
  for(int i=0;i<Mem->SP_player_types;i++){
    Mem->LogFile.width(10);
    Mem->LogFile<<Mem->SP_hetro_player_type[i].GetPlayerDecay();
  }
  Mem->LogFile<<endl;
  Mem->LogFile.width(20);
  Mem->LogFile<<"inertia_moment";
  for(int i=0;i<Mem->SP_player_types;i++){
    Mem->LogFile.width(10);
    Mem->LogFile<<Mem->SP_hetro_player_type[i].GetInertiaMoment();
  }
  Mem->LogFile<<endl;
  Mem->LogFile.width(20);
  Mem->LogFile<<"dash_power_rate";
  for(int i=0;i<Mem->SP_player_types;i++){
    Mem->LogFile.width(10);
    Mem->LogFile<<Mem->SP_hetro_player_type[i].GetDashPowerRate();
  }
  Mem->LogFile<<endl;
  Mem->LogFile.width(20);
  Mem->LogFile<<"player_size";
  for(int i=0;i<Mem->SP_player_types;i++){
    Mem->LogFile.width(10);
    Mem->LogFile<<Mem->SP_hetro_player_type[i].GetPlayerSize();
  }
  Mem->LogFile<<endl;
  Mem->LogFile.width(20);
  Mem->LogFile<<"kickable_margin";
  for(int i=0;i<Mem->SP_player_types;i++){
    Mem->LogFile.width(10);
    Mem->LogFile<<Mem->SP_hetro_player_type[i].GetKickableMargin();
  }
  Mem->LogFile<<endl;
  Mem->LogFile.width(20);
  Mem->LogFile<<"kick_rand";
  for(int i=0;i<Mem->SP_player_types;i++){
    Mem->LogFile.width(10);
    Mem->LogFile<<Mem->SP_hetro_player_type[i].GetKickRand();
  }
  Mem->LogFile<<endl;
  Mem->LogFile.width(20);
  Mem->LogFile<<"extra_stamina";
  for(int i=0;i<Mem->SP_player_types;i++){
    Mem->LogFile.width(10);
    Mem->LogFile<<Mem->SP_hetro_player_type[i].GetExtraStamina();
  }
  Mem->LogFile<<endl;
  Mem->LogFile.width(20);
  Mem->LogFile<<"effort_max";
  for(int i=0;i<Mem->SP_player_types;i++){
    Mem->LogFile.width(10);
    Mem->LogFile<<Mem->SP_hetro_player_type[i].GetEffortMax();
  }
  Mem->LogFile<<endl;
  Mem->LogFile.width(20);
  Mem->LogFile<<"effort_min";
  for(int i=0;i<Mem->SP_player_types;i++){
    Mem->LogFile.width(10);
    Mem->LogFile<<Mem->SP_hetro_player_type[i].GetEffortMin();
  }
  Mem->LogFile<<endl;
}
//~///////////////////////////////////////////////////////////////////
struct PT{
  PT():type(0),num(0),accel(0.0f),size(0),stamina(0),s(0.0f),a(0.0f),all(0.0f),div(0.0f){}
  float li;
  int type;
  int num;
  float accel;
  int size,stamina;
  float s,a,all;
  float div;//stamina_inc_max/li
  bool operator<(const PT& o)const{return li<o.li;}
	
  bool CanUse()const{return type==0||num<Mem->SP_pt_max;}
};
struct li_less:public binary_function<PT,PT,bool>{//0
  bool operator() (const PT& x,const PT& y)const{return x.li<y.li;}
};
struct size_greater:public binary_function<PT,PT,bool>{//1
  bool operator() (const PT& x,const PT& y)const{return x.size>y.size;}
};
struct accel_greater:public binary_function<PT,PT,bool>{//2
  bool operator() (const PT& x,const PT& y)const{return x.accel>y.accel;}
};
struct stamina_greater:public binary_function<PT,PT,bool>{//3
  bool operator() (const PT& x,const PT& y)const{return x.stamina>y.stamina;}
};
struct div_greater:public binary_function<PT,PT,bool>{//4
  bool operator() (const PT& x,const PT& y)const{return x.div>y.div;}
};
struct s_greater:public binary_function<PT,PT,bool>{//5
  bool operator() (const PT& x,const PT& y)const{return x.s>y.s;}
};
struct a_greater:public binary_function<PT,PT,bool>{//6
  bool operator() (const PT& x,const PT& y)const{return x.a>y.a;}
};
struct all_greater:public binary_function<PT,PT,bool>{//6
  bool operator() (const PT& x,const PT& y)const{return x.all>y.all;}
};

//~///////////////////////////////////////////////////////////////////
void SortByNumber(vector<PT>& l,int num){
  switch(num){
  case 0:
    sort(l.begin(),l.end(),li_less());
    break;
  case 1:
    sort(l.begin(),l.end(),size_greater());
    break;
  case 2:
    sort(l.begin(),l.end(),accel_greater());
    break;
  case 3:
    sort(l.begin(),l.end(),stamina_greater());
    break;
  case 4:
    sort(l.begin(),l.end(),div_greater());
    break;
  case 5:
    sort(l.begin(),l.end(),s_greater());
    break;
  case 6:
    sort(l.begin(),l.end(),a_greater());
    break;
  case 7:
    sort(l.begin(),l.end(),all_greater());
    break;
  default:
    my_error("Bad in SortByName");
  }
}
//~///////////////////////////////////////////////////////////////////
int AccelValue(int num,const vector<PT>& pt){
  int val=0;
  for(int i=0;i<Mem->SP_player_types;i++){
    if(pt[num].li<pt[i].li) val++;
    //if(pt[num].div>pt[i].div) val++;
    if(Mem->SP_hetro_player_type[num].GetDashPowerRate()>Mem->SP_hetro_player_type[i].GetDashPowerRate()) val++;
  }
  return val;
}
//~///////////////////////////////////////////////////////////////////
int SizeValue(int num,const vector<PT>& pt){
  int val=0;
  for(int i=0;i<Mem->SP_player_types;i++){
    if(pt[num].li<pt[i].li) val++;
    //if(pt[num].div>pt[i].div) val++;
    if(Mem->SP_hetro_player_type[num].GetKickableMargin()>Mem->SP_hetro_player_type[i].GetKickableMargin()) val++;
  }
  return val;
}
//~////////////////////////////////////////////////////////////////////
int StaminaValue(int num,const vector<PT>& pt){
  int val=0;
  for(int i=0;i<Mem->SP_player_types;i++){
    if(pt[num].li<pt[i].li) val++;
    //if(pt[num].div>pt[i].div) val++;
    if(Mem->SP_hetro_player_type[num].GetStaminaIncMax()>Mem->SP_hetro_player_type[i].GetStaminaIncMax()) val++;
  }
  return val;
}
//~///////////////////////////////////////////////////////////////////
void ChangeSetOfPlayersType(Ptype pt,Pside ps,PT* l,vector<int>* types){
  Formations::Iterator iter=Mem->begin();
  while(iter!=Mem->end()){
    Unum p=Mem->GetPlayerNumber(iter,pt,ps);
    if(p==Unum_Unknown)
      break;
    if(l[0].CanUse()){
      if(types[0][p-1]!=0)
	l[types[0][p-1]].num--;
      types[0][p-1]=l[0].type;
      l[0].num++;
    }
  }
}
//~///////////////////////////////////////////////////////////////////
bool IsStrongOpponent(){
  static const string strong_opp[]={
    "portugal",
    "tsinghuaeolus",
    "brainstormers",
    "wright eagle",
    "uva",
    "u_v_a",
    "everest",
    ""
  };
  string name=Mem->TheirTeamname;
  for(unsigned int i=0;i<name.size();i++)
    name[i]=tolower(name[i]);
  for(int i=0;strong_opp[i]!="";i++){
    if(name.find(strong_opp[i])!=string::npos)
      return true;
  }
  return false;
}
//~///////////////////////////////////////////////////////////////////
void SelectHetroPlayers(){
  static bool first=true;
  if(first&&Mem->GetPlayMode()!=PM_Play_On&&Mem->TheirTeamname.size()!=0){
    first=false;
    bool strong=IsStrongOpponent();
    if(Mem->MP_save_log){
      Mem->LogFile<<"Opponent "<<Mem->TheirTeamname;
      if(strong)
	Mem->LogFile<<" is strong"<<endl;
      else
	Mem->LogFile<<" is NOT strong"<<endl;
    }
    LogPlayerTypes();
    vector<int> types=NewSelelctPlayerTypes(strong);//OldSelectPlayerTypes(strong);
    for(int i=1;i<=Mem->SP_team_size;i++){
      if(i==Mem->OurGoalieNum) continue;
      ChangePlayerType(i,types[i-1]);
    }
  }
}
//////////////////////////////////////////////////////////////////////
vector<int> OldSelectPlayerTypes(bool strong)
{
 
  vector<PT> l(Mem->SP_player_types);
  if(Mem->MP_save_log){
    Mem->LogFile.width(20);
    Mem->LogFile<<"li:";
  }
  for(int i=0;i<Mem->SP_player_types;i++){
    l[i].li=(Mem->SP_hetro_player_type[i].GetPlayerSpeedMax()*(1.0-Mem->SP_hetro_player_type[i].GetPlayerDecay())/
	     (Mem->SP_hetro_player_type[i].GetEffortMax()*Mem->SP_hetro_player_type[i].GetDashPowerRate()))-Mem->SP_hetro_player_type[i].GetStaminaIncMax();
    l[i].type=i;
    if(Mem->MP_save_log){
      Mem->LogFile.width(10);
      Mem->LogFile<<l[i].li;
    }
  }
  if(Mem->MP_save_log){
    Mem->LogFile<<endl;
    Mem->LogFile.width(20);
    Mem->LogFile<<"accel value:";
  }
  for(int i=0;i<Mem->SP_player_types;i++){
    l[i].accel=AccelValue(i,l);
    if(Mem->MP_save_log){
      Mem->LogFile.width(10);
      Mem->LogFile<<l[i].accel;
    }
  }
  if(Mem->MP_save_log){
    Mem->LogFile<<endl;
    Mem->LogFile.width(20);
    Mem->LogFile<<"size value:";
  }
  for(int i=0;i<Mem->SP_player_types;i++){
    l[i].size=SizeValue(i,l);
    if(Mem->MP_save_log){
      Mem->LogFile.width(10);
      Mem->LogFile<<l[i].size;
    }
  }
  if(Mem->MP_save_log){		
    Mem->LogFile<<endl;
    Mem->LogFile.width(20);
    Mem->LogFile<<"stamina value:";
  }
  for(int i=0;i<Mem->SP_player_types;i++){
    l[i].stamina=StaminaValue(i,l);
    if(Mem->MP_save_log){
      Mem->LogFile.width(10);
      Mem->LogFile<<l[i].stamina;
    }
  }
  if(Mem->MP_save_log){		
    Mem->LogFile<<endl;
    Mem->LogFile.width(20);
    Mem->LogFile<<"stamina_inc_max/li value:";
  }
  for(int i=0;i<Mem->SP_player_types;i++){
    l[i].div=Mem->SP_hetro_player_type[i].GetStaminaIncMax()/l[i].li;
    if(Mem->MP_save_log){
      Mem->LogFile.width(10);
      Mem->LogFile<<l[i].div;
    }
  }
  if(Mem->MP_save_log) Mem->LogFile<<endl;
		
  float def=l[0].li;//l[0].div;
  l.erase(l.begin());
  vector<int> types(Mem->SP_team_size,0);
  const int NUM=4;
		
  Ptype aPT[NUM]={PT_Forward,PT_Midfielder,PT_Defender,PT_Sweeper};
  Ptype aPT_for_strong[NUM]={PT_Forward,PT_Defender,PT_Midfielder,PT_Sweeper};
  Pside aPS[NUM]={PS_All,PS_All,PS_All,PS_All};
  Pside aPS_for_strong[NUM]={PS_All,PS_All,PS_All,PS_All};
  int aSortTypes[NUM]={2,0,2,2};
  int aSortTypes_strong[NUM]={2,2,0,2};	
		
  for(int i=0;i<NUM;i++){
    SortByNumber(l,strong?aSortTypes_strong[i]:aSortTypes[i]);
    if(l[0].li>=def*0.87||Mem->SP_hetro_player_type[l[0].type].GetStaminaIncMax()<Mem->SP_stamina_inc*0.75){//was 0.67
      //if(l[0].div<def){
      if(Mem->MP_save_log)
	Mem->LogFile<<endl<<"At cycle "<<i<<" we have bad, so try save stamina"<<endl;
      sort(l.begin(),l.end(),stamina_greater());
      if(l[0].li>=def){
	if(Mem->MP_save_log)
	  Mem->LogFile<<endl<<"At cycle "<<i<<" we have bad in stamina, so not changed type"<<endl;
	continue;
      }
    }
    if(strong)
      ChangeSetOfPlayersType(aPT_for_strong[i],aPS_for_strong[i],&l[0],&types);
    else
      ChangeSetOfPlayersType(aPT[i],aPS[i],&l[0],&types);
    l.erase(l.begin());
  }
  return types;
}

//********************************************************************
//////////////////////////////////////////////////////////////////////
int SimpleStaminaValue(int num,const vector<PT> l)
{
  int val=1;
  
  for(int i=0;i<Mem->SP_player_types;i++){
    if(i==num)
      continue;
    if(l[num].s>l[i].s) val++;
  }
  return val;

}
//////////////////////////////////////////////////////////////////////
int SimpleAccValue(int num,const vector<PT> l)
{
  int val=1;
  for(int i=0;i<Mem->SP_player_types;i++){
    if(i==num)
      continue;
    if(l[num].a>l[i].a) val++;
  }
  return val;
}

//////////////////////////////////////////////////////////////////////
vector<int> NewSelelctPlayerTypes(bool strong)
{
  vector<PT> l(Mem->SP_player_types);

  for(int i=0;i<Mem->SP_player_types;i++){
    l[i].s=Mem->SP_hetro_player_type[i].GetStaminaIncMax()*(Mem->SP_hetro_player_type[i].GetPlayerDecay()/*+0.5f*/);
    l[i].a=(Mem->SP_hetro_player_type[i].GetEffortMax()/*+2.0f*/)*
      Mem->SP_hetro_player_type[i].GetDashPowerRate();
    l[i].accel=Mem->SP_hetro_player_type[i].GetDashPowerRate();
    l[i].type=i;
  }
  if(Mem->MP_save_log){
    cout<<endl;
    Mem->LogFile<<endl;
  }
  for(int i=0;i<Mem->SP_player_types;i++){
    l[i].all=l[i].a*(l[i].s+3.5f/*7.5f*/)+(0.05f*Mem->SP_hetro_player_type[i].GetKickableMargin());
    if(Mem->MP_save_log){
      cout<<i<<": "<<l[i].all<<" <- ";
      cout<<l[i].a<<"  ";
      cout<<l[i].s<<endl;
      Mem->LogFile<<i<<": "<<l[i].all<<" <- ";
      Mem->LogFile<<l[i].a<<"  ";
      Mem->LogFile<<l[i].s<<endl;
    }
  }
  if(Mem->MP_save_log){
    cout<<endl;
    Mem->LogFile<<endl;
  }
  for(int i=1;i<Mem->SP_player_types;i++){
    if(l[i].all<l[0].all||l[i].s<l[0].s*0.72f)
      l[i]=PT();
  }
  
  vector<int> types(Mem->SP_team_size,0);
  const int NUM=4;

  Ptype aPT[NUM]={PT_Forward,PT_Midfielder,PT_Defender,PT_Sweeper};
  Ptype aPT_for_strong[NUM]={PT_Forward,PT_Defender,PT_Midfielder,PT_Sweeper};
  Pside aPS[NUM]={PS_All,PS_All,PS_All,PS_All};
  Pside aPS_for_strong[NUM]={PS_All,PS_All,PS_All,PS_All};
  int aSortTypes[NUM]={6,7,7,7};
  int aSortTypes_strong[NUM]={6,7,7,7};	
		
  for(int i=0;i<NUM;i++){
    SortByNumber(l,strong?aSortTypes_strong[i]:aSortTypes[i]);
    if(strong){
      if(l.begin()->type==0&&aPT_for_strong[i]==PT_Forward)
	SortByNumber(l,2);
      ChangeSetOfPlayersType(aPT_for_strong[i],aPS_for_strong[i],&l[0],&types);
    }else{
      if(l.begin()->type==0&&aPT[i]==PT_Forward)
	SortByNumber(l,2);
      ChangeSetOfPlayersType(aPT[i],aPS[i],&l[0],&types);
    }
    
    if(l.begin()->type!=0)
      l.erase(l.begin());
  } 
  return types;
}
//////////////////////////////////////////////////////////////////////
void AnalizeTypesFromCin()
{
  bool strong=false;
  string temp;
  cin>>temp>>temp;
  Mem->TheirTeamname=temp;
  cout<<"opponent "<<temp<<" is ";
  if(IsStrongOpponent()){
    cout<<"strong"<<endl;
    strong=true;
  }else
    cout<<"not strong"<<endl;
  cin.ignore(100,'\n');//skip rest
  cin.ignore(200,'\n');//skip line with nubers
  Mem->SP_hetro_player_type=new HPlayerType[Mem->SP_player_types];
  //загружаем типы гетерогенных игроков
  cin>>temp;
  for(int i=0;i<Mem->SP_player_types;i++)
    cin>>Mem->SP_hetro_player_type[i].GetPlayerSpeedMax();
  cin>>temp;
  for(int i=0;i<Mem->SP_player_types;i++)
    cin>>Mem->SP_hetro_player_type[i].GetStaminaIncMax();
  cin>>temp;
  for(int i=0;i<Mem->SP_player_types;i++)
    cin>>Mem->SP_hetro_player_type[i].GetPlayerDecay();
  cin>>temp;
  for(int i=0;i<Mem->SP_player_types;i++)
    cin>>Mem->SP_hetro_player_type[i].GetInertiaMoment();
  cin>>temp;
  for(int i=0;i<Mem->SP_player_types;i++)
    cin>>Mem->SP_hetro_player_type[i].GetDashPowerRate();
  cin>>temp;
  for(int i=0;i<Mem->SP_player_types;i++)
    cin>>Mem->SP_hetro_player_type[i].GetPlayerSize();
  cin>>temp;
  for(int i=0;i<Mem->SP_player_types;i++)
    cin>>Mem->SP_hetro_player_type[i].GetKickableMargin();
  cin>>temp;
  for(int i=0;i<Mem->SP_player_types;i++)
    cin>>Mem->SP_hetro_player_type[i].GetKickRand();
  cin>>temp;
  for(int i=0;i<Mem->SP_player_types;i++)
    cin>>Mem->SP_hetro_player_type[i].GetExtraStamina();
  cin>>temp;
  for(int i=0;i<Mem->SP_player_types;i++)
    cin>>Mem->SP_hetro_player_type[i].GetEffortMax();
  cin>>temp;
  for(int i=0;i<Mem->SP_player_types;i++)
    cin>>Mem->SP_hetro_player_type[i].GetEffortMin();
  //теперь производим анализ
  vector<int> types= OldSelectPlayerTypes(strong);
  for(int i=1;i<=Mem->SP_team_size;i++)
    cout<<i<<"->"<<types[i-1]<<"; ";
  cout<<endl;
  types=NewSelelctPlayerTypes(strong);
  for(int i=1;i<=Mem->SP_team_size;i++)
    cout<<i<<"->"<<types[i-1]<<"; ";
}


//////////////////////////////////////////////////////////////////////
// код для анализа типов игроков противника
//////////////////////////////////////////////////////////////////////
int FindClosestPlayerDecayType(float decay)
{
  float cl_dist=10.0f;
  int cl_type=-1;
  for(int i=0;i<Mem->SP_player_types;i++){
    if(fabs(Mem->SP_hetro_player_type[i].GetPlayerDecay()-decay)<cl_dist){
      cl_dist=fabs(Mem->SP_hetro_player_type[i].GetPlayerDecay()-decay);
      cl_type=i;
    }
  }
  if(cl_type==-1)
    my_error("ERROR in  FindClosestPlayerDecayType (cl_dist=%f)",cl_dist);
  return cl_type;
}
//////////////////////////////////////////////////////////////////////
bool IsDecayClose(float decay)
{
  float left_dist=10.0f,right_dist=10.0f;
  int left_type=-1,right_type=-1;
  for(int i=0;i<Mem->SP_player_types;i++){
    if(Mem->SP_hetro_player_type[i].GetPlayerDecay()==decay)
      return true;
    if(Mem->SP_hetro_player_type[i].GetPlayerDecay()<decay&&decay-Mem->SP_hetro_player_type[i].GetPlayerDecay()<left_dist){
      left_dist=decay-Mem->SP_hetro_player_type[i].GetPlayerDecay();
      left_type=i;
    }
    if(Mem->SP_hetro_player_type[i].GetPlayerDecay()>decay&&Mem->SP_hetro_player_type[i].GetPlayerDecay()-decay<right_dist){
      right_dist=Mem->SP_hetro_player_type[i].GetPlayerDecay()-decay;
      right_type=i;
    }
  }
  float width=left_dist+right_dist;
  if(min(left_dist,right_dist)<width/6.0f)
    return true;
  if(Mem->MP_save_log)
    Mem->LogFile<<"   left_dist="<<left_dist<<";right_dist="<<right_dist<<";width/6.0="<<width/6.0f<<endl;
  return false;
}
//////////////////////////////////////////////////////////////////////
set<Unum> GetCloseTypes(float decay)
{
  const float CLOSE_BUFFER=0.025f;
  set<Unum> ret;
  for(int i=0;i<Mem->SP_player_types;i++){
    if(fabs(Mem->SP_hetro_player_type[i].GetPlayerDecay()-decay)<CLOSE_BUFFER)
      ret.insert(i);
  }
  return ret;
}
//////////////////////////////////////////////////////////////////////
struct TypeSort
{
  int type;
  float dist;

  TypeSort():type(-1),dist(0.0f){}
  TypeSort(int t,float d):type(t),dist(d){}
  bool operator<(const TypeSort& o)const{return fabs(dist)<fabs(o.dist);}
};
//////////////////////////////////////////////////////////////////////
int CorrectForMaxAcc(int num,float player_decay,const set<Unum>& close_types)
{
  if(close_types.empty())
    return -1;
  if(close_types.size()==1)
    return *close_types.begin();
  vector<TypeSort> types;
  float dist;
  if(Mem->MP_save_log)
    Mem->LogFile<<"   number of close types="<<close_types.size()<<endl;
  for(set<Unum>::const_iterator i=close_types.begin();i!=close_types.end();++i){
    if(Mem->max_opp_acc[num-1]<0.2f)
      return -1;
    types.push_back(TypeSort(*i,dist=(Mem->max_opp_acc[num-1]/
	(Mem->SP_hetro_player_type[*i].GetEffortMax()*Mem->SP_hetro_player_type[*i].GetDashPowerRate()*
	 player_decay)-(Mem->SP_max_power+8.0f))));//addition to SP_max_power is HACK
    
    if(Mem->MP_save_log)
      Mem->LogFile<<"   For type "<<*i<<" dist="<<dist<<endl;
  }
  sort(types.begin(),types.end());
  if(fabs(types[0].dist-types[1].dist)<7.0f){
    int type=fabs(Mem->SP_hetro_player_type[types[0].type].GetPlayerDecay()-player_decay)<=
      fabs(Mem->SP_hetro_player_type[types[1].type].GetPlayerDecay()-player_decay)?types[0].type:types[1].type;
    if(Mem->MP_save_log)
      Mem->LogFile<<"2 type "<<types[0].type<<" and "<<types[1].type<<
	" are very close, so may be "<<type<<endl;
    return type;
  }

  return types[0].type;
}
//////////////////////////////////////////////////////////////////////
void CheckOppponentPlayerTypes()
{
  bool close_player;
  float player_decay;
  int player_type;
  set<Unum> close_types;
  
  if(Mem->TheirTeamname.size()!=0&&!Mem->opp_with_unknown_pt.empty()){
    BallInfo* ball=Mem->GetBall();
    for(set<Unum>::iterator i=Mem->opp_with_unknown_pt.begin();i!=Mem->opp_with_unknown_pt.end();i++){
      PlayerInfo* prev_info=Mem->GetPrevPlayer(1,Mem->TheirSide,*i);
      PlayerInfo* current_info=Mem->GetPlayer(Mem->TheirSide,*i);
      if(prev_info==0||current_info==0)
	continue;
      if(current_info->vel.mod()>1.0f||prev_info->vel.mod()>1.0f){//у нас где-то ошибка в parser
	if(Mem->MP_save_log)
	  Mem->LogFile<<"ERROR IN VEL"<<endl;
	continue;
      }
      close_player=(Mem->GetPlayer(Mem->MySide,Mem->ClosestPlayerToPlayer(Mem->TheirSide,*i,Mem->MySide))->pos-current_info->pos).mod()<1.5f||
	(Mem->GetPlayer(Mem->TheirSide,Mem->ClosestPlayerToPlayer(Mem->TheirSide,*i,Mem->TheirSide))->pos-current_info->pos).mod()<1.5f;
      if(current_info->vel.mod()>prev_info->vel.mod()){
	//противник в предыдущем цикле сделал dash
	float acc=current_info->vel.mod()-prev_info->vel.mod();
	if(Mem->max_opp_acc[*i-1]<acc)
	  Mem->max_opp_acc[*i-1]=acc;
	//continue;
      }
      if(/*fabs(GetNormalizeAngleDeg(current_info->body_ang-prev_info->body_ang))>1.0f&&prev_info->vel.mod()>0.01f&&*/current_info->vel.mod()>0.01f){
	//в предыдущем цикле противник повернул тело
	player_decay=current_info->vel.mod()/(current_info->pos-prev_info->pos).mod();
	if((ball->pos-current_info->pos).mod()<=1.5f){
	  continue;
	}
	if(close_player){
	  continue;
	}
	  
	Mem->num_av_opp_player_decay[*i-1]++;
	Mem->av_opp_player_decay[*i-1]+=player_decay;
	if((Mem->GetTime()-Mem->start_opp_analyze[*i-1])>100/*200*/&&Mem->num_av_opp_player_decay[*i-1]>20/*30*/){
	  close_types=GetCloseTypes(player_decay=Mem->av_opp_player_decay[*i-1]/Mem->num_av_opp_player_decay[*i-1]);
	  player_type=CorrectForMaxAcc(*i,player_decay,close_types);
	  if(player_type!=-1){	   
	    if(Mem->MP_save_log)
	      Mem->LogFile<<"For opponent "<<*i<<" select type "<<player_type<<"(player_decay="<<
		player_decay<<")"<<endl;
	    Mem->opp_player_type[*i-1]=player_type;
	    Mem->opp_with_unknown_pt.erase(i);
	    if(!Mem->opp_with_unknown_pt.empty())
	      i++;
	    else
	      if(Mem->MP_save_log){
		for(int j=0;j<11;j++)
		  Mem->LogFile<<j+1<<"="<<Mem->opp_player_type[j]<<" ";
		Mem->LogFile<<endl;
	      }
		  
	  }
	}
	
      }
      if(Mem->opp_with_unknown_pt.empty()){
	if(Mem->MP_save_log)
	  Mem->LogFile<<"Now can send all information about opponent"<<endl;
	Mem->can_send_types=true;
      }
    }
  }
}
////////////////////////////////////////////////////////////
void SendOpponentTypes()
{
  ostringstream res;
  res<<0;
  for(int i=1;i<=Mem->SP_team_size;i++){
    if(i==Mem->TheirGoalieNum)
      continue;
    if(Mem->opp_player_type[i-1]!=-2){
      res<<" ("<<i<<" "<<Mem->opp_player_type[i-1]<<")";
      Mem->opp_player_type[i-1]=-2;
    }
  }
  FreeformSay(const_cast<char*>(res.str().c_str()));
}
/*****************************************************************************/

/* we just use this to recognize when the server dies */
void sigalrm_handler() 
{
  alrsigs_since_iosig++;
  if (alrsigs_since_iosig > max_alrs_since_io) {
    cout<<"Too many alarms between ios from server. Shutting down"<<endl;
    Mem->ShutDown = TRUE;
  }
  
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

const char* PmodeStrings[PM_MAX+1] = PLAYMODE_STRINGS;

void ChangePlayerType(Unum num,int player_type){
  sprintf(sendbuf, "(change_player_type %d %d)",num,player_type);
  send_message(sendbuf, &sock);
}

void ChangeMode(Pmode pm)
{
  sprintf(sendbuf, "(say %s)", PmodeStrings[pm]);
  send_message(sendbuf, &sock);
}

void CheckBall()
{
  send_message("(check_ball)", &sock);
}

void Ear(Bool OnOff)
{
  sprintf(sendbuf, "(ear %s)", OnOff ? "on" : "off");
  send_message(sendbuf, &sock);
}

void Eye(Bool OnOff)
{
  sprintf(sendbuf, "(eye %s)", OnOff ? "on" : "off");
  send_message(sendbuf, &sock);
}

void Look()
{
  send_message("(look)", &sock);
}

void MoveBall(Vector pos)
{
  sprintf(sendbuf, "(move (ball) %f %f)", pos.x, pos.y);
  send_message(sendbuf, &sock);
}

void MovePlayer(TeamSide side, Unum num, Vector pos, float ang)
{
  if (ang == NOANG)
    sprintf(sendbuf, "(move (player %c %d) %f %f)",
	    (side == TS_Left) ? 'l' : 'r', num,
	    pos.x, pos.y);
  else
    sprintf(sendbuf, "(move (player %c %d) %f %f %f)",
	    (side == TS_Left) ? 'l' : 'r', num,
	    pos.x, pos.y, ang);
  send_message(sendbuf, &sock);
}

void Say(char* mess)
{
  sprintf(sendbuf, "(say %s)",  mess);
  cout << "Say: " << sendbuf << endl;
  send_message(sendbuf, &sock);
}

void FreeformSay(char* mess)
{
  sprintf(sendbuf,"(say (freeform \"%s\"))",mess);
  cout<<sendbuf<<endl;
  send_message(sendbuf,&sock);
}
