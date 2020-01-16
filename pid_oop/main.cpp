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

int main() {
//    pid dataPID1(20, 5, 0, 1, 1, 0, 5, -5, 0.25, 1, 10); // PID data structure
    pid dataPID1 (0,0,0,0,0,0,0,0,0,0,0);
    dataPID1 = dataPID1.initialize_initial_pid_parameters(dataPID1);

    bool condition = true;
    while (condition){
        cout << " ---options---" << endl;
        cout << "1- update pid parameters" << endl;
        cout << "2- update tf parameters" << endl;
        cout << "3- return old pid parameters" << endl;
        cout << "4- return old tf parameters" << endl;
        cout << "5- start calculation" << endl;
        cout << "6- if you want to exit"<<endl;
        cout << "enter the operation number that you want to do" << endl;

        int option;
        cin >> option;
        switch (option) {
            case 1:
                dataPID1.change_pid_params(active_pid_params, inactive_pid_params);
                break;
            case 2:
                dataPID1.change_tf_params(active_tf_params, inactive_tf_params);
                break;
            case 3:
                dataPID1.return_old_pid();
                break;
            case 4:
                dataPID1.return_old_tf();
                break;
            case 5:
                dataPID1.calculate(active_pid_params, active_tf_params);
                break;
            case 6:
                condition = false;
                break;
            default:
                cout<<"invalid input,try again"<<endl;
        }
    }
}
