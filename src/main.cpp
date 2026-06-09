/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Kevin                                                     */
/*    Created:      6/9/2026, 9:06:24 AM                                      */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "stickInput.cpp"
#include <iostream>
#include <string>

#include<sstream>
template <typename T>
std::string to_string(T value)
{
    //create an output string stream
    std::ostringstream os ;

    //throw the value into the string stream
    os << value ;

    //convert the string stream into a string and return
    return os.str() ;
}

using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
vex::brain       Brain;

// define your global instances of motors and other devices here
controller Controller1;

void writeData(std::string file, float time){ 
    // 5 is a temp test value
    //std::string position = to_string(Controller1.Axis1.position(percent));
    std::string position = "5";
    std::string timeS = to_string(time); // converts input time to usable string, may not be needed
    
    std::string dataStr = (position + "|" + timeS + ",");
    const char* data = dataStr.c_str(); // create cstring of the combined data, probably more efficient way to do this
    size_t len = dataStr.size();


    uint8_t data1[len]; // initializing data
    memcpy(data1, data, len); // need to copy data to do unit conversion of cstring into uint8_t
    Brain.Screen.print("%d", len);
    Brain.Screen.newLine();
    Brain.SDcard.appendfile(file.c_str(), data1, sizeof(data1));
}



int main() {
    writeData("Text.txt", 10);


    // Add more text and display file size
    if (Brain.SDcard.exists("Text.txt")) {
        // Get the size of the file
        int fileSize = Brain.SDcard.size("Text.txt");

        // Make a buffer to hold the file data
        uint8_t buffer[fileSize];

        // Load the file from the SD card into the buffer
        Brain.SDcard.loadfile("Text.txt", buffer, sizeof(buffer));

        // Display the file contents on the screen
        Brain.Screen.print("%s", "file");
        Brain.Screen.newLine();
        char* data = reinterpret_cast<char*>(buffer);
        //strtok(data, "")
        Brain.Screen.print("%s", buffer);
    
    }
    else{
        Brain.Screen.print("%s", "File doens't exist");
    }
}


