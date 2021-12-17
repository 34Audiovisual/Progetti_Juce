/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

PeakHolder::PeakHolder()
{
    timeOfPeak = Time::getCurrentTime().toMilliseconds();
    startTimerHz(refreshRateFPS);
}
PeakHolder::~PeakHolder()
{
    stopTimer();
}
void PeakHolder::setHoldTime(int ms)
{
    durationToHoldForMs = ms;
}
void PeakHolder::timerCallback()
{
    auto now = Time::getCurrentTime().toMilliseconds();
    auto elapsed = now - timeOfPeak;
    if( elapsed > durationToHoldForMs )
    {
        valueToHoldDb = jlimit(static_cast<float>(NegativeInfinity),
                               static_cast<float>(MaxDecibels),
                               valueToHoldDb + decayRateDbPerFrame);
        decayRateDbPerFrame *= 1.05;
    }
}
void PeakHolder::updateHeldValue(float newValueDb)
{
    /*
     hold value for N ms
     after N ms, decrement at X rate until holdValue == 0

     if new Value > holdValue
        holdValue = newValue
        reset counter of N ms
     */

    if( newValueDb >= valueToHoldDb )
    {
        timeOfPeak = Time::getCurrentTime().toMilliseconds();
        valueToHoldDb = newValueDb;
        setDecayRate(decayRate);
    }
}
void PeakHolder::resetHeldValue()
{
    valueToHoldDb = NegativeInfinity;
}
void PeakHolder::setDecayRate(float rateDbPerSec)
{
    jassert(rateDbPerSec <= 0.f );
    decayRateDbPerFrame = rateDbPerSec / refreshRateFPS;
    decayRate = rateDbPerSec;
}
//==============================================================================
HoldMeter::HoldMeter(Atomic<float>& i, bool render, bool ticks, Orientation o, bool showHold) :
    input(i),
    renderSolid(render),
    drawTicks(ticks),
    showHoldTick(showHold),
    orientation(o)
{
    peakHolder.setDecayRate(-30.f); //db per second @ 30fps
}

void HoldMeter::update()
{
    auto newValueDb = input.get();

    peakHolder.updateHeldValue(newValueDb);
    cachedValueDb = newValueDb;
    repaint();
}

void HoldMeter::setThresholdDb(float thDb)
{
    jassert(thDb >= (float)NegativeInfinity);
    jassert((float)MaxDecibels >= thDb);
    thresholdDb = thDb;
    makeGradients();
    repaint();
}

ColourGradient makeGradientFromThreshold(Colour minColour,
                                         Colour maxColour,
                                         Colour overColour,
                                         float thresholdDb)
{
    jassert((float)NegativeInfinity <= thresholdDb);
    jassert((float)thresholdDb <= (float)MaxDecibels);

    ColourGradient cg1;
    cg1.addColour(jmap((float)NegativeInfinity,
                       (float)NegativeInfinity, (float)MaxDecibels,
                       0.f, 1.f),
                  minColour);
    cg1.addColour(jmap(0.f,
                       (float)NegativeInfinity, (float)MaxDecibels,
                       0.f, 1.f),
                  maxColour);
    maxColour = cg1.getColourAtPosition(jmap(thresholdDb,
                                             (float)NegativeInfinity, (float)MaxDecibels,
                                             0.f, 1.f));

    ColourGradient gradient;
    gradient.addColour(jmap((float)NegativeInfinity,
                            (float)NegativeInfinity, (float)MaxDecibels,
                            0.f, 1.f),
                       minColour);
    gradient.addColour(jmap(thresholdDb,
                            (float)NegativeInfinity, (float)MaxDecibels,
                            0.f, 1.f),
                       maxColour);
    gradient.addColour(jmap(thresholdDb + 0.1f,
                            (float)NegativeInfinity, (float)MaxDecibels,
                            0.f, 1.f),
                       overColour);
    gradient.addColour(jmap((float)MaxDecibels,
                            (float)NegativeInfinity, (float)MaxDecibels,
                            0.f, 1.f),
                       overColour);
    return gradient;
}

void HoldMeter::makeGradients()
{
    auto minColour = Colours::darkcyan;
    auto maxColour = Colours::yellow;
    auto overColor = Colours::red;

    borderGradient = makeGradientFromThreshold(minColour, maxColour, overColor, thresholdDb);

    switch (orientation)
    {
        case verticalUp:
        {
            borderGradient.point1 = bounds.getBottomLeft().toFloat();
            borderGradient.point2 = bounds.getTopLeft().toFloat();
            break;
        }
        case verticalDown:
        {
            borderGradient.point1 = bounds.getTopLeft().toFloat();
            borderGradient.point2 = bounds.getBottomLeft().toFloat();
            break;
        }
        case horizontalRight:
        {
            borderGradient.point1 = bounds.getTopLeft().toFloat();
            borderGradient.point2 = bounds.getTopRight().toFloat();
            break;
        }
        case horizontalLeft:
        {
            borderGradient.point1 = bounds.getTopRight().toFloat();
            borderGradient.point2 = bounds.getTopLeft().toFloat();
            break;
        }
    }

    fillGradient = borderGradient;
    fillGradient.multiplyOpacity(0.5f);
}

void HoldMeter::resized()
{
    bounds = getLocalBounds(); //cache this
    height = bounds.getHeight(); //cache this
    width = bounds.getWidth();

    makeGradients();

    ticks.clear();
    for( int i = MaxDecibels; i >= NegativeInfinity; i -= 3 )
    {
        ticks.emplace_back(TickPos{i, mapValueToPosition(i)});
    }
}

float HoldMeter::mapValueToPosition(float v)
{
    switch (orientation)
    {
        case verticalUp:
        {
            return jmap(v,
                        float(NegativeInfinity), (float)MaxDecibels,
                        float(height), 0.f);
        }
        case verticalDown:
        {
            return jmap(v,
                        float(NegativeInfinity), (float)MaxDecibels,
                        0.f, (float)height);
        }
        case horizontalRight:
        {
            return jmap(v,
                        float(NegativeInfinity), (float)MaxDecibels,
                        0.f, float(width));
        }
        case horizontalLeft:
        {
            return jmap(v,
                        float(NegativeInfinity), (float)MaxDecibels,
                        float(width), 0.f);
        }
    }
}

void HoldMeter::setShowHoldTick(bool s)
{
    showHoldTick = s;
    repaint();
}

void HoldMeter::setHoldTime(int ms)
{
    peakHolder.setHoldTime(ms);
    repaint();
}

void HoldMeter::setDecayRate(float rateDbPerSec)
{
    peakHolder.setDecayRate(rateDbPerSec);
    repaint();
}

void HoldMeter::resetHold()
{
    peakHolder.resetHeldValue();
    repaint();
}

