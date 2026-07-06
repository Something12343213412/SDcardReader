/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Kevin                                                     */
/*    Created:      6/9/2026, 9:06:24 AM                                      */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "instruction.h"

// list of files, make sure they line up with the list in log all
char files[][10] = 
{   "Axis1.txt", "Axis2.txt", "Axis3.txt", "Axis4.txt", 
    "BtnUp.txt", "BtnDn.txt", "BtnRt.txt", "BtnLt.txt",
    "BtnAA.txt", "BtnBB.txt", "BtnXX.txt", "BtnYY.txt",
    "BtnL1.txt", "BtnL2.txt", "BtnR1.txt", "BtnR2.txt"
};

// Files without txt ending
char files_2[][10] = 
{   "Axis1", "Axis2", "Axis3", "Axis4", 
    "BtnUp", "BtnDn", "BtnRt", "BtnLt",
    "BtnAA", "BtnBB", "BtnXX", "BtnYY",
    "BtnL1", "BtnL2", "BtnR1", "BtnR2"
};

// each index represents the value of a stick
std::array<int, 16> inputs = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

// will be very innefficient bc many times things will be called with no changes but should be fine
void updateInputs(){
    // axis 1, 2, 3, 4 ... same order as files
    
    inputs = {Controller1.Axis1.position(percent), Controller1.Axis2.position(percent),
         Controller1.Axis3.position(percent), Controller1.Axis4.position(percent),
         Controller1.ButtonUp.pressing(), Controller1.ButtonDown.pressing(),
         Controller1.ButtonRight.pressing(), Controller1.ButtonLeft.pressing(),
         Controller1.ButtonA.pressing(), Controller1.ButtonB.pressing(),
         Controller1.ButtonX.pressing(), Controller1.ButtonY.pressing(),
         Controller1.ButtonL1.pressing(), Controller1.ButtonL2.pressing(),
         Controller1.ButtonR1.pressing(), Controller1.ButtonR2.pressing()};
}

void printInputs(std::array<int, 16> values){
    Brain.Screen.clearLine(1);
    Brain.Screen.setCursor(1,1);
    for (int i = 0; i < 16; i++){
        Brain.Screen.print(files_2[i]);
        Brain.Screen.print(" ");
        Brain.Screen.print(values[i]);
        if (i%2){
            Brain.Screen.newLine();
            Brain.Screen.clearLine(i+1);
        }
        else
            Brain.Screen.print("   ");
    }
}



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

void writeMode(){
    uint8_t empty[0];

    for(int i = 0; i < sizeof(files)/10; i++){
        Brain.SDcard.savefile(files[i], empty, 0); // empty out all files
    }

    setWrites();

    Controller1.ButtonA.pressed(logAll);

    while (true){
        updateInputs();
        printInputs(inputs);
        wait(10, msec);
    }
}

//returns pointer of readfile, make sure to make a copy of the pointer as this data is probably going to be deleted
void readFile(uint8_t* buffer, char* file){
    int fileSize = Brain.SDcard.size(file);

    buffer = new uint8_t[fileSize+1];
    buffer[fileSize] = '\0'; // adding end code

    Brain.SDcard.loadfile(file, buffer, sizeof(buffer));
}


int main() {
    //writeMode();

    int fileSize = Brain.SDcard.size("Axis1.txt");
    uint8_t buffer[fileSize+1];
    buffer[fileSize] = '\0'; // adding end code

    Brain.SDcard.loadfile("Axis1.txt", buffer, sizeof(buffer));
    char* charPointer = (char*)buffer;

    std::vector<Instruction> result;
    convertStringToInstruction(&result, charPointer);

    for (int i = result.size()-1; i > -1; i--){
        printf("%d", result[i].power);
        printf("%s", " | ");
        printf("%d", result[i].time);
        printf("\n");
    }

    //printf("%s", buffer);
}


