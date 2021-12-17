/*
  ==============================================================================

    Tube.h
    Created: 30 Oct 2021 3:22:05pm
    Author:  Diego Capoccitti

  ==============================================================================
*/

/*
    // utility functions:
M(x) {
    EPS = 0.000000001;
    xgeps = x > EPS;
    xleps = x < -EPS;
    glps = clip((xgeps + xleps), 0, 1);
    return glps ? x : 0.;
}
D(x) {
    EPS = 0.000000001;
    xgeps = x > EPS;
    xleps = x < -EPS;
    geps = xgeps ? sqrt(x) : 0.;
    return xleps ? sqrt(-x) : geps;
}
    // effect:
// feedbacks:
    History med(0), prevmed(0), prevout(0);
// controls:
    ingain = dbtoa(clip(in2, -48., 48.));
    outgain = dbtoa(clip(in5, -24., 12.));
    drive = clip(in3, 0.1, 10.);
    blend = clip(in4, -10., 10.);
// variables:
    rdrive = 12. / drive;
    rdrive2 = rdrive * rdrive;
    rbdr = rdrive / (10.5 - blend) * 780. / 33.;
    rbdr2 = rbdr * rbdr;
    kpa = D(2. * rdrive2 - 1.) + 1.;
    kpb = (2. - kpa) / 2.;
    ap = (rdrive2 - kpa + 1.) / 2.;
    kc = kpa / D(2. * D(2. * rdrive2 - 1.) - 2. * rdrive2);
    kc2 = kc * kc;
// coefficients:
    srct = (0.1 * samplerate) / (0.1 * samplerate + 1.);
    sq = kc2 + 1.;
    knb = -1. * rbdr / D(sq);
    kna = 2. * kc * rbdr / D(sq);
    an = rbdr2 / sq;
    imr = 2. * knb + D(2. * kna + 4. * an - 1.);
    pwrq = 2. / (imr + 1.);
// perform:
    xin = in1 * ingain;
    ifmed = xin >= 0.;
    med1 = (D(ap + xin * (kpa - xin)) + kpb) * pwrq;
    med2 = (D(an - xin * (kna + xin)) + knb) * pwrq * -1.;
    med = ifmed ? med1 : med2;
    y = maximum((srct * (med - prevmed + prevout)), -1.);
// update:
    prevmed = M(med);
    prevout = M(y);
// output:
    out1 = dcblock(y * outgain);
*/

#pragma once
#include <JuceHeader.h>

class DCBlocker
{
public:
   float processSample(float input)
    {
       output = input - input_1 + 0.995f * output_1;
       input_1 = input;
       output_1 = output;
       return output;
    }
private:
   float input = 0.0;
   float output = 0.0;
   float input_1 = 0.0;
   float output_1 = 0.0;
};

struct Tube 
{
    Tube () { };
    ~Tube() { };
    
float M(float x)
{
    if ((x > EPS) || (x < -EPS))
            return x;
    else
            return 0.0f;
}
    
float D(float x)
{
    if (x > EPS)
        return sqrt(x);
    else if (x < -EPS)
        return sqrt(-x);
    else
        return 0.0f;
}
   
void feedCoontrols (float inG_in2, float outG_in5, float drv_in3, float bld_in4)
{
    // tra -.48. e 48.
    setInGain(inG_in2);
    
    // tra -24. e 12.
    setOutGain(outG_in5);
    
    //tra 0.1 e 10.
    setDrive(drv_in3);
    
    //tra -10. e 10.
    setBlend(bld_in4);
}
    
void setInGain (float ig) { ingain = juce::jlimit(-48.f, 48.f, juce::Decibels::decibelsToGain (ig)); }
    
void setOutGain (float og) { outgain = juce::jlimit(-24.f, 12.f, juce::Decibels::decibelsToGain (og)); }
    
void setDrive (float dr) { drive = juce::jlimit(0.01f, 10.f, dr); }
    
void setBlend (float bl) { blend = juce::jlimit(-10.f, 10.f, bl); }
    
float perform (float in1)
{
    // variables:
    auto rdrive = 12.f / drive;
    auto rdrive2 = rdrive * rdrive;
    auto rbdr = rdrive / (10.5f - blend) * 780.f / 33.f;
    auto rbdr2 = rbdr * rbdr;
    auto kpa = D(2.f * rdrive2 - 1.f) + 1.f;
    auto kpb = (2.f - kpa) / 2.f;
    auto ap = (rdrive2 - kpa + 1.f) / 2.f;
    auto kc = kpa / D(2.f * D(2.f * rdrive2 - 1.f) - 2. * rdrive2);
    auto kc2 = kc * kc;
    
    // coefficients:
    auto srct = (0.1f * (float)sampleRate) / (0.1f * (float)sampleRate + 1.f);
    auto sq = kc2 + 1.f;
    auto knb = -1.f * rbdr / D(sq);
    auto kna = 2.f * kc * rbdr / D(sq);
    auto an = rbdr2 / sq;
    auto imr = 2.f * knb + D(2.f * kna + 4.f * an - 1.f);
    auto pwrq = 2.f / (imr + 1.f);
    
    // perform:
    auto xin = in1 * ingain;
    
    if ( xin >= 0.f )
    {
        med = (D(ap + xin * (kpa - xin)) + kpb) * pwrq;
    }
    else
    {
        med = (D(an - xin * (kna + xin)) + knb) * pwrq * -1.f;
    }
    
    auto y = juce::jmax((srct * (med - prevmed + prevout)), -1.f);
    
    // update:
    prevmed = M(med);
    prevout = M(y);
    
    //dcblock
    return (dcblock.processSample(y * outgain));
}
    
void setSampleRate (double sr)
{
    sampleRate = sr;
}
    
private:
    DCBlocker dcblock;
    
    float EPS = 0.000000001;
    float ingain{0.f}, outgain{0.f}, drive{0.f}, blend{0.f};
    double sampleRate = 44100;
    float med = 0;
    float prevmed = 0;
    float prevout = 0;
};
