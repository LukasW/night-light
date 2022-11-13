#ifndef __LOG_H__
#define __LOG_H__

#include <string>

class Log 
{
public:
    Log();

    void enable();
    void disable();

    void info(const std::string & message);

    static Log LOG;
private:
    bool enabled;
};

#endif // __LOG_H__