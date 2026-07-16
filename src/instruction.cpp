#include "instruction.h"

// contains a list of Button Instructions that will later be looped through and updates called
std::vector<ButtonInstruction*> buttonInstructions;

Instruction::Instruction(int time, int power){
    this->time = time;
    this->power = power;
}

// takes in a vector pointer and a string, that string gets seperated and put into the vector
void convertStringToInstruction(std::vector<Instruction>* result, char* string){
    // the code on c++ ref used pch so just kept it, no idea what it stands for
    char * pch;
    // stores two values, tbf could optimize it more by using smaller datatype then int
    int val1;
    int val2;
    // start with splitting the string, done before to prevent immediate null exiting loop
    pch = strtok (string,"|,");
    while (pch != NULL)
    {
        // store value then repeat
        val1 = atoi(pch);
        pch = strtok (NULL, "|,");
        val2 = atoi(pch);
        // want to make sure not doing a null ptr
        if(pch!=NULL)
            pch = strtok (NULL, "|,");
        
        // ad the values to a vector that houses the instructions
        result->push_back(Instruction(val2, val1));   
    }
    
    // adds it in reverse (if we start by indexing the array with using .size) so reversing it ahead of time
    //std::reverse(result->begin(), result->end()); Think this is unnescary
}

void ButtonInstruction::update(std::function<void ()>){
    extern brain Brain;
    // if facing preformance issues could just create a local copy of last index of instructions so much less calls have to be made

    // checks if the current time is past or = to the time in instructions
    if(Brain.timer(msec) >= this->instructions[this->instructions.size()-1].time){
        // set the current state value to be the last thing instructions
        *this->stateValue = (bool)this->instructions[this->instructions.size()-1].power;
        
        // debug statements, idk how to use printf, don't judge me
        printf("Current state is %d", (bool)this->instructions[this->instructions.size()-1].power);
        printf("\n Current time is %d", Brain.timer(msec));
        printf("\n Instruction time was %d",this->instructions[this->instructions.size()-1].time);
        printf("\n");

        // delete the instruction that was just read
        this->instructions.pop_back();
    }
}

ButtonInstruction::ButtonInstruction(char* charPointer, bool* stateValue){
    extern std::vector<ButtonInstruction*> buttonInstructions;
    this->stateValue = stateValue;
    convertStringToInstruction(&instructions, charPointer);
    buttonInstructions.push_back(this);   
}

