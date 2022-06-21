#pragma once
#include <Interpreter/InterpreterCommon.h>

/*
    EventId=74,EventType=METHOD_ENTRY,ThreadId=0,DataId=464,Value=0,method:2,15,myLibrary/myMath,addInt,(II)I,9,myMath.java,8e0194cf7a9d0d8444202a95246cee9aa368f660,myLibrary/myMath:addInt,myMath.java:0:0";

*/
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
        std::string fileAndClassAndMethod;
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