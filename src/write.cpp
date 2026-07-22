#include "write.h"

extern brain Brain;

char fileSet1[16][11] = 
{   "1Axis1.txt", "1Axis2.txt", "1Axis3.txt", "1Axis4.txt", 
    "1BtnUp.txt", "1BtnDn.txt", "1BtnRt.txt", "1BtnLt.txt",
    "1BtnAA.txt", "1BtnBB.txt", "1BtnXX.txt", "1BtnYY.txt",
    "1BtnL1.txt", "1BtnL2.txt", "1BtnR1.txt", "1BtnR2.txt"
};

char fileSet2[16][11] = 
{   "2Axis1.txt", "2Axis2.txt", "2Axis3.txt", "2Axis4.txt", 
    "2BtnUp.txt", "2BtnDn.txt", "2BtnRt.txt", "2BtnLt.txt",
    "2BtnAA.txt", "2BtnBB.txt", "2BtnXX.txt", "2BtnYY.txt",
    "2BtnL1.txt", "2BtnL2.txt", "2BtnR1.txt", "2BtnR2.txt"
};

TimePowerHolder::TimePowerHolder(int time, unsigned short power){
    this->power = power;
    this->time = time;
}

FileInformationHolder::FileInformationHolder(char* fileName, controller::button linkedButton){
    this->fileName = fileName;
    this->linkedButton = &linkedButton;
}

FileInformationHolder::FileInformationHolder(char* fileName, controller::axis linkedAxis){
    this->fileName = fileName;
    this->linkedAxis = &linkedAxis;
}

void FileInformationHolder::update(){
    // checking if there is a linked button, if so run button code
    if (this->linkedButton != nullptr)
        this->information.push_back(TimePowerHolder((int)Brain.timer(msec), this->linkedButton->pressing()));
    else if (this->linkedAxis != nullptr)
        this->information.push_back(TimePowerHolder((int)Brain.timer(msec), this->linkedAxis->position(percent)));
    else
        printf("No button or axis is linked \n");
}

void FileInformationHolder::toUint8_tString(uint8_t* inputString){
    // error handling
    if (this->information.size() == 0 ){
        printf("empty string \n");
        return ;
    }

    // define a c-string that can just later memcopy, I'm not famaliar enough with uint8_t to just do stuff with them directly
    std::string container = "";
    for(auto a : this->information){
        // I don't want to hear abt this, I'm doing this at night
        container.append(to_string(a.power));
        container.append("|");
        container.append(to_string(a.time));
        container.append(",");
    }

    // finding size of the container
    size_t len = container.size();
    // making it a new pointer, watch out, it may cause issues
    inputString = new uint8_t[len+2];
    memcpy(inputString, container.c_str(), len);
    // adding end code, not sure how I would do this in one line, also probably more effective way to do this
    inputString[len] = uint8_t(255);
    inputString[len] = uint8_t('|');
}

void FileInformationHolder::writeToFile(){
    uint8_t* data;
    this->toUint8_tString(data);
    // sizeof may be wrong thing to call here
    Brain.SDcard.appendfile(this->fileName, data, sizeof(data));
}



char (*currentFileSet)[11] = fileSet1;

// stolen from somewhere on stack overflow
template <typename T>
std::string to_string(T value)
{
    std::ostringstream os;
    os << value;
    return os.str();
}

template std::string to_string<int>(int);
template std::string to_string<float>(float);

void writeData(std::string file, int pos, float time){
    std::string position = to_string(pos);
    std::string timeS = to_string(time);

    std::string dataStr = (position + "|" + timeS + ",");
    size_t len = dataStr.size();

    uint8_t data[len];
    memcpy(data, dataStr.c_str(), len);
    Brain.SDcard.appendfile(file.c_str(), data, len);
}

void writeAxis1(){
    writeData(to_string(currentFileSet[0]), Controller1.Axis1.position(percent), Brain.timer(msec));
}

void writeAxis2(){
    writeData(to_string(currentFileSet[1]), Controller1.Axis2.position(percent), Brain.timer(msec));
}

