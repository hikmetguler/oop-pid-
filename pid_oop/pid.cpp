#include <utility>
#include <iostream>
#include "pid.h"
using namespace std;

pid_struct pid_params_struct;
pid_struct inactive_pid_params;
pid_struct active_pid_params;
tf_struct tf_params_struct;
tf_struct inactive_tf_params;
tf_struct active_tf_params;

pid_struct experimental_active_pid;
pid_struct experimental_inactive_pid;
tf_struct experimental_active_tf;
tf_struct experimental_inactive_tf;

pid::pid(float _K, float _Ti, float _Td, float _N, float _b, float _c, float _CSmax, float _CSmin, float _Ts,float _mu,float _T) :K(_K),Ti(_Ti), Td(_Td),N(_N),b(_b),c(_c),CSmax(_CSmax),CSmin(_CSmin),Ts(_Ts),mu(_mu),T(_T)
         {
             cout<<"pid class constructor has been called"<<endl;
             cout<<" initial values has been set as SPo =0, PVo=0, CSo=0, Do=0"<<endl;
             pid_params_struct.K_str = get_K();
             pid_params_struct.Ti_str = get_Ti();
             pid_params_struct.Td_str = get_Td();
             pid_params_struct.N_str = get_N();
             pid_params_struct.b_str = get_b();
             pid_params_struct.c_str = get_c();
             pid_params_struct.CSmax_str = get_Csmax();
             pid_params_struct.CSmin_str = get_Csmin();
             pid_params_struct.Ts_str = get_Ts();

             pid_params_struct.SPo_str = 0;
             pid_params_struct.PVo_str = 0;
             pid_params_struct.CSo_str = 0;
             pid_params_struct.Do_str = 0;

             tf_params_struct.Ts_str = get_Ts();
             tf_params_struct.T_str = get_T();
             tf_params_struct.mu_str = get_mu();

             cout<<"initial values defined as y = 0, uo = 0, xo = 0."<<endl;
             tf_params_struct.y_str = 0;
             tf_params_struct.uo_str = 0;
             tf_params_struct.xo_str = 0;
         }

pid::~pid(){}

void pid::isa_pid_struct(pid_struct &pidparamsStruct)
{
    float DSP,DPV,DP,DI,D,DD,DCS;
    if(pidparamsStruct.TS_str==0){
        DSP    = pidparamsStruct.SP_str-pidparamsStruct.SPo_str;
        DPV    = pidparamsStruct.PV_str-pidparamsStruct.PVo_str;
        DP     = pidparamsStruct.K_str*(pidparamsStruct.b_str*DSP-DPV);
        DI     = pidparamsStruct.K_str*pidparamsStruct.Ts_str/pidparamsStruct.Ti_str*(pidparamsStruct.SP_str-pidparamsStruct.PV_str);
        D      = (pidparamsStruct.Td_str*pidparamsStruct.Do_str+pidparamsStruct.K_str*pidparamsStruct.N_str*pidparamsStruct.Td_str*(pidparamsStruct.c_str*DSP-DPV))/(pidparamsStruct.Td_str+pidparamsStruct.N_str*pidparamsStruct.Ts_str);
        DD     = D-pidparamsStruct.Do_str;
        DCS    = DP+DI+DD;
        pidparamsStruct.CS_str = pidparamsStruct.CSo_str+DCS;
    }
    else           // Tracking mode
    {
        pidparamsStruct.CS_str = pidparamsStruct.TR_str;
        D = 0; // Arbitrary (common practice) assignment
    }
    if (pidparamsStruct.CS_str>pidparamsStruct.CSmax_str) pidparamsStruct.CS_str = pidparamsStruct.CSmax_str; // Antiwindup
    if (pidparamsStruct.CS_str<pidparamsStruct.CSmin_str) pidparamsStruct.CS_str = pidparamsStruct.CSmin_str;

    pidparamsStruct.SPo_str = pidparamsStruct.SP_str;                         // Store state
    pidparamsStruct.PVo_str = pidparamsStruct.PV_str;                         // variables for
    pidparamsStruct.CSo_str = pidparamsStruct.CS_str;                         // the next step
    pidparamsStruct.Do_str  = D;

    pid_params_struct = pidparamsStruct;
}

