/*************************************************************************
 *
 *    DESCRIPTION :    
 *
 *    FILE 	 : MemOption.h
 *
 *    AUTHOR     : Anton Ivanov
 *
 *    $Revision: 1.3 $
 *
 *    $Id: MemOption.h,v 1.3 2004/04/19 08:01:38 anton Exp $
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

/* coach/MemOption.h
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

#ifndef _OPTIONS_H_
#define _OPTIONS_H_
#include <iostream>
#include "types.h"
#include "utils.h"

struct option_t {
  char	optname[50] ;
  void	*vptr ;
  int	vsize ;
};

enum InputType{
  V_INT,
  V_FLOAT,
  V_BOOL,
  V_STRING,	
  V_ONOFF,		
  V_NONE		
};

#define PROGRAM_NAME "coach"


#define MAX_TEAMNAME_LEN  20
#define MAX_HOST_LEN      50
#define MAX_FILE_LEN      256
#define MAX_FP_LEN        20
#define MAX_TREE_STEM_LEN 50
#define MAX_ADVCLASS_NAME_LEN 50

/* Things to be read at startup that never change */
//add by AI
class HPlayerType{
public:
  HPlayerType():player_speed_max(0.0),stamina_inc_max(0),player_decay(0),inertia_moment(0),dash_power_rate(0),player_size(0),kickable_margin(0),
		kick_rand(0),extra_stamina(0),effort_max(0),effort_min(0){}
  void Initialize(float psm,float sim,float pd,float im,float dpr,float ps,float km,float kr,float es,float emax,float emin){
    player_speed_max=psm;
    stamina_inc_max=sim;
    player_decay=pd;
    inertia_moment=im;
    dash_power_rate=dpr;
    player_size=ps;
    kickable_margin=km;
    kick_rand=kr;
    extra_stamina=es;
    effort_max=emax;
    effort_min=emin;
  }
  void Log();
  float& GetPlayerSpeedMax(){return player_speed_max;}
  float& GetStaminaIncMax(){return stamina_inc_max;}
  float& GetPlayerDecay(){return player_decay;}
  float& GetInertiaMoment(){return inertia_moment;}
  float& GetDashPowerRate(){return dash_power_rate;}
  float& GetPlayerSize(){return player_size;}
  float& GetKickableMargin(){return kickable_margin;}
  float& GetKickRand(){return kick_rand;}
  float& GetExtraStamina(){return extra_stamina;}
  float& GetEffortMax(){return effort_max;}
  float& GetEffortMin(){return effort_min;}
		
private:
  float player_speed_max;
  float stamina_inc_max;
  float player_decay;
  float inertia_moment;
  float dash_power_rate;
  float player_size;
  float kickable_margin;
  float kick_rand;
  float extra_stamina;
  float effort_max;
  float effort_min;
};
//~//////////////////////////////////////////////////////////
class OptionInfo {
public:
  OptionInfo();
  ~OptionInfo();
  void GetOptions(int, char**);
  void GetOptions(char*);
	
  void ParseParamsFromServer(char* info,option_t* msg);
  void Parse_Server_Param_message(char *ServerInfo);
  void Parse_Player_Param_message(char *PlayerInfo);
  void Parse_Player_Type_message(char *PlayerType);

  /* coach params */
  int CP_alarm_interval; /* in milliseconds */
  int CP_max_cycles_since_io;

  char CP_team_name[MAX_TEAMNAME_LEN];
  
  Bool CP_coach_setplays;


  Bool CP_analyze_log; /* will load a log file. If false, connects to server */
  
  int MP_cycles_to_store;
  Bool MP_save_log;
  char MP_left_team_name[MAX_TEAMNAME_LEN];
  char MP_right_team_name[MAX_TEAMNAME_LEN];
  int MP_analize_types_from_cin;

  /* Server params */
  float SP_pitch_length;
  float SP_pitch_width;
  float SP_pitch_margin;
  float SP_penalty_area_length;
  float SP_penalty_area_width;
  float SP_goal_area_length;
  float SP_goal_area_width;
  float SP_penalty_spot_dist;
  float SP_corner_arc_r;
  float SP_free_kick_buffer;
  int   SP_after_goal_wait;
  float SP_feel_distance;
  int   SP_num_lines;
  int   SP_num_markers;
  float SP_unum_far_length;
  float SP_unum_too_far_length;
  float SP_team_far_length;
  float SP_team_too_far_length;

