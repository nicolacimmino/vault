
#include "AlternateNostrilBreathModeExecutor.h"

void AlternateNostrilBreathModeExecutor::doLoop()
{
    if (this->exerciseEnded)
    {
        if (this->getTimeSinceModeChange() % 2000 < 300)
        {
            this->floodLight->setColor(CRGB::Gold);
            this->floodLight->setFade(190);
            return;
        }
        this->floodLight->setColor(CRGB::Black);
        return;
    }

    this->updateElapsedTimeBar(120000 /* 2 min */);
    this->breatheFloodLight();
}

void AlternateNostrilBreathModeExecutor::breatheFloodLight()
{
    unsigned long phaseDuration = 4000;

    unsigned long cycleTime = this->getTimeSinceModeChange() % (phaseDuration * 2);

    uint8_t fade = 0;

    if (cycleTime < phaseDuration)
    {
        fade = 255.0 * ((cycleTime - phaseDuration) / phaseDuration);        
    }
    else if (cycleTime < phaseDuration * 2)
    {
        fade = (255.0 * (phaseDuration - cycleTime)) / phaseDuration;
    }

    fade = min(250, fade);

    //CRGB color =  (cycleTime % phaseDuration < 300) ? CRGB::DarkOrange : CRGB::Green;
    CRGB color = CRGB::Green;

    this->leftNostril =  (this->getTimeSinceModeChange() % (phaseDuration * 4) < (phaseDuration * 2));    

    if(this->leftNostril) {
      this->floodLight->setColor(color , CRGB::Black);  
    } else {
        this->floodLight->setColor(CRGB::Black, color);  
    }

    //this->floodLight->setColor((this->leftNostril ? color : CRGB::Black), ((!this->leftNostril) ? CRGB::Black : color));
    this->floodLight->setFade(fade);

    
}

void AlternateNostrilBreathModeExecutor::doEnterMode()
{
    this->exerciseEnded = false;
}

void AlternateNostrilBreathModeExecutor::doExitMode()
{
    this->ledBarController->setDim();
    this->ledBarController->showBar(0);

    this->floodLight->setColor(CRGB::Black);
}

CRGB AlternateNostrilBreathModeExecutor::getModeSignatureColor()
{
    return CRGB::Violet;
}
