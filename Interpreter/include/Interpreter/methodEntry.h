#pragma once
#include <Interpreter/InterpreterCommon.h>

namespace POLDAM
{

    struct METHOD_ENTRY
    {
        std::string entryName;
        EventType eventType = EventType::METHOD_ENTRY;
    };

    class MethodEntry : public ILogType
    {

    public:
        MethodEntry(const std::string log_);
        std::string getLog() override;
        EventType getEventType() override;

    private:
        METHOD_ENTRY data;
        const std::string log;
        const EventType eventType = EventType::METHOD_ENTRY;
    };

    template <>
    class LogInterpreter<MethodEntry> : ILogInterpreter
    {

    public:
        LogInterpreter(const std::string log_) : methodEntry(MethodEntry(log_)){};

        void parseLog() override;

        MethodEntry
        getLogStruct()
        {
            return methodEntry;
        }

    private:
        MethodEntry methodEntry;
    };
}