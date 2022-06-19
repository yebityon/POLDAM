#pragma once
#include <Interpreter/InterpreterCommon.h>

namespace POLDAM
{

    struct METHOD_ENTRY
    {
        std::string entryName;
        unsigned int eventId;
        EventType eventType = EventType::METHOD_ENTRY;
        std::string eventTypeName = "METHOD_ENTRY";
        unsigned int threadId;
        unsigned int dataId;
        int value;
        std::string methodAndNum;
        int lineNum;
        std::string className;
        std::string methodName;
        std::string argType;
        unsigned int num; // WTF
        std::string fileName;
        std::string hashValue;
        std::string methodNameInFile;
        std::string other;
    };

    template <>
    class LogInterpreter<METHOD_ENTRY> : ILogInterpreter
    {

    public:
        LogInterpreter<METHOD_ENTRY>(const std::string log_) : log(log_){};

        void parseLog() override;
        std::string getLog() override;
        EventType getEventType() override;
        METHOD_ENTRY getParserResult();
        void debug();

    private:
        METHOD_ENTRY methodEntry;
        std::string log;
    };
}