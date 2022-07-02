#pragma once
#include <Interpreter/InterpreterCommon.h>

/*
"EventId=360,EventType=METHOD_NORMAL_EXIT,ThreadId=0,DataId=252,Value=5,objectType=int[],myLibrary/myMath:apply,myMath.java:35:28";
*/
namespace POLDAM
{

    struct METHOD_NORMAL_EXIT
    {
        unsigned int eventId;
        EventType eventType = EventType::METHOD_ENTRY;
        std::string eventTypeName = "METHOD_NORMAL_EXIT";
        unsigned int threadId;
        unsigned int dataId;
        int value;
        std::string objectType;
        std::string fileAndClassAndMethod;
        std::string other;
    };

    template <>
    class LogInterpreter<METHOD_NORMAL_EXIT> : ILogInterpreter
    {

    public:
        LogInterpreter<METHOD_NORMAL_EXIT>(const std::string log_) : log(log_){};

        void parseLog() override;
        std::string getLog() override;
        EventType getEventType() override;
        METHOD_NORMAL_EXIT getParserResult();
        void debug();

    private:
        METHOD_NORMAL_EXIT methodExit;
        std::string log;
    };
}