void pid::isa_TFZ_struct(tf_struct &params_tf)
{
    float x;
    x = (params_tf.T_str*(params_tf.xo_str+params_tf.uo_str))/(params_tf.T_str+params_tf.Ts_str);
    params_tf.y_str = params_tf.mu_str*params_tf.Ts_str/(params_tf.T_str+params_tf.Ts_str)*(x+params_tf.u_str);
    params_tf.xo_str = x;
    params_tf.uo_str = params_tf.u_str;
    tf_params_struct = params_tf;
}

void pid::calculate(pid_struct params_pid, tf_struct params_tf)
{   int nSteps;
    float Ts;
    float t;
    int k;
    nSteps = 800;
    Ts = params_pid.Ts_str;
    for (k=0;k<nSteps;k++)
    {
        t = k*Ts;
        params_pid.TS_str = stp(t-120)- stp(t-150);
        params_pid.TR_str = 1;
        params_pid.SP_str = ram(t-1) - ram(t-5) - 0.1 * ram(t - 50)+ 0.1 * ram(t - 80)+ stp(t - 100);
        params_pid.PV_str = params_tf.y_str;
        isa_pid_struct(params_pid);
        params_tf.u_str = params_pid.CS_str;
        isa_TFZ_struct(params_tf);
        cout << t << " = t   ***   "
             << params_pid.SP_str << " = SP   ***   "
             << params_pid.PV_str << " = PV   ***   "
             << params_pid.CS_str << " = CS   ***   "
             << params_pid.TS_str << " = TS   ***   "
             << params_pid.TR_str << " = TR   ***   "
             << params_pid.CSo_str << " = CSo" << endl;
    }
}

pid pid::initialize_initial_pid_parameters(pid null_pid) {
    cout<<"to initialize pid paramteres"<<endl;
    cout << "enter new pid K" << endl;
    float K_int;cin>>K_int;
    null_pid.set_K(K_int);
    cout<<null_pid.get_K();
    cout << "enter new Ti" << endl;
    float Ti_int; cin >>Ti_int;
    null_pid.set_Ti(Ti_int);
    cout << "enter new Td" << endl;
    float Td_int; cin >>Ti_int;
    null_pid.set_Td(Td_int);
    cout << "enter new N" << endl;
    float N_int; cin >>N_int;
    null_pid.set_N(N_int);
    cout << "enter new b" << endl;
    float b_int; cin >>b_int;
    null_pid.set_b(b_int);
    cout << "enter new c" << endl;
    float c_int; cin >>c_int;
    null_pid.set_c(c_int);;
    cout << "enter new Csmax" << endl;
    float Csmax_int; cin >>Csmax_int;
    null_pid.set_Csmax(Csmax_int);
    cout << "enter new Csmin" << endl;
    float Csmin_int; cin >>Csmin_int;
    null_pid.set_Csmin(Csmin_int);
    cout << "enter new Ts" << endl;
    float Ts_int; cin >>Ts_int;
    null_pid.set_Ts(Ts_int);
    cout << "enter new tf mu" << endl;
    float mu_int;cin>>mu_int;
    null_pid.set_mu(mu_int);
    cout << "enter new tf T" << endl;
    float T_int;cin>>T_int;
    null_pid.set_T(T_int);

    pid dataPID1 = null_pid;
    tf_params_struct.Ts_str = dataPID1.get_Ts();
    tf_params_struct.T_str = dataPID1.get_T();
    tf_params_struct.mu_str = dataPID1.get_mu();
    pid_params_struct.K_str = dataPID1.get_K();
    pid_params_struct.Ti_str = dataPID1.get_Ti();
    pid_params_struct.Td_str = dataPID1.get_Td();
    pid_params_struct.N_str = dataPID1.get_N();
    pid_params_struct.b_str = dataPID1.get_b();
    pid_params_struct.c_str = dataPID1.get_c();
    pid_params_struct.CSmax_str = dataPID1.get_Csmax();
    pid_params_struct.CSmin_str = dataPID1.get_Csmin();
    pid_params_struct.Ts_str = dataPID1.get_Ts();
    inactive_pid_params = pid_params_struct;
    inactive_tf_params = tf_params_struct;
    return dataPID1;
}

