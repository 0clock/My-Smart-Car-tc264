#ifndef _FUZZY_PID_H_
#define _FUZZY_PID_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "math.h"
#include "stdlib.h"
#include "stdio.h"

#ifndef bool
#define bool char
#endif

#ifndef false
#define false (char)0
#endif

#ifndef true
#define true (char)1
#endif


// Fuzzy quantity fields 模糊论域
enum quantity_fields {
    qf_small = 5,
    qf_middle = 7, //NB NM NS ZO PS PM PB
    qf_large = 8
};

#define qf_default qf_middle

struct fuzzy {
    unsigned int input_num;
    unsigned int output_num;
    unsigned int fo_type;
    unsigned int *mf_type;
    int *mf_params;
    unsigned int df_type;
    int *rule_base;
    float *output;
};

struct PID {
    float kp;
    float ki;
    float kd;

    float delta_kp_max;
    float delta_ki_max;
    float delta_kd_max;

    float delta_kp;
    float delta_ki;
    float delta_kd;

    float error_max;
    float delta_error_max;

    float last_error;
    float current_error;

    float intergral;
    float intergral_limit;

    float dead_zone;
    float feed_forward;

    float output;

    int output_min_value;
    int output_middle_value;
    int output_max_value;

    float linear_adaptive_kp;

    struct fuzzy *fuzzy_struct;
};

extern struct PID **pid_vector;

#define NB -3
#define NM -2
#define NS -1
#define ZO 0
#define PS 1
#define PM 2
#define PB 3

//#define pid_debug_print
//#define pid_dead_zone
//#define pid_integral_limit

//#define fuzzy_pid_debug_print
//#define fuzzy_pid_dead_zone
//#define fuzzy_pid_integral_limit
//#define fuzzy_pid_rule_base_deep_copy

#define pid_params_count 7
#define pid_params_count_full 9

#define torque_mode 1
#define position_mode 2
#define control_mode position_mode

#if control_mode == position_mode
#define max_error 3.0f //误差e最大值 需要根据实际测量结果更改**
#define max_delta_error .4f //误差e的微分的最大值 需要根据实际测量结果更改**
#else
#define max_error 12.0f
#define max_delta_error 12.0f
#endif

#define min_pwm_output 0
#define middle_pwm_output 500
#define max_pwm_output 1000

void My_Init_FuzzyPID_Speed(void);
struct fuzzy *fuzzy_init(unsigned int input_num, unsigned int output_num);

void fuzzy_params_init(struct fuzzy *fuzzy_struct, unsigned int mf_type, unsigned int fo_type, unsigned int df_type,
                       int mf_params[], int rule_base[][qf_default]);

void fuzzy_control(float e, float de, struct fuzzy *fuzzy_struct);

struct PID *raw_pid_init(float kp, float ki, float kd, float integral_limit, float dead_zone,
                         float feed_forward, float linear_adaptive_kp, float error_max, float delta_error_max,
                         int output_min_value, int output_middle_value, int output_max_value);

struct PID *raw_fuzzy_pid_init(float kp, float ki, float kd, float integral_limit, float dead_zone,
                               float feed_forward, float error_max, float delta_error_max, float delta_kp_max,
                               float delta_ki_max, float delta_kd_max, unsigned int mf_type, unsigned int fo_type,
                               unsigned int df_type, int *mf_params, int rule_base[][qf_default],
                               int output_min_value, int output_middle_value, int output_max_value);

//float params[pid_params_count] = {kp, ki, kd, integral_limit, dead_zonefeed_forward, linear_adaptive_kp};
struct PID *pid_init(float *params);

struct PID *fuzzy_pid_init(float *params, float delta_k, unsigned int mf_type, unsigned int fo_type,
                           unsigned int df_type, int mf_params[], int rule_base[][qf_default]);

struct PID **pid_vector_init(float params[][pid_params_count], unsigned int count);

struct PID **
fuzzy_pid_vector_init(float params[][pid_params_count], float delta_k, unsigned int mf_type, unsigned int fo_type,
                      unsigned int df_type, int *mf_params, int rule_base[][qf_default],
                      unsigned int count);
struct PID **
fuzzy_pid_vector_init_full(float params[][pid_params_count_full], float delta_k, unsigned int mf_type, unsigned int fo_type,
                      unsigned int df_type, int *mf_params, int rule_base[][qf_default],
                      unsigned int count);

float pid_control(float real, float idea, struct PID *pid);

float fuzzy_pid_control(float real, float idea, struct PID *pid);

int direct_control(int zero_value, int offset_value, bool direct);

int pid_motor_pwd_output(float real, float idea, bool direct, struct PID *pid);

int fuzzy_pid_motor_pwd_output(float real, float idea, bool direct, struct PID *pid);

void delete_pid(struct PID *pid);

void delete_pid_vector(struct PID **pid_vector, unsigned int count);

#ifdef __cplusplus
}
#endif

#endif //_FUZZY_PID_H_
