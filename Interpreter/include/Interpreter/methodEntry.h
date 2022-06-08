#pragma once
#include <Interpreter/InterpreterCommon.h>

namespace POLDAM
{

    struct METHOD_ENTRY
    {
        std::string entryName;
        EventType eventType = EventType::METHOD_ENTRY;
    };

    class IMethodEntry : public ILogType
    {

    public:
        IMethodEntry(const std::string log_);
        std::string getLog() override;
        EventType getEventType() override;

    private:
            METHOD_ENTRY data;
        const std::string log;
        const EventType eventType = EventType::METHOD_ENTRY;
    };

    template <>
    class LogInterpreter<IMethodEntry> : ILogInterpreter
    {

    public:
        LogInterpreter(const std::string log_) : methodEntry(IMethodEntry(log_)){};

        void parseLog() override{};

        IMethodEntry getLogStruct() { return methodEntry; }

    private:
        IMethodEntry methodEntry;
    };
}