void HoldMeter::paint(Graphics &g)
{
    auto peakValueDb = peakHolder.getCurrentValue();
    //draw peak as 2px tall line
    //convert to db

    if( showHoldTick )
    {
        g.setColour( peakValueDb > thresholdDb ? Colours::red : Colours::orange);
        switch (orientation)
        {
            case verticalUp:
            case verticalDown:
            {
                g.fillRect(bounds.withHeight(2).withY(mapValueToPosition(peakValueDb)));
                break;
            }
            case horizontalRight:
            case horizontalLeft:
            {
                g.fillRect(bounds.withWidth(2).withX(mapValueToPosition(peakValueDb)));
                break;
            }
        }
    }


    auto bw = bounds.getWidth();
    auto bh = bounds.getHeight();
    for( auto& tick : ticks )
    {
        if( tick.db == 0 )
        {
            //draw '0db' line always
            g.setColour(Colours::grey.brighter());
            g.fillRect(bounds.withHeight(1).withY(tick.pos));
        }
        else if( drawTicks && tick.db % 6 == 0 )
        {
            g.setColour(Colours::grey.withAlpha(0.5f));

            switch (orientation)
            {
                case verticalUp:
                case verticalDown:
                {
                    g.fillRect(bounds
                               .withHeight(1)
                               .withY(tick.pos)
                               .reduced((tick.db % 12 == 0) ?
                                        bw * 0.165 :
                                        bw * 0.33,
                                        0)
                               );
                    break;
                }
                case horizontalRight:
                case horizontalLeft:
                {
                    g.fillRect(bounds
                               .withWidth(1)
                               .withX(tick.pos)
                               .reduced(0, (tick.db % 12 == 0) ?
                                        bh * 0.165 :
                                        bh * 0.33)
                               );
                    break;
                }
            } //end switch
        } //end else if
    } //end for( auto& tick : ticks )

    //draw cached as filled rect
    auto db = cachedValueDb;
    Rectangle<float> meter;
    if( db > (float)NegativeInfinity )
    {
        db = mapValueToPosition(db);
        switch (orientation)
        {
            case verticalUp:
            {
                meter = Rectangle<float>(0, db, width, height - db);
                break;
            }
            case verticalDown:
            {
                meter = Rectangle<float>(0, 0, width, db);
                break;
            }
            case horizontalRight:
            {
                meter = Rectangle<float>(0, 0, db, height);
                break;
            }
            case horizontalLeft:
            {
                meter = Rectangle<float>(db, 0, width - db, height);
                break;
            }
        }
        if( !renderSolid )
        {
            g.setGradientFill(fillGradient);
            g.fillRect(meter);

            g.setGradientFill(borderGradient);
            g.drawRect(meter);
        }
        else
        {
            g.setGradientFill(borderGradient);
            g.fillRect(meter);
        }
    }
}
//==============================================================================
LevelMeter::LevelMeter(bool onLeft,
                       HoldMeter::Orientation o,
                       bool dt,
                       bool rs) :
instantOnLeft(onLeft),
orientation(o),
renderSolid(rs),
drawTicks(dt),
instantMeter(valueDb,
             true, //render solid always
             drawTicks,
             orientation,
             true ),
averageMeter(average.getAvg(),
             renderSolid,
             drawTicks,
             orientation,
             true)
{
    average.clear(NegativeInfinity);
    addAndMakeVisible(instantMeter);
    addAndMakeVisible(averageMeter);

    viewType = ViewType::Average; //force the setViewType to make the instant/average meters
    setViewType(ViewType::Both);
}

void LevelMeter::setShowHoldTick(bool s)
{
    instantMeter.setShowHoldTick(s);
    averageMeter.setShowHoldTick(s);
}

void LevelMeter::setHoldTime(int ms)
{
    instantMeter.setHoldTime(ms);
    averageMeter.setHoldTime(ms);
}

void LevelMeter::setDecayRate(float rateDbPerSec)
{
    instantMeter.setDecayRate(rateDbPerSec);
    averageMeter.setDecayRate(rateDbPerSec);
}

void LevelMeter::resetHold()
{
    instantMeter.resetHold();
    averageMeter.resetHold();
}

void LevelMeter::setThresholdDb(float thDb)
{
    instantMeter.setThresholdDb(thDb);
    averageMeter.setThresholdDb(thDb);
}

void LevelMeter::setAverageTime(int ms)
{
    double secondsPerFrame = 1.0 / (double)averagerNumFrames;

    average.resize(ms * secondsPerFrame, NegativeInfinity);
    repaint();
}

void LevelMeter::setViewType(ViewType vt)
{
    if( vt == viewType )
        return;

    viewType = vt;
    switch (viewType)
    {
        case ViewType::Both:
        {
            instantMeter.setVisible(true);
            averageMeter.setVisible(true);
            break;
        }
        case ViewType::Peak:
        {
            instantMeter.setVisible(true);
            averageMeter.setVisible(false);
            break;
        }
        case ViewType::Average:
        {
            instantMeter.setVisible(false);
            averageMeter.setVisible(true);
            break;
        }
    }
    resized();
    repaint();
}

void LevelMeter::update(float vDb)
{
    valueDb.set(vDb);
    average.add(vDb);
    instantMeter.update();
    averageMeter.update();
}

void LevelMeter::resized()
{
    auto bounds = getLocalBounds();

    if( viewType == ViewType::Both )
    {
        switch( orientation )
        {
            case HoldMeter::verticalUp:
            case HoldMeter::verticalDown:
            {
                bounds.reduce(1, 0);
                auto a = instantOnLeft ? bounds.removeFromLeft(4) : bounds.removeFromRight(4);
                auto b = bounds.reduced(1, 0);
                instantMeter.setBounds(a);
                averageMeter.setBounds(b);
                break;
            }
            case HoldMeter::horizontalRight:
            case HoldMeter::horizontalLeft:
            {
                bounds.reduce(0, 1);
                auto a = instantOnLeft ? bounds.removeFromTop(4) : bounds.removeFromBottom(4);
                auto b = bounds.reduced(0, 1);
                instantMeter.setBounds(a);
                averageMeter.setBounds(b);
                break;
            }
        }
    }
    else if( viewType == ViewType::Average )
    {
        switch( orientation )
        {
            case HoldMeter::verticalUp:
            case HoldMeter::verticalDown:
            {
                bounds.reduce(1, 0);
                averageMeter.setBounds(bounds);
                break;
            }
            case HoldMeter::horizontalRight:
            case HoldMeter::horizontalLeft:
            {
                bounds.reduce(0, 1);
                averageMeter.setBounds(bounds);
                break;
            }
        }
    }
    else
    {
        switch( orientation )
        {
            case HoldMeter::verticalUp:
            case HoldMeter::verticalDown:
            {
                bounds.reduce(1, 0);
                instantMeter.setBounds(bounds);
                break;
            }
            case HoldMeter::horizontalRight:
            case HoldMeter::horizontalLeft:
            {
                bounds.reduce(0, 1);
                instantMeter.setBounds(bounds);
                break;
            }
        }
    }
}

const std::vector<TickPos>& LevelMeter::getTickPositions()
{
    return instantMeter.getTicks();
}

void LevelMeter::paint(Graphics &g)
{
    g.fillAll(Colours::black);
}
//==============================================================================
TextMeter::TextMeter()
{
    valueHolder.setThreshold(0.f); //0dbFS
    valueHolder.updateHeldValue(NegativeInfinity);
//    valueHolder.setHoldTime(1 * 1000);
}
void TextMeter::update(float valueDb)
{
    cachedValueDb = valueDb;
    valueHolder.updateHeldValue(valueDb);
    repaint();
}

void TextMeter::setHoldTime(int ms)
{
    valueHolder.setHoldTime(ms);
}

void TextMeter::setThreshold(float thDb)
{
    valueHolder.setThreshold(thDb);
}

void TextMeter::paint(Graphics &g)
{
//    g.setColour(Colours::mediumvioletred);
//    g.drawRect(getLocalBounds());

    Colour textColor;
    float valueToDisplay;
    if( valueHolder.getIsOverThreshold() )
    {
        g.fillAll(Colours::red);
        textColor = Colours::black;
        valueToDisplay = valueHolder.getCurrentValue();
    }
    else
    {
        valueToDisplay = cachedValueDb;
        textColor = Colours::white;
    }
    g.setColour( textColor );

    g.setFont(12.f);

    g.drawFittedText((valueToDisplay != (float)NegativeInfinity ? String(valueToDisplay, 1) : "-inf"),
                     getLocalBounds(),
                     Justification::bottomRight,
                     1);

}

//==============================================================================
void Histogram::update(float valueDb)
{
    buffer.write( valueDb );

    repaint();
}

