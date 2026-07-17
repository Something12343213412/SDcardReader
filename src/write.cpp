#include "write.h"

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
    writeData("Axis1.txt", Controller1.Axis1.position(percent), Brain.timer(msec));
}

void writeAxis2(){
    writeData("Axis2.txt", Controller1.Axis2.position(percent), Brain.timer(msec));
}

void writeAxis3(){
    writeData("Axis3.txt", Controller1.Axis3.position(percent), Brain.timer(msec));
}

void writeAxis4(){
    writeData("Axis4.txt", Controller1.Axis4.position(percent), Brain.timer(msec));
}

void writeButtonUp(){
    writeData("BtnUp.txt", Controller1.ButtonUp.pressing(), Brain.timer(msec));
}

void writeButtonDown(){
    writeData("BtnDn.txt", Controller1.ButtonDown.pressing(), Brain.timer(msec));
}

void writeButtonRight(){
    writeData("BtnRt.txt", Controller1.ButtonRight.pressing(), Brain.timer(msec));
}

void writeButtonLeft(){
    writeData("BtnLt.txt", Controller1.ButtonLeft.pressing(), Brain.timer(msec));
}

void writeButtonA(){
    writeData("BtnAA.txt", Controller1.ButtonA.pressing(), Brain.timer(msec));
}

void writeButtonB(){
    writeData("BtnBB.txt", Controller1.ButtonB.pressing(), Brain.timer(msec));
}

void writeButtonX(){
    writeData("BtnXX.txt", Controller1.ButtonX.pressing(), Brain.timer(msec));
}

void writeButtonY(){
    writeData("BtnYY.txt", Controller1.ButtonY.pressing(), Brain.timer(msec));
}

void writeButtonL1(){
    writeData("BtnL1.txt", Controller1.ButtonL1.pressing(), Brain.timer(msec));
}

void writeButtonL2(){
    writeData("BtnL2.txt", Controller1.ButtonL2.pressing(), Brain.timer(msec));
}

void writeButtonR1(){
    writeData("BtnR1.txt", Controller1.ButtonR1.pressing(), Brain.timer(msec));
}

void writeButtonR2(){
    writeData("BtnR2.txt", Controller1.ButtonR2.pressing(), Brain.timer(msec));
}

void setWrites(){
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
}