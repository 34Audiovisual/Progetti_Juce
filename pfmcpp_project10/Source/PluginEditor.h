/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

#include <valarray>

const int NegativeInfinity = -66;
const int MaxDecibels = 12;

struct GraphicPosInfo
{
    static constexpr int gap {2};
    static constexpr int cornerSize {8};
    static constexpr int drop {8};

    static constexpr int colorR = 200;
    static constexpr int colorG = 225;
    static constexpr int colorB = 64;
};

struct TickPos
{
    int db;
    float pos;
};

ColourGradient makeGradientFromThreshold(Colour minColour,
                                         Colour maxColour,
                                         Colour overColour,
                                         float thresholdDb);

template<typename T>
struct Averager
{
    Averager(size_t numElements, T initialValue)
    {
        resize(numElements, initialValue);
    }

    void clear(T initialValue)
    {
        resize(size, initialValue);
    }

    void resize(size_t s, T initialValue)
    {
        jassert( s > 0 );
        idx.set(0);
        size = s;
        elements.clear();
        elements.resize(size, initialValue);
        computeSumAndAvg();
    }

    void add(T t)
    {
        auto i = idx.get();
        jassert( isPositiveAndBelow((int)i, (int)elements.size()) );
        auto old = elements[i];
        elements[ i ] = t;
        ++i;
        idx.set( i > size - 1 ? 0 : i );

        sum = sum - old + t;
        avg.set( sum / float(size) );
    }
    
    T get() const { return avg.get(); }
    size_t getSize() const { return size; }
    Atomic<float>& getAvg() { return avg; }
private:
    Atomic<float> avg { 0 };
    Atomic<size_t> idx { 0 };
    std::vector<T> elements;
    size_t size;
    T sum;
    void computeSumAndAvg()
    {
        sum = 0;
        for( auto& e : elements )
            sum += e;

        avg.set( sum / float(size) );
    }
};

/*
 holds some value over a threshold for N ms
 resets after the time expires.
 
 BUG:
 TextMeter is calling 'isOverThreshold in a different message loop than when the currentValue has been reset to the threshold.
 This can cause the ValueHolder's currentValue and Threshold to be the same, but the TextMeter's value to be different.
 This causes the Text meter to display the text as NOT over the threshold, even when the text meter cached value IS over the threshold.
 */
struct ValueHolder : Timer
{
    ValueHolder()
    {
        timeOfPeak = Time::getCurrentTime().toMilliseconds();
        startTimerHz(60);
    }
    ~ValueHolder() { stopTimer(); }
    void timerCallback() override
    {
        auto now = Time::getCurrentTime().toMilliseconds();
        auto elapsed = now - timeOfPeak;
        if( elapsed > durationToHoldForMs )
        {
            /*
             we need to make 'getIsOverThreshold()' stop returning true
             if the currentValue is no longer over the threshold.
             */
            isOverThreshold = currentValue > threshold;
        }
    }
    void setThreshold(float th)
    {
        threshold = th;
        isOverThreshold = currentValue > threshold;
    }
    void updateHeldValue(float v)
    {
        if( v > threshold )
        {
            isOverThreshold = true;
            timeOfPeak = Time::getCurrentTime().toMilliseconds();
            
            if( v > heldValue )
                heldValue = v;
        }
        
        currentValue = v;
    }
    void setHoldTime(int ms) { durationToHoldForMs = ms; }
    float getCurrentValue() const { return currentValue; }
    float getHeldValue() const { return heldValue; }
    bool getIsOverThreshold() const { return isOverThreshold; }
private:
    float threshold = 0;
    float currentValue = 0;
    float heldValue = 0.f;
    int64 timeOfPeak;
    int durationToHoldForMs {500};
    bool isOverThreshold { false };
};

struct PeakHolder : Timer
{
    PeakHolder();
    ~PeakHolder();
    void setHoldTime(int ms);
    void timerCallback() override;
    float getCurrentValue() { return valueToHoldDb; }
    ///sets the decay rate for the held value.
    void setDecayRate(float rateDbPerSec);
    void updateHeldValue(float newValueDb);
    void resetHeldValue();
private:
    float valueToHoldDb = NegativeInfinity;
    int durationToHoldForMs {500};
    static constexpr int refreshRateFPS = 30;
    float decayRateDbPerFrame { -0.02f };
    int64 timeOfPeak;
    float decayRate { -3 };
};
/*
 a meter that holds a level for a period of milliseconds
 */
