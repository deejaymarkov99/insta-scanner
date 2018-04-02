// Music 256a / CS 476a | fall 2016
// CCRMA, Stanford University
//
// Author: Romain Michon (rmichonATccrmaDOTstanfordDOTedu)
// Description: Simple sine wave oscillator

#ifndef SINE_H_INCLUDED
#define SINE_H_INCLUDED

class Sine{

public:
    
    Sine():currentSampleRate(0.0),
                currentAngle(0.0),
            currentFrequency(0.0),
            currentAmplitude(0.0),
             targetFrequency(0.0),
                  angleDelta(0.0),
                   amplitude(0.0),
                      maxAmp(1.0),
                     susAmp(0.75),
               attackTime(1000.0),
                decayTime(5000.0),
               releaseTime(2500.0){}
    
    ~Sine(){}
    
    enum State { ATTACKING, DECAYING, SUSTAINING, RELEASING, STOPPED };
    
    // sampling rate must be set to get an accurate frequency
    void setSamplingRate(int samplingRate){
        currentSampleRate = samplingRate;
    }
    
    int getSamplingRate(){
        return currentSampleRate;
    }
    
    void setFrequency(float frequency){
        targetFrequency = frequency;
    }
    
    void updateDelta(float frequency) {
        const double cyclesPerSample = frequency / currentSampleRate;
        angleDelta = cyclesPerSample * 2.0 * PI;
    }
    
    // compute one sample
    double tick(){
        
        if(currentFrequency != 0 && targetFrequency == 0 && s != RELEASING) {
            s = RELEASING;
            std::cout << "RELEASING NOW\n";
        }
        
        if(currentFrequency == 0 && targetFrequency != 0 &&  s != ATTACKING) {
            s = ATTACKING;
            std::cout << "ATTACKING NOW\n";
        }
        
        const float currentSample = (double)std::sin (currentAngle);
        
        if (s == ATTACKING) { //if in the attacking state
            currentAmplitude += 1.0/((attackTime/1000)*currentSampleRate);
            if (currentAmplitude >= maxAmp) {
                currentAmplitude = maxAmp;
                s = DECAYING;
                std::cout << "DECAYING NOW\n";
            }
        }
        
        if (s == DECAYING) {
            currentAmplitude -= 1.0/((decayTime/1000)*currentSampleRate);
            if (currentAmplitude <= susAmp) {
                currentAmplitude = susAmp;
                s = SUSTAINING;
                std::cout << "SUSTAINING NOW\n";
            }
        }
        
        if (s == RELEASING) {
            currentAmplitude -= 1.0/((releaseTime/1000)*currentSampleRate);
            if (currentAmplitude <= 0.0) {
                currentAmplitude = 0.0;
                s = STOPPED;
                currentFrequency = 0;
                std::cout << "STOPPED NOW\n";
            }
        }
        
        if (currentFrequency != targetFrequency && s != RELEASING){ //notes are switching
            
            const double increment = (targetFrequency - currentFrequency)/(512);
            
            currentFrequency += increment;
            updateDelta(currentFrequency);
            
        }
        
        currentAngle += angleDelta;
        return currentSample*currentAmplitude;
    }

private:
    
    double currentSampleRate; //current sample rate
    double currentAngle; //current angle
    double currentFrequency; //the frequency currently being played
    double currentAmplitude; //
    double targetFrequency; //the next frequency to be played
    double angleDelta; //change in angle
    double amplitude; //current volume of the signal
    double maxAmp; //the volume the attack time will peak at (at the end)
    double susAmp; //the volume the signal will be at after the decay
    double attackTime, decayTime, releaseTime; //times for ADR, in ms
    const double PI = 3.1415926535897931;
    Sine::State s = STOPPED;
    
};


#endif  // SINE_H_INCLUDED
