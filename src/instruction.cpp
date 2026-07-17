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

        /*
        // debug statements
        printf("val 1 is %d", val1);
        printf("\n val 2 is %d", val2);
        printf("\n");
        */
        // add the values to a vector that houses the instructions
        result->push_back(Instruction(val2, val1));   
    }
    
    // adds it in reverse (if we start by indexing the array with using .size) so reversing it ahead of time
    std::reverse(result->begin(), result->end());
}

void ButtonInstruction::update(std::function<void ()> callback){
    extern brain Brain;
    // if facing preformance issues could just create a local copy of last index of instructions so much less calls have to be made

    // checks if the current time is past or = to the time in instructions
    if((int)Brain.timer(msec) >= this->instructions[this->instructions.size()-1].time){
        // set the current state value to be the last thing instructions
        *this->stateValue = (bool)this->instructions[this->instructions.size()-1].power;
            
        // debug statements, idk how to use printf, don't judge me
        printf("Current state is %d", (bool)this->instructions[this->instructions.size()-1].power);
        printf("\n Current time is %f", Brain.timer(msec));
        printf("\n Instruction time was %d",this->instructions[this->instructions.size()-1].time);
        printf("\n");

        callback();
        // delete the instruction that was just read
        this->instructions.pop_back();
    }
}

ButtonInstruction::ButtonInstruction(char* fileName, bool* stateValue){
    int fileSize = Brain.SDcard.size(fileName);
    uint8_t buffer[fileSize+1];
    buffer[fileSize] = '\0'; // adding end code

    Brain.SDcard.loadfile(fileName, buffer, sizeof(buffer));
    char* charPointer = (char*)buffer;

    this->stateValue = stateValue;
    convertStringToInstruction(&this->instructions, charPointer);
    // this is not an ideal solution at all and idk really know why it is needed but w/o it update just skips over last index
    this->instructions.insert(this->instructions.begin(), Instruction(999999999,0));
}

void ButtonInstruction::PrintInstruction(){
    for (int i = this->instructions.size()-1; i > -1; i--){
        printf("%d", this->instructions[i].power);
        printf("%s", " | ");
        printf("%d", this->instructions[i].time);
        printf("\n");
    }
}