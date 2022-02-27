#include "headfile.h"

#include <stdlib.h>
#include "fuzzy_PID.h"

// 隶属度函数(mf) Membership function
// 模糊算子(fo) Fuzzy operator

struct PID **pid_vector;

void My_Init_FuzzyPID_Speed(void)
{
    int rule_base[][qf_default] = {
                //delta kp rule base
                {PB, PB, PM, PM, PS, ZO, ZO},
                {PB, PB, PM, PS, PS, ZO, NS},
                {PM, PM, PM, PS, ZO, NS, NS},
                {PM, PM, PS, ZO, NS, NM, NM},
                {PS, PS, ZO, NS, NS, NM, NM},
                {PS, ZO, NS, NM, NM, NM, NB},
                {ZO, ZO, NM, NM, NM, NB, NB},
                //delta ki rule base
                {NB, NB, NM, NM, NS, ZO, ZO},
                {NB, NB, NM, NS, NS, ZO, ZO},
                {NB, NM, NS, NS, ZO, PS, PS},
                {NM, NM, NS, ZO, PS, PM, PM},
                {NM, NS, ZO, PS, PS, PM, PB},
                {ZO, ZO, PS, PS, PM, PB, PB},
                {ZO, ZO, PS, PM, PM, PB, PB},
                //delta kd rule base
                {PS, NS, NB, NB, NB, NM, PS},
                {PS, NS, NB, NM, NM, NS, ZO},
                {ZO, NS, NM, NM, NS, NS, ZO},
                {ZO, NS, NS, NS, NS, NS, ZO},
                {ZO, ZO, ZO, ZO, ZO, ZO, ZO},
                {PB, PS, PS, PS, PS, PS, PB},
                {PB, PM, PM, PM, PS, PS, PB}};

    // Default parameters of membership function
    int mf_params[4 * qf_default] = {-3, -3, -2, 0,
                                     -3, -2, -1, 0,
                                     -2, -1,  0, 0,
                                     -1,  0,  1, 0,
                                      0,  1,  2, 0,
                                      1,  2,  3, 0,
                                      2,  3,  3, 0};
    
    /*
     * params[0]:Kp
     * params[1]:Ki
     * params[2]:Kd
     * params[3]:integral_limit （Ki部分上限值） 没有启用，可在.h中取消注释启用 （设为0）
     * params[4]：dead_zone （输入的下限值，小于下限值时不）没有启用，可在.h中取消注释启用 （设为0）
     * params[5]：feed_forward 前向反馈 设为1
     * delta_k：两次测量的间隔
     * 以上几个参数需自己设定
     *
     * 以下几个参数已有
     * mf_type： 设为4 即三角形隶属度函数
     * fo_type： 设为1 即并算子
     * df_type： 设为0 即中心平均值法
     * mf_params： 隶属度函数的参数 已有成熟的模糊PID参数
     * rule_base[][qf_default]： 模糊规则 已有成熟的模糊PID参数
     */
    float fuzzy_pid_params[1][pid_params_count_full] = {{7.0f/3.0f,  0.01f, 0.f, 0, 0, 0, 7.0f/3.0f, 0.01f, 0.f}};

    struct PID **subpid_vector = fuzzy_pid_vector_init_full(fuzzy_pid_params, 4.0f, 4, 1, 0, mf_params, rule_base, 1);

    pid_vector=subpid_vector;
    
}


//根据变量个数初始化
//在 struct PID *raw_fuzzy_pid_init 中已调用，无需手动设置
struct fuzzy *fuzzy_init(unsigned int input_num, unsigned int output_num) {
    struct fuzzy *fuzzy_struct = (struct fuzzy *) malloc(sizeof(struct fuzzy));
    fuzzy_struct->input_num = input_num;//输入个数
    fuzzy_struct->output_num = output_num;//输出个数
    fuzzy_struct->mf_type = (unsigned int *) malloc((input_num + output_num) * sizeof(unsigned int));

#ifdef fuzzy_pid_rule_base_deep_copy
    fuzzy_struct->mf_params = (int *) malloc(4 * qf_default * sizeof(int)); //qf_default 论域划分的数量 默认为中等大小 qf_middle 7个
    fuzzy_struct->rule_base = (int *) malloc(output_num * qf_default * qf_default * sizeof(int));
#endif

