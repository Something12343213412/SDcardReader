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

void printFile(char* file = "Text.txt"){
if (Brain.SDcard.exists(file)) {
        // Get the size of the file
        int fileSize = Brain.SDcard.size(file);

        // Make a buffer to hold the file data
        uint8_t buffer[fileSize+1];
        buffer[fileSize] = '\0'; // terminate string

        // Load the file from the SD card into the buffer
        Brain.SDcard.loadfile(file, buffer, sizeof(buffer));

        char* data = reinterpret_cast<char*>(buffer);
        Brain.Screen.print("%s", buffer);
        Brain.Screen.newLine();
    
    }
    else{
        Brain.Screen.print("%s", "File doens't exist");
    }
}

void writeData(std::string file, int pos, float time){ 
    // 5 is a temp test value
    std::string position = to_string(pos);
    //std::string position = "5";
    std::string timeS = to_string(time); // converts input time to usable string, may not be needed
    
    
    std::string dataStr = (position + "|" + timeS + ",");
    size_t len = dataStr.size();


    uint8_t data[len]; // initializing data
    memcpy(data, dataStr.c_str(), len); // need to copy data to do unit conversion of cstring into uint8_t
    Brain.SDcard.appendfile(file.c_str(), data, len);
    Brain.Screen.setCursor(1,1);
    Brain.Screen.clearLine(1);
    Brain.Screen.print("%s", data);
    //printFile("Axis1.txt");

}

void logData(char* file){
    if (Brain.SDcard.exists(file)) {
        // Get the size of the file
        int fileSize = Brain.SDcard.size(file);

        uint8_t buffer[fileSize+1];
        buffer[fileSize] = '\0'; // adding end code

        Brain.SDcard.loadfile(file, buffer, sizeof(buffer)); // actually load file
        char* data = reinterpret_cast<char*>(buffer); // probably not needed
        printf("%s", "\n \n AXIS 1 \n");
        printf("%s", buffer);
        printf("%s", "\n"); // make sure the previous is outputted
    }
    else{
        printf("%s Doesn't exist", file);
    }
}

void logAll(){
    char* files[] = {"Axis1.txt", "Axis2.txt", "Axis3.txt", "Axis4.txt"};
    for(int i = 0; i < sizeof(files); i++){
        logData(files[i]); // log all files in list
    }
}


void writeAxis1(){
    writeData("Axis1.txt", Controller1.Axis1.position(percent), Brain.timer(msec));
}

void writeAxis2(){
    writeData("Axis2.txt", Controller1.Axis2.position(percent), Brain.timer(msec));
}

void writeAxis3(){
    writeData("Axis3.txt", Controller1.Axis3.position(percent), Brain.timer(msec));
}

void writeAxis4(){
    writeData("Axis4.txt", Controller1.Axis4.position(percent), Brain.timer(msec));
}

int main() {
    uint8_t empty[0];

    Brain.SDcard.savefile("Axis1.txt", empty, 0);
    Brain.SDcard.savefile("Axis2.txt", empty, 0);
    Brain.SDcard.savefile("Axis3.txt", empty, 0);
    Brain.SDcard.savefile("Axis4.txt", empty, 0);

    
    
    Controller1.Axis1.changed(writeAxis1);
    Controller1.Axis2.changed(writeAxis2);
    Controller1.Axis3.changed(writeAxis3);
    Controller1.Axis4.changed(writeAxis4);



    Controller1.ButtonA.pressed(logAll);
}


