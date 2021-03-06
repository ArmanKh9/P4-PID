#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main()
{
  uWS::Hub h;

  PID pid;
  /*---------- Rough tuning for the car to drive one full round ----------*/
  //pid.Init(1.0, 0.0, 0.0);
  //pid.Init(0.01, 0.0, 0.0);
  //pid.Init(0.05, 0.0, 0.0);
  //pid.Init(0.05, 0.01, 0.0);
  //pid.Init(0.05, 0.001, 0.0);
  //pid.Init(0.05, -0.01, 0.0);
  //pid.Init(0.05, -0.001, 0.0);
  //pid.Init(0.05, 0.0001, 0.0);
  //pid.Init(0.025, 0.001, 0.0);
  //pid.Init(0.025, 0.001, 0.0001);
  //pid.Init(0.05, 1.0, 0.0001);
  //pid.Init(0.05, 0.1, 0.001);
  //pid.Init(0.03, 0.0, 0.0);
  //Kp = 0.03 provides an acceptable drive until after the bridge.
  // now trying to tune Kd to improve the already acceptable portion of the drive better
  //pid.Init(0.03, 0.0, 0.1);
  //pid.Init(0.03, 0.0, 1.0);
  // Kd = 1.0 improved the drive
  //pid.Init(0.03, 0.0, 1.1);
  //pid.Init(0.03, 0.0, 2.0);
  // drive improved
  //pid.Init(0.03, 0.0, 3.0);
  //with kp = 3.0 car drive a little bit more on the road but very wavery
  //pid.Init(0.03, 0.0, 4.0);
  //pid.Init(0.03, 0.0, 5.0);
  // Kd = 5.0 made the drive more wavy and no improvement on how far the car drove on the road. So Kd = 3.0
  // for now is a good point to start tuning the Ki
  //pid.Init(0.03, 0.1, 3.0);
  //pid.Init(0.03, 0.01, 3.0);
  //pid.Init(0.03, 0.001, 3.0);
  //pid.Init(0.03, 0.0001, 3.0);
  //pid.Init(0.03, 0.0005, 3.0);
  //pid.Init(0.03, 0.00001, 3.0);
  //Ki = 0.00001 resulted in a full round drive

  /*---------- Fine Tuning for 2000 time step ----------*/
  //pid.Init(0.03, 0.00001, 3.0); ----> total error = 6424
  //pid.Init(0.031, 0.00001, 3.0); ----> total error = 6064
  //pid.Init(0.032, 0.00001, 3.0); ----> total error = 5896
  //pid.Init(0.033, 0.00001, 3.0); ----> total error = 6071
  //pid.Init(0.032, 0.00001, 3.1); ----> total error = 5976
  //pid.Init(0.032, 0.00001, 2.9); ----> total error = 5864
  //pid.Init(0.032, 0.00002, 2.9); ----> total error = 4631
  //pid.Init(0.032, 0.00003, 2.9); ----> total error = 4401
  //pid.Init(0.032, 0.0001, 2.9); ----> total error = 4037
  //pid.Init(0.032, 0.0002, 2.9); ----> total error = 4426
  //pid.Init(0.032, 0.00011, 2.9); ----> total error = 3623
  //pid.Init(0.032, 0.00012, 2.9); ----> total error = 3342
  //pid.Init(0.032, 0.00013, 2.9); ----> total error = 3657
  //pid.Init(0.032, 0.00012, 2.8); ----> total error = 4329
  //pid.Init(0.032, 0.00012, 2.91); ----> total error = 3417
  pid.Init(0.032, 0.00012, 2.90);


  pid.p_error = 0.0;
  pid.tstep = 0;
  pid.prev_error = 0.0;

  h.onMessage([&pid](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
          double angle = std::stod(j[1]["steering_angle"].get<std::string>());
          double steer_value;
          /*
          * TODO: Calcuate steering value here, remember the steering value is
          * [-1, 1].
          * NOTE: Feel free to play around with the throttle and speed. Maybe use
          * another PID controller to control the speed!
          */
          pid.tstep += 1;

          pid.UpdateError(cte);
          steer_value = -pid.Kp*pid.p_error - pid.Kd*pid.d_error - pid.Ki*pid.i_error;

          if (steer_value>1.0){
            steer_value -= 1.0;
          }

          if (steer_value<-1.0){
            steer_value = -1.0;
          }

          pid.TotalError(cte);

          // DEBUG
          std::cout << "CTE: " << cte << " Steering Value: " << steer_value << std::endl;

          if (pid.tstep == 2000){
            std::cout << "--------------------------total error: " << pid.total_error<<std::endl;
          }
          std::cout << "time step: " << pid.tstep<<std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = 0.3;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