    fuzzy_struct->output = (float *) malloc(output_num * sizeof(float));//输出
    return fuzzy_struct;
}

void delete_fuzzy(struct fuzzy *fuzzy_struct) {
    free(fuzzy_struct->mf_type); //释放内存空间函数
    free(fuzzy_struct->output);
    free(fuzzy_struct);
}

//参数初始化
//在 struct PID *raw_fuzzy_pid_init 中已调用，无需手动设置
void fuzzy_params_init(struct fuzzy *fuzzy_struct, unsigned int mf_type, unsigned int fo_type, unsigned int df_type,
                       int mf_params[], int rule_base[][qf_default]) {

    for (unsigned int i = 0; i < fuzzy_struct->input_num + fuzzy_struct->output_num; ++i) { // 给所有变量的 隶属度函数 类型赋值
        fuzzy_struct->mf_type[i] = mf_type;
    }

    for (unsigned int i = 0; i < fuzzy_struct->output_num; ++i) { //输出全赋值为0
        fuzzy_struct->output[i] = 0;
    }

#ifdef fuzzy_pid_rule_base_deep_copy
    for (unsigned int j = 0; j < 4 * qf_default; ++j) {
         fuzzy_struct->mf_params[j] = mf_params[j];
     }

     for (unsigned int k = 0; k < fuzzy_struct->output_num * qf_default; ++k) {
         for (unsigned int i = 0; i < qf_default; ++i) {
             fuzzy_struct->rule_base[k * 7 + i] = rule_base[k][i];
         }
     }
#else
    fuzzy_struct->mf_params = mf_params;
    fuzzy_struct->rule_base = (int *) rule_base;
#endif

    fuzzy_struct->fo_type = fo_type;
    fuzzy_struct->df_type = df_type;
}


//浮点数类型的倒数
#define inverse(parameter) 1.0f/(float)parameter


// Gaussian membership function 高斯隶属度函数
float gaussmf(float x, float sigma, float c) {
    return expf(-powf(((x - c) / sigma), 2.0f));
}


// Generalized bell-shaped membership function 广义钟形隶属度函数
float gbellmf(float x, float a, float b, float c) {
    return inverse(1.0f + powf(fabsf((x - c) / a), 2.0f * b));
}

// Sigmoidal membership function S形隶属度函数
float sigmf(float x, float a, float c) {
    return inverse(1.0f + expf(a * (c - x)));
}

// Trapezoidal membership function 梯形隶属度函数
float trapmf(float x, float a, float b, float c, float d) {
    if (x >= a && x < b)
        return (x - a) / (b - a);
    else if (x >= b && x < c)
        return 1.0f;
    else if (x >= c && x <= d)
        return (d - x) / (d - c);
    else return 0.0f;
}

// Triangular membership function 三角形隶属度函数 使用较多
/* x为变量
 * a为三角形起点
 * b为三角形顶点
 * c为三级终点
 */
float trimf(float x, float a, float b, float c) {
    return trapmf(x, a, b, b, c);
}

// Z-shaped membership function Z形隶属度函数
float zmf(float x, float a, float b) {
    if (x <= a)
        return 1.0f;
    else if (x >= a && x <= (a + b) / 2.0f)
        return 1.0f - 2.0f * powf((x - a) / (b - a), 2.0f);
    else if (x >= (a + b) / 2.0f && x < b)
        return 2.0f * powf((x - b) / (b - a), 2.0f);
    else return 0;
}

// Membership function 隶属度函数
/*
 * x为变量
 * mf_type为隶属度函数类型 在以上6种内选取
 * *params 给定隶属度函数的参数
 * 默认为三角形隶属度函数 即 mf_type=4
 */
