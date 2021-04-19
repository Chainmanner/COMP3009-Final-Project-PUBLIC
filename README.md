# COMP3009-Final-Project

Repository for the COMP3009A project done by Gabriel Valachi (101068875) and [REDACTED FOR SAKE OF SUBJECT'S PRIVACY].

# 3D Pong

This is a 3D version of Pong. The rules are the same as the original, this time using 3D graphics. Lighting and shadow mapping is used extensively here. Unfortunately, we could not make particles work in time for the project submission.

This game is programmed in C++, is rendered using OpenGL, and only works on Windows at the time of writing, due to the use of SOIL (an OpenGL library).

This project uses GLEW, GLFW, and SOIL for OpenGL rendering. COMP3009A TAs should already have those libraries, but the libraries are also provided in this repository, under `Windows_Libs/`, and the `CMakeLists.txt` file already points to the libraries in this directory.

To create the project for Visual Studio, cd to the root directory of the cloned repository and run `cmake CMakeLists.txt`. Build the project within Visual Studio.

To run the game, you need to be in the root directory of the cloned repository, due to the use of relative paths. cd to the root directory from a command prompt and run `Debug\COMP3009_Final_Project_d.exe`, or use the `_RUN_PROJECT.bat` convenience batch script.

When playing the game, use A and D to control the closer paddle, and the LEFT and RIGHT arrow keys to control the farther paddle. The first player to reach 11 points wins.
