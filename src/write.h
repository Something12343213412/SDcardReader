#pragma once

#include <iostream>
#include <string>
#include <array>
#include <sstream>
#include "vexHardware.h"
#include <sstream>
#include <vector>

// allows to easily change what files are written too
//char (*currentFileSet)[11];

extern brain Brain;
extern controller Controller1;

// yoinked from someone else on stack overflow :)
template <typename T>
std::string to_string(T value);

// holds time and power, only used for the vector in FileInformationHolder
struct TimePowerHolder{
    int time;
    unsigned short power;
    // takes in an int and unsigned short, time is first index
    TimePowerHolder(int time, unsigned short power);
};

// Holds a vector that contains time and power and file name that it will change
class FileInformationHolder{
    public:
        char* fileName;
        std::vector<TimePowerHolder> information; 
        //pointer to linked button on the controller, declared nullptr if goes unused
        controller::button* linkedButton = nullptr;
        // same thing but instead for an axis
        controller::axis* linkedAxis = nullptr;

        // converts vector to string that allows it to be easily written to a file
        uint8_t* toUint8_tString();
        // writes to the file given, call when timer is finished
        void writeToFile();
        // adds an index to the vector based on whether a button or axis is linked and the value of that
        void update();
        // button constructor
        FileInformationHolder(char* fileName, controller::button* linkedButton);
        // axis constructor
        FileInformationHolder(char* fileName, controller::axis* linkedAxis);
};



void writeData(std::string file, int pos, float time);

void writeAxis1();
void writeAxis2();
void writeAxis3();
void writeAxis4();

void writeButtonUp();
void writeButtonDown();
void writeButtonRight();
void writeButtonLeft();
void writeButtonA();
void writeButtonB();
void writeButtonX();
void writeButtonY();
void writeButtonL1();
void writeButtonL2();
void writeButtonR1();
void writeButtonR2();

void setWrites();