void Histogram::makeGradients()
{
    gradient.clearColours();
    auto minColor = Colours::aqua;
    auto maxColor = Colours::yellow;
    auto overColor = Colours::red;

    gradient = makeGradientFromThreshold(minColor, maxColor, overColor, thresholdDb.getValue());

    gradient.point1 = Point<float>(0, getHeight());
    gradient.point2 = Point<float>(0, 0);

    gradientAlpha = gradient;
    gradientAlpha.multiplyOpacity(0.33f);
}

void Histogram::resized()
{
    auto halfWidth = getWidth() / 2;
    
    path.preallocateSpace((10 + halfWidth) * 3);
    
    buffer.resize( halfWidth, float(NegativeInfinity) );

    makeGradients();
}

void Histogram::mouseDown(const MouseEvent &e)
{
    buffer.clear(float(NegativeInfinity));
    repaint();
}

void Histogram::drawGrid(Graphics& g)
{
    const int fontHeight = 12;

    g.setFont(getLookAndFeel().getTypefaceForFont(Font(fontHeight)));

    auto bounds = getLocalBounds();
    auto strWidth = Font(fontHeight).getStringWidth("i") * 6;
    auto levelArea = bounds.removeFromLeft(strWidth);
    std::vector<float> dbs {};
    for( int i = 0; i > NegativeInfinity; i -= 18 )
    {
        dbs.push_back( i );
    }

    for( auto db : dbs )
    {
        auto y = jmap(db,
                      (float)MaxDecibels, (float)NegativeInfinity,
                      0.f, (float)bounds.getHeight());
        g.setColour(Colours::grey.withAlpha(0.5f));
        g.drawHorizontalLine(y /*+ fontHeight * 0.5f*/, strWidth + 2, getWidth() - strWidth - 2);

        g.setColour(Colours::grey);
        g.drawFittedText(String(db),
                         levelArea.getX(),
                         y - fontHeight * 0.5f,
                         strWidth,
                         fontHeight,
                         Justification::bottomRight,
                         1);
        g.drawFittedText(String(db),
                         getWidth() - strWidth - 2,
                         y - fontHeight * 0.5f,
                         strWidth,
                         fontHeight,
                         Justification::bottomRight,
                         1);
    }

    g.setColour(Colours::grey.withAlpha(0.5f));
    auto w = getWidth() / 7.f;
    for( int i = 0; i < 7; ++i )
    {
        g.drawVerticalLine(w * (i + 1), 0, getHeight());
    }

    g.setColour(Colours::grey);
    g.setFont(18);
    g.drawFittedText(title, bounds.reduced(1), Justification::centredBottom, 1);
}

Path Histogram::buildPath(Path& p,
                             ReadAllAfterWriteCircularBuffer<float>& buffer,
                             Graphics& g,
                             Rectangle<float> bounds
                             )
{
    auto bufferSize = buffer.getSize();
    auto increment = [size = bufferSize](int &idx) -> int
    {
        idx = (idx + 1);
        if( idx > size - 1 )
            idx = 0;
        return idx;
    };
    
    auto readIndex = buffer.getReadIndex();
    auto start = increment(readIndex);
    
    auto& data = buffer.getData();
    
    int x = 0;
    float h = bounds.getHeight();
    
    p.clear();

    p.startNewSubPath(x,
                      jmap(data.at(start),
                           (float)MaxDecibels, (float)NegativeInfinity,
                           0.f, h));
    x += 1;
    while( x < buffer.getSize() - 2 )
    {
        auto idx = increment(start);
        jassert( idx < bufferSize );
        
        p.lineTo(x,
                 jmap(data.at(idx),
                      (float)MaxDecibels, (float)NegativeInfinity,
                      0.f, h));
        x += 1;
    }
    
    auto pBounds = p.getBounds();
    if( pBounds.getHeight() > 0 )
    {
        p.applyTransform(AffineTransform().scale(bounds.getWidth() / pBounds.getWidth(),
                                                 1.f));
        
        auto fill = p;
        fill.lineTo(bounds.getBottomRight());
        fill.lineTo(bounds.getBottomLeft());
        fill.closeSubPath();
        
        return fill;
    }
    
    return {};
}

void Histogram::displayPath(Graphics& g, Rectangle<float> bounds)
{
    auto fill = buildPath(path, buffer, g, bounds);
    
    if( !fill.isEmpty() )
    {
        g.setGradientFill(gradientAlpha);
        g.fillPath(fill);
        
        g.setGradientFill(gradient);
        g.strokePath(path, PathStrokeType(1));
    }
}

void Histogram::paint(Graphics &g)
{
    auto bounds = getLocalBounds().toFloat();

    g.setColour(Colours::black);
    g.fillAll();

    drawGrid(g);

    bounds.reduce(1, 0);
    displayPath(g, bounds);

    bounds.expand(1, 0);
    Path p;
    p.addRectangle(bounds);
    p.setUsingNonZeroWinding(false);
    p.addRoundedRectangle(bounds.reduced(1), GraphicPosInfo::cornerSize);
    g.setColour(Colour(GraphicPosInfo::colorR,
                       GraphicPosInfo::colorG,
                       GraphicPosInfo::colorB));

    g.fillPath(p);
}

//void Histogram::setThreshold(float thDb)
//{
//    thresholdDb = thDb;
//    makeGradients();
//    repaint();
//}

void Histogram::valueChanged(Value &v)
{
    if( v == thresholdDb )
    {
        makeGradients();
        repaint();
    }
}
//==============================================================================
Goniometer::Goniometer(AudioBuffer<float>& buf) :
buffer(buf)
{
    internalBuffer.setSize(2, 512*3);
    internalBuffer.clear();
}

void Goniometer::resized()
{
    w = getWidth();
    h = getHeight();
    auto bounds = getLocalBounds();
    center = bounds.getCentre();
    p.preallocateSpace(3 * buffer.getNumSamples());

    gradient.clearColours();
    gradient.addColour(0.f, Colours::aqua);
    gradient.addColour(0.9f, Colours::yellow);
    gradient.addColour(1.f, Colours::orange);
    gradient.isRadial = true;
    gradient.point1 = center.toFloat();
    gradient.point2 = Point<float>(bounds.getX(), bounds.getCentreY());
}

void Goniometer::drawBackground(Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    Path circle;

    bounds.reduce(16, 16);
    circle.addEllipse(bounds);

    g.setColour(Colours::grey);
    g.strokePath(circle, PathStrokeType(1));

    auto radius = bounds.getHeight() / 2.f;
    auto center = bounds.getCentre();

    auto twoPi_over_8 = MathConstants<float>::twoPi / 8.f;
    Path p;

    for( int i = 0; i < 8; ++i )
    {
        p.startNewSubPath(center);
        p.lineTo(center.getPointOnCircumference(radius, twoPi_over_8 * i) );
    }

    g.setColour(Colours::darkgrey);
    g.strokePath(p, PathStrokeType(1));

    g.setColour(Colours::white);
    g.setFont(16);

    Array<String> chars{"+S", "L", "M", "R", "-S"};
    for(int c = 0; c < chars.size(); ++c)
    {
        auto w = Font(16).getStringWidth(chars.getReference(c));
        Rectangle<float> r(w, 16);
        r.setCentre( center.getPointOnCircumference(radius + 8, twoPi_over_8 * (6 + c) ) );
        g.drawFittedText(chars.getReference(c), r.toNearestInt(), Justification::centred, 1);
    }
}

