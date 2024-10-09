# Process-Scheduler


# How to compile & run the program on Windows
1. Ensure that you have [MinGW](https://sourceforge.net/projects/mingw/) installed for Windows users
1. Open a terminal on the same directory `Process-Scheduler`
1. Type the command `g++ -std=c++20 ConsoleManager.cpp MainConsole.cpp Process.cpp BaseScreen.cpp AConsole.cpp DummyProcessLayout.cpp MarqueeConsole.cpp Main.cpp -o start`
1. Type the command `./start.exe`
1. Wait for the Command Line Interface (CLI) to start running

# How to compile & run the program on MacOS
1. Install Xcode Command Line Tools `xcode-select --install`.
1. Open a terminal in the same directory as `Process-Schedule`.
1. Type the command `g++ -std=c++20 ConsoleManager.cpp MainConsole.cpp Process.cpp BaseScreen.cpp AConsole.cpp DummyProcessLayout.cpp MarqueeConsole.cpp Main.cpp -o start`.
1. Type the command `./start`.
1. Wait for the Terminal to start running. 

# App Features
1. <>
1. Recognizes the following commands
```
screen -s <session name>
screen -ls
screen -r <session name>
exit
```

# Command List
1. `screen -s <session name>` - Starts a new screen session and moves to the new screen session.
1. `screen -r <session name>` - Redraws an existing screen session.
1. `screen -ls` - List down all screen sessions.
1. `exit` - Returns to the main menu from a screen session.
2. `quit` - Stops the program.
