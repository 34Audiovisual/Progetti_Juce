#pragma once
#include "ApplyForce.h"

//==============================================================================
class Box2DDemo : public Component,
                  private Timer
{
public:

    Box2DDemo()
    {
        startTimerHz (60);
        setSize (500, 500);
        
        // Define the gravity vector.
        //b2Vec2 gravity(0.0f, -10.0f);

        // Construct a world object, which will hold and simulate the rigid bodies.
        //b2World world(gravity);

        // Define the ground body.
        b2BodyDef groundBodyDef;
        groundBodyDef.position.Set(0.0f, -2.0f);

        // Call the body factory which allocates memory for the ground body
        // from a pool and creates the ground box shape (also from a pool).
        // The body is also added to the world.
        b2Body* groundBody = world.CreateBody(&groundBodyDef);

        // Define the ground box shape.
        b2PolygonShape groundBox;

        // The extents are the half-widths of the box.
        groundBox.SetAsBox(2.0f, 1.0f);

        // Add the ground fixture to the ground body.
        groundBody->CreateFixture(&groundBox, 0.0f);
       

        // Define the dynamic body. We set its position and call the body factory.
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(0.0f, 16.0f);
        b2Body* body = world.CreateBody(&bodyDef);

        // Define another box shape for our dynamic body.
        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(1.0f, 1.0f);

        // Define the dynamic body fixture.
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;

        // Set the box density to be non-zero, so it will be dynamic.
        fixtureDef.density = 5.0f;

        // Override the default friction.
        fixtureDef.friction = 0.3f;

        // Add the shape to the body.
        body->CreateFixture(&fixtureDef);
    }

    ~Box2DDemo() override
    {
    }

    void paint (Graphics& g) override
    {
        g.fillAll (Colours::black);

        renderer.render (g, world,
                             -16.0f, 30.0f, 16.0f, -1.0f,
                             getLocalBounds().toFloat().reduced (8.0f));
        //renderer.render(<#Graphics &g#>, <#b2World &world#>, <#float box2DWorldLeft#>, <#float box2DWorldTop#>, <#float box2DWorldRight#>, <#float box2DWorldBottom#>, <#const Rectangle<float> &targetArea#>)
    }

    void resized() override
    {
    }


private:

    Box2DRenderer renderer;
    
    b2World world {b2Vec2(0.0f, -1.0f)};
    b2Body* m_body;



    void timerCallback() override
    {
        world.Step (1.0f / 60.0f, 6, 2);
        world.Step(<#float32 timeStep#>, <#juce::int32 velocityIterations#>, <#juce::int32 positionIterations#>)
        repaint();
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Box2DDemo)
};
