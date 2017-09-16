/*************************************************************************
 *
 *    DESCRIPTION :    
 *
 *    FILE 	 : MemOption.C
 *
 *    AUTHOR     : Anton Ivanov
 *
 *    $Revision: 1.3 $
 *
 *    $Id: MemOption.C,v 1.3 2004/04/19 08:01:38 anton Exp $
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

/* coach/MemOption.C
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

/* this file contains option file and command line processing and definitions.
   Note that many options may be meaningless in the released version of the code */


#include "netif.h"
#include "MemOption.h"
void HPlayerType::Log(){
  cout<< "player_speed_max="<<player_speed_max<<
    " stamina_inc_max="<<stamina_inc_max<<
    " player_decay="<<player_decay<<
    " inertia_moment="<<inertia_moment<<
    " dash_power_rate="<<dash_power_rate<<
    " player_size="<<player_size<<
    " kickable_margin="<<kickable_margin<<
    " kick_rand="<<kick_rand<<
    " extra_stamina="<<extra_stamina<<
    " effort_max="<<effort_max<<
    " effort_min="<<effort_min<<endl;
}
/* setting defaults to match version 7.00 server.conf */
OptionInfo::~OptionInfo(){
  delete [] SP_hetro_player_type;
}

OptionInfo::OptionInfo()
{
  /* coach params */
  CP_alarm_interval = 200;
  CP_max_cycles_since_io = 20;

  CP_analyze_log = TRUE;

  strcpy(CP_team_name, "ERA-Polytech");
  
  /* these are for setplays */
  CP_coach_setplays = FALSE;

  MP_cycles_to_store = 2;
  MP_save_log = FALSE;
  MP_left_team_name[0] = 0;
  MP_right_team_name[0] = 0;
  MP_analize_types_from_cin=0;
  
  /* no option flags for these */
  SP_pitch_length 	  = 105.0;
  SP_pitch_width 	  = 68.0;
  SP_pitch_margin	  = 5.0;
  SP_penalty_area_length  = 16.5;
  SP_penalty_area_width   = 40.32;
  SP_goal_area_length	  = 5.5;
  SP_goal_area_width	  = 18.32;					
  SP_penalty_spot_dist    = 11.0;
  SP_corner_arc_r	  = 1.0;
  SP_free_kick_buffer     = 9.15;
  SP_after_goal_wait  	  = 50;
  SP_feel_distance        = 3.0;
  SP_num_lines            = 4;
  SP_num_markers          = 55;
  SP_unum_far_length      = 20.0;
  SP_unum_too_far_length  = 40.0;
  SP_team_far_length      = 40.0;
  SP_team_too_far_length  = 60.0;

  SP_version              = 9.0;	
  SP_team_size            = 11;
  SP_half                 = 1;
  sprintf(SP_host,        "localhost");
  SP_goal_width           = 14.02;       
  SP_player_size          = 0.3;					//Change:AI
  SP_player_decay         = 0.4;					//***********
  SP_player_rand 	  = 0.1;						//**************
  SP_player_weight	  = 60.0;						//*************
  SP_player_speed_max     = 1.2;					//Change:AI
  SP_stamina_max	  = 4000.0;						//Change:AI
  SP_stamina_inc          = 45.0;					//Change:AI
  SP_recover_dec_thr      = 0.3;					//*******************
  SP_recover_dec          = 0.002; 					//Change:AI
  SP_recover_min          = 0.5;					//Change:AI
  SP_effort_dec_thr       = 0.3;					//Change:AI					
  SP_effort_min           = 0.6;					//Change:AI
  SP_effort_dec           = 0.005;					//Change:AI
  SP_effort_inc_thr       = 0.6;					//Change:AI
  SP_effort_inc           = 0.01;					//Change:AI
  SP_ball_size            = 0.085;					//**************		
  SP_ball_decay           = 0.94;					//Change:AI	
  SP_ball_rand            = 0.05;					//**************	
  SP_ball_weight          = 0.2;					//****************	
  SP_ball_speed_max       = 2.7;					//Change:AI	
  SP_dash_power_rate      = 0.006;					//Change:AI	
  SP_kick_power_rate      = 0.027;					//Change:AI
  SP_kickable_margin      = 0.7;					//Change:AI	
  SP_kickable_area        = SP_kickable_margin + SP_ball_size + SP_player_size;		//********
  SP_catch_prob           = 1.0;					//*********************8
  SP_catch_area_l         = 2.0;					//********************
  SP_catch_area_w         = 1.0;					//*******************	
  SP_catch_ban_cycle      = 5;
  SP_max_power            = 100;					//********
  SP_min_power            = -100;					//Change:AI	
  SP_max_moment           = 180;					//***************
  SP_min_moment           = -180;					//********************
  SP_min_neck_angle       = -90.0;					//*******************
  SP_max_neck_angle       = 90.0;					//*****************
  SP_min_neck_moment      = -180.0;					//****************
  SP_max_neck_moment      = 180.0;					//*************
  SP_visible_angle        = 90.0;					//***********************
  SP_audio_cut_dist       = 50.0;	
  SP_dist_qstep           = 0.1;	
  SP_land_qstep           = 0.01;	
  SP_ckmargin             = 1.0;					//*******************
  SP_wind_dir             = 0.0;					//********************
  SP_wind_force           = 0.0;					//********************
  SP_wind_rand            = 0.0;					//*******************	
  SP_wind_none            = FALSE;					//*************88
  SP_wind_random          = FALSE;					//
  SP_half_time            = 300;
  SP_port                 = 6000;
  SP_coach_port           = 6001;
  SP_olcoach_port         = 6002;
  SP_simulator_step       = 100;
  SP_send_step            = 150;
  SP_recv_step            = 10;
  SP_say_msg_size         = 10;//change by AI
  SP_hear_max             = 2;
  SP_hear_inc             = 1;
  SP_hear_decay           = 2;
  SP_coach_mode           = FALSE;
  SP_coach_w_referee_mode = FALSE;
  SP_say_coach_cnt_max    = 128;
  SP_say_coach_msg_size   = 128;
  SP_send_vi_step         = 100;
  SP_look_step            = 100;
  SP_use_offside          = FALSE;
  SP_forbid_kickoff_offside = TRUE;
  SP_verbose              = TRUE;
  SP_offside_area         = 9.15;
  SP_inertia_moment       = 5.0;				//************
  SP_sense_body_step      = 100;
  SP_offside_kick_margin  = 9.15;
  SP_record_messages      = FALSE;
  SP_clang_win_size		  =	300;
  SP_clang_define_win     = 1;
  SP_clang_meta_win       = 1;
  SP_clang_advice_win     = 1;
  SP_clang_info_win       = 1;
  SP_clang_mess_delay     = 50;
  SP_clang_mess_per_cycle = 1;
  SP_player_accel_max     = 1.0;
  SP_log_times            = FALSE;
  SP_goalie_max_moves     = 2;
  SP_baccel_max           = 2.7;
  SP_kick_rand            = 0.0;
  SP_slowness_on_top_for_left_team = 1.0;
  SP_slowness_on_top_for_left_team = 1.0;
  SP_synch_mode = FALSE;
  SP_synch_micro_sleep = 1;
  SP_ball_accel_max   =2.7;
  SP_drop_ball_time =200;
  SP_game_logging=TRUE;
  SP_send_comms=TRUE;
  SP_text_logging=TRUE;
  SP_game_log_version=3;
  sprintf(SP_text_log_dir,"./");
  sprintf(SP_game_log_dir,"./");
  sprintf(SP_text_log_fixed_name,"rcssserver");
  sprintf(SP_game_log_fixed_name,"rcssserver");
  SP_text_log_fixed=FALSE;
  SP_game_log_fixed=FALSE;
  SP_text_log_dated=TRUE;
  SP_game_log_dated=TRUE;
  //  sprintf(SP_log_date_format,"%Y%m%d%H%M-");
  SP_text_log_compression=0;
  SP_game_log_compression=0;
  sprintf(SP_landmark_file,"Ohh");
  //new
  SP_recover_init=0 ;
  SP_effort_init=0 ;
  SP_team_actuator_noise=0 ;
  SP_prand_factor_l=1 ;
  SP_prand_factor_r=1 ;
  SP_kick_rand_factor_l=1 ;
  SP_kick_rand_factor_r =1 ;
  SP_control_radius=1 ;
  SP_control_radius_width=1.7 ;
  SP_quantize_step_dir=-1 ;
  SP_quantize_step_dist_team_l=-1 ;
  SP_quantize_step_dist_team_r=-1 ;
  SP_quantize_step_dist_l_team_r=-1 ;
  SP_quantize_step_dist_l_team_l=-1 ;
  SP_quantize_step_dir_team_l=-1 ;
  SP_quantize_step_dir_team_r=-1 ;
  SP_wind_ang= 0;
  SP_lcm_step=300 ;
  SP_old_coach_hear=0 ;
  SP_slow_down_factor=1 ;
  SP_synch_offset=60 ;
  SP_start_goal_l=0 ;
  SP_start_goal_r=0 ;
  SP_fullstate_l=0 ;
  SP_fullstate_r=0 ;
  SP_profile=0 ;
  SP_point_to_ban=5 ;
  SP_point_to_duration=20 ;
  SP_tackle_dist=2.5 ;
  SP_tackle_back_dist=0.5 ;
  SP_tackle_width=1.25 ;
  SP_tackle_exponent=6 ;
  SP_tackle_cycles=10 ;
  SP_clang_rule_win=1;
  SP_clang_del_win=1;
  SP_freeform_send_period  =20;
  SP_freeform_wait_period  =600;
  SP_max_goal_kicks     = 3;
  SP_back_passes        =TRUE;
  SP_free_kick_faults   =TRUE;
  SP_proper_goal_kicks  =FALSE;
  SP_stopped_ball_vel   =0.01;
  SP_tackle_power_rate   =0.027;
  //AI: new at 9.0 and higher
  SP_connect_wait=300;
  SP_game_over_wait=100;
  SP_keepaway_start=-1;
  SP_kick_off_wait=100;
  SP_nr_extra_halfs=2;
  SP_nr_normal_halfs=2;
  SP_pen_before_setup_wait=30;
  SP_pen_setup_wait=100;
  SP_pen_ready_wait=50;
  SP_pen_taken_wait=100;
  SP_pen_nr_kicks=5;
  SP_pen_max_extra_kicks=10;
  sprintf(SP_keepaway_log_dir,"./");
  sprintf(SP_keepaway_log_fixed_name,"rcssserver");
  sprintf(SP_team_l_start," ");
  sprintf(SP_team_r_start," ");
  SP_auto_mode=FALSE;
  SP_keepaway=FALSE;
  SP_keepaway_width=20.0f;
  SP_keepaway_length=20.0f;
  SP_keepaway_logging=TRUE;
  SP_keepaway_log_fixed=FALSE;
  SP_keepaway_log_dated=TRUE;
  SP_pen_allow_mult_kicks=FALSE;
  SP_pen_random_winner=FALSE;
  SP_penalty_shoot_outs=TRUE;
  SP_pen_dist_x=11.0f;
  SP_pen_max_goalie_dist_x=4.0f;
  //AI: hetro players parametrs
  SP_player_types=7;
  SP_pt_max=3;
  SP_random_seed=-1.0;
  SP_subs_max=3;
  SP_dash_power_rate_delta_max=0;
  SP_dash_power_rate_delta_min=0;
  SP_effort_max_delta_factor=-0.002;
  SP_effort_min_delta_factor=-0.002;
  SP_extra_stamina_delta_max=100;
  SP_extra_stamina_delta_min=0;
  SP_inertia_moment_delta_factor=25;
  SP_kick_rand_delta_factor=0.5;
  SP_kickable_margin_delta_max=0.2;
  SP_kickable_margin_delta_min=0;
  SP_new_dash_power_rate_delta_max=0.002;
  SP_new_dash_power_rate_delta_min=0;
  SP_new_stamina_inc_max_delta_factor=-10000;
  SP_player_decay_delta_max=0.2;
  SP_player_decay_delta_min=0;
  SP_player_size_delta_factor=-100;
  SP_player_speed_max_delta_max=0;
  SP_player_speed_max_delta_min=0;
  SP_stamina_inc_max_delta_factor=0;
  sprintf(SP_module_dir," ");
  SP_pen_coach_moves_players=TRUE;

}

