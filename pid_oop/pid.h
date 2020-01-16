#ifndef UNTITLED_PID_H
#define UNTITLED_PID_H
#include <utility>
#include <iostream>

using namespace std;

//instead of using object parameters directly, its better to use structs parameters whose initialize directly from object parameters
struct pid_struct
{
    float K_str,Ti_str,Td_str,N_str,b_str,c_str,CSmax_str,CSmin_str,Ts_str; // parameters
    float SP_str,SPo_str,PV_str,PVo_str,Do_str,CS_str,CSo_str,TR_str,t_str,k_str; //variables
    char  TS_str;
};

struct tf_struct
{
    float mu_str,T_str,Ts_str;    // parameters
    float u_str,uo_str,y_str,xo_str;  // variables
};


class pid{
private:
    float K,Ti,Td,N,b,c,CSmax,CSmin,Ts; // parameters
    float SP,SPo,PV,PVo,Do,CS,CSo,TR,t,k; //variables
    char  TS;

private:
    float mu,T;    // parameters
    float u,uo,y,xo;  // variables

public:
//    constructor, the parameters of structs have been set. also few variables is set as zero as default.
    pid(float _K, float _Ti, float _Td, float _N, float _b, float _c, float _CSmax, float _CSmin, float _Ts,float _mu,float _T);
    ~pid();

//    in calculation, isa_pid_struct and isa_TFZ_struct functions are used. It is a simulation of the process and control actions.
    void calculate(pid_struct, tf_struct);
    void  isa_pid_struct(pid_struct &);
    void isa_TFZ_struct(tf_struct &);

    //change the pid and tf parameters
    void change_pid_params(pid_struct &active_params, pid_struct &inactive_params);
    void change_tf_params (tf_struct &active_params,tf_struct &inactive_params);

    //return the old pid-tf params
    pid_struct return_old_pid();
    tf_struct return_old_tf();

    //initialize the pid and tf parameters
    pid initialize_initial_pid_parameters(pid datanull);

    float stp(float t);
    float ram(float t);

    //get functions for pid members
    float get_K();
    float get_Ti();
    float get_Td();
    float get_N();
    float get_b();
    float get_c();
    float get_Csmax();
    float get_Csmin();
    float get_Ts();
    float get_SP();
    float get_SPo();
    float get_PV();
    float get_PVo();
    float get_Do();
    float get_CS();
    float get_CSo();
    float get_TR();
    float get_t();
    float get_k();
    char get_TS();
    float get_mu();
    float get_T();
    float get_u();
    float get_uo();
    float get_y();
    float get_xo();

    //set functions for pid members
    void set_K(float K_new);
    void set_Ti(float Ti_new);
    void set_Td(float Td_new);
    void set_N(float N_new);
    void set_b(float b_new);
    void set_c(float c_new);
    void set_Csmax(float Csmax_new);
    void set_Csmin(float Csmin_new);
    void set_Ts(float Ts_new);
    void set_SP(float SP_new);
    void set_SPo(float SPo_new);
    void set_PV(float PV_new);
    void set_PVo(float PVo_new);
    void set_Do(float Do_new);
    void set_CS(float CS_new);
    void set_CSo(float CSo_new);
    void set_TR(float TR_new);
    void set_t(float t_new);
    void set_k(float k_new);
    void set_TS(char TS_new);
    void set_mu(float mu_new);
    void set_T(float T_new);
    void set_u(float u_new);
    void set_uo(float uo_new);
    void set_y(float y_new);
    void set_xo(float xo_new);
};

#endif //UNTITLED_PID_H
