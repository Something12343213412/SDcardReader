/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Kevin                                                     */
/*    Created:      6/9/2026, 9:06:24 AM                                      */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "instruction.h"

// Robot configuration code.
motor rightDTMotorA = motor(PORT9, ratio18_1, false);
motor rightDTMotorB = motor(PORT10, ratio18_1, false);

motor_group rightDT = motor_group(rightDTMotorA, rightDTMotorB);

motor leftDTMotorA = motor(PORT11, ratio18_1, false);
motor leftDTMotorB = motor(PORT12, ratio18_1, false);
motor_group leftDT = motor_group(leftDTMotorA, leftDTMotorB);

double powerMult = 1;

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

// tracks if auton is currently running, used in each callback
bool autonRunning = false;

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
    //extern double powerMult;
    //powerMult = .7;

    Brain.resetTimer();
    uint8_t empty[0];
    extern char (*currentFileSet)[11];

    for(int i = 0; i < sizeof(files)/10; i++){
        Brain.SDcard.savefile(currentFileSet[i], empty, 0); // empty out all files
    }

    setWrites();

    //Controller1.ButtonA.pressed(logAll);

    while (Brain.timer(msec) < time){
        Controller1.Screen.clearScreen();
        Controller1.Screen.setCursor(1,1);
        Controller1.Screen.print(Brain.timer(msec) - time);
        updateInputs();
        printInputs(inputs);
        wait(10, msec);
    }
}


void updateAxis(){
    extern StateManager currentState;

    int x = -currentState.Axis1;
    int y = currentState.Axis2;
    

    double leftVolts = -y;
    double rightVolts = y;
  
    if (y<0) {
        //x *= -1;
    }
    if (x >= 0) {
        leftVolts = leftVolts - x;
        rightVolts = rightVolts - x;
    } else {
        rightVolts = rightVolts - x;
        leftVolts = leftVolts - x;
    }

    if (abs(rightVolts) < 5){
        rightDT.stop();
        rightVolts = 0;
    }
    if (abs(leftVolts) < 5){
        leftDT.stop();
        leftVolts = 0;
    }
    
    rightVolts *= powerMult;
    leftVolts *= powerMult;

    rightDT.spin(forward, rightVolts, vex::velocityUnits::pct);
    leftDT.spin(forward, leftVolts, vex::velocityUnits::pct);
    
    //backLeft1.spin(forward, leftVolts, vex::voltageUnits::volt);
    //backLeft2.spin(forward, leftVolts, vex::voltageUnits::volt);
}

void axis12(){
    extern bool autonRunning;
    extern StateManager currentState;
    if(!autonRunning){
        currentState.Axis1 = Controller1.Axis1.position(percent);
        currentState.Axis2 = Controller1.Axis2.position(percent);
    }
    updateAxis();

    extern StateManager currentState;
    Brain.Screen.drawRectangle(390,100,80,80,"#3d3737");
    //Brain.Screen.drawCircle(40,40,40,"#3d3737");
    int x = ((float)currentState.Axis1/100.0)*30 + 430;
    int y = (-(float)currentState.Axis2/100.0)*30 + 140;
    Brain.Screen.drawCircle(x,y,5,"#e9e2e2");
}

void axis34(){
    extern StateManager currentState;
    if(!autonRunning){
        currentState.Axis3 = Controller1.Axis3.position(percent);
        currentState.Axis4 = Controller1.Axis4.position(percent);
    }
    updateAxis();
    extern StateManager currentState;
    Brain.Screen.drawRectangle(0,100,80,80,"#3d3737");
    //Brain.Screen.drawCircle(40,40,40,"#3d3737");
    int x = ((float)currentState.Axis4/100.0)*30 + 40;
    int y = (-(float)currentState.Axis3/100.0)*30 + 140;
    Brain.Screen.drawCircle(x,y,5,"#e9e2e2");
}

void buttonA(){
    extern StateManager currentState;
    if(!autonRunning){
        currentState.A = Controller1.ButtonA.pressing();
    }
    if (currentState.A)
        Brain.Screen.setFillColor("#00c434");
    else
        Brain.Screen.setFillColor("#5c5c5c");

    Brain.Screen.drawCircle(380,190,15);
    Brain.Screen.printAt(375,190, "A");

}

void buttonB(){
    extern StateManager currentState;
    if(!autonRunning){
        currentState.B = Controller1.ButtonB.pressing();
    }
    if (currentState.B)
        Brain.Screen.setFillColor("#00c434");
    else
        Brain.Screen.setFillColor("#5c5c5c");

    Brain.Screen.drawCircle(360,210,15);
    Brain.Screen.printAt(355,210, "B");
}

void buttonX(){
    extern StateManager currentState;
    if(!autonRunning){
        currentState.X = Controller1.ButtonX.pressing();
    }
    if (currentState.X)
        Brain.Screen.setFillColor("#00c434");
    else
        Brain.Screen.setFillColor("#5c5c5c");

    Brain.Screen.drawCircle(360,170,15);
    Brain.Screen.printAt(355,170, "X");
}

