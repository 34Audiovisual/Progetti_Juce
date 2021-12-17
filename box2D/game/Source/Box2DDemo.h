/*
  ==============================================================================

   This file is part of the JUCE examples.
   Copyright (c) 2020 - Raw Material Software Limited

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES,
   WHETHER EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR
   PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the Projucer and used to generate a JUCE project.

 BEGIN_JUCE_PIP_METADATA

 name:             Box2DDemo
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Showcases 2D graphics features.

 dependencies:     juce_box2d, juce_core, juce_data_structures, juce_events,
                   juce_graphics, juce_gui_basics
 exporters:        xcode_mac, vs2019, linux_make, androidstudio, xcode_iphone

 moduleFlags:      JUCE_STRICT_REFCOUNTEDPOINTER=1

 type:             Component
 mainClass:        Box2DDemo

 useLocalCopy:     1

 END_JUCE_PIP_METADATA

*******************************************************************************/

#pragma once

#include "DemoUtilities.h"

//==============================================================================
// (These classes and random functions are used inside the 3rd-party Box2D demo code)
inline float32 RandomFloat()                           { return Random::getSystemRandom().nextFloat() * 2.0f - 1.0f; }
inline float32 RandomFloat (float32 lo, float32 hi)    { return Random::getSystemRandom().nextFloat() * (hi - lo) + lo; }

struct Settings
{
    b2Vec2 viewCenter  { 0.0f, 20.0f };
    float32 hz = 60.0f;
    int velocityIterations = 8;
    int positionIterations = 3;
    int drawShapes         = 1;
    int drawJoints         = 1;
    int drawAABBs          = 0;
    int drawPairs          = 0;
    int drawContactPoints  = 0;
    int drawContactNormals = 0;
    int drawContactForces  = 0;
    int drawFrictionForces = 0;
    int drawCOMs           = 0;
    int drawStats          = 0;
    int drawProfile        = 0;
    int enableWarmStarting = 1;
    int enableContinuous   = 1;
    int enableSubStepping  = 0;
    int pause              = 0;
    int singleStep         = 0;
};

struct Test
{
    Test()          {}
    virtual ~Test() {}

    virtual void Keyboard (unsigned char /*key*/)   {}
    virtual void KeyboardUp (unsigned char /*key*/) {}

    std::unique_ptr<b2World> m_world  { new b2World (b2Vec2 (0.0f, -10.0f)) };
};

JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wimplicit-int-float-conversion")

#include "ApplyForce.h"

JUCE_END_IGNORE_WARNINGS_GCC_LIKE


//==============================================================================
struct Box2DRenderComponent  : public Component
{
    Box2DRenderComponent()
    {
        setOpaque (true);
    }

    void paint (Graphics& g) override
    {
        g.fillAll (Colours::white);

        if (currentTest.get() != nullptr)
        {
            Box2DRenderer renderer;

            renderer.render (g, *currentTest->m_world,
                             -16.0f, 30.0f, 16.0f, -1.0f,
                             getLocalBounds().toFloat().reduced (8.0f));
        }
    }

    std::unique_ptr<Test> currentTest;
};

//==============================================================================
class Box2DDemo : public Component,
                  private Timer
{
public:

    Box2DDemo()
    {
        setOpaque (true);
        setWantsKeyboardFocus (true);

        addAndMakeVisible (renderComponent);
        renderComponent.currentTest.reset (new ApplyForce());

        startTimerHz (60);

        setSize (500, 500);
    }

    ~Box2DDemo() override
    {
    }

    void paint (Graphics& g) override
    {
        g.fillAll (getUIColourIfAvailable (LookAndFeel_V4::ColourScheme::UIColour::windowBackground));
    }

    void resized() override
    {
        renderComponent.setBounds (getBounds());
    }


private:

    Box2DRenderComponent renderComponent;

    void checkKeys()
    {
        if (renderComponent.currentTest.get() == nullptr)
            return;

        checkKeyCode ('a');
        checkKeyCode ('w');
        checkKeyCode ('d');
    }

    void checkKeyCode (const int keyCode)
    {
        if (KeyPress::isKeyCurrentlyDown (keyCode))
            renderComponent.currentTest->Keyboard ((unsigned char) keyCode);
    }

    void timerCallback() override
    {
        if (renderComponent.currentTest.get() == nullptr)
            return;

        if (isShowing())
            grabKeyboardFocus();

        checkKeys();
        renderComponent.currentTest->m_world->Step (1.0f / 60.0f, 6, 2);
        repaint();
    }



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Box2DDemo)
};