struct HoldMeter : Component
{
    enum Orientation
    {
        verticalUp,
        verticalDown,
        horizontalRight,
        horizontalLeft
    };
    HoldMeter(Atomic<float>& input,
              bool renderSolid,
              bool drawTicks,
              Orientation orientation,
              bool showHoldTick);
    void update();

    void paint(Graphics& g) override;
    void updateHoldTime(int ms);
    void resized() override;
    const std::vector<TickPos>& getTicks() { return ticks; }
    void setShowHoldTick(bool s);
    void setHoldTime(int ms);
    void setDecayRate(float rateDbPerSec);
    void resetHold();
    bool getShowHoldTick() const { return showHoldTick; }
    void setThresholdDb(float thDb);
private:
    Atomic<float>& input;
    float cachedValueDb {(float)NegativeInfinity};
    int holdTime{0};
    PeakHolder peakHolder;
    Rectangle<int> bounds;
    ColourGradient fillGradient, borderGradient;
    int height, width;
    bool renderSolid;
    bool drawTicks = true;
    bool showHoldTick = true;

    std::vector<TickPos> ticks;
    Orientation orientation;
    float mapValueToPosition(float v);
    float thresholdDb = 0;
    void makeGradients();
};

struct LevelMeter : Component
{
    enum class ViewType
    {
        Both,
        Peak,
        Average
    };
    LevelMeter(bool instantOnLeft,
               HoldMeter::Orientation o,
               bool drawTicks = true,
               bool renderSolid = false);

    void paint(Graphics& g) override;
    void resized() override;
    const std::vector<TickPos>& getTickPositions();

    void setShowHoldTick(bool s);
    void setHoldTime(int ms);
    void setDecayRate(float rateDbPerSec);
    void resetHold();
    void setAverageTime(int ms);
    void setViewType(ViewType vt);

    ///expects a db value
    void update(float valuedb);
    void setThresholdDb(float thDb);
private:
    Atomic<float> valueDb { (float)NegativeInfinity };

    static constexpr size_t averagerNumFrames = 30;
    Averager<float> average { averagerNumFrames, (float)NegativeInfinity };

    bool instantOnLeft;
    HoldMeter::Orientation orientation;
    ViewType viewType = ViewType::Both;
    bool renderSolid;
    bool drawTicks;
    HoldMeter instantMeter;
    HoldMeter averageMeter;
};

struct TextMeter : Component
{
    TextMeter();
    void paint(Graphics& g) override;
    ///expects a decibel value
    void update(float valueDb);
    void setHoldTime(int ms);
    void setThreshold(float thDb);
private:
    float cachedValueDb;
    ValueHolder valueHolder;
};

template<typename T>
struct ReadAllAfterWriteCircularBuffer
{
    ReadAllAfterWriteCircularBuffer(T fillValue)
    {
        jassert(MessageManager::getInstance()->isThisTheMessageThread());
        resize(1, fillValue);
    }

    void resize(std::size_t s, T fillValue)
    {
        jassert(MessageManager::getInstance()->isThisTheMessageThread());
        size = static_cast<int>(s);
        data.resize(s, fillValue);
        updateWriteIndex();
    }

    void clear(T fillValue)
    {
        jassert(MessageManager::getInstance()->isThisTheMessageThread());
        data.clear();
        writeIndex.set(0);
        resize(size, fillValue);
    }

    using DataType = std::vector<T>;

    void write(T t)
    {
        jassert(MessageManager::getInstance()->isThisTheMessageThread());
        data[ updateWriteIndex() ] = t;
    }

    DataType& getData()
    {
        jassert(MessageManager::getInstance()->isThisTheMessageThread());
        return data;
    }
    int getReadIndex() const
    {
        jassert(MessageManager::getInstance()->isThisTheMessageThread());
        int idx = (int)writeIndex.get() + 1;
        if( idx < size - 1 )
            return idx;

        return 0;
    }
    int getSize() const { return size; }
private:
    std::size_t updateWriteIndex()
    {
        auto currentIndex = writeIndex.get();
        auto idx = currentIndex + 1;
        if( idx > size - 1 )
            idx = 0;

        writeIndex.set( idx );
        return currentIndex;
    }
    Atomic<std::size_t> writeIndex {0};
    DataType data;
    int size {0};
    JUCE_DECLARE_NON_COPYABLE(ReadAllAfterWriteCircularBuffer)
};

