#pragma once
#include "write.h"
#include <vector>
#include <algorithm>
#include <functional>

extern brain Brain;

// holds a time value nad a power value, meant for what the brain will actually read
struct Instruction{
    Instruction(int time, int power);
    int time;
    int power;
};

// takes vector pointer and takes a string of data from a file and then puts all that data into the vector
void convertStringToInstruction(std::vector<Instruction>* result, char* string);

// holds the button instructions and the update function which is then put into a global array
class ButtonInstruction{
    std::vector<Instruction> instructions;
    char* button;
    void* callback;
    void* update();
    ButtonInstruction(char* fileName, char* button, void* callback);
};

// contains a list of Button Instructions that will later be looped through and updates called
std::vector<ButtonInstruction*> updateButtons;

