#pragma once
#include <Interpreter/InterpreterCommon.h>
#include <Interpreter/methodEntry.h>
#include <Interpreter/methodParam.h>

#include <string>
#include <cassert>

using namespace POLDAM;

void test_method_entry_1()
{
    const std::string s = "EventId=74,EventType=METHOD_ENTRY,ThreadId=0,DataId=464,Value=0,method:2,15,myLibrary/myMath,addInt,(II)I,9,myMath.java,8e0194cf7a9d0d8444202a95246cee9aa368f660,myLibrary/myMath:addInt,myMath.java:0:0";

    LogInterpreter<METHOD_ENTRY> p(s);
    p.parseLog();

    auto res = p.getParserResult();

    assert(res.eventId == 74);
    assert(res.eventType == POLDAM::EventType::METHOD_ENTRY);
    assert(res.threadId == 0);
    assert(res.dataId == 464);
    assert(res.value == 0);
    assert(res.hashValue == "8e0194cf7a9d0d8444202a95246cee9aa368f660");
    assert(res.argType == "(II)I");

    std::cout << "test_method_entry_1 is success" << std::endl;
}

void test_method_param_1()
{
    const std::string s = "EventId=47,EventType=METHOD_PARAM,ThreadId=0,DataId=197,Value=2,myLibrary/myMath:multiple,myMath.java:0:0";

    LogInterpreter<METHOD_PARAM> p(s);
    p.parseLog();

    auto res = p.getParserResult();
    assert(res.eventId == 47);
    assert(res.eventType == POLDAM::EventType::METHOD_PARAM);
    assert(res.threadId == 0);
    assert(res.dataId == 197);
    assert(res.value == 2);
    assert(res.fullMethodName == "myLibrary/myMath:multiple");
    std::cout << "test_method_param_1 is success" << std::endl;
}

void test_method_param_2()
{
    return;
}
int main()
{
    test_method_entry_1();
    test_method_param_1();
}