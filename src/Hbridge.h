#ifndef __H_BRIDGE__
#define __H_BRIDGE__

#include <Arduino.h>

class HBridgeChannel
{
    const int freq = 30000;
    const int resolution = 8;
    int pwmChannel;
    int enablePin;
    int dirPin1;
    int dirPin2;

public:
    HBridgeChannel(int _enablePin, int _dirPin1, int _dirPin2, int _pwmChannel)
        : pwmChannel(_pwmChannel), enablePin(_enablePin), dirPin1(_dirPin1), dirPin2(_dirPin2)
    {
    }

    void setup()
    {
        pinMode(dirPin1, OUTPUT);
        pinMode(dirPin2, OUTPUT);
        pinMode(enablePin, OUTPUT);
        ledcSetup(pwmChannel, freq, resolution);
        ledcAttachPin(enablePin, pwmChannel);
        velocity(0);
    }

    void velocity(int vel)
    {
        if (vel == 0)
        {
            digitalWrite(dirPin1, LOW);
            digitalWrite(dirPin2, LOW);
        }
        else if (vel > 0)
        {
            digitalWrite(dirPin1, HIGH);
            digitalWrite(dirPin2, LOW);
        }
        else
        {
            digitalWrite(dirPin1, LOW);
            digitalWrite(dirPin2, HIGH);
        }
        ledcWrite(pwmChannel, abs(vel));
    }
};

#endif // __H_BRIDGE