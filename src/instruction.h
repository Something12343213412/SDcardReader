#pragma once
#include "write.h"
#include <vector>
#include <algorithm>
#include <functional>
#include "stateManager.h"

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
    public:
        bool* stateValue;
        // contains vectorized list of what this should do
        std::vector<Instruction> instructions;
        // updates the global state, if did pops instruction then runs callback
        void update(std::function<void ()> callback);
        // takes in a button and the data of a file and the state that it effects
        ButtonInstruction(char* charPointer, bool* stateValue);
        // used for debugging
        void PrintInstruction();
};



