/*
  ==============================================================================

    DragWaveform.h
    Created: 30 Nov 2021 7:46:06pm
    Author:  Diego Capoccitti

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class DragWaveform : public juce::Component, public juce::FileDragAndDropTarget, public juce::Timer
{
public:
    DragWaveform(GranulatorAudioProcessor& p) : processor(p), granulatorsL (p.getGranulatorL()), granulatorL(granulatorsL.gran0()), granulatorsR (p.getGranulatorR()), granulatorR(granulatorsR.gran0())
    {
        mFormatManager.registerBasicFormats();
        startTimer(30);
        juce::String filePath = processor.setting.getProperty("path");
        
        if (juce::File::isAbsolutePath(filePath))
                    loadFile(filePath);
    };
    ~DragWaveform(){};
    
    void timerCallback () override
    {
        auto sampleIstL = granulatorL.getIndex();
        float percSampleL = (float)sampleIstL / (float)processor.loadedFile.getNumSamples();
        posIstL = percSampleL * getWidth();
        
        auto sampleIstR = granulatorR.getIndex();
        float percSampleR = (float)sampleIstR / (float)processor.loadedFile.getNumSamples();
        posIstR = percSampleR * getWidth();
        repaint();
    }
    
    
    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colours::black);
         
         if (granulatorL.isFileLoaded() && granulatorR.isFileLoaded())
         {
             g.setColour(juce::Colours::green);
             juce::Path p;
             mAudioPoints.clear();
             auto ratio = processor.loadedFile.getNumSamples() / getWidth();
             auto buffer = processor.loadedFile.getReadPointer(0);
             
             //scale x axis
             for (int sample = 0; sample < processor.loadedFile.getNumSamples(); sample += ratio)
             {
                 mAudioPoints.push_back(buffer[sample]);
             }
             
             p.startNewSubPath(0, getHeight()/2);
             
             //scale y axis
             for (int sample = 0; sample < mAudioPoints.size(); ++sample)
             {
                 auto point = juce::jmap<float>(mAudioPoints[sample], -1.f, 1.f, 200, 0);
                 p.lineTo(sample, point);
             }
             
             g.strokePath(p, juce::PathStrokeType(1));
             
             //position lineL
             g.setColour(juce::Colours::white);
             auto xPosL = (int)((float)granulatorL.getPosition() * (float)getWidth());
             g.drawLine(xPosL, 0, xPosL, getHeight(), 2);
             
             //position lineR
             auto xPosR = 0;
             
             if (processor.mono)
                 xPosR = xPosL;
             else
                 xPosR = (int)((float)granulatorR.getPosition() * (float)getWidth());
             
             g.drawLine(xPosR, 0, xPosR, getHeight(), 2);
             
             //DBG(granulator.getPosition() << " " << xPos);
             
             //grainLenghtL
             auto nSampleRndL = granulatorL.getGrainLen() * processor.getSampleRate() / 1000.f;
             auto xLenL = (int)(((float)nSampleRndL / (float)processor.loadedFile.getNumSamples()) * getWidth());
             g.setColour(juce::Colours::red.withAlpha(0.2f));
             g.fillRect(xPosL, 0, xLenL, getHeight());
             
             //grainLenghtR
             auto nSampleRndR = granulatorR.getGrainLen() * processor.getSampleRate() / 1000.f;
             auto xLenR = (int)(((float)nSampleRndR / (float)processor.loadedFile.getNumSamples()) * getWidth());
             g.setColour(juce::Colours::blue.withAlpha(0.2f));
             g.fillRect(xPosR, 0, xLenR, getHeight());
             
             //draw Instant Cursor L
             g.setColour(juce::Colours::red);
             auto xPosIstL = xPosL + posIstL;
             g.drawLine(xPosIstL, 0, xPosIstL, getHeight(), 1);
             
             //draw Instant Cursor R
             g.setColour(juce::Colours::blue);
             auto xPosIstR = 0;
             
             if (processor.mono)
                 xPosIstR = xPosIstL;
             else
                 xPosIstR = xPosR + posIstR;
             
             g.drawLine(xPosIstR, 0, xPosIstR, getHeight(), 1);
             
             //position read line
             if (processor.isPlaying)
             {
                 g.setColour(juce::Colours::limegreen);
                 auto xReadPos = (int)((float)processor.getReadIdx() /  (float)processor.loadedFile.getNumSamples() * (float)getWidth());
                 g.drawLine(xReadPos, 0, xReadPos, getHeight(), 2);
             }
         }
        else
        {
            g.setColour(juce::Colours::green);
            g.setFont(25);
            g.drawFittedText("DRAG AND DROP AUDIO FILE", 0, getHeight() / 2 - 10, getWidth(), 20, juce::Justification::centred, 1);
        };
    }
    void resized() override
    {
        
    }
    
    //FileDragAndDropTarget pure virtual method
    bool isInterestedInFileDrag(const juce::StringArray& files) override
    {
        for (auto file : files)
        {
            if (file.contains (".wav") || file.contains (".mp3") || file.contains (".aif") )
            {
                repaint();
                return true;
            }
        }

        return false;
    }

    void filesDropped (const juce::StringArray& files, int x, int y) override
    {
        for (auto file : files)
        {
            if (isInterestedInFileDrag(file))
            {
                loadFile(file);
            }
        }
    }
    
    void loadFile ( const juce::String& path)
     {
        
        granulatorsL.setOn(false);
        granulatorL.setFileLoaded(false);
        granulatorsR.setOn(false);
        granulatorR.setFileLoaded(false);
        processor.loadedFile.clear();
        granulatorsL.clear();
        granulatorsR.clear();

         auto file = juce::File (path);
         auto* reader = mFormatManager.createReaderFor (file);

         if (reader != nullptr)
         {
             auto sampleLenght = static_cast<int>(reader->lengthInSamples);

             processor.loadedFile.setSize(1, sampleLenght);
             reader->read(&processor.loadedFile, 0, sampleLenght, 0, true, false);
             
             //Salva il percorso del file per richiamarlo
             processor.setting.setProperty("path", path, nullptr);
             processor.prepareGranulator();
             reader->~AudioFormatReader();
         }
        else
        {
            granulatorL.setFileLoaded(false);
            granulatorR.setFileLoaded(false);
        }

     }
private:
    std::vector<float> mAudioPoints;
    juce::AudioFormatManager mFormatManager;
    std::unique_ptr<juce::FileChooser> chooser;
    int posIstL = 0;
    int posIstR = 0;
    
    GranulatorAudioProcessor& processor;
    GranulatorVoices& granulatorsL;
    Granulator& granulatorL;
    
    GranulatorVoices& granulatorsR;
    Granulator& granulatorR;

};