float mf(float x, unsigned int mf_type, int *params) {
    switch (mf_type) {
        case 0:
            return gaussmf(x, params[0], params[1]);
        case 1:
            return gbellmf(x, params[0], params[1], params[2]);
        case 2:
            return sigmf(x, params[0], params[2]);
        case 3:
            return trapmf(x, params[0], params[1], params[2], params[3]);
        case 5:
            return zmf(x, params[0], params[1]);
        default: // set triangular as default membership function
            return trimf(x, params[0], params[1], params[2]);
    }
}


//3类模糊算子
// Union operator 并算子
float or(float a, float b, unsigned int type) {
    if (type == 1) { // algebraic sum 代数和
        return a + b - a * b;
    } else if (type == 2) { // bounded sum 有界和
        return fminf(1, a + b); //返回两个浮点参数中的较小者
    } else { // fuzzy union 模糊并集
        return fmaxf(a, b); //返回两个浮点参数中的较大者
    }
}

// Intersection operator 交算子
float and(float a, float b, unsigned int type) {
    if (type == 1) { // algebraic product 代数积
        return a * b;
    } else if (type == 2) { // bounded product 边界积
        return fmaxf(0, a + b - 1);
    } else { // fuzzy intersection 模糊交集
        return fminf(a, b);
    }
}

// Equilibrium operator 平衡算子
float equilibrium(float a, float b, float params) {
    return powf(a * b, 1 - params) * powf(1 - (1 - a) * (1 - b), params);
}


// Fuzzy operator 模糊算子调用函数
/*
 * a、b为操作对象
 * type为算子类型
 */
float fo(float a, float b, unsigned int type) {
    if (type < 3)
    {
        return and(a, b, type);
    }
    else if (type < 6)
    {
        return or(a, b, type - 3);
    }
    else
    {
        return equilibrium(a, b, 0.5f);
    }
}

// Mean of centers defuzzifier, only for two input multiple index
//解模糊 中心平均值法
void moc(const float *joint_membership, const unsigned int *index, const unsigned int *count, struct fuzzy *fuzzy_struct) {

    float denominator_count = 0;
    float numerator_count[fuzzy_struct->output_num];
    for (unsigned int l = 0; l < fuzzy_struct->output_num; ++l) {
        numerator_count[l] = 0;
    }

    for (int i = 0; i < count[0]; ++i) {
        for (int j = 0; j < count[1]; ++j) {
            denominator_count += joint_membership[i * count[1] + j];
        }
    }

    for (unsigned int k = 0; k < fuzzy_struct->output_num; ++k) {
        for (unsigned int i = 0; i < count[0]; ++i) {
            for (unsigned int j = 0; j < count[1]; ++j) {
                numerator_count[k] += joint_membership[i * count[1] + j] *
                                      fuzzy_struct->rule_base[k * qf_default * qf_default + index[i] * qf_default +
                                                              index[count[0] + j]];
            }
        }
    }

#ifdef fuzzy_pid_debug_print
    printf("output:\n");
#endif
    for (unsigned int l = 0; l < fuzzy_struct->output_num; ++l) {
        fuzzy_struct->output[l] = numerator_count[l] / denominator_count;
#ifdef fuzzy_pid_debug_print
        printf("%f,%f,%f\n", numerator_count[l], denominator_count, fuzzy_struct->index[l]);
#endif
    }
}



// Defuzzifier 解模糊器
void df(const float *joint_membership, const unsigned int *output, const unsigned int *count, struct fuzzy *fuzzy_struct,
        int df_type)
{
    if(df_type == 0)//中心平均值法
        moc(joint_membership, output, count, fuzzy_struct);
    else {
        printf("Waring: No such of defuzzifier!\n"); //只有中心平均值法
        moc(joint_membership, output, count, fuzzy_struct);
    }
}


/*
 *  e 误差
 *  de 误差的微分
 *  *fuzzy_struct 模糊控制参数结构体
 */
