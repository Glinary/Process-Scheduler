# Process-Scheduler


# How to compile & run the program on Windows
1. Ensure that you have [MinGW](https://sourceforge.net/projects/mingw/) installed for Windows users
1. Open a terminal on the same directory `W2-GroupHomework`
1. Type the command `gcc W2-GroupHomework.cpp allfunctions.cpp -o processScheduler.exe`
1. Type the command `./processScheduler.exe`
1. Wait for the Command Line Interface (CLI) to start running

# How to compile & run the program on MacOS
1. Install Xcode Command Line Tools `xcode-select --install`.
1. Open a terminal on the same directory `W2-GroupHomework`.
1. Type the command `g++ W2-GroupHomework.cpp allfunctions.cpp -o processScheduler`.
1. Wait for the Terminal to start running. 

# App Features
1. Displays a menu that prints a header text CSOPESY, followed by welcoming instructions
1. Recognizes the following commands
```
initialize
screen
scheduler-test
scheduler-stop
report-util
clear
exit
```

# Command List
1. `initialize` - Prints out "initialize command recognized. Doing something."
1. `screen` - Prints out "screen command recognized. Doing something."
1. `scheduler-test` - Prints out "initialize command recognized. Doing something."
1. `scheduler-stop` - Prints out "initialize command recognized. Doing something."
1. `report-util` - Prints out "initialize command recognized. Doing something."
1. `clear` - Clears the screen and reprints the header text
1. `exit` - Closes the application/CLI