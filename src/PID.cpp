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

  /*best_error = pid.TotalError();
  std::vector<double> dp = {1, 1, 1};
  std::vector<double> vec_p = {pid.Kp, pid.Kd, pid.Ki};
  int it =0;

  double tol = 0.2;
  while (dp[0]+dp[1]+dp[2]>tol) {
    for (int i=0; i<pvec.size(); i++){
      vec_p[i] += dp[i];*/


}

double PID::TotalError(double cte) {
  total_error += cte*cte;

}
