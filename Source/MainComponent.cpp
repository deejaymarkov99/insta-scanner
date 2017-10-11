/*
  ==============================================================================

    MainComponent.cpp
    Created: 15 Aug 2017 6:50:58pm
    Author:  Damjan Markovic
    Other credits: https://github.com/ottobonn/additive-synth and the tutorials found
    at https://www.juce.com/tutorials  
    frequency values found at: http://pages.mtu.edu/~suits/notefreqs.html

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"


//==============================================================================
MainComponent::MainComponent(Image image) : upButton("up", 0.75, Colours::darkblue),
                                 downButton("down", 0.25, Colours::darkblue),
                                 leftButton("left", 0.5,Colours::darkblue),
                                 rightButton("right", 0.0, Colours::darkblue),
                                 lens(&lensX, &lensY, &LensState, &mid),
                                 eye(&lensX, &lensY, image)
{
    //initialize position of the lens
    lensX = 0;
    lensY = 0;
    
    //initialize state of lens
    LensState = HALT;
    
    //load the image
    img = image;
    
    //freqs = new float*[440];
    
    //check which colour it is and assign the appropriate frequency in hertz
    Colour white =     Colour(255,255,255);
    Colour black =     Colour(0,0,0);
    Colour magent =   Colour(255,0,128);
    Colour violet =    Colour(128,0,255);
    Colour blue =      Colour(0,0,255);
    Colour cerulean =  Colour(0,128,255);
    Colour turquoise = Colour(0,255,255);
    Colour teal = Colour(0,255,128);
    Colour green =     Colour(0,255,0);
    Colour lime = Colour(128,255,0);
    Colour yellow =    Colour(255,255,0);
    Colour orange =    Colour(255,128,0);
    Colour red =       Colour(255,0,0);
    
    /*std::tuple <int, int, int, float> white, black, magenta, violet, blue, cerulean, turquoise, teal, green, lime, yellow, orange, red, purple;
    
    std::tuple <int, int, int> currentColour;
    
    white =     std::make_tuple(255,255,255,0.00);
    black =     std::make_tuple(0,0,0,0.00);
    magenta =   std::make_tuple(255,0,128,55.00);
    violet =    std::make_tuple(128,0,255,61.74);
    blue =      std::make_tuple(0,0,255,92.50);
    cerulean =  std::make_tuple(0,128,255,69.30);
    turquoise = std::make_tuple(0,255,255,103.83);
    teal =      std::make_tuple(0,255,128,77.78);
    green =     std::make_tuple(0,255,0,58.27);
    lime =      std::make_tuple(128,255,0,87.31);
    yellow =    std::make_tuple(255,255,0,65.41);
    orange =    std::make_tuple(255,128,0,98.00);
    red =       std::make_tuple(255,0,0,73.42);
    purple =    std::make_tuple(255,0,255,82.41);
    
    std::vector<std::tuple <int, int, int, float>> colours;
    
    colours.push_back(white);
    colours.push_back(black);
    colours.push_back(magenta);
    colours.push_back(violet);
    colours.push_back(blue);
    colours.push_back(cerulean);
    colours.push_back(turquoise);
    colours.push_back(teal);
    colours.push_back(green);
    colours.push_back(lime);
    colours.push_back(yellow);
    colours.push_back(orange);
    colours.push_back(red);
    colours.push_back(purple);
    
    std::vector<std::tuple <int, int, int, float>>::iterator it;*/
    
    for(int i = 0; i < 440; i++){
        for(int j = 0; j < 440; j++) {
            //get the colour at pixel (j,i) and split into red, green, and blue components
            Colour pixCol = img.getPixelAt(j, i);
            uint8 rComp = pixCol.getRed();
            uint8 bComp = pixCol.getBlue();
            uint8 gComp = pixCol.getGreen();
            
            int red = int(rComp);
            int blue = int(bComp);
            int green = int(gComp);
            
            /*
            int r = int(rComp);
            int b = int(bComp);
            int g = int(gComp);
            
            currentColour = std::make_tuple(r,g,b);
            
            float freq = 0.00;
            float minDist = std::numeric_limits<float>::infinity();
            float dist;
            
            for(it=colours.begin() ; it < colours.end(); it++) {
                dist = getCDistance(currentColour, *it);
                if(dist < minDist) {
                    minDist = dist;
                    freq = std::get<3>(*it);
                }
            }
            
            lens.setFreqsAt(i,j,freq);*/
            
            //filter the colour to the one of the chosen 12
            if(red <= 85) {
                red = 0;
            } else if (red <= 170) {
                red = 128;
            } else {
                red = 255;
            }
            
            if(green <= 85) {
                green = 0;
            } else if (green <= 170) {
                green = 128;
            } else {
                green = 255;
            }
            
            if(blue <= 85) {
                blue = 0;
            } else if (blue <= 170) {
                blue = 128;
            } else {
                blue = 255;
            }
            
            int a=rand()%2;
            
            if(red == 0 && green == 0 && blue == 0) {
                red = 255;
                green = 255;
                blue = 255;
            } else if(red == 128 && green == 128 && blue == 128) {
                red = 255;
                green = 255;
                blue = 255;
            } else if(red == 128 && green == 0 && blue == 0) {
                red = 255;
            } else if(red == 0 && green == 128 && blue == 0) {
                green = 255;
            } else if(red == 0 && green == 0 && blue == 128) {
                blue = 255;
            } else if(red == 128 && green == 128 && blue == 0) {
                if (a == 0) {
                    red = 255;
                } else {
                    green = 255;
                }
            } else if(red == 0 && green == 128 && blue == 128) {
                if (a == 0) {
                    green = 255;
                } else {
                    blue = 255;
                }
            } else if(red == 128 && green == 0 && blue == 128) {
                if (a == 0) {
                    red = 255;
                } else {
                    blue = 255;
                }
            } else if(red == 128 && green == 128 && blue == 255) {
                if (a == 0) {
                    red = 0;
                } else {
                    green = 0;
                }
            } else if(red == 255 && green == 128 && blue == 128) {
                if (a == 0) {
                    green = 0;
                } else {
                    blue = 0;
                }
            } else if(red == 128 && green == 255 && blue == 128) {
                if (a == 0) {
                    red = 0;
                } else {
                    blue = 0;
                }
            } else if(red == 255 && green == 128 && blue == 255) {
                green = 0;
            } else if(red == 255 && green == 255 && blue == 128) {
                blue = 0;
            } else if(red == 255 && green == 255 && blue == 128) {
                blue = 0;
            } else {
                red = 255;
                green = 255;
                blue = 255;
            }
            
            //check which colour it is and assign the appropriate frequency in hertz
            if (red == 255 && green == 255 && blue == 255){ //White, Silence
                lens.setFreqsAt(i,j,0.00);
            } else if(red == 255 && green == 0 && blue == 128){//Reddish Purple, A
                lens.setFreqsAt(i,j,55.00);
            } else if (red == 255 && green == 0 && blue == 255) {//Purple, E
                lens.setFreqsAt(i,j,82.41);
            } else if (red == 128 && green == 0 && blue == 255) {//Violet, B
                lens.setFreqsAt(i,j,61.74);
            } else if (red == 0 && green == 0 && blue == 255) {//Blue, F#/Gb
                lens.setFreqsAt(i,j,92.50);
            } else if (red == 0 && green == 128 && blue == 255) {//Turquoise Blue, C#/Db
                lens.setFreqsAt(i,j,69.30);
            } else if (red == 0 && green == 255 && blue == 255) {//Turquoise, G#/Ab
                lens.setFreqsAt(i,j,103.83);
            } else if (red == 0 && green == 255 && blue == 128) {//Turquoise Green, D#/Eb
                lens.setFreqsAt(i,j,77.78);
            } else if (red == 0 && green == 255 && blue == 0) {//Green, A#/Bb
                lens.setFreqsAt(i,j,58.27);
            } else if (red == 128 && green == 255 && blue == 0) {//Lime Green, F
                lens.setFreqsAt(i,j,87.31);
            } else if (red == 255 && green == 255 && blue == 0) {//Yellow, C
                lens.setFreqsAt(i,j,65.41);
            } else if (red == 255 && green == 128 && blue == 0) {//Orange, G
                lens.setFreqsAt(i,j,98.00);
            } else {//Red, D
                lens.setFreqsAt(i,j,73.42);
            }
        }
    }
    
    addAndMakeVisible (lens);
    lens.setColour(0x1000100, Colours::darkblue);
    lens.setBounds(663, 45, 440, 440);
    
    lens.startTimer(20);
    
    addAndMakeVisible(eye);
    eye.setBounds(90, 225, 440, 440);
    eye.startTimer(20);
    
    //Initialize Arrow Buttons
    addAndMakeVisible (upButton);
    upButton.setBounds(279, 20, 60, 60);
    upButton.addListener(this);
    
    addAndMakeVisible (downButton);
    downButton.setBounds(279, 140, 60, 60);
    downButton.addListener(this);
    
    addAndMakeVisible (rightButton);
    rightButton.setBounds(339, 80, 60, 60);
    rightButton.addListener(this);
    
    addAndMakeVisible (leftButton);
    leftButton.setBounds(219, 80, 60, 60);
    leftButton.addListener(this);
    
    //Initialize Frequency Range Buttons
    addAndMakeVisible (lowButton);
    lowButton.setBounds(575, 554, 104, 104);
    lowButton.setColour(0x1000100, Colours::darkblue);
    lowButton.setButtonText("L");
    
    addAndMakeVisible (lowMidButton);
    lowMidButton.setBounds(680, 554, 104, 104);
    lowMidButton.setColour(0x1000100, Colours::darkblue);
    lowMidButton.setButtonText("LM");
    
    addAndMakeVisible (midButton);
    midButton.setBounds(785, 554, 104, 104);
    midButton.setColour(0x1000100, Colours::darkblue);
    midButton.setButtonText("M");
    
    addAndMakeVisible (highMidButton);
    highMidButton.setBounds(890, 554, 104, 104);
    highMidButton.setColour(0x1000100, Colours::darkblue);
    highMidButton.setButtonText("HM");
    
    addAndMakeVisible (highButton);
    highButton.setBounds(995, 554, 104, 104);
    highButton.setColour(0x1000100, Colours::darkblue);
    highButton.setButtonText("H");
    
    setSize (1146, 708);
    
    numCh = 2;
    gain = 0.5;
    setAudioChannels (0, numCh); // no inputs, one output
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::paint (Graphics& g)
{
    g.fillAll (Colours::lightblue);
    g.drawImageAt(img, 663, 45);
    
}