void buttonY(){
    extern StateManager currentState;
    if(!autonRunning){
        currentState.Y = Controller1.ButtonY.pressing();
    }
    if (currentState.Y)
        Brain.Screen.setFillColor("#00c434");
    else
        Brain.Screen.setFillColor("#5c5c5c");

    Brain.Screen.drawCircle(340,190,15);
    Brain.Screen.printAt(335,190, "Y");
}

void buttonRight(){
    extern StateManager currentState;
    if(!autonRunning){
        currentState.Right = Controller1.ButtonRight.pressing();
    }
    if (currentState.Right)
        Brain.Screen.setFillColor("#00c434");
    else
        Brain.Screen.setFillColor("#5c5c5c");

    Brain.Screen.drawCircle(100,190,15);
    Brain.Screen.printAt(90,195, "Rt");

}

void buttonDown(){
    extern StateManager currentState;
    if(!autonRunning){
        currentState.Down = Controller1.ButtonDown.pressing();
    }
    if (currentState.Down)
        Brain.Screen.setFillColor("#00c434");
    else
        Brain.Screen.setFillColor("#5c5c5c");

    Brain.Screen.drawCircle(120,210,15);
    Brain.Screen.printAt(110,215, "Dn");
}

void buttonUp(){
    extern StateManager currentState;
    if(!autonRunning){
        currentState.Up = Controller1.ButtonUp.pressing();
    }
    if (currentState.Up)
        Brain.Screen.setFillColor("#00c434");
    else
        Brain.Screen.setFillColor("#5c5c5c");

    Brain.Screen.drawCircle(120,170,15);
    Brain.Screen.printAt(110,175, "Up");
}

void buttonLeft(){
    extern StateManager currentState;
    if(!autonRunning){
        currentState.Left = Controller1.ButtonLeft.pressing();
    }
    if (currentState.Left)
        Brain.Screen.setFillColor("#00c434");
    else
        Brain.Screen.setFillColor("#5c5c5c");

    Brain.Screen.drawCircle(140,190,15);
    Brain.Screen.printAt(130,195, "Lt");
}

void buttonL1(){
    extern StateManager currentState;
    if(!autonRunning){
        currentState.L1 = Controller1.ButtonL1.pressing();
    }
    if (currentState.L1)
        Brain.Screen.setFillColor("#00c434");
    else
        Brain.Screen.setFillColor("#5c5c5c");
    
    Brain.Screen.drawRectangle(5,5,100,35);
    Brain.Screen.printAt(45,30, "L1");
}

void buttonL2(){
    extern StateManager currentState;
    if(!autonRunning){
        currentState.L2 = Controller1.ButtonL2.pressing();
    }
    if (currentState.L2)
        Brain.Screen.setFillColor("#00c434");
    else
        Brain.Screen.setFillColor("#5c5c5c");
    
    Brain.Screen.drawRectangle(5,45,100,35);
    Brain.Screen.printAt(45,70, "L2");
}

void buttonR1(){
    extern StateManager currentState;
    if(!autonRunning){
        currentState.R1 = Controller1.ButtonR1.pressing();
    }
    if (currentState.R1)
        Brain.Screen.setFillColor("#00c434");
    else
        Brain.Screen.setFillColor("#5c5c5c");
    
    Brain.Screen.drawRectangle(375,5,100,35);
    Brain.Screen.printAt(415,30, "R1");
}

void buttonR2(){
    extern StateManager currentState;
    if(!autonRunning){
        currentState.R2 = Controller1.ButtonR2.pressing();
    }
    if (currentState.R2)
        Brain.Screen.setFillColor("#00c434");
    else
        Brain.Screen.setFillColor("#5c5c5c");
    
    Brain.Screen.drawRectangle(375,45,100,35);
    Brain.Screen.printAt(415,70, "R2");
}


bool normalRun = false;

void auton(){
    extern bool autonRunning;
    extern StateManager currentState;
    extern char (*currentFileSet)[11];
    extern char fileSet1[16][11];
    extern char fileSet2[16][11];

    //extern double powerMult;
    //powerMult =.7;

    autonRunning = true;

    // loading correct fileset
    if (Brain.SDcard.exists("CurrentState.txt")){
        uint8_t* buffer;
        int fileSize = Brain.SDcard.size("CurrentState.txt");
        printf("test \n");
        buffer = new uint8_t[fileSize+1];
        Brain.SDcard.loadfile("CurrentState.txt", buffer, fileSize);
        buffer[fileSize] = '\0';
        printf("%s", buffer);
        printf("\n");
        if (!strcmp((char*)buffer, "1"))
            currentFileSet = fileSet1;
        else
            currentFileSet = fileSet2;
        printf("test \n");

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
    // can change the time, defauly is just 15 seconds though
    while (Brain.timer(msec) < 15000){
        for (int i = 0; i < buttons.size(); i++)
            buttons[i].update();
    }  

    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1,1);
    Controller1.Screen.print(rightDT.position(rotationUnits::rev));
    Controller1.Screen.newLine();
    Controller1.Screen.print(leftDT.position(rotationUnits::rev));


    while (!Controller1.ButtonL1.pressing()){
        leftDT.stop();
        rightDT.stop();
    }

    /*

    std::vector<ButtonInstruction> newButtons = {
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

    for (int i = newButtons.size()-1; i>-1; i--){
        newButtons[i].flipInstruction();
    }

    Brain.resetTimer();
    while (Brain.timer(msec) < 15000){
        for (int i = 0; i < buttons.size(); i++)
            buttons[i].update();
    }  
    */
}