void OptionInfo::GetOptions(int argc, char **argv)
{
  option_t opt[] = {

    {"alarm_interval",          (void *)&CP_alarm_interval,         V_INT},
    {"max_cycles_since_io",     (void *)&CP_max_cycles_since_io,      V_INT},
	
    {"analyze_log",             (void *)&CP_analyze_log,            V_ONOFF},

    {"coach_setplays",          (void *)&CP_coach_setplays,         V_ONOFF},
    {"team_name",               (void *)&CP_team_name,              V_STRING},
    {"cycles_to_store",         (void *)&MP_cycles_to_store,        V_INT},
    {"save_log",                (void *)&MP_save_log,               V_ONOFF},
    {"left_team_name",          (void *)&MP_left_team_name,         V_STRING},
    {"right_team_name",         (void *)&MP_right_team_name,        V_STRING},
    {"analize_types_from_cin",  (void*)&MP_analize_types_from_cin,  V_INT},
#include<serveroptions.h>	
    {"\0",				NULL, 			       	0}
  } ;

  /* skip command name */
  argv++ ; argc-- ;

  /* first, search option '-file' */
  int i ;
  FILE *fp ;
  for(i = 0 ; i < argc ; i++) {
    if (!strcmp(*(argv + i),"-file")) {
      if ((fp = fopen(*(argv+i+1),"r")) == NULL) {
	cerr << "can't open config file " << *(argv+i+1) << endl ;
	break ;
      }

      char buf[100] ;
      while(fgets(buf,100,fp) != NULL) {
	/* ignore remark line */
	if (buf[0] == '#' || buf[0] == '\n')
	  continue ;

	/* replace from ':' to ' ' */
	char *t = buf ;
	while(*t != NULLCHAR) {
	  if (*t == ':') *t = ' ' ;
	  t++ ;
	}

	int n, p ;
	char com[256] ;
	char onoff[16] ;
	n = sscanf(buf,"%s", com) ;
	if (n < 1) {
	  cerr << "Illegal line : " << buf ;
	  continue ;
	}

	for (p = 0 ; opt[p].vptr != NULL ; p++) {
	  if (strcmp(com, opt[p].optname))
	    continue ;

	  /* match */
	  switch(opt[p].vsize) {
	  case V_INT:
	    n = sscanf(buf, "%s %d", com, (int *)opt[p].vptr) ;
	    break ;

	  case V_STRING:
	    n = sscanf(buf, "%s %s", com, (char *)opt[p].vptr) ;
	    break ;

	  case V_FLOAT:
	    n = sscanf(buf, "%s %f", com, (float *)opt[p].vptr) ;
	    break ;

	  case V_BOOL:
	    n = 2 ;
	    *((Bool *)opt[p].vptr) = TRUE ;
	    break ;

	  case V_ONOFF:
	    n = sscanf(buf, "%s %s", com, onoff) ;
	    if (n == 1) {
	      n = 2;
	      *((Bool *)opt[p].vptr) = TRUE;
	    } else
	      *((Bool *)opt[p].vptr) = (!strcmp(onoff, "on")) ? TRUE :FALSE;
	    break ;
	  }

	  if (n < 2)
	    cerr << "Illegal line (" << com << ") " << endl ;

	  break ;
	}
				
	if (opt[p].vptr == NULL)
	  cerr << "Illegal line (" << com << ") " << endl ;
      }

      fclose(fp) ;
      /* break ; */   // Without this, more than one file can be read
    }
  }

  /* next, analyze command line option */
  int p ;

  while (argc) {
    if (!strcmp(*argv, "-file")) {
      argv += 2 ;
      argc -= 2 ;
      continue ;
    }
			
    for (p = 0 ; opt[p].vptr != NULL ; p++) {
      if (strcmp(*argv + 1, opt[p].optname))
	continue ;

      /* match */
      argv++ ;
      argc-- ;

      switch(opt[p].vsize) {
      case V_INT:
	*((int *)opt[p].vptr) = atoi(*argv) ;
	break ;

      case V_STRING:
	strcpy((char *)opt[p].vptr, *argv) ;
	break ;

      case V_FLOAT:
	*((float *)opt[p].vptr) = atof(*argv) ;
	break ;

      case V_BOOL:
	*((Bool *)opt[p].vptr) = TRUE ;
	argv-- ;
	argc++ ;
	break ;

      case V_ONOFF:
	if (argc > 0 && (*argv)[0] != '-') {	  
	  *((Bool *)opt[p].vptr) = (!strcmp(*argv, "on")) ? TRUE : FALSE ;
	} else {
	  /* if there's nothing specified, then we set it to true */
	  *((Bool *)opt[p].vptr) = TRUE;
	  argv-- ;
	  argc++ ;
	}
	break ;
      }

      break ;
    }

    if (opt[p].vptr == NULL)
      cerr << "Unrecognized Option : " << *argv << endl ;

    argv++ ;
    argc-- ;
  }

  SP_half_time = SP_half_time * 1000 / SP_simulator_step ;
  SP_kickable_area = SP_kickable_margin + SP_ball_size + SP_player_size ;
  SP_hetro_player_type=0;
}
//~////////////////////////////////////////////////////////