void initialize_tf_struct_parameters(pid null_pid){
    cout<<"to initialize tf paramteres"<<endl;
    cout << "enter new tf Ts" << endl;
    float Ts_int;cin>>Ts_int;
    null_pid.set_Ts(Ts_int);
    cout << "enter new tf mu" << endl;
    float mu_int;cin>>mu_int;
    null_pid.set_mu(mu_int);
    cout << "enter new tf T" << endl;
    float T_int;cin>>T_int;
    null_pid.set_T(T_int);
    tf_params_struct.Ts_str = null_pid.get_Ts();
    tf_params_struct.T_str = null_pid.get_T();
    tf_params_struct.mu_str = null_pid.get_mu();
    inactive_tf_params = tf_params_struct;



}

void pid::change_pid_params(pid_struct &active_params, pid_struct &inactive_params) {
    char answer;

    active_params.K_str = pid_params_struct.K_str;
    active_params.Ti_str = pid_params_struct.Ti_str;
    active_params.Td_str = pid_params_struct.Td_str;
    active_params.N_str = pid_params_struct.N_str;
    active_params.b_str = pid_params_struct.b_str;
    active_params.c_str = pid_params_struct.c_str;
    active_params.CSmax_str = pid_params_struct.CSmax_str;
    active_params.CSmin_str = pid_params_struct.CSmin_str;
    active_params.Ts_str = pid_params_struct.Ts_str;

    cout << "enter new pid K" << endl;
    cin >> inactive_params.K_str;
    cout << "enter new Ti" << endl;
    cin >> inactive_params.Ti_str;
    cout << "enter new Td" << endl;
    cin >> inactive_params.Td_str;
    cout << "enter new N" << endl;
    cin >> inactive_params.N_str;
    cout << "enter new b" << endl;
    cin >> inactive_params.b_str;
    cout << "enter new c" << endl;
    cin >> inactive_params.c_str;
    cout << "enter new Csmax" << endl;;
    cin >> inactive_params.CSmax_str;
    cout << "enter new Csmin" << endl;
    cin >> inactive_params.CSmin_str;
    cout << "enter new Ts" << endl;
    cin >> inactive_params.Ts_str;


    cout << "are you sure to change pid parameter (y/n)" << endl;
    cin >> answer;
    if (answer == 'y') {
        swap(active_params, inactive_params);
//        set_K(active_params.K_str);
//        set_Ti(active_params.Ti_str);
//        set_Td(active_params.Td_str);
//        set_N(active_params.N_str);
//        set_b(active_params.b_str);
//        set_c(active_params.c_str);
//        set_Csmax(active_params.CSmax_str);
//        set_Csmin(active_params.CSmin_str);
//        set_Ts(active_params.Ts_str);

        active_pid_params = active_params;
        inactive_pid_params = inactive_params;

        cout << "K active = " << active_pid_params.K_str << "   ***   " << "Ti active = " << active_pid_params.Ti_str << "   ***   "
             << "Td active = " << active_pid_params.Td_str << "   ***   " << "N  active = " << active_pid_params.N_str << "   ***   " << "b  active = "
             << active_pid_params.b_str << "   ***   " << "c active = " << active_pid_params.c_str << "   ***   " << "Csmax active = "
             << active_pid_params.CSmax_str << "   ***   " << "Csmin active = " << active_pid_params.CSmin_str << "   ***   "
             << "Ts active = " << active_pid_params.Ts_str << endl;

        cout << "K inactive = " << inactive_pid_params.K_str << "   ***   " << "Ti inactive = " << inactive_pid_params.Ti_str << "   ***   "
             << "Td inactive = " << inactive_pid_params.Td_str << "   ***   " << "N inactive = " << inactive_pid_params.N_str << "   ***   "
             << "b inactive = " << inactive_pid_params.b_str << "   ***   " << "c inactive = " << inactive_pid_params.c_str << "   ***   "
             << "Csmax inactive = " << inactive_pid_params.CSmax_str << "   ***   " << "Csmin inactive = " << inactive_pid_params.CSmin_str
             << "   ***   " << "Ts inactive = " << inactive_pid_params.Ts_str << endl;}

    else if (answer == 'n') {cout << "operation is cancelled" << endl;}

    else {cout << "invalid input" << endl; }
}
void pid::change_tf_params(tf_struct &active_params, tf_struct &inactive_params)
{
    active_params.mu_str = active_tf_params.mu_str;
    active_params.T_str = active_tf_params.T_str;

    cout<<"enter new mu"<<endl;
    cin>>inactive_tf_params.mu_str;
    cout<<"enter new T"<<endl;
    cin>>inactive_tf_params.T_str;

    char answer;
    cout<<"are you sure to change pid parameter (y/n)"<<endl;
    cin>>answer;
    if (answer == 'y')
    {
        swap(active_params,inactive_params);
        set_mu(active_tf_params.mu_str);
        set_T(active_tf_params.T_str);
    }
    else if(answer =='n') {cout<<"operation is cancelled"<<endl;}

    else {cout<<"invalid input"<<endl;}

    active_tf_params = active_params;
    inactive_tf_params = inactive_params;

    cout<<"mu active = "<<active_tf_params.mu_str<<"   ***   "<<"T active ="<<active_tf_params.T_str<<endl;
    cout<<"mu inactive = "<<inactive_tf_params.mu_str<<"   ***   "<<"T inactive ="<<inactive_tf_params.T_str<<endl;
}