competition Competition;

void driverControl(){
    if (!autonRunning){
        Controller1.Axis1.changed(axis12);
        Controller1.Axis2.changed(axis12);
        Controller1.Axis3.changed(axis34);
        Controller1.Axis4.changed(axis34);
        Controller1.ButtonA.pressed(buttonA);
        Controller1.ButtonB.pressed(buttonB);
        Controller1.ButtonX.pressed(buttonX);
        Controller1.ButtonY.pressed(buttonY);
        Controller1.ButtonUp.pressed(buttonUp);
        Controller1.ButtonDown.pressed(buttonDown);
        Controller1.ButtonRight.pressed(buttonRight);
        Controller1.ButtonLeft.pressed(buttonLeft);
        Controller1.ButtonR1.pressed(buttonR1);
        Controller1.ButtonR2.pressed(buttonR2);
        Controller1.ButtonL1.pressed(buttonL1);
        Controller1.ButtonL2.pressed(buttonL2);
        Controller1.ButtonA.released(buttonA);
        Controller1.ButtonB.released(buttonB);
        Controller1.ButtonX.released(buttonX);
        Controller1.ButtonY.released(buttonY);
        Controller1.ButtonUp.released(buttonUp);
        Controller1.ButtonDown.released(buttonDown);
        Controller1.ButtonRight.released(buttonRight);
        Controller1.ButtonLeft.released(buttonLeft);
        Controller1.ButtonR1.released(buttonR1);
        Controller1.ButtonR2.released(buttonR2);
        Controller1.ButtonL1.released(buttonL1);
        Controller1.ButtonL2.released(buttonL2);
    }
}

int main() {
    rightDT.setStopping(brake);
    leftDT.setStopping(brake);
    extern bool normalRun;

    Competition.autonomous(auton);
    Competition.drivercontrol(driverControl);
    
    Brain.Screen.setFillColor("#000000");
    //Brain.Screen.setPenColor("#ffffff");

    // I think this works (to only run during no competition selection) as there is no 3 second window
    if (Competition.isEnabled()){
        normalRun = true;
        Controller1.Screen.clearScreen();
        Brain.Screen.clearScreen();
        Controller1.Screen.setCursor(1,1);
        Brain.Screen.setCursor(1,1);

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
            driverControl();
            writeMode(15000);
        }

        else if(Controller1.ButtonB.pressing()){
            wait(200, msec);
            Brain.Screen.print("Run auton (A) or set auton file set (B)");

            Controller1.Screen.print("Run auton (A)");
            Controller1.Screen.newLine();
            Controller1.Screen.print("set auton file (B)");

            while (!(Controller1.ButtonA.pressing() || Controller1.ButtonB.pressing())){
                // wait
            }

            Controller1.Screen.clearScreen();
            Brain.Screen.clearScreen();
            Controller1.Screen.setCursor(1,1);
            Brain.Screen.setCursor(1,1);

            if (Controller1.ButtonA.pressing())
                auton();
            else{
                wait(200, msec);
                
                Brain.Screen.print("File set A(A) or B(B)");
                Controller1.Screen.print("File set A(A) or B(B)");

                while (!(Controller1.ButtonA.pressing() || Controller1.ButtonB.pressing())){
                // wait
                }

                Controller1.Screen.clearScreen();
                Brain.Screen.clearScreen();
                Controller1.Screen.setCursor(1,1);
                Brain.Screen.setCursor(1,1);
                    
                if(Controller1.ButtonA.pressing()){
                    uint8_t data[2];
                    memcpy(data, "1\0", 2);
                    Brain.SDcard.savefile("CurrentState.txt", data, 1);
                    Brain.Screen.print("File set 1 selected");
                    Controller1.Screen.print("File set 1 selected");
                }
                else{
                    uint8_t data[2];
                    memcpy(data, "2\0", 2);
                    Brain.SDcard.savefile("CurrentState.txt", data, 2);
                    Brain.Screen.print("File set 2 selected");
                    Controller1.Screen.print("File set 2 selected");
                }
                wait(1000, msec);


            }
                

        }
        else if(Controller1.ButtonX.pressing()){
            wait(200, msec);
            driverControl();
        }
    }

    
    

    
    
}


