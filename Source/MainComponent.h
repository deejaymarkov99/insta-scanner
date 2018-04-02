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

class MainComponent    : public AudioAppComponent,
                         //public KeyListener,
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
    
    void handleUp();
    
    void handleDown();
    
    void handleRight();
    
    void handleLeft();
    
    void buttonClicked (Button* button) override;
    
    bool keyPressed (const KeyPress &key) override;
    
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
            //g.drawRect(x, y, 10, 10, 1);
        }
        
        void timerCallback() override
        {
            int x = *lensX;
            int y = *lensY;
            
            LensMovingState s = *state;
            
            switch(s)
            {
                case N    :
                    if(y > 0){
                        incUp();
                        repaint();
                    } else {
                        *state = S;
                    }
                    break;
                case S    :
                    if(y < 396){
                        incDown();
                        repaint();
                    } else {
                        y = 395;
                        *state = N;
                    }
                    break;
                case E    :
                    if(x < 396){
                        incRight();
                        repaint();
                    } else {
                        x = 395;
                        *state = W;
                    }
                    break;
                case W    :
                    if(x > 0){
                        incLeft();
                        repaint();
                    } else {
                        *state = E;
                    }
                    break;
                case NE   :
                    if(x < 396 && y > 0){
                        incRight();
                        incUp();
                        repaint();
                    } else if (x < 396) {
                        *state = SE;
                    } else if (y > 0) {
                        x = 395;
                        *state = NW;
                    }
                    else {
                        x = 395;
                        *state = HALT;
                    }
                    break;
                case NW   :
                    if(x > 0 && y > 0){
                        incLeft();
                        incUp();
                        repaint();
                    } else if (x > 0) {
                        *state = SW;
                    } else if (y > 0) {
                        *state = NE;
                    }
                    else {
                        *state = HALT;
                    }
                    break;
                case SE   :
                    if(x < 396 && y < 396){
                        incRight();
                        incDown();
                        repaint();
                    } else if (x < 396) {
                        *state = NE;
                    } else if (y < 396) {
                        x = 395;
                        *state = SW;
                    }
                    else {
                        x = 395;
                        y = 395;
                        *state = HALT;
                    }
                    break;
                case SW   :
                    if(x > 0 && y < 396){
                        incLeft();
                        incDown();
                        repaint();
                    } else if (x > 0) {
                        *state = NW;
                    } else if (y < 396) {
                        *state = SE;
                    }
                    else {
                        y = 395;
                        *state = HALT;
                    }
                    break;
                case HALT :
                    break;
            }
            
            if (y == 396) {
                y = 395;
            }
            
            low->setFrequency(freqs[y][x+3]);
            lomid->setFrequency(2*freqs[y+13][x+29]);
            mid->setFrequency(4*freqs[y+22][x+27]);
            himid->setFrequency(8*freqs[y+44][x+7]);
            high->setFrequency(16*freqs[y+36][x+31]);
            
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
            im = i.rescaled(4400,4400);
        }
        
        void setImage(Image i) {
            im = i.rescaled(4400,4400);
        }
        
        void paint (Graphics& g) override
        {
            int x = *lensX;
            int y = *lensY;
            g.drawImage(im, 0, 0, 440, 440, x*10, y*10, 440, 440);
            //g.drawImageAt(img, 0, 0);
            //g.drawImage(img, 0, 0, 438, 438, x, y, 10, 10);
        }
        
        void timerCallback() override
        {
            repaint();
        }
        
    private:
        int *lensX;
        int *lensY;
        Image im;
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
