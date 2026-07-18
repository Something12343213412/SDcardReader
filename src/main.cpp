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

void writeMode(int time){
    Brain.resetTimer();
    uint8_t empty[0];
    extern char (*currentFileSet)[11];

    for(int i = 0; i < sizeof(files)/10; i++){
        Brain.SDcard.savefile(currentFileSet[i], empty, 0); // empty out all files
    }

    setWrites();

    //Controller1.ButtonA.pressed(logAll);

    while (Brain.timer(msec) < time){
        updateInputs();
        printInputs(inputs);
        wait(10, msec);
    }
}


void axis12(){
    extern StateManager currentState;
    Brain.Screen.drawRectangle(390,100,80,80,"#3d3737");
    //Brain.Screen.drawCircle(40,40,40,"#3d3737");
    int x = ((float)currentState.Axis1/100.0)*30 + 430;
    int y = (-(float)currentState.Axis2/100.0)*30 + 140;
    Brain.Screen.drawCircle(x,y,5,"#e9e2e2");
}

void axis34(){
    extern StateManager currentState;
    Brain.Screen.drawRectangle(0,100,80,80,"#3d3737");
    //Brain.Screen.drawCircle(40,40,40,"#3d3737");
    int x = ((float)currentState.Axis4/100.0)*30 + 40;
    int y = (-(float)currentState.Axis3/100.0)*30 + 140;
    Brain.Screen.drawCircle(x,y,5,"#e9e2e2");
}

void buttonA(){
    extern StateManager currentState;
    if (currentState.A)
        Brain.Screen.setFillColor("#00c434");
    else
        Brain.Screen.setFillColor("#5c5c5c");

    Brain.Screen.drawCircle(380,190,15);
    Brain.Screen.printAt(375,190, "A");

}

void buttonB(){
    extern StateManager currentState;
    if (currentState.B)
        Brain.Screen.setFillColor("#00c434");
    else
        Brain.Screen.setFillColor("#5c5c5c");

    Brain.Screen.drawCircle(360,210,15);
    Brain.Screen.printAt(355,210, "B");
}

void buttonX(){
    extern StateManager currentState;
    if (currentState.X)
        Brain.Screen.setFillColor("#00c434");
    else
        Brain.Screen.setFillColor("#5c5c5c");

    Brain.Screen.drawCircle(360,170,15);
    Brain.Screen.printAt(355,170, "X");
}

void buttonY(){
    extern StateManager currentState;
    if (currentState.Y)
        Brain.Screen.setFillColor("#00c434");
    else
        Brain.Screen.setFillColor("#5c5c5c");

    Brain.Screen.drawCircle(340,190,15);
    Brain.Screen.printAt(335,190, "Y");
}

void buttonRight(){
    extern StateManager currentState;
    if (currentState.Right)
        Brain.Screen.setFillColor("#00c434");
    else
        Brain.Screen.setFillColor("#5c5c5c");

    Brain.Screen.drawCircle(100,190,15);
    Brain.Screen.printAt(90,195, "Rt");

}

void buttonDown(){
    extern StateManager currentState;
    if (currentState.Down)
        Brain.Screen.setFillColor("#00c434");
    else
        Brain.Screen.setFillColor("#5c5c5c");

    Brain.Screen.drawCircle(120,210,15);
    Brain.Screen.printAt(110,215, "Dn");
}

void buttonUp(){
    extern StateManager currentState;
    if (currentState.Up)
        Brain.Screen.setFillColor("#00c434");
    else
        Brain.Screen.setFillColor("#5c5c5c");

    Brain.Screen.drawCircle(120,170,15);
    Brain.Screen.printAt(110,175, "Up");
}

void buttonLeft(){
    extern StateManager currentState;
    if (currentState.Left)
        Brain.Screen.setFillColor("#00c434");
    else
        Brain.Screen.setFillColor("#5c5c5c");

    Brain.Screen.drawCircle(140,190,15);
    Brain.Screen.printAt(130,195, "Lt");
}

void buttonL1(){
    extern StateManager currentState;
    if (currentState.L1)
        Brain.Screen.setFillColor("#00c434");
    else
        Brain.Screen.setFillColor("#5c5c5c");
    
    Brain.Screen.drawRectangle(5,5,100,35);
    Brain.Screen.printAt(45,30, "L1");
}

void buttonL2(){
extern StateManager currentState;
    if (currentState.L2)
        Brain.Screen.setFillColor("#00c434");
    else
        Brain.Screen.setFillColor("#5c5c5c");
    
    Brain.Screen.drawRectangle(5,45,100,35);
    Brain.Screen.printAt(45,70, "L2");
}

void buttonR1(){
    extern StateManager currentState;
    if (currentState.R1)
        Brain.Screen.setFillColor("#00c434");
    else
        Brain.Screen.setFillColor("#5c5c5c");
    
    Brain.Screen.drawRectangle(375,5,100,35);
    Brain.Screen.printAt(415,30, "R1");
}

void buttonR2(){
extern StateManager currentState;
    if (currentState.R2)
        Brain.Screen.setFillColor("#00c434");
    else
        Brain.Screen.setFillColor("#5c5c5c");
    
    Brain.Screen.drawRectangle(375,45,100,35);
    Brain.Screen.printAt(415,70, "R2");
}

