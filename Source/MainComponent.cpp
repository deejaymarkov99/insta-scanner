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
                                 lens(&lensX, &lensY, &LensState, &low, &lomid, &mid, &himid, &high),
                                 eye(&lensX, &lensY, image),
                                 startStopButton("start", Colour(0,180,0), Colour(0,180,0), Colour(180,0,0)),
                                 browseButton("browse", Colour(0,0,180), Colour(0,0,180), Colour(0,0,180))
{
    //initialize position of the lens
    lensX = 0;
    lensY = 0;
    
    //initialize state of lens
    LensState = HALT;
    
    //load the image
    img = image;
    
    ColourNote white;
    white.colour = Colour(255,255,255);
    white.freq = 0.00;
    
    ColourNote black;
    black.colour = Colour(0,0,0);
    black.freq = 0.00;
    
    ColourNote magenta;
    magenta.colour = Colour(255,0,128);
    magenta.freq = 55.00;
    
    ColourNote violet;
    violet.colour = Colour(128,0,255);
    violet.freq = 61.74;
    
    ColourNote blue;
    blue.colour = Colour(0,0,255);
    blue.freq = 92.50;
    
    ColourNote cerulean;
    cerulean.colour = Colour(0,128,255);
    cerulean.freq = 69.30;
    
    ColourNote turquoise;
    turquoise.colour = Colour(0,255,255);
    turquoise.freq = 103.83;
    
    ColourNote teal;
    teal.colour = Colour(0,255,128);
    teal.freq = 77.78;
    
    ColourNote green;
    green.colour = Colour(0,255,0);
    green.freq = 58.27;
    
    ColourNote lime;
    lime.colour = Colour(128,255,0);
    lime.freq = 87.31;
    
    ColourNote yellow;
    yellow.colour = Colour(255,255,0);
    yellow.freq = 65.41;
    
    ColourNote orange;
    orange.colour = Colour(255,128,0);
    orange.freq = 98.00;
    
    ColourNote red;
    red.colour = Colour(255,0,0);
    red.freq = 73.42;
    
    ColourNote purple;
    purple.colour = Colour(255,0,255);
    purple.freq = 82.41;
    
    //Colour currentColour;
    //ColourNote compColour;
    
    //std::vector<ColourNote> colours;
    
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
    
    /*std::vector<ColourNote>::iterator it;
    
    for(int i = 0; i < 440; i++){
        for(int j = 0; j < 440; j++) {
            //get the colour at pixel (j,i) and split into red, green, and blue components
            Colour pixCol = img.getPixelAt(j, i);
            uint8 rComp = pixCol.getRed();
            uint8 bComp = pixCol.getBlue();
            uint8 gComp = pixCol.getGreen();
            
            /*int red = int(rComp);
            int blue = int(bComp);
            int green = int(gComp);*/
            
            /*
            int r = int(rComp);
            int b = int(bComp);
            int g = int(gComp);
            
            currentColour = Colour(r,g,b);
            
            float freq = 0.00;
            float minDist = std::numeric_limits<float>::infinity();
            float dist;
            
            for(it=colours.begin() ; it < colours.end(); it++) {
                compColour = *it;
                dist = getCDistance(currentColour, compColour.colour);
                if(dist < minDist) {
                    minDist = dist;
                    freq = compColour.freq;
                }
            }
            
            lens.setFreqsAt(i,j,freq);
        }
    }*/
    
    updateLensFreqs();
    
    addAndMakeVisible (lens);
    lens.setColour(0x1000100, Colours::darkblue);
    lens.setBounds(663, 45, 440, 440);
    
    //lens.startTimerHz(samplingRate);
    
    addAndMakeVisible(eye);
    eye.setBounds(90, 225, 440, 440);
    
    //eye.startTimerHz(samplingRate);
    
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
    
    Path circle;
    circle.addEllipse(0.00,0.00,50.00,50.00);
    
    Path triangle;
    triangle.addTriangle(0, 0, 20, 0, 10, 20);
    
    Audio = OFF;
    
    addAndMakeVisible (startStopButton);
    startStopButton.setShape(circle, true, true, false);
    startStopButton.setBounds(283, 84, 50, 50);
    startStopButton.addListener(this);
    
    addAndMakeVisible (browseButton);
    browseButton.setShape(triangle, true, true, false);
    browseButton.setBounds(800, 6, 20, 20);
    browseButton.addListener(this);
    
    setSize (1146, 708);
    
    numCh = 2;
    gain = 0.25;
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

void MainComponent::browseForImage()
{
    
    FileChooser myChooser ("Please select the image you want to load...",
                           File::getSpecialLocation (File::userHomeDirectory),
                           "*.jpg");
    
    if (myChooser.browseForFileToOpen())
    {
        File imageFile (myChooser.getResult());
        img = ImageFileFormat::loadFrom(imageFile);
    }
    
    updateLensFreqs();
    
}


void MainComponent::updateLensFreqs()
{
    Colour currentColour;
    ColourNote compColour;
    
    std::vector<ColourNote>::iterator it;
    
    for(int i = 0; i < 440; i++){
        for(int j = 0; j < 440; j++) {
            //get the colour at pixel (j,i) and split into red, green, and blue components
            Colour pixCol = img.getPixelAt(j, i);
            uint8 rComp = pixCol.getRed();
            uint8 bComp = pixCol.getBlue();
            uint8 gComp = pixCol.getGreen();
            
            /*int red = int(rComp);
             int blue = int(bComp);
             int green = int(gComp);*/
            
            
            int r = int(rComp);
            int b = int(bComp);
            int g = int(gComp);
            
            currentColour = Colour(r,g,b);
            
            float freq = 0.00;
            float minDist = std::numeric_limits<float>::infinity();
            float dist;
            
            for(it=colours.begin() ; it < colours.end(); it++) {
                compColour = *it;
                dist = getCDistance(currentColour, compColour.colour);
                if(dist < minDist) {
                    minDist = dist;
                    freq = compColour.freq;
                }
            }
            
            lens.setFreqsAt(i,j,freq);
        }
    }
}

void MainComponent::prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate)
{
    samplingRate = sampleRate;
    
    low.setSamplingRate(sampleRate);
    lomid.setSamplingRate(sampleRate);
    mid.setSamplingRate(sampleRate);
    himid.setSamplingRate(sampleRate);
    high.setSamplingRate(sampleRate);
    
    lens.startTimerHz(int(sampleRate*2));
    eye.startTimerHz(int(sampleRate*2));
    
    std::cout << sampleRate;
    
}

