/*
  ==============================================================================

    MainComponent.h
    Created: 15 Aug 2017 6:50:58pm
    Author:  Damjan Markovic
    Other credits: https://github.com/ottobonn/additive-synth and the tutorials found
    at https://www.juce.com/tutorials

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <iostream>
#include <string>
#include <tuple>
#include <cmath>
#include <vector>
#include <limits>
#include "Sine.h"
#include "ColourNote.h"

//==============================================================================
/*
*/

/*
 // Example program
 #include <iostream>
 #include <string>
 #include <tuple>
 #include <cmath>
 
 float getCDistance(std::tuple <int, int, int> from, std::tuple <int, int, int> to) {
 
 float r = (std::get<0>(from) + std::get<0>(to))/2;
 
 float rDist = (float)(std::get<0>(from) - std::get<0>(to));
 float gDist = (float)(std::get<1>(from) - std::get<1>(to));
 float bDist = (float)(std::get<2>(from) - std::get<2>(to));
 
 return sqrt(((2 + r/256)*pow(rDist,2)) + (4*pow(gDist,2)) + ((2 + (255.f-r)/256)*pow(bDist,2)));
 }
 
 int main()
 {
 std::tuple <int, int, int> c1;
 std::tuple <int, int, int> c2;
 
 c1 = std::make_tuple(100,100,100);
 c2 = std::make_tuple(250,250,250);
 
 float x = getCDistance(c1,c2);
 
 std::cout << x;
 }*/

class MainComponent    : public AudioAppComponent,
                         public Button::Listener
{
public:
    MainComponent(Image image);
    ~MainComponent();
    
    enum LensMovingState { N, S, E, W, NE, NW, SE, SW, HALT };
    
    enum AudioState { ON, OFF };

    void paint (Graphics&) override;
    //void resized() override;
    
    void browseForImage();
    
    void updateLensFreqs();
    
    void buttonClicked (Button* button) override;
    
    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate) override;
    
    void releaseResources() override;
    
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    
    float getCDistance(Colour from, Colour to);
   
    class Lens : public Component, public Timer
    {
    public:
        
        Lens(int *x, int *y, LensMovingState *s, Sine *l, Sine *lm, Sine *m, Sine *hm, Sine *h)
        {
            lensX = x;
            lensY = y;
            state = s;
            low = l;
            lomid = lm;
            mid = m;
            himid = hm;
            high = h;
            
            freqs = new float*[440];
            for(int i = 0; i < 440; i++){
                freqs[i] = new float[440];
            }
        }
        
        ~Lens(){
            
            for(int i = 0; i < 440; i++){
                delete[] freqs[i];
            }
            delete[] freqs;
            
        }
        
        void paint (Graphics& g) override
        {
            g.setColour (Colours::red);
            int x = *lensX;
            int y = *lensY;
            g.drawRect(x, y, 44, 44, 1);
        }
        
        void timerCallback() override
        {
            int x = *lensX;
            int y = *lensY;
            
            LensMovingState s = *state;
            
            low->setFrequency(freqs[y][x]);
            lomid->setFrequency(2*freqs[y][x+44]);
            mid->setFrequency(3*freqs[y+22][x+22]);
            himid->setFrequency(4*freqs[y+44][x]);
            high->setFrequency(5*freqs[y+44][x+44]);
            
            switch(s)
            {
                case N    :
                    if(y > 0){
                        incUp();
                        repaint();
                    } else {
                        *state = HALT;
                    }
                    break;
                case S    :
                    if(y < 394){
                        incDown();
                        repaint();
                    } else {
                        *state = HALT;
                    }
                    break;
                case E    :
                    if(x < 394){
                        incRight();
                        repaint();
                    } else {
                        *state = HALT;
                    }
                    break;
                case W    :
                    if(x > 0){
                        incLeft();
                        repaint();
                    } else {
                        *state = HALT;
                    }
                    break;
                case NE   :
                    if(x < 394 && y > 0){
                        incRight();
                        incUp();
                        repaint();
                    } else if (x < 394) {
                        *state = E;
                    } else if (y > 0) {
                        *state = N;
                    }
                    else {
                        *state = HALT;
                    }
                    break;
                case NW   :
                    if(x > 0 && y > 0){
                        incLeft();
                        incUp();
                        repaint();
                    } else if (x > 0) {
                        *state = W;
                    } else if (y > 0) {
                        *state = N;
                    }
                    else {
                        *state = HALT;
                    }
                    break;
                case SE   :
                    if(x < 394 && y < 394){
                        incRight();
                        incDown();
                        repaint();
                    } else if (x < 394) {
                        *state = E;
                    } else if (y < 394) {
                        *state = S;
                    }
                    else {
                        *state = HALT;
                    }
                    break;
                case SW   :
                    if(x > 0 && y < 394){
                        incLeft();
                        incDown();
                        repaint();
                    } else if (x > 0) {
                        *state = W;
                    } else if (y < 394) {
                        *state = S;
                    }
                    else {
                        *state = HALT;
                    }
                    break;
                case HALT :
                    break;
            }
            
        }
        
        void incUp()
        {
            *lensY -= 1;
        }
        
        void incDown()
        {
            *lensY += 1;
        }
        
        void incRight()
        {
            *lensX += 1;
        }
        
        void incLeft()
        {
            *lensX -= 1;
        }
        
        void setFreqsAt(int i, int j, float f) {
            freqs[i][j] = f;
        }
    
    private:
        int *lensX;
        int *lensY;
        LensMovingState *state;
        Sine *low, *lomid, *mid, *himid, *high;
        float **freqs;
        
    };
    
    class Eyepiece : public Component, public Timer
    {
    public:
        
        Eyepiece(int *x, int *y, Image i) {
            lensX = x;
            lensY = y;
            img = i;
        }
        
        void paint (Graphics& g) override
        {
            int x = *lensX;
            int y = *lensY;
            g.drawImage(img, 0, 0, 438, 438, x, y, 44, 44);
        }
        
        void timerCallback() override
        {
            repaint();
        }
        
    private:
        int *lensX;
        int *lensY;
        Image img;
    };
    
    

private:
    String currentSizeAsString;
    
    int lensX;
    int lensY;
    
    LensMovingState LensState;
    AudioState Audio;
    
    Image img;
    
    ArrowButton upButton;
    ArrowButton downButton;
    ArrowButton leftButton;
    ArrowButton rightButton;
    
    Lens lens;
    Eyepiece eye;
    
    ShapeButton startStopButton;
    ShapeButton browseButton;
    
    bool upPressed = false;
    bool downPressed = false;
    bool leftPressed = false;
    bool rightPressed = false;
    
    TextButton lowButton;
    TextButton lowMidButton;
    TextButton midButton;
    TextButton highMidButton;
    TextButton highButton;
    
    Colour t;
    
    std::vector<ColourNote> colours;
    
    Sine low, lomid, mid, himid, high;
    
    float gain;
    int samplingRate, numCh;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