void fuzzy_control(float e, float de, struct fuzzy *fuzzy_struct) {
    float membership[qf_default * 2]; // Store membership
    unsigned int index[qf_default * 2]; // Store the index of each membership
    unsigned int count[2] = {0, 0};

    {
        int j = 0;
        for (int i = 0; i < qf_default; ++i) {
            float temp = mf(e, fuzzy_struct->mf_type[0], fuzzy_struct->mf_params + 4 * i);
            if (temp > 1e-4) { // 变量 误差e 经隶属度函数后得到的值 （注意浮点数 需要> 1e-4
                membership[j] = temp; //将该值存入
                index[j++] = i;
            }
        }

        count[0] = j; //储存 membership 内  变量 误差e 经隶属度函数后得到的非零值的个数

        for (int i = 0; i < qf_default; ++i) {
            float temp = mf(de, fuzzy_struct->mf_type[1], fuzzy_struct->mf_params + 4 * i);
            if (temp > 1e-4) {// 变量 误差e的微分de 经隶属度函数后得到的值
                membership[j] = temp;
                index[j++] = i;
            }
        }

        count[1] = j - count[0]; //储存 membership 内  变量 误差e的微分de 经隶属度函数后得到的非零值的个数
    }

#ifdef fuzzy_pid_debug_print
    printf("membership:\n");
    for (unsigned int k = 0; k < j; ++k) {
        printf("%f\n", membership[k]);
    }

    printf("index:\n");
    for (unsigned int k = 0; k < j; ++k) {
        printf("%d\n", index[k]);
    }

    printf("count:\n");
    for (unsigned int k = 0; k < 2; ++k) {
        printf("%d\n", count[k]);
    }
#endif

    if (count[0] == 0 || count[1] == 0) {
        //若 变量 误差e 经隶属度函数后得到的非零值 或者 变量 误差e的微分de 经隶属度函数后得到的非零值 个数为0
        //说明该变量经隶属度函数后得到的值均为零
        for (unsigned int l = 0; l < fuzzy_struct->output_num; ++l) {
            fuzzy_struct->output[l] = 0; //输出均赋值为0
        }
        return;
    }

    // Joint membership
    float joint_membership[count[0] * count[1]];

    for (int i = 0; i < count[0]; ++i) {
        for (int j = 0; j < count[1]; ++j) {
            joint_membership[i * count[1] + j] = fo(membership[i], membership[count[0] + j], fuzzy_struct->fo_type);
        }
    }

    df(joint_membership, index, count, fuzzy_struct, 0);
}


//以下两函数用于完成fuzzy pid初始化
//第一个函数的输入为具体参数
//第二个函数的输入为一指针
struct PID *raw_fuzzy_pid_init(float kp, float ki, float kd, float integral_limit, float dead_zone,
                               float feed_forward, float error_max, float delta_error_max, float delta_kp_max,
                               float delta_ki_max, float delta_kd_max, unsigned int mf_type, unsigned int fo_type,
                               unsigned int df_type, int mf_params[], int rule_base[][qf_default],
                               int output_min_value, int output_middle_value, int output_max_value) {
    struct PID *pid = (struct PID *) malloc(sizeof(struct PID));
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;

    pid->delta_kp_max = delta_kp_max;
    pid->delta_ki_max = delta_ki_max;
    pid->delta_kd_max = delta_kd_max;

    pid->delta_kp = 0;
    pid->delta_ki = 0;
    pid->delta_kd = 0;

    pid->error_max = error_max;
    pid->delta_error_max = delta_error_max;

    int output_count = 1;
    if (ki > 1e-4) {
        output_count += 1;
        if (kd > 1e-4)
            output_count += 1;
    }

    pid->fuzzy_struct = fuzzy_init(2, output_count); //根据变量个数初始化fuzzy pid（输入个数为2，输出个数根据k是否为非零值决定
    fuzzy_params_init(pid->fuzzy_struct, mf_type, fo_type, df_type, mf_params, rule_base);

    pid->last_error = 0;//上一次误差
    pid->current_error = 0;//这次误差

    ////调参时主要注意以下参数 决定输出的范围？

    pid->intergral = 0;
    pid->intergral_limit = integral_limit;

    pid->dead_zone = dead_zone;
    pid->feed_forward = feed_forward;


    pid->output_max_value = output_max_value;
    pid->output_middle_value = output_middle_value;
    pid->output_min_value = output_min_value;

    return pid;
}


