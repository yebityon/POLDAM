#pragma once
#include "methodEntry.h"

namespace POLDAM
{

    void LogInterpreter<METHOD_ENTRY>::parseLog()
    {
        const std::vector<std::string> parsedLog = POLDAM_UTIL::parse(log);
        METHOD_ENTRY rec = {};

        // if selogger has objectType, size of indices is different from the one which do not have.
        bool hasObjectType = (this->log.find("objectType") != std::string::npos);

        // FIXME: There are may be better way.
        if (hasObjectType)
        {
            for (unsigned int i = 0; i < parsedLog.size(); ++i)
            {
                const std::string str = parsedLog[i];

                if (i == 0)
                {
                    // eventId
                    rec.eventId = std::stoul(POLDAM_UTIL::split(str, '=')[1]);
                }
                else if (i == 1)
                {
                    // eventType, No need to recored
                }
                else if (i == 2)
                {
                    // Thread Id
                    rec.threadId = std::stoi(POLDAM_UTIL::split(str, '=')[1]);
                }
                else if (i == 3)
                {
                    // Data Id
                    rec.dataId = std::stoul(POLDAM_UTIL::split(str, '=')[1]);
                }
                else if (i == 4)
                {
                    // Value
                    rec.value = std::stoi(POLDAM_UTIL::split(str, '=')[1]);
                }
                else if (i == 5)
                {
                    // Object Type
                    rec.objectType = POLDAM_UTIL::split(str, '=')[1];
                }
                else if (i == 6)
                {
                    rec.methodAndNum = str;
                }
                else if (i == 7)
                {

                    rec.lineNum = std::stoi(str);
                }
                else if (i == 8)
                {
                    rec.className = str;
                }
                else if (i == 9)
                {
                    rec.methodName = str;
                }
                else if (i == 10)
                {
                    rec.argType = str;
                }
                else if (i == 11)
                {
                    rec.num = std::stoi(str);
                }
                else if (i == 12)
                {
                    rec.fileName = str;
                }
                else if (i == 13)
                {
                    rec.hashValue = str;
                }
                else
                {
                    rec.fileAndClassAndMethod = str;
                }
            }
        }
        else
        {
            for (int i = 0; i < parsedLog.size(); ++i)
            {
                const std::string str = parsedLog[i];
                if (i == 0)
                {
                    // eventId
                    rec.eventId = std::stoul(POLDAM_UTIL::split(str, '=')[1]);
                }
                else if (i == 1)
                {
                    // eventType, No need to recored
                }
                else if (i == 2)
                {
                    // Thread Id
                    rec.threadId = std::stoi(POLDAM_UTIL::split(str, '=')[1]);
                }
                else if (i == 3)
                {
                    // Data Id
                    rec.dataId = std::stoul(POLDAM_UTIL::split(str, '=')[1]);
                }
                else if (i == 4)
                {
                    // Value
                    rec.value = std::stoi(POLDAM_UTIL::split(str, '=')[1]);
                }
                else if (i == 5)
                {
                    rec.methodAndNum = str;
                }
                else if (i == 6)
                {

                    rec.lineNum = std::stoi(str);
                }
                else if (i == 7)
                {
                    rec.className = str;
                }
                else if (i == 8)
                {
                    rec.methodName = str;
                }
                else if (i == 9)
                {
                    rec.argType = str;
                }
                else if (i == 10)
                {
                    rec.num = std::stoi(str);
                }
                else if (i == 11)
                {
                    rec.fileName = str;
                }
                else if (i == 12)
                {
                    rec.hashValue = str;
                }
                else
                {
                    rec.fileAndClassAndMethod = str;
                }
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