/* explode the line into argc and argv */
void OptionInfo::GetOptions(char* line) 
{
  const int MAXOPT = 100;
  char* argv[MAXOPT];
  int argc = 1; /* executable name */
  char* pc;

  advance_past_space(&line);
  while (*line != 0) {
    pc = line;
    get_token(&line);
    argv[argc] = new char[line-pc+1];
    strncpy(argv[argc], pc, line-pc);
    argv[argc][line-pc] = 0; /* null terminate */
    argc++;
    advance_past_space(&line);
  }

  argv[argc] = NULL;

  GetOptions(argc, argv);

  for (int i = 1; i<argc; i++)
    delete [] argv[i];
}

//////////////////////////////////////////////////////////////////////////
void OptionInfo::ParseParamsFromServer(char *Info,option_t* opt){
  int p;
  Info+=13;

  while (*Info!=')')
    {
      char buf[50];
      int i=0;
      while (*Info==' ' || *Info=='(')  Info++;
      while (*Info!=' ')  buf[i++]=*(Info++);
      buf[i]=0;

      for (p = 0 ; opt[p].vptr != NULL ; p++)
	{
  	
	  if (strcmp(buf, opt[p].optname))
	    continue ;

	  advance_past_space(&Info);
	  switch(opt[p].vsize)
	    {
	    case V_INT:
	      *(int *)opt[p].vptr=get_int (&Info) ;	
	      break ;

	    case V_STRING:
	      char* temp;
	      temp=(char *)opt[p].vptr;
	      while (*Info!=')')  *(temp++)=*(Info++);
	      *temp=0;
	      break ;

	    case V_FLOAT:
	      *(float *)opt[p].vptr=get_float (&Info) ;
	      break ;

	    case V_ONOFF:
	    case V_BOOL:
	
	      *((Bool *)opt[p].vptr) = (get_int (&Info)==1? TRUE : FALSE); ;
	      break ;
	    } //switch
	  break;
	} //for
      if (opt[p].vptr == NULL)
	cout<< "Unknown param from server "<<buf<<endl;
      advance_to (')',&Info);
      Info++;
    } //while	
}
/////////////////////////////////////////////////////////////////////////

