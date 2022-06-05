#include <Interpreter/LogEventType.h>
#include <Interpreter/methodEntry.h>

#include <string>
#include <cassert>

using namespace POLDAM;

int main(){

const std::string s = "EventId=74,EventType=METHOD_ENTRY,ThreadId=0,DataId=464,Value=0,method:2,15,myLibrary/myMath,addInt,(II)I,9,myMath.java,8e0194cf7a9d0d8444202a95246cee9aa368f660,myLibrary/myMath:addInt,myMath.java:0:0";

LogInterpreter<IMethodEntry> p(s);

assert(p.getEventType() == EventType::METHOD_ENTRY);

}