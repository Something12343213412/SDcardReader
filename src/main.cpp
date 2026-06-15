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
// stolen from somewhere on stack overflow
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

// stole this code from the vex site I think
void printFile(char* file = "Text.txt"){
if (Brain.SDcard.exists(file)) {
        // Get the size of the file
        int fileSize = Brain.SDcard.size(file);

        // Make a buffer to hold the file data
        uint8_t buffer[fileSize+1];
        buffer[fileSize] = '\0'; // terminate string

        // Load the file from the SD card into the buffer
        Brain.SDcard.loadfile(file, buffer, sizeof(buffer));

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
        printf("\n %s \n", file);
        printf("%s", buffer);
        printf("\n"); // make sure the previous is outputted
    }
    else{
        printf("%s Doesn't exist", file);
    }
}

void logAll(){
    char files[][10] = 
    {   "Axis1.txt", "Axis2.txt", "Axis3.txt", "Axis4.txt", 
        "BtnUp.txt", "BtnDn.txt", "BtnRt.txt", "BtnLt.txt",
        "BtnAA.txt", "BtnBB.txt", "BtnXX.txt", "BtnYY.txt",
        "BtnL1.txt", "BtnL2.txt", "BtnR1.txt", "BtnR2.txt"
    };
    
    
    // we take total number of bytes then /10 as each thing has 10 bytes to get total size
    for(int i = 0; i < sizeof(files)/10; i++){
        logData(files[i]);
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

void writeButtonUp(){
    // since size of the file matters that is why there is weird naming scheme, could make better system but idc
    writeData("BtnUp.txt", Controller1.ButtonUp.pressing(), Brain.timer(msec));
}

void writeButtonDown(){
    writeData("BtnDn.txt", Controller1.ButtonDown.pressing(), Brain.timer(msec));
}

void writeButtonRight(){
    writeData("BtnRt.txt", Controller1.ButtonRight.pressing(), Brain.timer(msec));
}

void writeButtonLeft(){
    writeData("BtnLt.txt", Controller1.ButtonLeft.pressing(), Brain.timer(msec));
}

void writeButtonA(){
    writeData("BtnAA.txt", Controller1.ButtonA.pressing(), Brain.timer(msec));
}

void writeButtonB(){
    writeData("BtnBB.txt", Controller1.ButtonB.pressing(), Brain.timer(msec));
}

void writeButtonX(){
    writeData("BtnXX.txt", Controller1.ButtonX.pressing(), Brain.timer(msec));
}

void writeButtonY(){
    writeData("BtnYY.txt", Controller1.ButtonY.pressing(), Brain.timer(msec));
}

void writeButtonL1(){
    writeData("BtnL1.txt", Controller1.ButtonL1.pressing(), Brain.timer(msec));
}

void writeButtonL2(){
    writeData("BtnL2.txt", Controller1.ButtonL2.pressing(), Brain.timer(msec));
}

void writeButtonR1(){
    writeData("BtnR1.txt", Controller1.ButtonR1.pressing(), Brain.timer(msec));
}

void writeButtonR2(){
    writeData("BtnR2.txt", Controller1.ButtonR2.pressing(), Brain.timer(msec));
}

int main() {
    uint8_t empty[0];

    // list of files, make sure they line up with the list in log all
    char files[][10] = 
    {   "Axis1.txt", "Axis2.txt", "Axis3.txt", "Axis4.txt", 
        "BtnUp.txt", "BtnDn.txt", "BtnRt.txt", "BtnLt.txt",
        "BtnAA.txt", "BtnBB.txt", "BtnXX.txt", "BtnYY.txt",
        "BtnL1.txt", "BtnL2.txt", "BtnR1.txt", "BtnR2.txt"
    };

    for(int i = 0; i < sizeof(files)/10; i++){
        Brain.SDcard.savefile(files[i], empty, 0); // empty out all files
    }

    Controller1.Axis1.changed(writeAxis1);
    Controller1.Axis2.changed(writeAxis2);
    Controller1.Axis3.changed(writeAxis3);
    Controller1.Axis4.changed(writeAxis4);

    // I hate this solution, I spent so much time trying to pass in callbacks automatically generated but apparently you can't pass in a function from a this pointer which makes sense but makes me angry
    Controller1.ButtonUp.pressed(writeButtonUp);
    Controller1.ButtonUp.released(writeButtonUp);

    Controller1.ButtonDown.pressed(writeButtonDown);
    Controller1.ButtonDown.released(writeButtonDown);

    Controller1.ButtonRight.pressed(writeButtonRight);
    Controller1.ButtonRight.released(writeButtonRight);

    Controller1.ButtonLeft.pressed(writeButtonLeft);
    Controller1.ButtonLeft.released(writeButtonLeft);

    //Controller1.ButtonA.pressed(writeButtonA);
    //Controller1.ButtonA.released(writeButtonA);

    Controller1.ButtonB.pressed(writeButtonB);
    Controller1.ButtonB.released(writeButtonB);

    Controller1.ButtonX.pressed(writeButtonX);
    Controller1.ButtonX.released(writeButtonX);

    Controller1.ButtonY.pressed(writeButtonY);
    Controller1.ButtonY.released(writeButtonY);

    Controller1.ButtonL1.pressed(writeButtonL1);
    Controller1.ButtonL1.released(writeButtonL1);

    Controller1.ButtonL2.pressed(writeButtonL2);
    Controller1.ButtonL2.released(writeButtonL2);

    Controller1.ButtonR1.pressed(writeButtonR1);
    Controller1.ButtonR1.released(writeButtonR1);

    Controller1.ButtonR2.pressed(writeButtonR2);
    Controller1.ButtonR2.released(writeButtonR2);



    Controller1.ButtonA.pressed(logAll);
}


