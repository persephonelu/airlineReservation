#ifndef CFLIGHT_H
#define CFLIGHT_H

#include <string>

typedef struct CFlightInfo
{
    std::string flightId;
    std::string flightNum;
    std::string departDay;

    std::string departHour;
    std::string departMin;
    std::string source;
    std::string destination;
    std::string seatNum;
    std::string reservedSeatNum;
} Flight;

#endif