pid_struct pid::return_old_pid()
{
    swap(active_pid_params,inactive_pid_params);
    cout << "K active = " << active_pid_params.K_str << "   ***   " << "Ti active = " << active_pid_params.Ti_str << "   ***   "
         << "Td active = " << active_pid_params.Td_str << "   ***   " << "N  active = " << active_pid_params.N_str << "   ***   " << "b  active = "
         << active_pid_params.b_str << "   ***   " << "c active = " << active_pid_params.c_str << "   ***   " << "Csmax active = "
         << active_pid_params.CSmax_str << "   ***   " << "Csmin active = " << active_pid_params.CSmin_str << "   ***   "
         << "Ts active = " << active_pid_params.Ts_str << endl;

    cout << "K inactive = " << inactive_pid_params.K_str << "   ***   " << "Ti inactive = " << inactive_pid_params.Ti_str << "   ***   "
         << "Td inactive = " << inactive_pid_params.Td_str << "   ***   " << "N inactive = " << inactive_pid_params.N_str << "   ***   "
         << "b inactive = " << inactive_pid_params.b_str << "   ***   " << "c inactive = " << inactive_pid_params.c_str << "   ***   "
         << "Csmax inactive = " << inactive_pid_params.CSmax_str << "   ***   " << "Csmin inactive = " << inactive_pid_params.CSmin_str
         << "   ***   " << "Ts inactive = " << inactive_pid_params.Ts_str << endl;
}

tf_struct pid::return_old_tf()
{
    swap(active_tf_params, inactive_tf_params);
    cout<<"mu active = "<<active_tf_params.mu_str<<"   ***   "<<"T active ="<<active_tf_params.T_str<<endl;
    cout<<"mu inactive = "<<inactive_tf_params.mu_str<<"   ***   "<<"T inactive ="<<inactive_tf_params.T_str<<endl;
}

float pid::stp(float t) { return t>=0?1:0; }

float pid::ram(float t) { return t*stp(t); }

//get functions for pid members
float pid::get_K(){ return K;}
float pid::get_Ti(){ return Ti;}
float pid::get_Td(){ return Td;}
float pid::get_N(){ return N;}
float pid::get_b(){ return b;}
float pid::get_c(){ return c;}
float pid::get_Csmax(){ return CSmax;}
float pid::get_Csmin(){ return CSmin;}
float pid::get_Ts(){ return Ts;}
float pid::get_SP(){ return SP;}
float pid::get_SPo(){ return SPo;}
float pid::get_PV(){ return PV;}
float pid::get_PVo(){ return PVo;}
float pid::get_Do(){ return Do;}
float pid::get_CS(){ return CS;}
float pid::get_CSo(){ return CSo;}
float pid::get_TR(){ return TR;}
float pid::get_t(){ return t;}
float pid::get_k(){ return k;}
float pid::get_mu() { return mu;}
float pid::get_T() { return T;}
float pid::get_u() { return u;}
float pid::get_uo() { return uo;}
float pid::get_xo() { return xo;}
float pid::get_y() { return y;}
char pid::get_TS(){ return TS;}

