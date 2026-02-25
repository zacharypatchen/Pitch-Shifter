#include "../JuceLibraryCode/JuceHeader.h"

#include "MrShifty.h"
#include "MrShifty_types.h"

#if JUCE_VERSION >= 0x050400
using Parameter = AudioProcessorValueTreeState::Parameter;
#endif

struct onParamChangeListener : AudioProcessorValueTreeState::Listener
{
    onParamChangeListener(MrShiftyStackData* sd) : SD(sd)
    {
    }

    void parameterChanged (const String& parameterID, float newValue) override
    {
        const int idx = getParameterIndex(parameterID);
        onParamChangeCImpl(SD, idx, static_cast<double>(newValue));
    }
    
private:
    int getParameterIndex (const String& parameterID)
    {
        (void)parameterID;
    
        if (parameterID == "PITCHSHIFT") {
            return 0;
        }
        if (parameterID == "OVERLAY") {
            return 1;
        }
        if (parameterID == "LF_FREQ") {
            return 2;
        }
        if (parameterID == "LF_GAIN") {
            return 3;
        }
        if (parameterID == "LF_Q") {
            return 4;
        }
        if (parameterID == "MF_FREQ") {
            return 5;
        }
        if (parameterID == "MF_GAIN") {
            return 6;
        }
        if (parameterID == "MF_Q") {
            return 7;
        }
        if (parameterID == "HF_FREQ") {
            return 8;
        }
        if (parameterID == "HF_GAIN") {
            return 9;
        }
        if (parameterID == "HF_Q") {
            return 10;
        }
        if (parameterID == "DRIVE") {
            return 11;
        }
        if (parameterID == "PITCHSHIFT_MIX") {
            return 12;
        }
    
        return (-1);
    }    
    
    MrShiftyStackData *SD;
};

