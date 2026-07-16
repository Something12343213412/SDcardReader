#pragma once

// tracks all inputs and assigns an integer value to axis inputs and bool to buttons
struct StateManager
{
    short Up=0;
    short Down=0;
    short Left=0;
    short Righ=0;
    bool A=0;
    bool B=0;
    bool X=0;
    bool Y=0;
    bool R1=0;
    bool R2=0;
    bool L1=0;
    bool L2=0;
};