void Goniometer::paint(Graphics &g)
{
    drawBackground(g);
    const auto thinner = 4;

    auto numSamples = buffer.getNumSamples();
    p.clear();
    
    /*
     shift 'buffer samples' of internalBuffer's samples from the back to the front.
     copy buffer samples to the end of internal buffer
     */
    auto internalNumSamples = internalBuffer.getNumSamples();
    if( numSamples < internalBuffer.getNumSamples() )
    {
        /*
         shift
         */
        juce::FloatVectorOperations::copy(internalBuffer.getWritePointer(0),
                                          internalBuffer.getReadPointer(0, numSamples),
                                          internalNumSamples - numSamples);
        juce::FloatVectorOperations::copy(internalBuffer.getWritePointer(1),
                                          internalBuffer.getReadPointer(1, numSamples),
                                          internalNumSamples - numSamples);
        
        /*
         copy
         */
        juce::FloatVectorOperations::copy(internalBuffer.getWritePointer(0, internalNumSamples - numSamples),
                                          buffer.getReadPointer(0),
                                          numSamples);
        juce::FloatVectorOperations::copy(internalBuffer.getWritePointer(1, internalNumSamples - numSamples),
                                          buffer.getReadPointer(1),
                                          numSamples);
    }
    else
    {
        /*
         only copy as many samples as we can fit.
         */
        internalBuffer.copyFrom(0, 0, buffer, 0, 0, internalNumSamples);
        internalBuffer.copyFrom(1, 0, buffer, 1, 0, internalNumSamples);
    }

    float L, R, M, S;
    const float root2inv = 1.f / std::sqrt(2.f);

    const auto radius = getLocalBounds().toFloat().reduced(16).getHeight() / 2.f;

    auto leftChan = internalBuffer.getReadPointer(0);
    auto rightChan = internalBuffer.getReadPointer(1);
    
    for( auto s = 0; s < internalNumSamples; s += thinner )
    {
        L = leftChan[s];
        R = rightChan[s];
        M = jlimit(-1.f, 1.f, (L + R) * root2inv) * scale;
        S = jlimit(-1.f, 1.f, (L - R) * root2inv) * scale;
        Point<float> point(center.x + radius * 0.5f * S,  //remember, Side is the X axis
                           center.y + radius * 0.5f * M); //Mid is the Y axis

        if( s == 0 )
        {
            p.startNewSubPath(point);
        }
        else
        {
            if( point.isFinite() )
                p.lineTo(point);
        }
    }

    if( !p.isEmpty() )
    {
        p.applyTransform(AffineTransform().verticalFlip(h)); //Remember, Y=0 is the top of the screen. Y=H is the bottom.  you have to flip the whole path upside down to draw it correctly.  No need to rotate anymore either!

        g.setGradientFill(gradient);
        g.strokePath(p, PathStrokeType(1));
    }
}

void Goniometer::adjustScale(float s)
{
    scale = s;
    repaint();
}
//==============================================================================
CorrelationMeter::CorrelationMeter(AudioBuffer<float>& buf, double sampleRate) : buffer(buf)
{
    auto coefficients = dsp::FilterDesign<float>::designFIRLowpassWindowMethod(50.f, sampleRate, 2, dsp::FilterDesign<float>::WindowingMethod::rectangular);
    filters[0] = dsp::FIR::Filter<float>(coefficients);
    filters[1] = dsp::FIR::Filter<float>(coefficients);
    filters[2] = dsp::FIR::Filter<float>(coefficients);
}

void CorrelationMeter::resized()
{
    auto bounds = getLocalBounds().reduced(0, 1);
    auto strW = Font(12).getStringWidth("+1");
    bounds.removeFromLeft(strW);
    bounds.removeFromRight(strW);

    gradient.clearColours();
    gradient.addColour(0.f, Colours::red);
    gradient.addColour(0.5f, Colours::green);
    gradient.addColour(1.f, Colours::yellow);
    gradient.point1 = bounds.getTopLeft().toFloat();
    gradient.point2 = bounds.getTopRight().toFloat();

    alphaGradient = gradient;
    alphaGradient.multiplyOpacity(0.5f);
}

void CorrelationMeter::update()
{
    auto numSamples = buffer.getNumSamples();
    float c_t = 0;
    auto leftChan = buffer.getReadPointer(0);
    auto rightChan = buffer.getReadPointer(1);
    for( int i = 0; i < numSamples / 2; i += 2 )
    {
        auto L = leftChan[i];
        auto R = rightChan[i];

        auto numerator = filters[0].processSample(L * R);
        auto denominator = std::sqrt( filters[1].processSample(L*L) * filters[2].processSample(R*R) );
        if( denominator != 0.f )
        {
            c_t = numerator / denominator;
            slowAverager.add(c_t);
            peakAverager.add(c_t);
        }
        else
        {
            slowAverager.add(0.f);
            peakAverager.add(0.f);
        }
    }

    repaint();
}

void CorrelationMeter::paint(Graphics &g)
{
    auto strW = Font(12).getStringWidth("+1");
    auto bounds = getLocalBounds();

    g.setColour(Colours::white);
    g.drawFittedText("-1", bounds.removeFromLeft(strW), Justification::centredLeft, 1);
    g.drawFittedText("+1", bounds.removeFromRight(strW), Justification::centredRight, 1);

    g.setColour(Colours::darkgrey);

    g.drawRect(bounds.reduced(2, 0));

    g.drawRect(bounds.withSizeKeepingCentre(1, bounds.getHeight()));

    bounds.reduce(2, 2);
    drawAverage(g, bounds.removeFromTop(2), peakAverager.get(), true);
    bounds.removeFromTop(1);
    drawAverage(g, bounds, slowAverager.get(), false);
}

void CorrelationMeter::drawAverage(Graphics &g, Rectangle<int> bounds, float avg, bool drawBorder)
{
    auto centerX = bounds.getCentreX();
    auto x = centerX + bounds.getWidth() * 0.5 * avg;
    auto r = Rectangle<float>(Point<float>(centerX,
                                           bounds.getY()),
                              Point<float>(x,
                                           bounds.getBottom()));
    
    if( ! r.isEmpty() )
    {
        g.setGradientFill(drawBorder ? alphaGradient : gradient);
        g.fillRect(r);
        if( drawBorder )
        {
            g.setGradientFill(gradient);
            g.drawRect(r);
        }
    }
}
//==============================================================================
StereoImageMeter::StereoImageMeter(AudioBuffer<float>& buf, double sampleRate) : buffer(buf)
{
    goniometer = std::make_unique<Goniometer>(buffer);
    correlationMeter = std::make_unique<CorrelationMeter>(buffer, sampleRate);

    addAndMakeVisible(goniometer.get());
    addAndMakeVisible(correlationMeter.get());
}

void StereoImageMeter::resized()
{
    auto bounds = getLocalBounds();

    correlationMeter->setBounds(bounds.removeFromBottom(20));
    auto size = jmin(bounds.getWidth(), bounds.getHeight());
    goniometer->setBounds(Rectangle<int>(size, size).withCentre(bounds.getCentre()));
}

void StereoImageMeter::update()
{
    goniometer->repaint();
    correlationMeter->update();
}

void StereoImageMeter::adjustGonioMeterScale(float scale)
{
    if( goniometer )
        goniometer->adjustScale(scale);
}
//==============================================================================
MacroMeter::MacroMeter(const String& title) :
label(title, title)//,
//valueResponder( thresholdSlider.getValueObject(), f )
{
    label.setEditable(false);
    label.setJustificationType(Justification::centredBottom);
    addAndMakeVisible(label);

    addAndMakeVisible(leftTextMeter);
    addAndMakeVisible(rightTextMeter);

    addAndMakeVisible(leftMeter);
    addAndMakeVisible(rightMeter);

    thresholdSlider.setRange(NegativeInfinity, MaxDecibels, 0.5);

    addAndMakeVisible(thresholdSlider);
    thresholdSlider.getValueObject().addListener(this);
}

MacroMeter::~MacroMeter()
{
    thresholdSlider.getValueObject().removeListener(this);
}

