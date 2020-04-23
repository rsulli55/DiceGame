# DiceGame

This is a working protoype of a networked Qwixx clone.  There is still much needed
to be done to make the game and network communication more robust, but it is 
currently possible to successfully play the game with remote players.

### Clips
![](clips/demo1.gif)

### Goals
This project was started for these two main goals

- Implement a networked version of the dice game Qwixx to play
with friends and family during the coronavirus lockdown
- Learn/practice modern software development tools and techniques e.g.
using CMake for the build system, conan for package management, writing 
unit tests with Catch2, etc.

### Current Network Architecture
The game currently uses SFML for networking.  Messages are sent through the `SFML::Network::Packet` class
by overloading `operator`'s `<<` and `>>`. 

Initially the server
sets up a TCPListener and adds clients, making the first client the host.  
Clients can submit there name and when the host sets "Ready", the game starts.


At the end of the game there is an option to replay, and again the host is the 
authority on when the game should restart.

### Planned Improvements
- Move to a new networking library, add a lobby system, and handle disconecting more gracefully
- Add the ability to resize the game window making sure graphics are scaled appropriately

### Possible Changes
- Move to SDL2 for graphics





