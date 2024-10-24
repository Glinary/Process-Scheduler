#pragma once

#include "AConsole.h"
#include <string>
#include <thread>
#include <atomic>

class MarqueeConsole : public AConsole {
public:
    MarqueeConsole();           // Constructor
    ~MarqueeConsole();          // Destructor to stop the marquee when destroyed

    // Override AConsole pure virtual methods
    void onEnabled() override;  // Called when the console is enabled/shown
    void display() override;    // Called to update the screen
    void process() override;    // Called to handle input from the user

private:
    std::atomic<bool> running;              // To control the running state of the marquee thread
    std::string marqueeText;                // Text to display in the marquee
    int position;                           // Current position of the marquee text
    int consoleWidth;                       // Width of the console to manage text wrapping
    std::thread marqueeThread;              // Thread to handle marquee animation

    void startMarquee();                    // Function to start the marquee animation
    void stopMarquee();                     // Function to stop the marquee animation
    void displayMarquee();                  // Actual function that handles the marquee animation
    void headerMarquee();
};