void MacroMeter::valueChanged(Value &v)
{
    auto thDb = thresholdSlider.getValue();
    leftMeter.setThresholdDb(thDb);
    rightMeter.setThresholdDb(thDb);
    leftTextMeter.setThreshold(thDb);
    rightTextMeter.setThreshold(thDb);
}

void MacroMeter::updateThresholdReferral(ValueTree tree, const Identifier& property)
{
    thresholdSlider.getValueObject().referTo(tree.getPropertyAsValue(property, nullptr));
}

void MacroMeter::update(float leftDb, float rightDb)
{
    leftTextMeter.update(leftDb);
    rightTextMeter.update(rightDb);
    leftMeter.update(leftDb);
    rightMeter.update(rightDb);
}

void MacroMeter::setShowHoldTick(bool s)
{
    leftMeter.setShowHoldTick(s);
    rightMeter.setShowHoldTick(s);
}

void MacroMeter::setHoldTime(int ms)
{
    leftMeter.setHoldTime(ms);
    rightMeter.setHoldTime(ms);
    leftTextMeter.setHoldTime(ms);
    rightTextMeter.setHoldTime(ms);
}

void MacroMeter::setDecayRate(float rateDbPerSec)
{
    leftMeter.setDecayRate(rateDbPerSec);
    rightMeter.setDecayRate(rateDbPerSec);
}

void MacroMeter::resetHold()
{
    leftMeter.resetHold();
    rightMeter.resetHold();
}

void MacroMeter::setAverageTime(int ms)
{
    leftMeter.setAverageTime(ms);
    rightMeter.setAverageTime(ms);
}

void MacroMeter::setViewType(LevelMeter::ViewType vt)
{
    leftMeter.setViewType(vt);
    rightMeter.setViewType(vt);
    repaint();
}

void MacroMeter::resized()
{
    auto bounds = getLocalBounds();
    label.setBounds(bounds.removeFromBottom(18));

    auto textMeterArea = bounds.removeFromTop(14);
    const auto fontHeight = 12;
    auto dbScaleWidth = Font(fontHeight).getStringWidth("1") * 3;
    auto textMeterWidth = (textMeterArea.getWidth() - dbScaleWidth) / 2;
    leftTextMeter.setBounds(textMeterArea.removeFromLeft(textMeterWidth));
    rightTextMeter.setBounds(textMeterArea.removeFromRight(textMeterWidth));

    leftMeter.setBounds(bounds.removeFromLeft(textMeterWidth));
    rightMeter.setBounds(bounds.removeFromRight(textMeterWidth));
    thresholdSlider.setBounds(bounds);
}
void MacroMeter::paint(Graphics &g)
{
//    g.setColour(Colours::red);
    auto bounds = getLocalBounds();
//    g.drawRect(bounds);

    auto yOffset = leftMeter.getY();
    auto& ticks = leftMeter.getTickPositions();

    g.setColour(Colours::grey);

    g.setFont(getLookAndFeel().getTypefaceForFont(Font(12)));

    for( const auto& tick : ticks )
    {
        if( tick.db % 6 == 0 )
        {
            auto y = tick.pos + yOffset;
            String str(tick.db);
            auto strWidth = Font().getStringWidth(str);
            g.drawSingleLineText(String(tick.db), bounds.getCentreX() - strWidth/2, y);
        }
    }
}
//==============================================================================
PFMLnF::PFMLnF()
{
    //https://github.com/be5invis/Iosevka
    setDefaultSansSerifTypefaceName("Iosevka Term Slab Extralight");
    setDefaultSansSerifTypeface(Font("Iosevka Term Slab Extralight", 12, 0).getTypeface());
    setColour(ComboBox::outlineColourId,
              Colour(GraphicPosInfo::colorR,
                     GraphicPosInfo::colorG,
                     GraphicPosInfo::colorB));

    setColour(ComboBox::backgroundColourId, Colours::black);

    setColour(TextButton::ColourIds::buttonColourId, Colours::black);
    setColour(TextButton::ColourIds::buttonOnColourId, Colours::white);
    setColour(TextButton::ColourIds::textColourOffId, Colours::white);
    setColour(TextButton::ColourIds::textColourOnId, Colours::black);
    setColour(Label::ColourIds::textColourId, Colours::white);
}
void PFMLnF::drawLabel(Graphics &g, Label & l)
{
    auto r = l.getLocalBounds();
    GlyphArrangement garr;
    //need to fit the text in the dropdown
//    Font f("Baskerville", r.getHeight(), 0);
    Font f(getTypefaceForFont(Font()));
    garr.addFittedText(f.withHeight(r.getHeight()),
                       l.getText(),
                       r.getX(), r.getY(),
                       r.getWidth(), r.getHeight(),
                       Justification::centredBottom, 1);
    g.setColour(l.findColour(Label::ColourIds::textColourId));
    Path p;
    p.clear();
    garr.createPath(p);
    g.fillPath(p);
}

void PFMLnF::drawLinearSlider (Graphics& g, int x, int y, int width, int height,
                                       float sliderPos,
                                       float minSliderPos,
                                       float maxSliderPos,
                                       const Slider::SliderStyle style, Slider& slider)
{
    Rectangle<float> bounds = slider.getLocalBounds().toFloat().reduced(0, 1);

    g.setColour(Colours::red);
    auto range = slider.getRange();

    auto yVal = jmap(slider.getValue(),
                     range.getStart(), range.getEnd(),
                     double(bounds.getHeight()), 0.0);

    g.fillRect(bounds.withHeight(1).withY( yVal ));
}

void PFMLnF::drawButtonBackground(Graphics &g,
                                  Button &button,
                                  const Colour &backgroundColour,
                                  bool shouldDrawButtonAsHighlighted,
                                  bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds().toFloat().reduced (0.5f, 0.5f);

    auto baseColour = backgroundColour.withMultipliedSaturation (button.hasKeyboardFocus (true) ? 1.3f : 0.9f)
    .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f);

    if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
        baseColour = baseColour.contrasting (shouldDrawButtonAsDown ? 0.2f : 0.05f);

    g.setColour (baseColour);
    g.fillRect(bounds);

    g.setColour(button.findColour (ComboBox::outlineColourId));
    g.drawRect(bounds);
}

void PFMLnF::drawComboBox(Graphics &g, int width, int height, bool isButtonDown, int buttonX, int buttonY, int buttonW, int buttonH, ComboBox & box)
{
    g.setColour (box.findColour (ComboBox::backgroundColourId));
    Rectangle<int> boxBounds (0, 0, width, height);
    g.fillRect(boxBounds);
    g.setColour (box.findColour (ComboBox::outlineColourId));
    g.drawRect(boxBounds);

    Path p;
    boxBounds.removeFromRight(4);
    auto pathArea = boxBounds.removeFromRight(6);
    p.startNewSubPath(pathArea.getX(), pathArea.getCentreY() - pathArea.getHeight() * 0.165);
    p.lineTo(pathArea.getCentreX(), pathArea.getCentreY() + pathArea.getHeight() * 0.165 );
    p.lineTo(pathArea.getRight(), pathArea.getCentreY() - pathArea.getHeight() * 0.165);
    g.setColour(box.findColour (ComboBox::outlineColourId));
    g.strokePath(p, PathStrokeType(1));
}

void PFMLnF::positionComboBoxText (ComboBox& box, Label& label)
{
    label.setBounds( box.getLocalBounds().reduced(2,2).withTrimmedRight(8) );
    label.setFont (getComboBoxFont (box));
}