struct Histogram : Component, Value::Listener
{
    Histogram(const String& t) : title(t) { thresholdDb.addListener(this); }
    ~Histogram() { thresholdDb.removeListener(this); }
    void paint(Graphics& g) override;
    void resized() override;
    void mouseDown(const MouseEvent& e) override;
    void update(float value);
//    void setThreshold(float thDb);
    Value& getThresholdDbAsValue() { return thresholdDb; }
    void valueChanged(Value& v) override;
private:
    ReadAllAfterWriteCircularBuffer<float> buffer {float(NegativeInfinity)};
    Path path;
    Image bkgd;
    void drawGrid(Graphics& g);
    ColourGradient gradient, gradientAlpha;
    
    void displayPath(Graphics& g, Rectangle<float> bounds);
    
    static Path buildPath(Path& p, ReadAllAfterWriteCircularBuffer<float>& buffer, Graphics& g, Rectangle<float> bounds);
    const String title;
    Value thresholdDb { var(0) };
    void makeGradients();
};

struct Goniometer : Component
{
    Goniometer(AudioBuffer<float>& buffer);
    void paint(Graphics& g) override;
    void resized() override;
    void adjustScale(float s);
private:
    AudioBuffer<float>& buffer;
    AudioBuffer<float> internalBuffer;
    Path p;
    int w, h;
    Point<int> center;

    void drawBackground(Graphics& g);
    ColourGradient gradient;
    float scale = 1.f;
};

struct CorrelationMeter : Component
{
    CorrelationMeter(AudioBuffer<float>& buf, double sampleRate);
    void paint(Graphics& g) override;
    void resized() override;
    void update();
private:
    AudioBuffer<float>& buffer;
    using FilterType = dsp::FIR::Filter<float>;
    std::array<FilterType, 3> filters;

    Averager<float> slowAverager{1024*3, 0}, peakAverager{512, 0};

    ColourGradient gradient, alphaGradient;
    
    void drawAverage(Graphics& g,
                     Rectangle<int> bounds,
                     float avg,
                     bool drawBorder);
};

struct StereoImageMeter : Component
{
    StereoImageMeter(AudioBuffer<float>& buffer, double sampleRate);

    void resized() override;
    void adjustGonioMeterScale(float scale);
    void update();
private:

    AudioBuffer<float>& buffer;
    std::unique_ptr<Goniometer> goniometer;

    std::unique_ptr<CorrelationMeter> correlationMeter;
};
//==============================================================================
struct PFMLnF : LookAndFeel_V4
{
    PFMLnF();
    void drawLabel (Graphics &, Label &) override;
    void drawButtonBackground (Graphics& g,
                               Button& button,
                               const Colour& backgroundColour,
                               bool shouldDrawButtonAsHighlighted,
                               bool shouldDrawButtonAsDown) override;
    Font getTextButtonFont (TextButton&, int buttonHeight) override;
    Font getComboBoxFont (ComboBox& box) override;
    void drawComboBox (Graphics&, int width, int height, bool isButtonDown,
                       int buttonX, int buttonY, int buttonW, int buttonH,
                       ComboBox&) override;
    void positionComboBoxText (ComboBox& box, Label& label) override;
    void drawRotarySlider (Graphics&, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle,
                           float rotaryEndAngle, Slider&) override;
    void drawLinearSlider (Graphics&, int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           const Slider::SliderStyle, Slider&) override;

};
//==============================================================================
struct ValueResponder : Value::Listener
{
    ValueResponder(Value& valueToFollow, std::function<void(float)> f) : func(std::move(f))
    {
        value.referTo(valueToFollow);
        value.addListener(this);
    }
    void valueChanged (Value& v) override
    {
        if( v == value )
            if( func )
                func(value.getValue());
    }
private:
    std::function<void(float)> func;
    Value value;
};
//==============================================================================
struct MacroMeter : Component, Value::Listener
{
    MacroMeter(const String& title);
    ~MacroMeter();
    void update(float leftGain, float rightGain);
    void resized() override;
    void paint(Graphics& g) override;