void OptionInfo::Parse_Server_Param_message(char *ServerInfo)
{
  option_t opt[] = {
#include "serveroptions.h"
    {"\0",NULL,0}
  };
  ParseParamsFromServer(ServerInfo,opt);
}

//------------------------------------------------------------------------//
void OptionInfo::Parse_Player_Param_message(char *PlayerInfo)
{
  option_t opt[] = {
    {"player_types",(void*)&SP_player_types,V_INT},
    {"pt_max",(void*)&SP_pt_max,V_INT},
    {"random_seed",(void*)&SP_random_seed,V_FLOAT},
    {"subs_max",(void*)&SP_subs_max,V_INT},
    {"dash_power_rate_delta_max",(void*)&SP_dash_power_rate_delta_max,V_FLOAT},
    {"dash_power_rate_delta_min",(void*)&SP_dash_power_rate_delta_min,V_FLOAT},
    {"effort_max_delta_factor",(void*)&SP_effort_max_delta_factor,V_FLOAT},
    {"effort_min_delta_factor",(void*)&SP_effort_min_delta_factor,V_FLOAT},
    {"extra_stamina_delta_max",(void*)&SP_extra_stamina_delta_max,V_FLOAT},
    {"extra_stamina_delta_min",(void*)&SP_extra_stamina_delta_min,V_FLOAT},
    {"inertia_moment_delta_factor",(void*)&SP_inertia_moment_delta_factor,V_FLOAT},
    {"kick_rand_delta_factor",(void*)&SP_kick_rand_delta_factor,V_FLOAT},
    {"kickable_margin_delta_max",(void*)&SP_kickable_margin_delta_max,V_FLOAT},
    {"kickable_margin_delta_min",(void*)&SP_kickable_margin_delta_min,V_FLOAT},
    {"new_dash_power_rate_delta_max",(void*)&SP_new_dash_power_rate_delta_max,V_FLOAT},
    {"new_dash_power_rate_delta_min",(void*)&SP_new_dash_power_rate_delta_min,V_FLOAT},
    {"new_stamina_inc_max_delta_factor",(void*)&SP_new_stamina_inc_max_delta_factor,V_FLOAT},
    {"player_decay_delta_max",(void*)&SP_player_decay_delta_max,V_FLOAT},
    {"player_decay_delta_min",(void*)&SP_player_decay_delta_min,V_FLOAT},
    {"player_size_delta_factor",(void*)&SP_player_size_delta_factor,V_FLOAT},
    {"player_speed_max_delta_max",(void*)&SP_player_speed_max_delta_max,V_FLOAT},
    {"player_speed_max_delta_min",(void*)&SP_player_speed_max_delta_min,V_FLOAT},
    {"stamina_inc_max_delta_factor",(void*)&SP_stamina_inc_max_delta_factor,V_FLOAT},
    {"\0",NULL,0}
  };
  ParseParamsFromServer(PlayerInfo,opt);
}
////////////////////////////////////////////////////////////////////////////////
void OptionInfo::Parse_Player_Type_message(char *PlayerType)
{
  char* res=strstr(PlayerType,"id");
  if(res==0){
    cout<<"Parse_Player_Type_message: i`m not found id"<<endl;
    return;
  }
  if(SP_hetro_player_type==0){
    SP_hetro_player_type=new HPlayerType[SP_player_types];		
    for(int i=0;i<SP_player_types;i++){
      HPlayerType hpt;
      hpt.Initialize(SP_player_speed_max,SP_stamina_inc,SP_player_decay,SP_inertia_moment,SP_dash_power_rate,
		     SP_player_size,SP_kickable_margin,SP_kick_rand,0,1.0,SP_effort_min);
      SP_hetro_player_type[i]=hpt;
    }
  }
  res++;//space
  int id=get_int(&res);
  option_t opt[]={
    {"id",(void*)&id,V_INT},//from avoid error msg
    {"player_speed_max",(void*)&SP_hetro_player_type[id].GetPlayerSpeedMax(),V_FLOAT},
    {"stamina_inc_max",(void*)&SP_hetro_player_type[id].GetStaminaIncMax(),V_FLOAT},
    {"player_decay",(void*)&SP_hetro_player_type[id].GetPlayerDecay(),V_FLOAT},
    {"inertia_moment",(void*)&SP_hetro_player_type[id].GetInertiaMoment(),V_FLOAT},
    {"dash_power_rate",(void*)&SP_hetro_player_type[id].GetDashPowerRate(),V_FLOAT},
    {"player_size",(void*)&SP_hetro_player_type[id].GetPlayerSize(),V_FLOAT},
    {"kickable_margin",(void*)&SP_hetro_player_type[id].GetKickableMargin(),V_FLOAT},
    {"kick_rand",(void*)&SP_hetro_player_type[id].GetKickRand(),V_FLOAT},
    {"extra_stamina",(void*)&SP_hetro_player_type[id].GetExtraStamina(),V_FLOAT},
    {"effort_max",(void*)&SP_hetro_player_type[id].GetEffortMax(),V_FLOAT},
    {"effort_min",(void*)&SP_hetro_player_type[id].GetEffortMin(),V_FLOAT},
    {"\0",NULL,0}
  };
  ParseParamsFromServer(PlayerType,opt);
}