void drawTick(Graphics& g,
              Rectangle<float> r,
              float valueToMap,
              float minValue,
              float maxValue,
              float rotaryStartAngle,
              float rotaryEndAngle,
              int tickSize,
              int offset)
{
    /*
     this rectangle represents how the slider tick should look at
     sliderPosProportional == 0.5f, the 12 o'clock position

     it needs to be rotated to the correct angle
     */
    auto tickMarkBounds = r.withSizeKeepingCentre(2, tickSize).withY(r.getY() - offset);
    /*
     we can compute this by using jmap() to map our normalized position
     to between the rotaryStart and rotaryEnd angles
     */
    auto sliderAng = jmap(valueToMap,
                          minValue,
                          maxValue,
                          rotaryStartAngle,
                          rotaryEndAngle);
    /*
     we'll use an AffineTransform to rotate the rect
     apply this rotation, centered at our knob's center,
     because this drawing is happening inside the OWNER of the knob, not the knob itself
     */
    Point<float> center = r.getCentre();
    auto at = AffineTransform().rotation(sliderAng,
                                         center.getX(),
                                         center.getY());
    /*
     save our state
     apply the transform,
     draw the rectangle,
     restore to pre-transform state
     */
    Graphics::ScopedSaveState sss(g);
    g.addTransform(at);
    g.fillRect(tickMarkBounds);
}

void PFMLnF::drawRotarySlider(Graphics &g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider & slider)
{
    g.setColour(findColour(ComboBox::outlineColourId));

    auto size = jmin(width, height);
    auto r = slider.getLocalBounds().withSizeKeepingCentre(size, size).reduced(2).toFloat();
    g.drawEllipse(r, 2);

    //draw a tick at 100%
    drawTick(g, r, 100, 50, 200, rotaryStartAngle, rotaryEndAngle, 6, 6);
    //draw a tick at 50%
    drawTick(g, r, 50, 50, 200, rotaryStartAngle, rotaryEndAngle, 6, 6);
    //draw a tick at 200%
    drawTick(g, r, 200, 50, 200, rotaryStartAngle, rotaryEndAngle, 6, 6);
    //draw the actual tick
    drawTick(g, r, sliderPosProportional, 0.f, 1.f, rotaryStartAngle, rotaryEndAngle, size/2, 0);
}

Font PFMLnF::getTextButtonFont(TextButton &, int buttonHeight)
{
    Font f("Baskerville", jmin(15.f, buttonHeight * 0.6f), 0);
    return f;
}

Font PFMLnF::getComboBoxFont(ComboBox& box)
{
    Font f("Baskerville", jmin(15.f, box.getHeight() * 0.6f), 0);
    return f;
}
//==============================================================================
Pfmcpp_project10AudioProcessorEditor::Pfmcpp_project10AudioProcessorEditor (Pfmcpp_project10AudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), safePtr(this)
{
    buffer.clear();
    setLookAndFeel(&labelLnf);

    addAndMakeVisible(rmsMeter);
    addAndMakeVisible(magMeter);

    addAndMakeVisible(rmsHistogram);
    addAndMakeVisible(magHistogram);

    stereoImageMeter = std::make_unique<StereoImageMeter>(buffer,
                                                          processor.sampleRate);
    sampleRate = processor.sampleRate;
    addAndMakeVisible(*stereoImageMeter);

    pfmcppForMusiciansLabel.setColour(Label::ColourIds::textColourId, Colours::black);
    addAndMakeVisible(pfmcppForMusiciansLabel);
//=========================== Hold Button stuff =================================
    holdButton.setClickingTogglesState(true);

    holdButton.onClick = [safe = safePtr]()
    {
        if(auto* comp = safe.getComponent() )
            comp->updateHoldVisibility();
    };

    addAndMakeVisible(holdButton);

    holdTimeCB.addItemList({"0s", "0.5s", "2s", "4s", "6s", "inf"}, 1);
    holdTimeCB.setEditableText(false);

    holdTimeCB.onChange = [safe = safePtr]()
    {
        if(auto* comp = safe.getComponent() )
            comp->updateHoldTime();
    };

    addChildComponent( holdTimeCB ); //this is only visible if hold is on

    resetButton.onClick = [safe = safePtr]()
    {
        if(auto* comp = safe.getComponent() )
        {
            comp->rmsMeter.resetHold();
            comp->magMeter.resetHold();
        }
    };

    addChildComponent(resetButton); //this is only visible when the holdTimeCB is 'inf'
    addAndMakeVisible(holdSeparator);
//=========================== Decay stuff =================================
    decayLabel.setEditable(false);
    decayLabel.setColour(Label::ColourIds::textColourId, Colours::lightgrey);
    addAndMakeVisible(decayLabel);

    decayCB.addItemList({"-3dB/s", "-6dB/s", "-12dB/s", "-24dB/s", "-36dB/s"}, 1);
    decayCB.onChange = [safe = safePtr]()
    {
        if(auto* comp = safe.getComponent() )
            comp->updateDecayTime();
    };

    addAndMakeVisible(decayCB);
    addAndMakeVisible(decaySeparator);
//=========================== Average stuff =================================
    averageCB.addItemList({"100ms", "250ms", "500ms", "1000ms", "2000ms"}, 1);
    averageCB.onChange = [safe = safePtr]()
    {
        if(auto* comp = safe.getComponent() )
            comp->updateAverageTime();
    };

    addAndMakeVisible(averageLabel);
    addAndMakeVisible(averageCB);
//=========================== Meter View Mode stuff =================================
    addAndMakeVisible(meterViewSeparator);

    addAndMakeVisible(meterViewLabel);

    meterViewCB.addItemList({"Both", "Peak", "Avg"}, 1);
    meterViewCB.onChange = [safe = safePtr]()
    {
        if(auto* comp = safe.getComponent() )
            comp->updateMeterView();
    };

    addAndMakeVisible(meterViewCB);
//=========================== Histogram View stuff ==============================
    addAndMakeVisible(histoViewSeparator);
    addAndMakeVisible(histogramLabel);
    histogramCB.addItemList({"Stacked", "Side-by-Side"}, 1);
    histogramCB.onChange = [safe = safePtr]()
    {
        if(auto* comp = safe.getComponent() )
            comp->resized();
    };
    addAndMakeVisible(histogramCB);
//=========================== Gonio Scale stuff =================================
    gonioScaleSlider.setRange(50, 200);
    gonioScaleSlider.onValueChange = [safe = safePtr]()
    {
        if(auto* comp = safe.getComponent() )
            comp->stereoImageMeter->adjustGonioMeterScale( comp->gonioScaleSlider.getValue() / 100.f );
    };
    addAndMakeVisible(gonioScaleLabel);
    addAndMakeVisible(gonioRangeLabel);
    addAndMakeVisible(gonioScaleSlider);
//=========================== Settings restoration stuff =================================
    settings.addListener(this);

    Timer::callAfterDelay(5, [safe = safePtr]()
    {
        if(auto* comp = safe.getComponent() )
            comp->settings = comp->processor.settings;
    });

    setSize (700, 572);

    startTimerHz(30);
}

