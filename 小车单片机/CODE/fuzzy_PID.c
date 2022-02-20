#include "headfile.h"

#include <stdlib.h>
#include "fuzzy_PID.h"

// �����Ⱥ���(mf) Membership function
// ģ������(fo) Fuzzy operator

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
     * params[3]:integral_limit ��Ki��������ֵ�� û�����ã�����.h��ȡ��ע������ ����Ϊ0��
     * params[4]��dead_zone �����������ֵ��С������ֵʱ����û�����ã�����.h��ȡ��ע������ ����Ϊ0��
     * params[5]��feed_forward ǰ���� ��Ϊ1
     * delta_k�����β����ļ��
     * ���ϼ����������Լ��趨
     *
     * ���¼�����������
     * mf_type�� ��Ϊ4 �������������Ⱥ���
     * fo_type�� ��Ϊ1 ��������
     * df_type�� ��Ϊ0 ������ƽ��ֵ��
     * mf_params�� �����Ⱥ����Ĳ��� ���г����ģ��PID����
     * rule_base[][qf_default]�� ģ������ ���г����ģ��PID����
     */
    float fuzzy_pid_params[1][pid_params_count] = {{290.f,  1500.f, 65.f, 0, 0, 0, 1}};

    struct PID **subpid_vector = fuzzy_pid_vector_init(fuzzy_pid_params, 4.0f, 4, 1, 0, mf_params, rule_base, 1);

    pid_vector=subpid_vector;
    
}


//���ݱ���������ʼ��
//�� struct PID *raw_fuzzy_pid_init ���ѵ��ã������ֶ�����
struct fuzzy *fuzzy_init(unsigned int input_num, unsigned int output_num) {
    struct fuzzy *fuzzy_struct = (struct fuzzy *) malloc(sizeof(struct fuzzy));
    fuzzy_struct->input_num = input_num;//�������
    fuzzy_struct->output_num = output_num;//�������
    fuzzy_struct->mf_type = (unsigned int *) malloc((input_num + output_num) * sizeof(unsigned int));

#ifdef fuzzy_pid_rule_base_deep_copy
    fuzzy_struct->mf_params = (int *) malloc(4 * qf_default * sizeof(int)); //qf_default ���򻮷ֵ����� Ĭ��Ϊ�еȴ�С qf_middle 7��
    fuzzy_struct->rule_base = (int *) malloc(output_num * qf_default * qf_default * sizeof(int));
#endif

    fuzzy_struct->output = (float *) malloc(output_num * sizeof(float));//���
    return fuzzy_struct;
}

void delete_fuzzy(struct fuzzy *fuzzy_struct) {
    free(fuzzy_struct->mf_type); //�ͷ��ڴ�ռ亯��
    free(fuzzy_struct->output);
    free(fuzzy_struct);
}

