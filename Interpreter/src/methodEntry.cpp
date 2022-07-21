#pragma once
#include "methodEntry.h"

namespace POLDAM
{

    void LogInterpreter<METHOD_ENTRY>::parseLog()
    {
        const std::vector<std::string> parsedLog = POLDAM_UTIL::parse(log);
        METHOD_ENTRY rec = {};

        for (int i = 0; i < parsedLog.size(); ++i)
        {
            std::string str = parsedLog[i];
            switch (i)
            {
            case 0:
                rec.eventId = std::stoul(POLDAM_UTIL::split(str, '=')[1]);
                break;
            case 1:
                // No need to recored. struc has default value
                break;
            case 2:
                // Thread Id
                rec.threadId = std::stoi(POLDAM_UTIL::split(str, '=')[1]);
                break;

            case 3:
                // Data Id
                rec.dataId = std::stoul(POLDAM_UTIL::split(str, '=')[1]);
                break;

            case 4:
                // Value
                rec.value = std::stoi(POLDAM_UTIL::split(str, '=')[1]);
                break;
            case 5:
                // method
                rec.methodAndNum = str;
                break;
            case 6:
                rec.lineNum = std::stoi(str);
                break;
            case 7:
                rec.className = str;
                break;
            case 8:
                rec.methodName = str;
                break;
            case 9:
                rec.argType = str;
                break;
            case 10:
                rec.num = std::stoi(str);
                break;
            case 11:
                rec.fileName = str;
                break;
            case 12:
                rec.hashValue = str;
                break;
            case 13:
                rec.fileAndClassAndMethod = str;
                break;

            default:
                rec.other += str;
                rec.other += ",";
                break;
            }
        }

        methodEntry = rec;
    }

    std::string LogInterpreter<METHOD_ENTRY>::getLog()
    {
        return log;
    }

    EventType LogInterpreter<METHOD_ENTRY>::getEventType()
    {
        return EventType::METHOD_ENTRY;
    }

    METHOD_ENTRY LogInterpreter<METHOD_ENTRY>::getParserResult()
    {
        return methodEntry;
    }

    void LogInterpreter<METHOD_ENTRY>::debug()
    {
        std::cout << "eventId " << methodEntry.eventId << std::endl;
    }

}
