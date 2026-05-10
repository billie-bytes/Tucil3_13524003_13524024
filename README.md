# Tucil3_13524003_13524024

# Project Description

This project contains a gamified simulation of common pathfinding algorithms like A*, Uniform Cost Search, Greedy Best First Search, and others. The algorithms are implemented in a puzzle board game where an object (named as a pin) is sliding through the board without the capability of stopping; needing to hit a wall to stop its movement to one direction. 

<img src="img/readmepic.png">

# Dependancies: OpenGL & GLFW
To install these dependancies (on Linux or WSL):
```bash
sudo apt update
sudo apt install libglfw3-dev libgl1-mesa-dev xorg-dev
```

# How to compile and run
## Running on native machine
Open terminal on project folder
```bash
cd build #(do "mkdir build" if it doesn't exist yet)
cmake ..
make all
```
## Running through Docker
If you are running through WSL, and assuming you have docker installed, you can immediately use the run script by
```bash
./run.sh
```
Note that this script is specifically for WSL and NOT native linux. This is because there is a difference on how to run a docker and forwards the display to you host machine on WSL and on native linux.

Also this is a heavy script because docker will need to install g++ and cmake to its container. So expect longer launch time for the price of convenience.


# Authors:
- Faiq Azzam Nafidz (13524003)
- Billie Bhaskara Wibawa (13524024)