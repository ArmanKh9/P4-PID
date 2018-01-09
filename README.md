# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

---
In this project, a PID controller implemented to control steering of a car wheels to keep the car driving between lane lines on the road. The PID controller gains were found and tuned manually in order to achieve an optimum control point.

Based on my background in Mechanical Engineering, I knew setting D and I gains to zero and guessing P gain is an easy way to start finding a controller gains. Also, behavior of a controller with too large or too small P gain is obvious. The second parameter to guess was the D gain. After finding a good enough D gain, I guessed the I gain.

Once all three parameter initial estimation was done, I performed a fine tuning process. In this process, I recorded the car total error after 2000 time steps. This number of time steps allows the error to settle and be more reliable compare to 1000 step. 2000 time step is about two rounds in the driving loop. At each step of the fine tuning process, I varied one of the gains by one order of magnitude smaller than the initial estimation and recorded the total error. I refined the gains based on decrease in the error. Below is a portion of the fine tuning steps that I took:

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

The fine tuning process can done for more time steps and smaller tuning values to provide a more optimized gains.

Effect of P gain:
The P gain determines how much the steering angle proportionally needs to change based on the evaluated error. A large P causes instability in the car movement around the center of the road and can result the car to move out of the track. A small P gain cannot change the steering angle enough for the car to decrease the error.

Effect of D gain:
The D gain helps the car to somehow predict the future error based on the last two time step errors. In other words, if the car moving away from the road center the D controller will increase the steering angle to move the car back to the center. A large D gain causes wavy movement specially at turns. A small D gain cannot adjust the steering angle enough when the car is suddenly moving away from the center.

Effect of I gain:
The I gain helps the car to reduce the steady error to zero. A large I gain causes oscillation and a small I gain fails to reduce the steady error. This becomes obvious at turns as well.  

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1(mac, linux), 3.81(Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

There's an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`.

Tips for setting up your environment can be found [here](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/0949fca6-b379-42af-a919-ee50aa304e6a/lessons/f758c44c-5e40-4e01-93b5-1a82aa4e044f/concepts/23d376c7-0195-4276-bdf0-e02f1f3c665d)

## Editor Settings

We've purposefully kept editor configuration files out of this repo in order to
keep it as simple and environment agnostic as possible. However, we recommend
using the following settings:

* indent using spaces
* set tab width to 2 spaces (keeps the matrices in source code aligned)

## Code Style

Please (do your best to) stick to [Google's C++ style guide](https://google.github.io/styleguide/cppguide.html).

## Project Instructions and Rubric

Note: regardless of the changes you make, your project must be buildable using
cmake and make!

More information is only accessible by people who are already enrolled in Term 2
of CarND. If you are enrolled, see [the project page](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/f1820894-8322-4bb3-81aa-b26b3c6dcbaf/lessons/e8235395-22dd-4b87-88e0-d108c5e5bbf4/concepts/6a4d8d42-6a04-4aa6-b284-1697c0fd6562)
for instructions and the project rubric.

## Hints!

* You don't have to follow this directory structure, but if you do, your work
  will span all of the .cpp files here. Keep an eye out for TODOs.

## Call for IDE Profiles Pull Requests

Help your fellow students!

We decided to create Makefiles with cmake to keep this project as platform
agnostic as possible. Similarly, we omitted IDE profiles in order to we ensure
that students don't feel pressured to use one IDE or another.

However! I'd love to help people get up and running with their IDEs of choice.
If you've created a profile for an IDE that you think other students would
appreciate, we'd love to have you add the requisite profile files and
instructions to ide_profiles/. For example if you wanted to add a VS Code
profile, you'd add:

* /ide_profiles/vscode/.vscode
* /ide_profiles/vscode/README.md

The README should explain what the profile does, how to take advantage of it,
and how to install it.

Frankly, I've never been involved in a project with multiple IDE profiles
before. I believe the best way to handle this would be to keep them out of the
repo root to avoid clutter. My expectation is that most profiles will include
instructions to copy files to a new location to get picked up by the IDE, but
that's just a guess.

One last note here: regardless of the IDE used, every submitted project must
still be compilable with cmake and make./

## How to write a README
A well written README file can enhance your project and portfolio.  Develop your abilities to create professional README files by completing [this free course](https://www.udacity.com/course/writing-readmes--ud777).
