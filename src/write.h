#pragma once

#include <iostream>
#include <string>
#include <array>
#include <sstream>
#include "vex.h"
#include "vexHardware.cpp"

using namespace vex;

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