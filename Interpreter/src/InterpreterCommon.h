#pragma once

#include <string>
#include <iostream>
#include "../../Util/include/src/poldamUtil.h"
namespace POLDAM
{
    enum class EventType
    {
        RESERVED,
        METHOD_ENTRY,
        METHOD_PARAM,
        METHOD_OBJECT_INITIALIZED,
        METHOD_NORMAL_EXIT,
        METHOD_THROW,
        METHOD_EXCEPTIONAL_EXIT,
        CALL,
        CALL_PARAM,
        CALL_RETURN,
        CATCH_LAVEL,
        CATCH,
        NEW_OBJECT,
        NEW_OBJECT_CREATED,
        GET_INSTANCE_FIELD,
        GET_INSTANCE_FIELD_RESULT,
        GET_STATIC_FIELD,
        PUT_INSTANCE_FIELD,
        PUT_INSTANCE_FIELD_VALUE,
        PUT_INSTANCE_FIELD_BEFORE_INITIALIZATION,
        PUT_STATIC_FIELD,
        ARRAY_LOAD,
        ARRAY_LOAD_INDEX,
        ARRAY_LOAD_RESULT,
        ARRAY_STORE,
        ARRAY_STORE_INDEX,
        ARRAY_STORE_VALUE,
        NEW_ARRAY,
        NEW_ARRAY_RESULT,
        MULTI_NEW_ARRAY,
        MULTI_NEW_ARRAY_OWNER,
        MULTI_NEW_ARRAY_ELEMENT,
        ARRAY_LENGTH,
        ARRAY_LENGTH_RESULT,
        MONITOR_ENTER,
        MONITOR_ENTER_RESULT,
        MONITOR_EXIT,
        OBJECT_CONSTANT_LOAD,
        OBJECT_INSTANCEOF,
        OBJECT_INSTANCEOF_RESULT,
        INVOKE_DYNAMIC,
        INVOKE_DYNAMIC_PARAM,
        INVOKE_DYNAMIC_RESULT,
        LABEL,
        JUMP,
        LOCAL_LOAD,
        LOCAL_STORE,
        LOCAL_INCREMENT,
        RET,
        DIVIDE,

        NUM_EVENT,
        UNDEFINED,
    };

    class ILogInterpreter
    {

    public:
        virtual void parseLog() = 0;
        virtual std::string getLog() = 0;
        virtual EventType getEventType() = 0;
    };

    template <typename T>
    class LogInterpreter : ILogInterpreter
    {

    public:
        void parseLog() override{};
        std::string getLog() override { return std::string(0); };
        EventType getEventType() override { return EventType::UNDEFINED; }
    };
}