void MainComponent::releaseResources()
{
    
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    
    // getting the audio output buffer to be filled
    float* const bufferLeft = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
    
    float* const bufferRight = bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample);
    
    for (int index = 0; index < bufferToFill.numSamples; ++index)
    {
        float sample = 0;
        sample += (1.f/5)*(low.tick() + lomid.tick() + mid.tick() + himid.tick() + high.tick());
        
        sample *= gain;
        bufferLeft[index] = sample;
        bufferRight[index] = sample;
    }
}

float MainComponent::getCDistance(Colour from, Colour to)
{
    float r = (from.getFloatRed() + to.getFloatRed())/2;
    
    float rDist = (from.getFloatRed() - to.getFloatRed());
    float gDist = (from.getFloatGreen() - to.getFloatGreen());
    float bDist = (from.getFloatBlue() - to.getFloatBlue());
    
    return sqrt(((2 + r/256)*pow(rDist,2)) + (4*pow(gDist,2)) + ((2 + (255.f-r)/256)*pow(bDist,2)));
}

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
    
    if (button == &startStopButton)
    {
        switch(Audio) {
            case ON:
                startStopButton.setColours(Colour(0,180,0), Colour(0,180,0), Colour(180,0,0));
                Audio = OFF;
                browseButton.setEnabled(true);
                break;
            case OFF:
                startStopButton.setColours(Colour(180,0,0), Colour(180,0,0), Colour(0,180,0));
                Audio = ON;
                browseButton.setEnabled(false);
                break;
        }
        std::cout << Audio;
    }
    
    if (button == &browseButton)
    {
        browseForImage();
    }
    
}