/*
 * params[0]:Kp
 * params[1]:Ki
 * params[2]:Kd
 * params[3]:integral_limit （Ki部分上限值） 没有启用，可在.h中取消注释启用 （设为0）
 * params[4]：dead_zone （输入的下限值，小于下限值时不）没有启用，可在.h中取消注释启用 （设为0）
 * params[5]：feed_forward 前向反馈 没有时设为0
 * delta_k：
 * 以上几个参数需自己设定
 *
 * 以下几个参数已有
 * mf_type： 设为4 即三角形隶属度函数
 * fo_type： 设为1 即并算子
 * df_type： 设为0 即中心平均值法
 * mf_params： 隶属度函数的参数 已有成熟的模糊PID参数
 * rule_base[][qf_default]： 模糊规则 已有成熟的模糊PID参数
 */
struct PID *fuzzy_pid_init(float *params, float delta_k, unsigned int mf_type, unsigned int fo_type,
                           unsigned int df_type, int mf_params[], int rule_base[][qf_default]) {
    return raw_fuzzy_pid_init(params[0], params[1], params[2], params[3], params[4], params[5],
                              max_error,max_delta_error, params[0] / delta_k, params[1] / delta_k, params[2] / delta_k, mf_type,
                              fo_type, df_type, mf_params,
                              rule_base, min_pwm_output, middle_pwm_output, max_pwm_output);
}

struct PID *fuzzy_pid_init_full(float *params, float delta_k, unsigned int mf_type, unsigned int fo_type,
                           unsigned int df_type, int mf_params[], int rule_base[][qf_default]) {
    return raw_fuzzy_pid_init(params[0], params[1], params[2], params[3], params[4], params[5],
                              max_error,max_delta_error, params[6], params[7], params[8] , mf_type,
                              fo_type, df_type, mf_params,
                              rule_base, min_pwm_output, middle_pwm_output, max_pwm_output);
}


//以下两函数用于完成pid初始化
//第一个函数的输入为具体参数
//第二个函数的输入为一指针
struct PID *raw_pid_init(float kp, float ki, float kd, float integral_limit, float dead_zone,
                         float feed_forward, float linear_adaptive_kp, float error_max, float delta_error_max,
                         int output_min_value, int output_middle_value, int output_max_value) {
    struct PID *pid = (struct PID *) malloc(sizeof(struct PID));
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;

    pid->delta_kp_max = 0;
    pid->delta_ki_max = 0;
    pid->delta_kd_max = 0;

    pid->delta_kp = 0;
    pid->delta_ki = 0;
    pid->delta_kd = 0;

    pid->error_max = error_max;
    pid->delta_error_max = delta_error_max;

    pid->fuzzy_struct = NULL;

    pid->last_error = 0;
    pid->current_error = 0;

    pid->intergral = 0;
    pid->intergral_limit = integral_limit;

    pid->dead_zone = dead_zone;
    pid->feed_forward = feed_forward;

    pid->output_max_value = output_max_value;
    pid->output_middle_value = output_middle_value;
    pid->output_min_value = output_min_value;

    pid->linear_adaptive_kp = linear_adaptive_kp;
    return pid;
}

struct PID *pid_init(float *params) {
    return raw_pid_init(params[0], params[1], params[2], params[3], params[4], params[5], params[6], max_error,
                        max_delta_error, min_pwm_output, middle_pwm_output, max_pwm_output);
}


//浮点数的四舍五入
int round_user(float parameter) {
    if ((int) (parameter * 10.0) % 10 >= 5)
        return parameter + 1;
    else
        return parameter;
}

/*
//判断给定值是否在范围内
//不在范围内时取最值
int limit(int value, int max_limit, int min_limit)
{
    if (value > max_limit)
        return max_limit;
    if (value < min_limit)
        return min_limit;
    return value;
}
*/

float limits(float value, float max_limit, float min_limit)
{
    if (value > max_limit)
        return max_limit;
    if (value < min_limit)
        return min_limit;
    return value;
}


