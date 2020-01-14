#include <iostream>
#include <utility>
#include "pid.h"

using namespace std;
extern pid_struct pid_params_struct;
extern tf_struct tf_params_struct;

extern pid_struct inactive_pid_params;
extern tf_struct inactive_tf_params;

extern pid_struct active_pid_params;
extern tf_struct active_tf_params;

extern pid_struct experimental_active_pid;
extern pid_struct experimental_inactive_pid;
extern tf_struct experimental_active_tf;
extern tf_struct experimental_inactive_tf;

int main() {
//    pid dataPID1(20, 5, 0, 1, 1, 0, 5, -5, 0.25, 1, 10); // PID data structure
    pid dataPID1 (0,0,0,0,0,0,0,0,0,0,0);
    dataPID1 = dataPID1.initialize_initial_pid_parameters(dataPID1);
    cout<<"mu active = "<<tf_params_struct.mu_str<<"   ***   "<<"T active ="<<tf_params_struct.T_str<<endl;
    cout<<"mu inactive = "<<inactive_tf_params.mu_str<<"   ***   "<<"T inactive ="<<inactive_tf_params.T_str<<endl;

    cout << "K active = " << pid_params_struct.K_str << "   ***   " << "Ti active = " << pid_params_struct.Ti_str << "   ***   "
         << "Td active = " << pid_params_struct.Td_str << "   ***   " << "N  active = " << pid_params_struct.N_str << "   ***   " << "b  active = "
         << pid_params_struct.b_str << "   ***   " << "c active = " << pid_params_struct.c_str << "   ***   " << "Csmax active = "
         << pid_params_struct.CSmax_str << "   ***   " << "Csmin active = " << pid_params_struct.CSmin_str << "   ***   "
         << "Ts active = " << pid_params_struct.Ts_str << endl;

    cout << "K inactive = " << inactive_pid_params.K_str << "   ***   " << "Ti inactive = " << inactive_pid_params.Ti_str << "   ***   "
         << "Td inactive = " << inactive_pid_params.Td_str << "   ***   " << "N inactive = " << inactive_pid_params.N_str << "   ***   "
         << "b inactive = " << inactive_pid_params.b_str << "   ***   " << "c inactive = " << inactive_pid_params.c_str << "   ***   "
         << "Csmax inactive = " << inactive_pid_params.CSmax_str << "   ***   " << "Csmin inactive = " << inactive_pid_params.CSmin_str
         << "   ***   " << "Ts inactive = " << inactive_pid_params.Ts_str << endl;

    bool condition = true;
    while (condition){
        cout << " ---options---" << endl;
//        cout << "1- enter pid parameters" << endl;
//        cout << "2- enter tf parameters" << endl;
        cout << "3- update pid parameters" << endl;
        cout << "4- update tf parameters" << endl;
        cout << "5- return old pid parameters" << endl;
        cout << "6- return old tf parameters" << endl;
        cout << "7- start calculation" << endl;
        cout << "8- if you want to exit"<<endl;
        cout << "enter the operation number that you want to do" << endl;

        int option;
        cin >> option;
        switch (option) {
//            case 1:
//                dataPID1.change_pid_params(experimental_active_pid, experimental_inactive_pid);
//                break;
//            case 2:
//                break;
            case 3:
                dataPID1.change_pid_params(active_pid_params, inactive_pid_params);
                break;
            case 4:
                dataPID1.change_tf_params(active_tf_params, inactive_tf_params);
                break;
            case 5:
                dataPID1.return_old_pid();
                break;
            case 6:
                dataPID1.return_old_tf();
                break;
            case 7:
                dataPID1.calculate(pid_params_struct, tf_params_struct);
                break;
            case 8:
                condition = false;
                break;
        }
    }
}