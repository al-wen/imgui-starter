# imgui-starter

Using the provided code provided here you have a base Dear IMGUI project for Windows and macOS. There is a basic CMakefile in this project that includes the necessary files to create an application that brings up just a basic screen.

You must add a logging system to this code that you will be using THROUGHOUT the quarter. This logging system should do two things.

It should be able to log to a Dear IMGUI debug console
It should also log its output to a file.

For the submission, make a branch of this repo and submit a new GitHub URL

# Approach

I used the Singleton design pattern so that only one instance of the logger exists throughout the program. This allows all systems to write to the same log buffer and log file without making duplicate or conflicting loggers. Each log message is made with a timestamp and a log level before being stored.

No questions were asked. The demo code helped me with understanding how to use IMGUI. 

# OS
Windows