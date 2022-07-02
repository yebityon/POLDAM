#pragma once
#include "methodParam.h"
#include <poldam_util.h>

namespace POLDAM
{
    void LogInterpreter<METHOD_PARAM>::parseLog()
    {
        const std::vector<std::string> parsedLog = POLDAM_UTIL::parse(log);
        METHOD_PARAM rec = {};

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
                rec.fullMethodName = str;
                break;

            default:
                rec.other += str;
                rec.other += ",";
                break;
            }
        }

        methodParam = rec;
    }

    std::string LogInterpreter<METHOD_PARAM>::getLog()
    {
        return log;
    }

    EventType LogInterpreter<METHOD_PARAM>::getEventType()
    {
        return EventType::METHOD_PARAM;
    }

    METHOD_PARAM LogInterpreter<METHOD_PARAM>::getParserResult()
    {
        return methodParam;
    }

    void LogInterpreter<METHOD_PARAM>::debug()
    {
        std::cout << "eventId " << methodParam.eventId << std::endl;
    }
}