    void setShowHoldTick(bool s);
    void setHoldTime(int ms);
    void setDecayRate(float rateDbPerSec);
    void resetHold();
    void setAverageTime(int ms);
    void setViewType(LevelMeter::ViewType vt);
    void updateThresholdReferral(ValueTree tree, const Identifier& property);
    Slider& getThresholdSlider() { return thresholdSlider; }
    void valueChanged (Value& v) override;
private:
    TextMeter leftTextMeter;
    TextMeter rightTextMeter;

    LevelMeter leftMeter
    {
        false,
        HoldMeter::Orientation::verticalUp
    };
    LevelMeter rightMeter
    {
        true,
        HoldMeter::Orientation::verticalUp
    };

    Label label;
    Slider thresholdSlider { Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox };
//    ValueResponder valueResponder;
};


struct Separator : Component
{
    void paint(Graphics& g) override
    {
        g.setColour(Colours::darkgrey);
        g.fillAll();
    }
};
//==============================================================================
/**
*/
class Pfmcpp_project10AudioProcessorEditor  : public AudioProcessorEditor, public ValueTree::Listener,
public Timer
{
public:
    Pfmcpp_project10AudioProcessorEditor (Pfmcpp_project10AudioProcessor&);
    ~Pfmcpp_project10AudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    void valueTreeRedirected (ValueTree& treeWhichHasBeenChanged) override;
    void valueTreePropertyChanged(ValueTree &treeWhosePropertyHasChanged, const Identifier &property) override
    {
        DBG( "property changed: " << property.toString() << " value: " << (float)treeWhosePropertyHasChanged.getProperty(property));
    }
    void timerCallback() override;
private:
    AudioBuffer<float> buffer {2, 256};
    AudioPlayHead::CurrentPositionInfo currentPositionInfo;

    void restoreSettings();
    void updateHoldVisibility();
    void updateHoldTime();
    void updateDecayTime();
    void updateAverageTime();
    void updateMeterView();

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Pfmcpp_project10AudioProcessor& processor;


    Histogram rmsHistogram {"RMS"}, magHistogram {"PEAK"};
    std::unique_ptr<StereoImageMeter> stereoImageMeter;

    double sampleRate = 44100;
    void makeStereoImageMeterIfNeeded();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Pfmcpp_project10AudioProcessorEditor)

    const int meterWidth = Font(12).getStringWidth("1") * 4;

//    std::unique_ptr<MacroMeter> rmsMeter, magMeter;
    MacroMeter rmsMeter
    {
        "L RMS R"//,
//        [this](float v) { this->rmsHistogram.setThreshold(v); },
//        [this](Slider& s)
//        {
//            s.getValueObject().referTo(processor.settings.getPropertyAsValue(Identifiers::rms_threshold, nullptr));
//        }
    };
    MacroMeter magMeter
    {
        "L PEAK R",//
//        [this](float v) { this->magHistogram.setThreshold(v); },
//        [this](Slider& s)
//        {
//            s.getValueObject().referTo(processor.settings.getPropertyAsValue(Identifiers::mag_threshold, nullptr));
//        }
    };


    PFMLnF labelLnf;
    Label pfmcppForMusiciansLabel { "pfm", "PFM::C++ For Musicians" };

    Separator holdSeparator;

    TextButton holdButton { "HOLD" };
    ComboBox holdTimeCB;
    TextButton resetButton { "RESET" };
    Label decayLabel { "Decay", "DECAY" };
    ComboBox decayCB;
    Separator decaySeparator;
    Label averageLabel { "avg", "AVG" };
    ComboBox averageCB;
    Separator meterViewSeparator;
    Label meterViewLabel { "view", "METER" };
    ComboBox meterViewCB;

    Separator histoViewSeparator;
    Label histogramLabel { "hgView", "HIST" };
    ComboBox histogramCB;

    Slider gonioScaleSlider { Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox };
    Label gonioScaleLabel { "gnoscale", "SCALE" };
    Label gonioRangeLabel { "gno", "50%   200%" };

    ValueTree settings;
    Component::SafePointer<Pfmcpp_project10AudioProcessorEditor> safePtr;
};

template<typename T>
T* getEditor(Component* c)
{
    if( c == nullptr )
        return nullptr;

    if( auto* e = dynamic_cast<T*>(c) )
       return e;

    return getEditor<T>(c->getParentComponent());
}

