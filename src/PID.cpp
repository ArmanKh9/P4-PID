#include "PID.h"

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {
  PID::Kp = Kp;
  PID::Ki = Ki;
  PID::Kd = Kd;
}

void PID::UpdateError(double cte) {
  PID::d_error = cte - p_error;
  PID::p_error = cte;
  PID::i_error += cte;
}

double PID::TotalError() {

}