void Pfmcpp_project10AudioProcessorEditor::timerCallback()
{
    bool isPlaying = true; //assume we're playing.
    if( processor.positionInfoSender.pull(currentPositionInfo) )
    {
//        DBG( "is playing? " << (currentPositionInfo.isPlaying ? "true" : "false") );
        isPlaying = currentPositionInfo.isPlaying; //if we are actually playing.
    }
    else
    {
        DBG( "couldn't determine currentPositionInfo" );
    }

    if( !processor.bufferSender.pull(buffer) )
    {
        DBG( "couldn't pull buffer from processor!" );
        if( !isPlaying )
        {
            DBG( "not playing, clearing internal buffer" );
            buffer.clear();
        }
        //else the last pull will get pushed again to all modules.
    }

    auto numSamples = buffer.getNumSamples();
    auto rmsLeftDb = Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, numSamples),
                                              float(NegativeInfinity));
    auto rmsRightDb = Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, numSamples),
                                               float(NegativeInfinity));
    auto magLeftDb = Decibels::gainToDecibels(buffer.getMagnitude(0, 0, numSamples),
                                              float(NegativeInfinity));
    auto magRightDb = Decibels::gainToDecibels(buffer.getMagnitude(1, 0, numSamples),
                                               float(NegativeInfinity));

    //histogram needs average of rms, average of mag
    rmsHistogram.update((rmsLeftDb + rmsRightDb) * 0.5f);
    magHistogram.update((magLeftDb + magRightDb) * 0.5f);
    //rmsMeter needs rmsLeft/Right
    rmsMeter.update(rmsLeftDb, rmsRightDb);
    //mag meter needs magLeft/Right
    magMeter.update(magLeftDb, magRightDb);
    //stereo image meter needs the buffer
    makeStereoImageMeterIfNeeded();
    stereoImageMeter->update();
}

void Pfmcpp_project10AudioProcessorEditor::makeStereoImageMeterIfNeeded()
{
    if( sampleRate != processor.sampleRate )
    {
        sampleRate = processor.sampleRate;
        stereoImageMeter = std::make_unique<StereoImageMeter>(buffer, sampleRate);
        addAndMakeVisible(*stereoImageMeter);
        resized();
    }
}

void Pfmcpp_project10AudioProcessorEditor::updateHoldVisibility()
{
    auto showHold = holdButton.getToggleState();
    rmsMeter.setShowHoldTick(showHold);
    magMeter.setShowHoldTick(showHold);

    holdTimeCB.setVisible(showHold);
    resetButton.setVisible( holdTimeCB.getText() == "inf" );
}

void Pfmcpp_project10AudioProcessorEditor::updateMeterView()
{
    auto selectedID = meterViewCB.getSelectedId();
    DBG( "rmsAverageCB selection: " << selectedID );
    LevelMeter::ViewType type = LevelMeter::ViewType::Both;
    switch (selectedID)
    {
        case 1: //Both
            break;
        case 2: //Peak
            type = LevelMeter::ViewType::Peak;
            break;
        case 3: //Avg
            type = LevelMeter::ViewType::Average;
            break;
        default:
            jassertfalse;
            break;
    }
    rmsMeter.setViewType(type);
    magMeter.setViewType(type);
}

void Pfmcpp_project10AudioProcessorEditor::updateAverageTime()
{
    auto selectedID = averageCB.getSelectedId();
    DBG( "rmsAverageCB selection: " << selectedID );
    int avgTime = 100;
    switch (selectedID)
    {
        case 2: //250ms
            avgTime = 250;
            break;
        case 3: //500ms
            avgTime = 500;
            break;
        case 4: //1000ms
            avgTime = 1000;
            break;
        case 5: //2000ms
            avgTime = 2000;
            break;
        case 1: //30ms
        default:
        {
            avgTime = 100;
            break;
        }
    }
    rmsMeter.setAverageTime(avgTime);
    magMeter.setAverageTime(avgTime);
}

void Pfmcpp_project10AudioProcessorEditor::updateDecayTime()
{
    auto selectedID = decayCB.getSelectedId();
    DBG( "decayRateCB selection: " << selectedID );
    float decayRate = -36.f;
    switch (selectedID)
    {
        case 1: //-3
            decayRate = -3.f;
            break;
        case 2: //-6
            decayRate = -6.f;
            break;
        case 3: //-18
            decayRate = -18.f;
            break;
        case 4: //-36
            decayRate = -36.f;
            break;
        case 5: //-54
            decayRate = -54.f;
            break;
        default:
        {
            jassertfalse;
            decayRate = -36.f;
            break;
        }
    }
    rmsMeter.setDecayRate(decayRate);
    magMeter.setDecayRate(decayRate);
}

void Pfmcpp_project10AudioProcessorEditor::updateHoldTime()
{
    auto selectedID = holdTimeCB.getSelectedId();

    double holdTime = 0; //0s or inf hold time.
    if( selectedID == 2 )
    {
        //0.5s )
        holdTime = 0.5 * 1000;
    }
    else if( selectedID == 3 )
    {
        //2s
        holdTime = 2 * 1000;
    }
    else if( selectedID == 4 )
    {
        //4s
        holdTime = 4 * 1000;
    }
    else if( selectedID == 5 )
    {
        //6s
        holdTime = 6 * 1000;
    }

    //TODO set meter decayRate to decay CB value
    rmsMeter.setHoldTime(holdTime);
    magMeter.setHoldTime(holdTime);

    if( selectedID == 6 ) //infinite
    {
        rmsMeter.setDecayRate(0.); //infinite peak holder doesn't decay.
        magMeter.setDecayRate(0.);
        resetButton.setVisible(true); //you must use the reset button to reset it.
    }
    else
    {
        //potentially the last decay rate set was 0, so we must change this to the decay CB's current settings.
        rmsMeter.setDecayRate( String(decayCB.getText()).getIntValue() );
        magMeter.setDecayRate( String(decayCB.getText()).getIntValue() );
        resetButton.setVisible(false); //we don't need the reset button if we're not holding indefinitely.
    }
}

void Pfmcpp_project10AudioProcessorEditor::valueTreeRedirected(ValueTree &treeWhichHasBeenChanged)
{
    restoreSettings();
}

void Pfmcpp_project10AudioProcessorEditor::restoreSettings()
{
    DBG( settings.toXmlString() );
    holdButton.getToggleStateValue().referTo(settings.getPropertyAsValue(Identifiers::enable_meterHold, nullptr));
    updateHoldVisibility();

    holdTimeCB.getSelectedIdAsValue().referTo(settings.getPropertyAsValue(Identifiers::hold_duration_ms, nullptr));
    updateHoldTime();

    decayCB.getSelectedIdAsValue().referTo(settings.getPropertyAsValue(Identifiers::decay_rate_db_sec, nullptr));
    updateDecayTime();

    averageCB.getSelectedIdAsValue().referTo(settings.getPropertyAsValue(Identifiers::average_duration_ms, nullptr));
    updateAverageTime();

    meterViewCB.getSelectedIdAsValue().referTo(settings.getPropertyAsValue(Identifiers::meter_display, nullptr));
    updateMeterView(); //This resets the showHoldTick flag in the HoldMeter because it makes new meters!

    histogramCB.getSelectedIdAsValue().referTo(settings.getPropertyAsValue(Identifiers::histogram_display, nullptr));
    resized();

    gonioScaleSlider.getValueObject().referTo(settings.getPropertyAsValue(Identifiers::goniometer_scale, nullptr));

    auto rmsThresh = settings.getPropertyAsValue(Identifiers::rms_threshold, nullptr);
    rmsMeter.getThresholdSlider().getValueObject().referTo(rmsThresh);
    rmsMeter.valueChanged(rmsThresh);
    rmsHistogram.getThresholdDbAsValue().referTo(rmsThresh);
    rmsHistogram.valueChanged(rmsThresh);

    auto magThresh = settings.getPropertyAsValue(Identifiers::mag_threshold, nullptr);
    magMeter.getThresholdSlider().getValueObject().referTo(magThresh);
    magMeter.valueChanged(magThresh);
    magHistogram.getThresholdDbAsValue().referTo(magThresh);
    magHistogram.valueChanged(magThresh);
}