  float SP_version;
  int   SP_team_size;
  int   SP_half;
  char  SP_host[MAX_HOST_LEN];
  float SP_goal_width;       
  float SP_player_size;	
  float SP_player_decay;    	
  float SP_player_rand;	
  float SP_player_weight;	
  float SP_player_speed_max;	
  float SP_stamina_max;	
  float SP_stamina_inc;	
  float SP_recover_dec_thr;	
  float SP_recover_min; 	
  float SP_recover_dec; 	
  float SP_effort_dec_thr;	
  float SP_effort_min;	
  float SP_effort_dec;	
  float SP_effort_inc_thr;	
  float SP_effort_inc;	
  float SP_ball_size;		
  float SP_ball_decay;	
  float SP_ball_rand;		
  float SP_ball_weight;	
  float SP_ball_speed_max;	
  float SP_dash_power_rate;	
  float SP_kick_power_rate;	
  float SP_kickable_margin;	
  float SP_kickable_area;	
  float SP_catch_prob;	
  float SP_catch_area_l;	
  float SP_catch_area_w;	
  float SP_max_power;		
  float SP_min_power;		
  float SP_max_moment;		
  float SP_min_moment;		
  float SP_max_neck_angle;
  float SP_min_neck_angle;
  float SP_max_neck_moment;
  float SP_min_neck_moment;
  float SP_visible_angle;	
  float SP_visible_dist;	
  float SP_audio_cut_dist;	
  float SP_dist_qstep;	
  float SP_land_qstep;	
  float SP_ckmargin;	
  float SP_wind_dir;	
  float SP_wind_force;	
  float SP_wind_rand;	
  Bool SP_wind_none;
  Bool SP_wind_random;
  int SP_half_time;
  int SP_port;
  int SP_coach_port;
  int SP_olcoach_port;
  int SP_simulator_step;
  int SP_send_step;
  int SP_recv_step;
  int SP_say_msg_size;
  int SP_hear_max;
  int SP_hear_inc;
  int SP_hear_decay;
  int SP_catch_ban_cycle;
  Bool SP_coach_mode;
  Bool SP_coach_w_referee_mode;
  int SP_say_coach_cnt_max;	
  int SP_say_coach_msg_size;
  int SP_send_vi_step;
  int SP_look_step;
  float SP_player_accel_max;
  Bool SP_log_times;
  int SP_goalie_max_moves;
  float SP_baccel_max;
  float SP_kick_rand;
  //new messages from trainer
  int SP_clang_win_size;
  int SP_clang_define_win;
  int SP_clang_meta_win;
  int SP_clang_advice_win;
  int SP_clang_info_win;
  int SP_clang_mess_delay;
  int SP_clang_mess_per_cycle;
  //new messages
  float SP_slowness_on_top_for_left_team;
  float SP_slowness_on_top_for_right_team;
  Bool SP_synch_mode;
  int SP_synch_micro_sleep;
  float SP_ball_accel_max;
  int SP_drop_ball_time;

  Bool SP_use_offside;
  Bool SP_forbid_kickoff_offside;
  char SP_logfile[MAX_FILE_LEN];
  char SP_recfile[MAX_FILE_LEN];
  Bool SP_rec_log;
  int  SP_rec_ver;
  char SP_replay[MAX_FILE_LEN];
  Bool SP_verbose;
  Bool SP_send_log;
  float SP_offside_area;
  float SP_inertia_moment;
  int   SP_sense_body_step;
  float SP_offside_kick_margin;
  Bool  SP_record_messages;
  Bool SP_game_logging;
  Bool SP_send_comms;
  Bool SP_text_logging;
  int  SP_game_log_version;
  char SP_text_log_dir[256];
  char SP_game_log_dir[256];
  char SP_text_log_fixed_name[256];
  char SP_game_log_fixed_name[256];
  Bool SP_text_log_fixed;
  Bool SP_game_log_fixed;
  Bool SP_text_log_dated;
  Bool SP_game_log_dated;
  char SP_log_date_format[256];
  int SP_text_log_compression;
  int SP_game_log_compression;