void writeAxis3(){
    writeData(to_string(currentFileSet[2]), Controller1.Axis3.position(percent), Brain.timer(msec));
}

void writeAxis4(){
    writeData(to_string(currentFileSet[3]), Controller1.Axis4.position(percent), Brain.timer(msec));
}

void writeButtonUp(){
    writeData(to_string(currentFileSet[4]), Controller1.ButtonUp.pressing(), Brain.timer(msec));
}

void writeButtonDown(){
    writeData(to_string(currentFileSet[5]), Controller1.ButtonDown.pressing(), Brain.timer(msec));
}

void writeButtonRight(){
    writeData(to_string(currentFileSet[6]), Controller1.ButtonRight.pressing(), Brain.timer(msec));
}

void writeButtonLeft(){
    writeData(to_string(currentFileSet[7]), Controller1.ButtonLeft.pressing(), Brain.timer(msec));
}

void writeButtonA(){
    writeData(to_string(currentFileSet[8]), Controller1.ButtonA.pressing(), Brain.timer(msec));
}

void writeButtonB(){
    writeData(to_string(currentFileSet[9]), Controller1.ButtonB.pressing(), Brain.timer(msec));
}

void writeButtonX(){
    writeData(to_string(currentFileSet[10]), Controller1.ButtonX.pressing(), Brain.timer(msec));
}

void writeButtonY(){
    writeData(to_string(currentFileSet[11]), Controller1.ButtonY.pressing(), Brain.timer(msec));
}

void writeButtonL1(){
    writeData(to_string(currentFileSet[12]), Controller1.ButtonL1.pressing(), Brain.timer(msec));
}

void writeButtonL2(){
    writeData(to_string(currentFileSet[13]), Controller1.ButtonL2.pressing(), Brain.timer(msec));
}

void writeButtonR1(){
    writeData(to_string(currentFileSet[14]), Controller1.ButtonR1.pressing(), Brain.timer(msec));
}

void writeButtonR2(){
    writeData(to_string(currentFileSet[15]), Controller1.ButtonR2.pressing(), Brain.timer(msec));
}

void setWrites(){
    extern char (*currentFileSet)[11];

    FileInformationHolder Axis1(currentFileSet[0], Controller1.Axis1);
    /*
    Controller1.Axis1.changed(writeAxis1);
    Controller1.Axis2.changed(writeAxis2);
    Controller1.Axis3.changed(writeAxis3);
    Controller1.Axis4.changed(writeAxis4);

    Controller1.ButtonUp.pressed(writeButtonUp);
    Controller1.ButtonUp.released(writeButtonUp);

    Controller1.ButtonDown.pressed(writeButtonDown);
    Controller1.ButtonDown.released(writeButtonDown);

    Controller1.ButtonRight.pressed(writeButtonRight);
    Controller1.ButtonRight.released(writeButtonRight);

    Controller1.ButtonLeft.pressed(writeButtonLeft);
    Controller1.ButtonLeft.released(writeButtonLeft);

    Controller1.ButtonA.pressed(writeButtonA);
    Controller1.ButtonA.released(writeButtonA);

    Controller1.ButtonB.pressed(writeButtonB);
    Controller1.ButtonB.released(writeButtonB);

    Controller1.ButtonX.pressed(writeButtonX);
    Controller1.ButtonX.released(writeButtonX);

    Controller1.ButtonY.pressed(writeButtonY);
    Controller1.ButtonY.released(writeButtonY);

    Controller1.ButtonL1.pressed(writeButtonL1);
    Controller1.ButtonL1.released(writeButtonL1);

    Controller1.ButtonL2.pressed(writeButtonL2);
    Controller1.ButtonL2.released(writeButtonL2);

    Controller1.ButtonR1.pressed(writeButtonR1);
    Controller1.ButtonR1.released(writeButtonR1);

    Controller1.ButtonR2.pressed(writeButtonR2);
    Controller1.ButtonR2.released(writeButtonR2);
    */
}