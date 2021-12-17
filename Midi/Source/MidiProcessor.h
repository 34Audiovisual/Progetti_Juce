
#pragma once
#include <JuceHeader.h>

constexpr int interval = 4;

class MidiProcessor
{
public:
    
    void process (juce::MidiBuffer& midiMessages)
    {
        processedBuffer.clear();
        
        juce::MidiBuffer::Iterator it (midiMessages);
        /*
         for (auto m: midiMessage)
         doSomethingWithMessage(m.getMessage());
         
         ----------
         oppure:
         
         for (const auto meta : midiMessages) {
                     auto currentMessage = meta.getMessage();
                     
                     if (currentMessage.isNoteOnOrOff())
                    {
                         currentMessage.setNoteNumber(90);
                         
                         processedBuffer.addEvent(currentMessage, meta.samplePosition);
                     }
                     
                     juce::String description = currentMessage.getDescription();
                     
                     DBG (description);
                 }
         
         
         */
        juce::MidiMessage currentMessage;
        int samplePos;
        
        while (it.getNextEvent(currentMessage, samplePos))
        {
           // DBG(currentMessage.getDescription());
            
            if (currentMessage.isNoteOnOrOff())
            {
                //currentMessage.setNoteNumber(50);
                auto transposedMessage = currentMessage;
                auto oldNoteNum = transposedMessage.getNoteNumber();
                
                transposedMessage.setNoteNumber(oldNoteNum + interval);
                
                processedBuffer.addEvent(transposedMessage, samplePos);
            }
            
            processedBuffer.addEvent(currentMessage, samplePos);
            
        }
        
        midiMessages.swapWith(processedBuffer);
    }
    
    juce::MidiBuffer processedBuffer;
};
