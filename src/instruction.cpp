#include "instruction.h"

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
    std::reverse(result->begin(), result->end());
}

void* ButtonInstruction::update(){

}

ButtonInstruction::ButtonInstruction(char* fileName, char* button, void* callback){
    convertStringToInstruction(&instructions, fileName);
    this->button = button;
    this->callback = callback; 
    updateButtons.push_back(this);
}

