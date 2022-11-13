#include <log.h>
#include <Arduino.h>

Log Log::LOG;

Log::Log() : enabled(true)
{
}

void Log::enable()
{
    enabled = true;
}
    
void Log::disable()
{
    enabled = false;
}


void Log::info(const std::string & message) 
{
    if (enabled)
    {
        Serial.println(message.c_str());
    }
}

