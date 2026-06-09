/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Kevin                                                     */
/*    Created:      6/9/2026, 9:06:24 AM                                      */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"

using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
vex::brain       Brain;

// define your global instances of motors and other devices here


int main() {
    // Add more text and display file size
    if (Brain.SDcard.exists("Text.txt")) {
    // Get the size of the file
    int fileSize = Brain.SDcard.size("Text.txt");

    // Make a buffer to hold the file data
    uint8_t buffer[fileSize];

    // Load the file from the SD card into the buffer
    Brain.SDcard.loadfile("Text.txt", buffer, sizeof(buffer));

    // Display the file contents on the screen
    Brain.Screen.print("%s", buffer);
    }
}