//������ʼ��
//�� struct PID *raw_fuzzy_pid_init ���ѵ��ã������ֶ�����
void fuzzy_params_init(struct fuzzy *fuzzy_struct, unsigned int mf_type, unsigned int fo_type, unsigned int df_type,
                       int mf_params[], int rule_base[][qf_default]) {

    for (unsigned int i = 0; i < fuzzy_struct->input_num + fuzzy_struct->output_num; ++i) { // �����б����� �����Ⱥ��� ���͸�ֵ
        fuzzy_struct->mf_type[i] = mf_type;
    }

    for (unsigned int i = 0; i < fuzzy_struct->output_num; ++i) { //���ȫ��ֵΪ0
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


//���������͵ĵ���
#define inverse(parameter) 1.0f/(float)parameter


// Gaussian membership function ��˹�����Ⱥ���
float gaussmf(float x, float sigma, float c) {
    return expf(-powf(((x - c) / sigma), 2.0f));
}


// Generalized bell-shaped membership function �������������Ⱥ���
float gbellmf(float x, float a, float b, float c) {
    return inverse(1.0f + powf(fabsf((x - c) / a), 2.0f * b));
}

// Sigmoidal membership function S�������Ⱥ���
float sigmf(float x, float a, float c) {
    return inverse(1.0f + expf(a * (c - x)));
}

// Trapezoidal membership function ���������Ⱥ���
float trapmf(float x, float a, float b, float c, float d) {
    if (x >= a && x < b)
        return (x - a) / (b - a);
    else if (x >= b && x < c)
        return 1.0f;
    else if (x >= c && x <= d)
        return (d - x) / (d - c);
    else return 0.0f;
}

// Triangular membership function �����������Ⱥ��� ʹ�ý϶�
/* xΪ����
 * aΪ���������
 * bΪ�����ζ���
 * cΪ�����յ�
 */
float trimf(float x, float a, float b, float c) {
    return trapmf(x, a, b, b, c);
}

// Z-shaped membership function Z�������Ⱥ���
float zmf(float x, float a, float b) {
    if (x <= a)
        return 1.0f;
    else if (x >= a && x <= (a + b) / 2.0f)
        return 1.0f - 2.0f * powf((x - a) / (b - a), 2.0f);
    else if (x >= (a + b) / 2.0f && x < b)
        return 2.0f * powf((x - b) / (b - a), 2.0f);
    else return 0;
}

// Membership function �����Ⱥ���
/*
 * xΪ����
 * mf_typeΪ�����Ⱥ������� ������6����ѡȡ
 * *params ���������Ⱥ����Ĳ���
 * Ĭ��Ϊ�����������Ⱥ��� �� mf_type=4
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


//3��ģ������
// Union operator ������
float or(float a, float b, unsigned int type) {
    if (type == 1) { // algebraic sum ������
        return a + b - a * b;
    } else if (type == 2) { // bounded sum �н��
        return fminf(1, a + b); //����������������еĽ�С��
    } else { // fuzzy union ģ������
        return fmaxf(a, b); //����������������еĽϴ���
    }
}

// Intersection operator ������
float and(float a, float b, unsigned int type) {
    if (type == 1) { // algebraic product ������
        return a * b;
    } else if (type == 2) { // bounded product �߽��
        return fmaxf(0, a + b - 1);
    } else { // fuzzy intersection ģ������
        return fminf(a, b);
    }
}

// Equilibrium operator ƽ������
float equilibrium(float a, float b, float params) {
    return powf(a * b, 1 - params) * powf(1 - (1 - a) * (1 - b), params);
}


// Fuzzy operator ģ�����ӵ��ú���
/*
 * a��bΪ��������
 * typeΪ��������
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
//��ģ�� ����ƽ��ֵ��
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



// Defuzzifier ��ģ����
void df(const float *joint_membership, const unsigned int *output, const unsigned int *count, struct fuzzy *fuzzy_struct,
        int df_type)
{
    if(df_type == 0)//����ƽ��ֵ��
        moc(joint_membership, output, count, fuzzy_struct);
    else {
        printf("Waring: No such of defuzzifier!\n"); //ֻ������ƽ��ֵ��
        moc(joint_membership, output, count, fuzzy_struct);
    }
}


/*
 *  e ���
 *  de ����΢��
 *  *fuzzy_struct ģ�����Ʋ����ṹ��
 */
void fuzzy_control(float e, float de, struct fuzzy *fuzzy_struct) {
    float membership[qf_default * 2]; // Store membership
    unsigned int index[qf_default * 2]; // Store the index of each membership
    unsigned int count[2] = {0, 0};

    {
        int j = 0;
        for (int i = 0; i < qf_default; ++i) {
            float temp = mf(e, fuzzy_struct->mf_type[0], fuzzy_struct->mf_params + 4 * i);
            if (temp > 1e-4) { // ���� ���e �������Ⱥ�����õ���ֵ ��ע�⸡���� ��Ҫ> 1e-4
                membership[j] = temp; //����ֵ����
                index[j++] = i;
            }
        }

        count[0] = j; //���� membership ��  ���� ���e �������Ⱥ�����õ��ķ���ֵ�ĸ���

        for (int i = 0; i < qf_default; ++i) {
            float temp = mf(de, fuzzy_struct->mf_type[1], fuzzy_struct->mf_params + 4 * i);
            if (temp > 1e-4) {// ���� ���e��΢��de �������Ⱥ�����õ���ֵ
                membership[j] = temp;
                index[j++] = i;
            }
        }

        count[1] = j - count[0]; //���� membership ��  ���� ���e��΢��de �������Ⱥ�����õ��ķ���ֵ�ĸ���
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
        //�� ���� ���e �������Ⱥ�����õ��ķ���ֵ ���� ���� ���e��΢��de �������Ⱥ�����õ��ķ���ֵ ����Ϊ0
        //˵���ñ����������Ⱥ�����õ���ֵ��Ϊ��
        for (unsigned int l = 0; l < fuzzy_struct->output_num; ++l) {
            fuzzy_struct->output[l] = 0; //�������ֵΪ0
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


//�����������������fuzzy pid��ʼ��
//��һ������������Ϊ�������
//�ڶ�������������Ϊһָ��
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

    pid->fuzzy_struct = fuzzy_init(2, output_count); //���ݱ���������ʼ��fuzzy pid���������Ϊ2�������������k�Ƿ�Ϊ����ֵ����
    fuzzy_params_init(pid->fuzzy_struct, mf_type, fo_type, df_type, mf_params, rule_base);

    pid->last_error = 0;//��һ�����
    pid->current_error = 0;//������

    ////����ʱ��Ҫע�����²��� ��������ķ�Χ��

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
 * params[3]:integral_limit ��Ki��������ֵ�� û�����ã�����.h��ȡ��ע������ ����Ϊ0��
 * params[4]��dead_zone �����������ֵ��С������ֵʱ����û�����ã�����.h��ȡ��ע������ ����Ϊ0��
 * params[5]��feed_forward ǰ���� û��ʱ��Ϊ0
 * delta_k�����β����ļ��
 * ���ϼ����������Լ��趨
 *
 * ���¼�����������
 * mf_type�� ��Ϊ4 �������������Ⱥ���
 * fo_type�� ��Ϊ1 ��������
 * df_type�� ��Ϊ0 ������ƽ��ֵ��
 * mf_params�� �����Ⱥ����Ĳ��� ���г����ģ��PID����
 * rule_base[][qf_default]�� ģ������ ���г����ģ��PID����
 */
struct PID *fuzzy_pid_init(float *params, float delta_k, unsigned int mf_type, unsigned int fo_type,
                           unsigned int df_type, int mf_params[], int rule_base[][qf_default]) {
    return raw_fuzzy_pid_init(params[0], params[1], params[2], params[3], params[4], params[5],
                              max_error,max_delta_error, params[0] / delta_k, params[1] / delta_k, params[2] / delta_k, mf_type,
                              fo_type, df_type, mf_params,
                              rule_base, min_pwm_output, middle_pwm_output, max_pwm_output);
}



//�����������������pid��ʼ��
//��һ������������Ϊ�������
//�ڶ�������������Ϊһָ��
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


//����������������
int round_user(float parameter) {
    if ((int) (parameter * 10.0) % 10 >= 5)
        return parameter + 1;
    else
        return parameter;
}

/*
//�жϸ���ֵ�Ƿ��ڷ�Χ��
//���ڷ�Χ��ʱȡ��ֵ
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


//����ʽģ��PID����������
/*
 *  realΪʵ�ʲ���ֵ
 *  ideaΪ�������ֵ
 *  �ṹ��ΪPID����������ʵ������趨��
 */
float fuzzy_pid_control(float real, float idea, struct PID *pid) {
    pid->last_error = pid->current_error;
    pid->current_error = idea - real;
    float delta_error = pid->current_error - pid->last_error;

#ifdef fuzzy_pid_dead_zone  //û�����ã�����.h��ȡ��ע������
    if (pid->current_error < pid->dead_zone && pid->current_error > -pid->dead_zone)//������趨��Χ��ʱԼ����0
    {
        pid->current_error = 0;
    }
    else //���Ϊ�������ȥԤ��ֵ
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

    //���������ֱ�Ϊ ���e ���e��΢�� �ṹ��
    //ǰ���������ȹ�һ�� �ٳ�ϵ��3��ϵ��3�Ѿ���ģ�������������
    fuzzy_control(pid->current_error / pid->error_max * 3.0f, delta_error / pid->delta_error_max * 3.0f,
                  pid->fuzzy_struct);

    //���� ģ��pid�õ������ ����ϵ��3��һ��  �ٳ����ֵ�õ�ʵ��ֵ��+����ԭkp
    pid->delta_kp = pid->fuzzy_struct->output[0] / 3.0f * pid->delta_kp_max + pid->kp;

    //�����������1ʱ ����Kiʱ �������
    if (pid->fuzzy_struct->output_num >= 2)
        pid->delta_ki = pid->fuzzy_struct->output[1] / 3.0f * pid->delta_ki_max;
    else pid->delta_ki = 0;

    //�����������2ʱ ����Kdʱ �������
    if (pid->fuzzy_struct->output_num >= 3)
        pid->delta_kd = pid->fuzzy_struct->output[2] / 3.0f * pid->delta_kd_max;
    else pid->delta_kd = 0;

#ifdef fuzzy_pid_debug_print
    printf("kp : %f, ki : %f, kd : %f\n", kp, ki, kd);
#endif

    //���²��ּ���PID���
    //����Ki���ֵļ�����
    pid->intergral += (pid->ki + pid->delta_ki) * pid->current_error;
#ifdef fuzzy_pid_integral_limit //��Ki���ֳ˻���������ֵ�涨  û�����ã�����.h��ȡ��ע������
    if (pid->intergral > pid->intergral_limit)
        pid->intergral = pid->intergral_limit;
    else {
        if (pid->intergral < -pid->intergral_limit)
            pid->intergral = -pid->intergral_limit;
    }
#endif

    //�����������
    pid->output = (pid->kp + pid->delta_kp) * pid->current_error + pid->intergral +
                  (pid->kd + pid->delta_kd) * (pid->current_error - pid->last_error);

    //����һ���õ��ĺ;����������
    pid->output += pid->feed_forward * (float) idea;
    return pid->output;
}


//����ʽPID����������
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


//ģ��PID������������������ ��ʼ��
struct PID **pid_vector_init(float params[][pid_params_count], unsigned int count) {
    struct PID **pid = (struct PID **) malloc(sizeof(struct PID *) * count);
    for (unsigned int i = 0; i < count; ++i) {
        pid[i] = pid_init(params[i]);
    }
    return pid;
}

//ģ��PID���������������������������ɶ�������������ڶ����ɶȿ�������
/*���� count �����趨������������
 * rule_base ���ڴ���ģ������⣬
 * delta_k ���ڿ���PID�����������ڳ�ʼֵ�Ļ����Ͽ��Ե��ڵĳ̶ȣ�
 * params ���ڴ��ݻ�����PID������
 * mf_type��fo_type��df_type���������ֱ�����������Ⱥ������͡�ģ�����������Լ���ģ�������͡�
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