//增量式模糊PID控制主函数
/*
 *  real为实际测量值
 *  idea为理想控制值
 *  结构体为PID参数（根据实际情况设定）
 */
float fuzzy_pid_control(float real, float idea, struct PID *pid) {
    pid->last_error = pid->current_error;
    pid->current_error = idea - real;
    float delta_error = pid->current_error - pid->last_error;

#ifdef fuzzy_pid_dead_zone  //没有启用，可在.h中取消注释启用
    if (pid->current_error < pid->dead_zone && pid->current_error > -pid->dead_zone)//误差在设定范围内时约等于0
    {
        pid->current_error = 0;
    }
    else //误差为测得误差减去预设值
    {
        if (pid->current_error > pid->dead_zone)
            pid->current_error = pid->current_error - pid->dead_zone;
        else
        {
            if (pid->current_error < -pid->dead_zone)
                pid->current_error = pid->current_error + pid->dead_zone;
        }
    }
#endif

    //三个变量分别为 误差e 误差e的微分 结构体
    //前两个变量先归一化 再乘系数3（系数3已经由模糊器本身决定）
    fuzzy_control(fabs(pid->current_error / pid->error_max * 3.0f)>3.0f?
                  ((pid->current_error / pid->error_max * 3.0f)>0?3.0f:-3.0f):
                  (pid->current_error / pid->error_max * 3.0f),
                  fabs(delta_error / pid->delta_error_max * 3.0f)>3.0f?
                  ((delta_error / pid->delta_error_max * 3.0f)>0?3.0f:-3.0f):
                  (delta_error / pid->delta_error_max * 3.0f),
                  pid->fuzzy_struct);

    //（由 模糊pid得到的输出 除以系数3归一化  再乘最大值得到实际值）+加上原kp
    pid->delta_kp = pid->fuzzy_struct->output[0] / 3.0f * pid->delta_kp_max;

    //输出个数大于1时 即有Ki时 继续输出
    if (pid->fuzzy_struct->output_num >= 2)
        pid->delta_ki = pid->fuzzy_struct->output[1] / 3.0f * pid->delta_ki_max;
    else pid->delta_ki = 0;

    //输出个数大于2时 即有Kd时 继续输出
    if (pid->fuzzy_struct->output_num >= 3)
        pid->delta_kd = pid->fuzzy_struct->output[2] / 3.0f * pid->delta_kd_max;
    else pid->delta_kd = 0;

#ifdef fuzzy_pid_debug_print
    printf("kp : %f, ki : %f, kd : %f\n", kp, ki, kd);
#endif

    //以下部分计算PID结果
    //计算Ki部分的计算结果
    pid->intergral += (pid->ki + pid->delta_ki) * pid->current_error;
#ifdef fuzzy_pid_integral_limit //对Ki部分乘积做出上限值规定  没有启用，可在.h中取消注释启用
    if (pid->intergral > pid->intergral_limit)
        pid->intergral = pid->intergral_limit;
    else {
        if (pid->intergral < -pid->intergral_limit)
            pid->intergral = -pid->intergral_limit;
    }
#endif

    //将各部分相加
    pid->output = (pid->kp + pid->delta_kp) * pid->current_error + pid->intergral +
                  (pid->kd + pid->delta_kd) * (pid->current_error - pid->last_error);

    //将上一步得到的和经反馈后输出
    pid->output += pid->feed_forward * (float) idea;
    return pid->output;
}


