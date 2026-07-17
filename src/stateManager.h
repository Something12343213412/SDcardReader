#pragma once

// tracks all inputs and assigns an integer value to axis inputs and bool to buttons
struct StateManager
{
    short Axis1=0;
    short Axis2=0;
    short Axis3=0;
    short Axis4=0;
    bool Up=0;
    bool Down=0;
    bool Left=0;
    bool Right=0;
    bool A=0;
    bool B=0;
    bool X=0;
    bool Y=0;
    bool R1=0;
    bool R2=0;
    bool L1=0;
    bool L2=0;
};

