/*
  ==============================================================================

    Sequence.h
    Created: 12 Nov 2021 7:02:28pm
    Author:  Diego Capoccitti

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

struct Sequence
{
    Sequence() { }
    ~Sequence() { }
    
    int takeNum(int p) { return coeff[p][0]; }
    int takeDenom(int p) { return coeff[p][1]; }
    
private:
    
    int coeff[14][2]
    {
        {1 , 2},
        {3 , 8},
        {1 , 3},
        {4 , 9},
        {1 , 4},
        {1 , 5},
        {2 , 5},
        {1 , 6},
        {1 , 7},
        {2 , 7},
        {1 , 8},
        {3 , 8},
        {1 , 9},
        {2 , 9}
    };
};