/*
void MainComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    currentSizeAsString = std::to_string(getWidth()) + " X " + std::to_string(getHeight());

}*/

void MainComponent::prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate)
{
    samplingRate = sampleRate;
    mid.setSamplingRate(sampleRate);
    
}

void MainComponent::releaseResources()
{
    
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // getting the audio output buffer to be filled
    float* const buffer = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
    
    for (int index = 0; index < bufferToFill.numSamples; ++index)
    {
        float sample = 0;
        sample += mid.tick();
        
        sample *= gain;
        buffer[index] = sample;
    }
}

/*float getCDistance(std::tuple <int, int, int> from, std::tuple <int, int, int, float> to)
{
    float r = (std::get<0>(from) + std::get<0>(to))/2;
    
    float rDist = (float)(std::get<0>(from) - std::get<0>(to));
    float gDist = (float)(std::get<1>(from) - std::get<1>(to));
    float bDist = (float)(std::get<2>(from) - std::get<2>(to));
    
    return sqrt(((2 + r/256)*pow(rDist,2)) + (4*pow(gDist,2)) + ((2 + (255.f-r)/256)*pow(bDist,2)));
}*/

void MainComponent::buttonClicked (Button* button)
{
    if (button == &upButton)
    {
        switch(LensState)
        {
            case N    :
                LensState = HALT;
                break;
            case S    :
                LensState = N;
                break;
            case E    :
                LensState = NE;
                break;
            case W    :
                LensState = NW;
                break;
            case NE   :
                LensState = E;
                break;
            case NW   :
                LensState = W;
                break;
            case SE   :
                LensState = NE;
                break;
            case SW   :
                LensState = NW;
                break;
            case HALT :
                LensState = N;
                break;
        }
    }
    
    if (button == &downButton)
    {
        switch(LensState)
        {
            case N    :
                LensState = S;
                break;
            case S    :
                LensState = HALT;
                break;
            case E    :
                LensState = SE;
                break;
            case W    :
                LensState = SW;
                break;
            case NE   :
                LensState = SE;
                break;
            case NW   :
                LensState = SW;
                break;
            case SE   :
                LensState = E;
                break;
            case SW   :
                LensState = W;
                break;
            case HALT :
                LensState = S;
                break;
        }
    }
    
    if (button == &rightButton)
    {
        switch(LensState)
        {
            case N    :
                LensState = NE;
                break;
            case S    :
                LensState = SE;
                break;
            case E    :
                LensState = HALT;
                break;
            case W    :
                LensState = E;
                break;
            case NE   :
                LensState = N;
                break;
            case NW   :
                LensState = NE;
                break;
            case SE   :
                LensState = S;
                break;
            case SW   :
                LensState = SE;
                break;
            case HALT :
                LensState = E;
                break;
        }
    }
    
    if (button == &leftButton)
    {
        switch(LensState)
        {
            case N    :
                LensState = NW;
                break;
            case S    :
                LensState = SW;
                break;
            case E    :
                LensState = W;
                break;
            case W    :
                LensState = HALT;
                break;
            case NE   :
                LensState = NW;
                break;
            case NW   :
                LensState = N;
                break;
            case SE   :
                LensState = SW;
                break;
            case SW   :
                LensState = S;
                break;
            case HALT :
                LensState = W;
                break;
        }
    }
}
