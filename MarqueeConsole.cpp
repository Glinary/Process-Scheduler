#include "MarqueeConsole.h"
#include "ConsoleManager.h"
#include <iostream>
#include <chrono>
#include <conio.h>  // For _kbhit() and _getch()

MarqueeConsole::MarqueeConsole()
    : AConsole("MarqueeConsole"), running(false), marqueeText("Hello world in marquee!"), position(0) {
    // Assume the console width is 80 for simplicity
    consoleWidth = 80;
}

MarqueeConsole::~MarqueeConsole() {
    stopMarquee();  // Ensure the thread stops when the object is destroyed
}

// Function to start the marquee
void MarqueeConsole::startMarquee() {
    running = true;
    marqueeThread = std::thread([this]() {
        while (running) {
            displayMarquee();  // Continuously update the marquee text
            std::this_thread::sleep_for(std::chrono::milliseconds(150));  // Adjust speed as necessary
        }
        });
}

// Function to stop the marquee
void MarqueeConsole::stopMarquee() {
    running = false;
    if (marqueeThread.joinable()) {
        marqueeThread.join();  // Wait for the thread to finish
    }
}

// Called when the console is enabled
void MarqueeConsole::onEnabled() {
    headerMarquee();
    startMarquee();  // Start the marquee when the console is enabled
}

// Called to update the marquee display
void MarqueeConsole::display() {
    // The actual display is handled in the marquee thread
    headerMarquee();
}

// Handle user input while the marquee is running
void MarqueeConsole::process() {
    std::string input;
    while (running) {
        if (_kbhit()) {  // If a key is pressed
            char ch = _getch();
            if (ch == '\r') {  // Enter key
                std::cout << "\nCommand entered: " << input << std::endl;
                input.clear();
            }
            else if (ch == 27) {  // Escape key to stop marquee
                stopMarquee();
                ConsoleManager::getInstance()->returnToPreviousConsole();
            }
            else {
                input += ch;
                std::cout << ch;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Adjust speed as necessary
    }
}

// Function to display the marquee text at the current position
void MarqueeConsole::displayMarquee() {
    ConsoleManager::getInstance()->setCursorPosition(0, 0);  // Reset cursor position at the top of the console
    std::cout << std::string(consoleWidth, ' ') << "\r";      // Clear the current line
    std::cout << marqueeText.substr(position) << marqueeText.substr(0, position) << std::flush;  // Print the marquee text

    // Move the position, wrap around if necessary
    position = (position + 1) % marqueeText.length();
}

void MarqueeConsole::headerMarquee() {
    std::cout << "*****************************************" << std::endl;
    std::cout << "* Displaying a marquee console! *" << std::endl;
    std::cout << "*****************************************" << std::endl;
}