//==============================================================================
class MrShiftyAudioProcessor  : public AudioProcessor
{
    //==============================================================================
#if JUCE_VERSION >= 0x050400

public:
    MrShiftyAudioProcessor()
        : paramListener(&mStackData),
          parameters(*this, nullptr, "MrShifty", {
                std::make_unique<Parameter>("PITCHSHIFT", juce::String::fromUTF8(u8" "), juce::String::fromUTF8(u8"semitones"),
NormalisableRange<float>(-12.f,12.f,1.f), 0.f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("OVERLAY", juce::String::fromUTF8(u8" "), juce::String::fromUTF8(u8""),
NormalisableRange<float>(0.01f,0.5f), 0.45f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("LF_FREQ", juce::String::fromUTF8(u8" "), juce::String::fromUTF8(u8"Hz"),
NormalisableRange<float>(20.f,400.f,[](float min, float max, float norm) {return min*powf(max/min,norm);}, [](float min, float max, float val) {return logf(val/min)/logf(max/min);}), 100.f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("LF_GAIN", juce::String::fromUTF8(u8" "), juce::String::fromUTF8(u8"dB"),
NormalisableRange<float>(-15.f,15.f), 0.f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("LF_Q", juce::String::fromUTF8(u8" "), juce::String::fromUTF8(u8""),
NormalisableRange<float>(0.1f,2.f), 0.707f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("MF_FREQ", juce::String::fromUTF8(u8" "), juce::String::fromUTF8(u8"Hz"),
NormalisableRange<float>(200.f,8000.f,[](float min, float max, float norm) {return min*powf(max/min,norm);}, [](float min, float max, float val) {return logf(val/min)/logf(max/min);}), 1000.f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("MF_GAIN", juce::String::fromUTF8(u8" "), juce::String::fromUTF8(u8"dB"),
NormalisableRange<float>(-15.f,15.f), 0.f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("MF_Q", juce::String::fromUTF8(u8" "), juce::String::fromUTF8(u8""),
NormalisableRange<float>(0.1f,2.f), 0.707f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("HF_FREQ", juce::String::fromUTF8(u8" "), juce::String::fromUTF8(u8"Hz"),
NormalisableRange<float>(2000.f,16000.f,[](float min, float max, float norm) {return min*powf(max/min,norm);}, [](float min, float max, float val) {return logf(val/min)/logf(max/min);}), 8000.f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("HF_GAIN", juce::String::fromUTF8(u8" "), juce::String::fromUTF8(u8"dB"),
NormalisableRange<float>(-15.f,15.f), 0.f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("HF_Q", juce::String::fromUTF8(u8" "), juce::String::fromUTF8(u8""),
NormalisableRange<float>(0.1f,2.f), 0.707f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("DRIVE", juce::String::fromUTF8(u8" "), juce::String::fromUTF8(u8"%"),
NormalisableRange<float>(0.f,100.f), 0.f, [](float val) {return String(val, 3);}, nullptr),
                std::make_unique<Parameter>("PITCHSHIFT_MIX", juce::String::fromUTF8(u8" "), juce::String::fromUTF8(u8"%"),
NormalisableRange<float>(1.f,100.f), 50.f, [](float val) {return String(val, 3);}, nullptr) })

    {
        mStackData.pd = &mPersistentData;
        
        MrShifty_initialize(&mStackData);

        createPluginInstance(&mStackData, reinterpret_cast<unsigned long long>(this));

        parameters.addParameterListener("PITCHSHIFT", &paramListener);
        parameters.addParameterListener("OVERLAY", &paramListener);
        parameters.addParameterListener("LF_FREQ", &paramListener);
        parameters.addParameterListener("LF_GAIN", &paramListener);
        parameters.addParameterListener("LF_Q", &paramListener);
        parameters.addParameterListener("MF_FREQ", &paramListener);
        parameters.addParameterListener("MF_GAIN", &paramListener);
        parameters.addParameterListener("MF_Q", &paramListener);
        parameters.addParameterListener("HF_FREQ", &paramListener);
        parameters.addParameterListener("HF_GAIN", &paramListener);
        parameters.addParameterListener("HF_Q", &paramListener);
        parameters.addParameterListener("DRIVE", &paramListener);
        parameters.addParameterListener("PITCHSHIFT_MIX", &paramListener);

    }
    //==============================================================================
#else // For JUCE prior to 5.4.0
public:
    MrShiftyAudioProcessor()
    :   paramListener(&mStackData), parameters (*this, nullptr)
    {
        mStackData.pd = &mPersistentData;
        
        MrShifty_initialize(&mStackData);

        createPluginInstance(&mStackData, reinterpret_cast<unsigned long long>(this));

        //
        // Parameter property PITCHSHIFT
        //
        parameters.createAndAddParameter ("PITCHSHIFT", juce::String::fromUTF8(u8" "), juce::String::fromUTF8(u8"semitones"),
            NormalisableRange<float>(-12.f, 12.f, 1.f), 0.f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("PITCHSHIFT", &paramListener);

        //
        // Parameter property OVERLAY
        //
        parameters.createAndAddParameter ("OVERLAY", juce::String::fromUTF8(u8" "), juce::String::fromUTF8(u8""),
            NormalisableRange<float>(0.01f, 0.5f), 0.45f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("OVERLAY", &paramListener);

        //
        // Parameter property LF_FREQ
        //
        parameters.createAndAddParameter ("LF_FREQ", juce::String::fromUTF8(u8" "), juce::String::fromUTF8(u8"Hz"),
            NormalisableRange<float>(20.f, 400.f, 
                [](float min, float max, float norm) {return min * powf(max/min, norm);},
                [](float min, float max, float val) {return logf(val/min)/logf(max/min);} ),
            100.f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("LF_FREQ", &paramListener);

        //
        // Parameter property LF_GAIN
        //
        parameters.createAndAddParameter ("LF_GAIN", juce::String::fromUTF8(u8" "), juce::String::fromUTF8(u8"dB"),
            NormalisableRange<float>(-15.f, 15.f), 0.f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("LF_GAIN", &paramListener);

        //
        // Parameter property LF_Q
        //
        parameters.createAndAddParameter ("LF_Q", juce::String::fromUTF8(u8" "), juce::String::fromUTF8(u8""),
            NormalisableRange<float>(0.1f, 2.f), 0.707f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("LF_Q", &paramListener);

        //
        // Parameter property MF_FREQ
        //
        parameters.createAndAddParameter ("MF_FREQ", juce::String::fromUTF8(u8" "), juce::String::fromUTF8(u8"Hz"),
            NormalisableRange<float>(200.f, 8000.f, 
                [](float min, float max, float norm) {return min * powf(max/min, norm);},
                [](float min, float max, float val) {return logf(val/min)/logf(max/min);} ),
            1000.f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("MF_FREQ", &paramListener);

        //
        // Parameter property MF_GAIN
        //
        parameters.createAndAddParameter ("MF_GAIN", juce::String::fromUTF8(u8" "), juce::String::fromUTF8(u8"dB"),
            NormalisableRange<float>(-15.f, 15.f), 0.f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("MF_GAIN", &paramListener);

        //
        // Parameter property MF_Q
        //
        parameters.createAndAddParameter ("MF_Q", juce::String::fromUTF8(u8" "), juce::String::fromUTF8(u8""),
            NormalisableRange<float>(0.1f, 2.f), 0.707f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("MF_Q", &paramListener);

        //
        // Parameter property HF_FREQ
        //
        parameters.createAndAddParameter ("HF_FREQ", juce::String::fromUTF8(u8" "), juce::String::fromUTF8(u8"Hz"),
            NormalisableRange<float>(2000.f, 16000.f, 
                [](float min, float max, float norm) {return min * powf(max/min, norm);},
                [](float min, float max, float val) {return logf(val/min)/logf(max/min);} ),
            8000.f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("HF_FREQ", &paramListener);

        //
        // Parameter property HF_GAIN
        //
        parameters.createAndAddParameter ("HF_GAIN", juce::String::fromUTF8(u8" "), juce::String::fromUTF8(u8"dB"),
            NormalisableRange<float>(-15.f, 15.f), 0.f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("HF_GAIN", &paramListener);

        //
        // Parameter property HF_Q
        //
        parameters.createAndAddParameter ("HF_Q", juce::String::fromUTF8(u8" "), juce::String::fromUTF8(u8""),
            NormalisableRange<float>(0.1f, 2.f), 0.707f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("HF_Q", &paramListener);

        //
        // Parameter property DRIVE
        //
        parameters.createAndAddParameter ("DRIVE", juce::String::fromUTF8(u8" "), juce::String::fromUTF8(u8"%"),
            NormalisableRange<float>(0.f, 100.f), 0.f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("DRIVE", &paramListener);

        //
        // Parameter property PITCHSHIFT_MIX
        //
        parameters.createAndAddParameter ("PITCHSHIFT_MIX", juce::String::fromUTF8(u8" "), juce::String::fromUTF8(u8"%"),
            NormalisableRange<float>(1.f, 100.f), 50.f,
            [](float val) {return String(val, 3);},
            nullptr);
        parameters.addParameterListener("PITCHSHIFT_MIX", &paramListener);

        parameters.state = ValueTree(Identifier("MrShifty"));
    }
#endif

    //==============================================================================
    ~MrShiftyAudioProcessor()
    {
        MrShifty_terminate(&mStackData);
    }
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override
    {
        (void)samplesPerBlock;
        resetCImpl(&mStackData, sampleRate);
        setLatencySamples(getLatencyInSamplesCImpl(&mStackData));
    }

    void releaseResources() override                { }
    
    
    void processBlock (AudioBuffer<double>& buffer, MidiBuffer&) override
    {
        ScopedNoDenormals noDenormals;
        const int nSamples = buffer.getNumSamples();
        const int nChannels = buffer.getNumChannels();
        const double* const* pin = buffer.getArrayOfReadPointers();
        double* const* pout = buffer.getArrayOfWritePointers();
        std::vector<const double*> inputs(pin,pin+nChannels);
        std::vector<double*> outputs(pout,pout+nChannels);
        int i_;

        MrShiftyStackData *SD = &mStackData;

        if (nChannels < 2) {
            const int numExtraChannels = 2 - nChannels;
            tempBuffer.setSize(numExtraChannels, nSamples);
            if (nChannels < 2) {
                tempBuffer.clear(0, nSamples);
                const double* const* p = tempBuffer.getArrayOfReadPointers();
                std::copy(p, p+numExtraChannels, std::back_inserter(inputs));
            }
            if (nChannels < 2) {
                double* const* p = tempBuffer.getArrayOfWritePointers();
                std::copy(p, p+numExtraChannels, std::back_inserter(outputs));
            }
        }

        int osz0_;
        int osz1_;
        if (nSamples <= MAX_SAMPLES_PER_FRAME) {
            /* Fast path for common frame sizes. */
            const int isz0_ = nSamples;
            const int isz1_ = nSamples;
            processEntryPoint(SD, (double)nSamples,
                    inputs[0], &isz0_,
                    inputs[1], &isz1_,
                    outputs[0], &osz0_,
                    outputs[1], &osz1_);
        } else {
            /* Fallback for unusually large frames. */
            int isz0_ = MAX_SAMPLES_PER_FRAME;
            int isz1_ = MAX_SAMPLES_PER_FRAME;
            int n = MAX_SAMPLES_PER_FRAME;
            for (i_ = 0; i_ < nSamples; i_ += MAX_SAMPLES_PER_FRAME) {
                if (i_ + MAX_SAMPLES_PER_FRAME > nSamples) {
                    n = nSamples - i_;
                    isz0_ = nSamples - i_;
                    isz1_ = nSamples - i_;
                }
                processEntryPoint(SD, (double)n,
                        inputs[0]+i_, &isz0_,
                        inputs[1]+i_, &isz1_,
                        outputs[0]+i_, &osz0_,
                        outputs[1]+i_, &osz1_);
            }
        }

    }
    
    void processBlock (AudioBuffer<float>& buffer,  MidiBuffer& midiMessages) override
    {
        AudioBuffer<double> doubleBuffer;
        doubleBuffer.makeCopyOf(buffer);
        processBlock(doubleBuffer, midiMessages);
        buffer.makeCopyOf(doubleBuffer);
    }
    
    //==============================================================================
    bool hasEditor() const override                 { return true; }
    AudioProcessorEditor* createEditor() override;
    
    //==============================================================================
    const String getName() const override           { return JucePlugin_Name; }

    bool acceptsMidi() const override               { return false; }
    bool producesMidi() const override              { return false; }
    bool isMidiEffect () const override             { return false; }
    double getTailLengthSeconds() const override    { return 0.0;   }

    //==============================================================================
    // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
    int getNumPrograms() override                       { return 1;  }
    int getCurrentProgram() override                    { return 0;  }
    void setCurrentProgram (int index) override         { (void) index; }
    const String getProgramName (int index) override    { (void) index; return {}; }
    void changeProgramName (int index, const String& newName) override  { (void) index; (void) newName; }
    
    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override
    {
        auto xml (parameters.state.createXml());
        copyXmlToBinary (*xml, destData);
    }
    
    void setStateInformation (const void* data, int sizeInBytes) override
    {
        auto xmlState (getXmlFromBinary (data, sizeInBytes));
        if (xmlState != nullptr)
            if (xmlState->hasTagName (parameters.state.getType()))
                parameters.state = ValueTree::fromXml (*xmlState);
    }
    
    bool supportsDoublePrecisionProcessing() const override  { return true; }
    
private:
    //==============================================================================
    static const int MAX_SAMPLES_PER_FRAME = 4096;

    MrShiftyStackData mStackData;
    MrShiftyPersistentData mPersistentData;
    onParamChangeListener paramListener;
    AudioBuffer<double> tempBuffer;
    
    //==============================================================================
    AudioProcessorValueTreeState parameters;
 
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MrShiftyAudioProcessor)
};

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MrShiftyAudioProcessor();
}

#include "MrShiftyPluginEditor.h"

AudioProcessorEditor* MrShiftyAudioProcessor::createEditor()
{
    return new MrShiftyAudioProcessorEditor(*this, parameters);
}