void auton(){
    extern StateManager currentState;
    extern char (*currentFileSet)[11];
    extern char fileSet1[16][11];
    extern char fileSet2[16][11];

    // loading correct fileset
    if (Brain.SDcard.exists("CurrentState.txt")){
        uint8_t* buffer;
        int fileSize = Brain.SDcard.size("CurrentState.txt");
        buffer = new uint8_t[fileSize];
        Brain.SDcard.loadfile("CurrentState.txt", buffer, fileSize);
        if (strcmp((char*)buffer, "1"))
            currentFileSet = fileSet1;
        else
            currentFileSet = fileSet2;


    }


    std::vector<ButtonInstruction> buttons = {
        ButtonInstruction(currentFileSet[0], &currentState.Axis1, axis12),
        ButtonInstruction(currentFileSet[1], &currentState.Axis2, axis12),
        ButtonInstruction(currentFileSet[2], &currentState.Axis3, axis34),
        ButtonInstruction(currentFileSet[3], &currentState.Axis4, axis34),
        ButtonInstruction(currentFileSet[4], &currentState.Up, buttonUp),
        ButtonInstruction(currentFileSet[5], &currentState.Down, buttonDown),
        ButtonInstruction(currentFileSet[6], &currentState.Right, buttonRight),
        ButtonInstruction(currentFileSet[7], &currentState.Left, buttonLeft),
        ButtonInstruction(currentFileSet[8], &currentState.A, buttonA),
        ButtonInstruction(currentFileSet[9], &currentState.B, buttonB),
        ButtonInstruction(currentFileSet[10], &currentState.X, buttonX),
        ButtonInstruction(currentFileSet[11], &currentState.Y, buttonY),
        ButtonInstruction(currentFileSet[12], &currentState.L1, buttonL1),
        ButtonInstruction(currentFileSet[13], &currentState.L2, buttonL2),
        ButtonInstruction(currentFileSet[12], &currentState.R1, buttonR1),
        ButtonInstruction(currentFileSet[13], &currentState.R2, buttonR2)
    };

    Brain.Screen.setPenColor("#000000");
    Brain.resetTimer();
    Brain.Screen.clearScreen();
    while (true){
        for (int i = 0; i < buttons.size(); i++)
            buttons[i].update();
    }  
}

void driverControl(){

}


int main() {
    competition Competition;

    Competition.autonomous(auton);
    Competition.drivercontrol(driverControl);

    // I think this works (to only run during no competition selection) as there is no 3 second window
    if (Competition.isEnabled()){

        Brain.Screen.print("Write to a file (A)");
        Brain.Screen.newLine();
        Brain.Screen.print("Run a file/set auton (B)");
        Brain.Screen.newLine();
        Brain.Screen.print("Driver Control (X)");

        Controller1.Screen.clearScreen();
        Controller1.Screen.setCursor(1,1);
        Controller1.Screen.print("Write to a file (A)");
        Controller1.Screen.newLine();
        Controller1.Screen.print("Run file/set auton (B)");
        Controller1.Screen.newLine();
        Controller1.Screen.print("Driver Control (X)");

        // while none is pressed wait
        while (!(Controller1.ButtonA.pressing() || Controller1.ButtonB.pressing() || Controller1.ButtonX.pressing())){
            // wait
        }
        
        Brain.Screen.clearScreen();
        Brain.Screen.setCursor(1,1);

        Controller1.Screen.clearScreen();
        Controller1.Screen.setCursor(1,1);

        if (Controller1.ButtonA.pressing()){       
            // here so one A doesn't carry over to the next option
            wait(200, msec);     
            Brain.Screen.print("What File set? 1(A) or 2(B)");
            Controller1.Screen.print("What File set?");
            Controller1.Screen.newLine();
            Controller1.Screen.print("1(A) or 2(B)");
            while (!(Controller1.ButtonA.pressing() || Controller1.ButtonB.pressing())){
                // wait
            }
            
            extern char (*currentFileSet)[11];
            extern char fileSet1[16][11];
            extern char fileSet2[16][11];

            if (Controller1.ButtonA.pressing())
                currentFileSet = fileSet1;
            else
                currentFileSet = fileSet2;

            Brain.resetTimer();
            while (3000-Brain.timer(msec) > 0){
                Brain.Screen.setCursor(1,1);
                Brain.Screen.clearScreen();
                Brain.Screen.print(3000-Brain.timer(msec));

                Controller1.Screen.setCursor(1,1);
                Controller1.Screen.clearScreen();
                Controller1.Screen.print(3000-Brain.timer(msec));
            }
            
            writeMode(15000);
        }

        else if(Controller1.ButtonB.pressing()){
            wait(200, msec);
            Brain.Screen.print("Run auton (A) or set auton file set (B)");

            Controller1.Screen.print("Run auton (A)");
            Controller1.Screen.newLine();
            Controller1.Screen.print("set auton file (B)");
        }
        else if(Controller1.ButtonX.pressing()){
            wait(200, msec);
            driverControl();
        }
    }

    
    
}


