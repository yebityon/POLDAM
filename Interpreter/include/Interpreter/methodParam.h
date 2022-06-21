#pragma once
#include <Interpreter/InterpreterCommon.h>

/*
    "EventId=47,EventType=METHOD_PARAM,ThreadId=0,DataId=197,Value=2,myLibrary/myMath:multiple,myMath.java:0:0";
*/
namespace POLDAM
{
    struct METHOD_PARAM
    {
        unsigned int eventId;
        EventType eventType = EventType::METHOD_PARAM;
        std::string eventTypeName = "METHOD_PARAM";
        unsigned int threadId;
        unsigned int dataId;
        int value;
        std::string methodNameInFile;
        std::string other;
    };

    template <>
    class LogInterpreter<METHOD_PARAM> : ILogInterpreter
    {
    public:
        LogInterpreter<METHOD_PARAM>(const std::string log_) : log(log_){};

        void parseLog() override;
        std::string getLog() override;
        EventType getEventType() override;
        METHOD_PARAM getParserResult();
        void debug();

    private:
        METHOD_PARAM methodParam;
        std::string log;
    };
}
