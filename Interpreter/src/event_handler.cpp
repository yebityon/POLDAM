
#include "event_handler.h"
namespace POLDAM
{
    SELOGGER_EVENT_TYPE getEventType(const std::string &log)
    {
        if (log.find("METHOD_ENTRY") != std::string::npos)
        {
            return SELOGGER_EVENT_TYPE::METHOD_ENTRY;
        }
        else if (log.find("METHOD_PARAM") != std::string::npos)
        {
            return SELOGGER_EVENT_TYPE::METHOD_PARAM;
        }
        else if (log.find("METHOD_NORMAL_EXIT") != std::string::npos)
        {
            return SELOGGER_EVENT_TYPE::METHOD_NORMAL_EXIT;
        }
        else if (log.find("METHOD_EXCEPTIONAL_EXIT") != std::string::npos)
        {
            return SELOGGER_EVENT_TYPE::METHOD_EXCEPTIONAL_EXIT;
        }
        else if (log.find("CALL_PARAM") != std::string::npos)
        {
            return SELOGGER_EVENT_TYPE::CALL_PARAM;
        }
        else if (log.find("CALL_RETURN") != std::string::npos)
        {
            return SELOGGER_EVENT_TYPE::CALL_RETURN;
        }
        else if (log.find("CALL") != std::string::npos)
        {
            return SELOGGER_EVENT_TYPE::CALL;
        }
        else
        {
            return SELOGGER_EVENT_TYPE::UNDEFINED;
        }
    }

    bool isPrimitiveType(const std::string &objectType)
    {
        return objectType.find("Ljava") == std::string::npos;
    }
}