void pid::set_K(float K_new)
{
//    pid_params_struct.K_str = K_new;
    K = K_new;
    cout<<"K is updated"<<endl;
}
void pid::set_Ti(float Ti_new)
{
//    pid_params_struct.Ti_str = Ti_new;
    Ti = Ti_new;
    cout<<"Ti is updated"<<endl;
}
void pid::set_Td(float Td_new)
{
    Td = Td_new;
    cout<<"Td is updated"<<endl;
}
void pid::set_N(float N_new)
{
//    pid_params_struct.N_str = N_new;
    N = N_new;
    cout<<"N is updated"<<endl;
}
void pid::set_b(float b_new)
{
//    pid_params_struct.b_str = b_new;
    b = b_new;
    cout<<"b is updated"<<endl;
}
void pid::set_c(float c_new)
{
//    pid_params_struct.c_str = c_new;
    c = c_new;
    cout<<"c is updated"<<endl;
}
void pid::set_Csmax(float CSmax_new)
{
//    pid_params_struct.CSmax_str = CSmax_new;
    CSmax = CSmax_new;
    cout<<"CSmax is updated"<<endl;
}
void pid::set_Csmin(float CSmin_new)
{
//    pid_params_struct.CSmin_str = CSmin_new;
    CSmin = CSmin_new;
    cout<<"CSmin is updated"<<endl;
}
void pid::set_Ts(float Ts_new)
{
//    pid_params_struct.Ts_str = Ts_new;
    Ts = Ts_new;
    cout<<"Ts is updated"<<endl;
}
void pid::set_SP(float SP_new)
{
//    pid_params_struct.SP_str = SP_new;
    SP=SP_new;}
void pid::set_SPo(float SPo_new)
{
//    pid_params_struct.SPo_str = SPo_new;
    SPo=SPo_new;}
void pid::set_PV(float PV_new)
{
//    pid_params_struct.PV_str = PV_new;
    PV=PV_new;}
void pid::set_PVo(float PVo_new)
{
//    pid_params_struct.PVo_str = PVo_new;
    PVo=PVo_new;}
void pid::set_Do(float Do_new)
{
//    pid_params_struct.Do_str = Do_new;
    Do=Do_new;}
void pid::set_CS(float CS_new)
{
//    pid_params_struct.CS_str = CS_new;
    CS=CS_new;}
void pid::set_CSo(float CSo_new)
{
//    pid_params_struct.CSo_str = CSo;
    CSo=CSo_new;}
void pid::set_TR(float TR_new)
{
//    pid_params_struct.TR_str = TR_new;
    TR=TR_new;}
void pid::set_t(float t_new)
{
//    pid_params_struct.t_str = t_new;
    t=t_new;}
void pid::set_k(float k_new)
{
//    pid_params_struct.k_str = k_new;
    k=k_new;}
void pid::set_TS(char TS_new)
{
//    pid_params_struct.TS_str = TS_new;
    TS=TS_new;}
void pid::set_mu(float mu_new)
{
    mu=mu_new;
    cout<<"mu is updated"<<endl;
//    tf_params_struct.mu_str = mu_new;
}
void pid::set_T(float T_new)
{    T=T_new;
    cout<<"T is updated"<<endl;
//    tf_params_struct.T_str = T_new;
}
void pid::set_u(float u_new)
{   u = u_new;
    cout<<"u is updated"<<endl;
//    tf_params_struct.u_str = u_new;
}
void pid::set_uo(float uo_new)
{   uo=uo_new;
    cout<<"uo is updated"<<endl;
//    tf_params_struct.uo_str = uo_new;
}
void pid::set_xo(float xo_new)
{   xo=xo_new;
    cout<<"xoo is updated"<<endl;
//    tf_params_struct.xo_str = xo_new;
}
void pid::set_y(float y_new)
{   y=y_new;
    cout<<"y is updated"<<endl;
//    tf_params_struct.y_str = y_new;
}