//增量式PID控制主函数
float pid_control(float real, float idea, struct PID *pid) {
    pid->last_error = pid->current_error;
    pid->current_error = idea - real;

#ifdef pid_dead_zone
    if (pid->current_error < pid->dead_zone && pid->current_error > -pid->dead_zone) {
        pid->current_error = 0;
    } else {
        if (pid->current_error > pid->dead_zone)
            pid->current_error = pid->current_error - pid->dead_zone;
        else {
            if (pid->current_error < -pid->dead_zone)
                pid->current_error = pid->current_error + pid->dead_zone;
        }
    }
#endif

#ifdef pid_debug_print
    printf("kp : %f, ki : %f, kd : %f\n", kp, ki, kd);
#endif

    pid->intergral += (pid->ki) * pid->current_error;
#ifdef pid_integral_limit
    if (pid->intergral > pid->intergral_limit)
        pid->intergral = pid->intergral_limit;
    else {
        if (pid->intergral < -pid->intergral_limit)
            pid->intergral = -pid->intergral_limit;
    }
#endif

    float linear_adaptive_kp = 1;
    if (pid->linear_adaptive_kp > 1e-4)
        linear_adaptive_kp =
                (1 - pid->linear_adaptive_kp) * pid->current_error / pid->error_max + pid->linear_adaptive_kp;

    pid->output = pid->kp * linear_adaptive_kp * pid->current_error + pid->intergral +
                  (pid->kd) * (pid->current_error - pid->last_error);
    pid->output += pid->feed_forward * (float) idea;
    return pid->output;
}


void delete_pid(struct PID *pid) {
    if (pid->fuzzy_struct != NULL) {
        delete_fuzzy(pid->fuzzy_struct);
    }
    free(pid);
}

void delete_pid_vector(struct PID **pid_vector, unsigned int count) {
    for (unsigned int i = 0; i < count; ++i) {
        delete_pid(pid_vector[i]);
    }
    free(pid_vector);
}


//模糊PID控制器的数组生成器 初始化
struct PID **pid_vector_init(float params[][pid_params_count], unsigned int count) {
    struct PID **pid = (struct PID **) malloc(sizeof(struct PID *) * count);
    for (unsigned int i = 0; i < count; ++i) {
        pid[i] = pid_init(params[i]);
    }
    return pid;
}

//模糊PID控制器的数组生成器，便于生成多个控制器服务于多自由度控制需求
/*其中 count 用于设定控制器个数，
 * rule_base 用于传递模糊规则库，
 * delta_k 用于控制PID的三个参数在初始值的基础上可以调节的程度，
 * params 用于传递基础的PID参数，
 * mf_type、fo_type、df_type三个参数分别决定了隶属度函数类型、模糊算子类型以及解模糊器类型。
 */
struct PID **
fuzzy_pid_vector_init(float params[][pid_params_count], float delta_k, unsigned int mf_type, unsigned int fo_type,
                      unsigned int df_type, int *mf_params, int rule_base[][qf_default],
                      unsigned int count) {
    struct PID **pid = (struct PID **) malloc(sizeof(struct PID *) * count);
    for (unsigned int i = 0; i < count; ++i) {
        pid[i] = fuzzy_pid_init(params[i], delta_k, mf_type, fo_type, df_type, mf_params, rule_base);
    }
    return pid;
}

struct PID **
fuzzy_pid_vector_init_full(float params[][pid_params_count_full], float delta_k, unsigned int mf_type, unsigned int fo_type,
                      unsigned int df_type, int *mf_params, int rule_base[][qf_default],
                      unsigned int count) {
    struct PID **pid = (struct PID **) malloc(sizeof(struct PID *) * count);
    for (unsigned int i = 0; i < count; ++i) {
        pid[i] = fuzzy_pid_init_full(params[i], delta_k, mf_type, fo_type, df_type, mf_params, rule_base);
    }
    return pid;
}



int direct_control(int zero_value, int offset_value, bool direct) {
    if (direct == true) {
        return zero_value + offset_value;
    } else {
        return zero_value - offset_value;
    }
}

/*
int fuzzy_pid_motor_pwd_output(float real, float idea, bool direct, struct PID *pid) {
    return limit(direct_control(pid->output_middle_value, fuzzy_pid_control(real, idea, pid), direct),
                 pid->output_max_value, pid->output_min_value);
}

int pid_motor_pwd_output(float real, float idea, bool direct, struct PID *pid) {
    return limit(direct_control(pid->output_middle_value, pid_control(real, idea, pid), direct), pid->output_max_value,
                 pid->output_min_value);
}
*/
