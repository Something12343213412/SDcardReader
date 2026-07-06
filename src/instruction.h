#pragma once
#include "write.h"
#include <vector>
#include <algorithm>
#include <functional>


// holds a time value nad a power value, meant for what the brain will actually read
struct Instruction{
    Instruction(int time, short power);
    int time;
    int power;
};
void convertStringToInstruction(std::vector<Instruction>* result, char* string);