  float SP_recover_init;
  float SP_effort_init;
  float SP_team_actuator_noise;
  float SP_prand_factor_l;
  float SP_prand_factor_r;
  float SP_kick_rand_factor_l;
  float SP_kick_rand_factor_r ;
  float SP_control_radius;
  float SP_control_radius_width;
  float SP_quantize_step_dir;
  float SP_quantize_step_dist_team_l;
  float SP_quantize_step_dist_team_r;
  float SP_quantize_step_dist_l_team_r;
  float SP_quantize_step_dist_l_team_l;
  float SP_quantize_step_dir_team_l;
  float SP_quantize_step_dir_team_r;
  float SP_wind_ang;
	
  float SP_lcm_step;
  int SP_old_coach_hear;
  float SP_slow_down_factor;
  float SP_synch_offset;
  float SP_start_goal_l;
  float SP_start_goal_r;
  float SP_fullstate_l;
  float SP_fullstate_r;
  float SP_profile;
  int SP_point_to_ban;
  int SP_point_to_duration;
  float SP_tackle_dist;
  float SP_tackle_back_dist;
  float SP_tackle_width;
  int SP_tackle_exponent;
  int SP_tackle_cycles;
	
  int SP_clang_rule_win;
  int SP_clang_del_win;
  int SP_freeform_send_period;
  int SP_freeform_wait_period;
  int SP_max_goal_kicks;
  Bool SP_back_passes;
  Bool SP_free_kick_faults;
  Bool SP_proper_goal_kicks;
  float SP_stopped_ball_vel;
  float SP_tackle_power_rate;
  char SP_landmark_file[256];
  //AI: new at 9.0 and higher
  int SP_connect_wait;
  int SP_game_over_wait;
  int SP_keepaway_start;
  int SP_kick_off_wait;
  int SP_nr_extra_halfs;
  int SP_nr_normal_halfs;
  int SP_pen_before_setup_wait;
  int SP_pen_setup_wait;
  int SP_pen_ready_wait;
  int SP_pen_taken_wait;
  int SP_pen_nr_kicks;
  int SP_pen_max_extra_kicks;
  char SP_keepaway_log_dir[256];
  char SP_keepaway_log_fixed_name[256];
  char SP_team_l_start[256];
  char SP_team_r_start[256];
  Bool SP_auto_mode;
  Bool SP_keepaway;
  float SP_keepaway_width;
  float SP_keepaway_length;
  Bool SP_keepaway_logging;
  Bool SP_keepaway_log_fixed;
  Bool SP_keepaway_log_dated;
  Bool SP_pen_allow_mult_kicks;
  Bool SP_pen_random_winner;
  Bool SP_penalty_shoot_outs;
  float SP_pen_dist_x;
  float SP_pen_max_goalie_dist_x;
  char SP_module_dir[256];
  Bool SP_pen_coach_moves_players;
  //AI: parametres for hetro players
  int SP_player_types;
  int SP_pt_max;
  float SP_random_seed;
  int SP_subs_max;//maximum of substitution
  float SP_dash_power_rate_delta_max;
  float SP_dash_power_rate_delta_min;
  float SP_effort_max_delta_factor;
  float SP_effort_min_delta_factor;
  float SP_extra_stamina_delta_max;
  float SP_extra_stamina_delta_min;
  float SP_inertia_moment_delta_factor;
  float SP_kick_rand_delta_factor;
  float SP_kickable_margin_delta_max;
  float SP_kickable_margin_delta_min;
  float SP_new_dash_power_rate_delta_max;
  float SP_new_dash_power_rate_delta_min;
  float SP_new_stamina_inc_max_delta_factor;
  float SP_player_decay_delta_max;
  float SP_player_decay_delta_min;
  float SP_player_size_delta_factor;
  float SP_player_speed_max_delta_max;
  float SP_player_speed_max_delta_min;
  float SP_stamina_inc_max_delta_factor;
	
  HPlayerType* SP_hetro_player_type;
};

#endif
