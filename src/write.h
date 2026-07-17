#pragma once

#include <iostream>
#include <string>
#include <array>
#include <sstream>
#include "vexHardware.h"
#include <sstream>

// allows to easily change what files are written too
//char (*currentFileSet)[11];

extern brain Brain;
extern controller Controller1;

// yoinked from someone else on stack overflow :)
template <typename T>
std::string to_string(T value);

void writeData(std::string file, int pos, float time);

void writeAxis1();
void writeAxis2();
void writeAxis3();
void writeAxis4();

void writeButtonUp();
void writeButtonDown();
void writeButtonRight();
void writeButtonLeft();
void writeButtonA();
void writeButtonB();
void writeButtonX();
void writeButtonY();
void writeButtonL1();
void writeButtonL2();
void writeButtonR1();
void writeButtonR2();

void setWrites();