Pfmcpp_project10AudioProcessorEditor::~Pfmcpp_project10AudioProcessorEditor()
{
    DBG( "closing editor" );
    DBG( settings.toXmlString() );
    stopTimer();
    setLookAndFeel(nullptr);
}
//==============================================================================
void Pfmcpp_project10AudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll(Colour(GraphicPosInfo::colorR,
                     GraphicPosInfo::colorG,
                     GraphicPosInfo::colorB));
    auto bounds = getLocalBounds();

    bounds = bounds.withTrimmedLeft(8).withTrimmedRight(8).withTrimmedTop(4).withTrimmedBottom(2);
    Path p;

    //start middle left
    p.startNewSubPath(bounds.getX(), bounds.getCentreY());
    //up
    p.lineTo(bounds.getX(), bounds.getY() + GraphicPosInfo::cornerSize);
    //upper left corner
    p.quadraticTo(bounds.getX(), bounds.getY(),
                  bounds.getX() + GraphicPosInfo::cornerSize, bounds.getY());
    //to the right
    auto meterRight = rmsMeter.getRight();
    p.lineTo(meterRight - GraphicPosInfo::cornerSize + 2, bounds.getY());
    //curve down
    p.quadraticTo(meterRight + 2, bounds.getY(),
                  meterRight + 2, bounds.getY() + GraphicPosInfo::cornerSize);
    //curve to the right
    p.quadraticTo(meterRight + 2,
                  pfmcppForMusiciansLabel.getBottom(),
                  meterRight + GraphicPosInfo::cornerSize,
                  pfmcppForMusiciansLabel.getBottom());
    //line to the middle
    p.lineTo(bounds.getCentreX(),
             pfmcppForMusiciansLabel.getBottom() /*+ GraphicPosInfo::cornerSize*/);
    //line straight down
    p.lineTo(bounds.getCentreX(), bounds.getBottom());
    //line to the left
    p.lineTo(bounds.getX() + GraphicPosInfo::cornerSize, bounds.getBottom());
    //curve up
    p.quadraticTo(bounds.getX(), bounds.getBottom(),
                  bounds.getX(), bounds.getBottom() - GraphicPosInfo::cornerSize);
    //close it off
    p.closeSubPath();

    g.setColour(Colours::black);
    g.fillPath(p);

    //mirror p around y axis at x=0
    //first, translate to x = 0, y = 0
    p.applyTransform(AffineTransform().translated(-bounds.getX(), -bounds.getY()));
    //https://upload.wikimedia.org/wikipedia/commons/2/2c/2D_affine_transformation_matrix.svg
    //then mirror
    p.applyTransform(AffineTransform(-1, 0, 0, 0, 1, 0));
    //then translate back
    p.applyTransform(AffineTransform().translated(bounds.getRight(), bounds.getY()));

    g.fillPath(p);
}



void Pfmcpp_project10AudioProcessorEditor::resized()
{
    DBG( "editor size: " << getLocalBounds().toString() );
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto bounds = getLocalBounds().withTrimmedLeft(8).withTrimmedRight(8).withTrimmedTop(8).withTrimmedBottom(2);

    auto histogramArea = bounds.removeFromBottom(112+112);
    if( histogramCB.getText() == "Stacked" )
    {
        magHistogram.setBounds(histogramArea.removeFromBottom(112));
        rmsHistogram.setBounds(histogramArea.removeFromBottom(112));
    }
    else
    {
        rmsHistogram.setBounds(histogramArea.removeFromLeft(histogramArea.getWidth() * 0.5));
        magHistogram.setBounds(histogramArea);
    }
    bounds.removeFromBottom(1);

    const auto fontHeight = 12;
    auto dbScaleWidth = Font(fontHeight).getStringWidth("1") * 3;
    auto meterW = meterWidth * 2 + dbScaleWidth;

    rmsMeter.setBounds(bounds.removeFromLeft(meterW));
    magMeter.setBounds(bounds.removeFromRight(meterW));

    pfmcppForMusiciansLabel.setBounds(Rectangle<int>(Point<int>(rmsMeter.getRight() + 5,
                                                                rmsMeter.getY() + fontHeight * 2),
                                                     Point<int>(magMeter.getX() - 5,
                                                                rmsMeter.getY() - fontHeight)));

    bounds.removeFromTop(pfmcppForMusiciansLabel.getHeight());
    auto rmsControlsArea = bounds.removeFromLeft(100);
    rmsControlsArea.removeFromLeft(20);
    rmsControlsArea.removeFromRight(20);
    auto widgetGap = 10;
    auto widgetHeight = 20;

    FlexBox rmsSide, magSide;

    rmsSide.flexWrap = FlexBox::Wrap::noWrap;
    rmsSide.flexDirection = FlexBox::Direction::column;

    rmsSide.items.add(FlexItem(decayLabel).withHeight(widgetHeight));
    rmsSide.items.add(FlexItem().withHeight(4));
    rmsSide.items.add(FlexItem(decayCB).withHeight(widgetHeight));
    rmsSide.items.add(FlexItem().withFlex(1.f));
    rmsSide.items.add(FlexItem(decaySeparator).withHeight(2));

    rmsSide.items.add(FlexItem().withFlex(1.f)); //gap

    rmsSide.items.add(FlexItem(averageLabel).withHeight(widgetHeight));
    rmsSide.items.add(FlexItem().withHeight(4));
    rmsSide.items.add(FlexItem(averageCB).withHeight(widgetHeight));
    rmsSide.items.add(FlexItem().withFlex(1.f));
    rmsSide.items.add(FlexItem(meterViewSeparator).withHeight(2));

    rmsSide.items.add(FlexItem().withFlex(1.f)); //gap

    rmsSide.items.add(FlexItem(meterViewLabel).withHeight(widgetHeight));
    rmsSide.items.add(FlexItem().withHeight(4));
    rmsSide.items.add(FlexItem(meterViewCB).withHeight(widgetHeight));
    rmsSide.items.add(FlexItem().withFlex(1.f));

    rmsSide.performLayout(rmsControlsArea);

    auto gonioControlArea = bounds.removeFromRight(100);

    gonioScaleLabel.setBounds(gonioControlArea.removeFromTop(widgetHeight));
    gonioControlArea.removeFromTop(widgetGap/2);
    gonioScaleSlider.setBounds(gonioControlArea.removeFromTop(50));
    gonioControlArea.removeFromTop(widgetGap/2);
    gonioRangeLabel.setBounds(gonioControlArea.removeFromTop(widgetHeight));

    gonioControlArea.removeFromTop(widgetGap);
    gonioControlArea.removeFromLeft(20);
    gonioControlArea.removeFromRight(20);

    magSide.flexWrap = FlexBox::Wrap::noWrap;
    magSide.flexDirection = FlexBox::Direction::column;

    magSide.items.add(FlexItem().withFlex(1.f)); //gap
    //hold controls.
    magSide.items.add(FlexItem(holdSeparator).withHeight(2));
    magSide.items.add(FlexItem().withFlex(1.f)); //gap

    magSide.items.add(FlexItem(holdButton).withHeight(widgetHeight));
    magSide.items.add(FlexItem().withHeight(widgetGap));
    magSide.items.add(FlexItem(holdTimeCB).withHeight(widgetHeight));
    magSide.items.add(FlexItem().withHeight(widgetGap));
    magSide.items.add(FlexItem(resetButton).withHeight(widgetHeight));

    magSide.items.add(FlexItem().withFlex(1.f)); //gap
    //histogram view controls.
    magSide.items.add(FlexItem(histoViewSeparator).withHeight(2));
    magSide.items.add(FlexItem().withFlex(1.f)); //gap

    magSide.items.add(FlexItem(histogramLabel).withHeight(widgetHeight));
    magSide.items.add(FlexItem().withHeight(widgetGap));
    magSide.items.add(FlexItem(histogramCB).withHeight(widgetHeight));
    magSide.items.add(FlexItem().withHeight(widgetGap));
    magSide.items.add(FlexItem().withFlex(1.f)); //gap

    magSide.performLayout(gonioControlArea);

    stereoImageMeter->setBounds(bounds);
}
