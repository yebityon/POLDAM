#pragma once
#include "../src/factory.h"
#include <iostream>
#include <cassert>

int main()
{
    std::cout << "========================= MetafileParser TEST is running =========================\n";

    const std::string inputDir = "./Data/Main_LOG";

    POLDAM::metafileFactory factory(inputDir);

    auto dataids = factory.createInstance<POLDAM::dataidsParser>(inputDir);
    auto seloggerParser = factory.createInstance<POLDAM::seloggerLogParser>(inputDir);

    std::cout << "===================== DATAID PARSER TEST ====================" << std::endl;

    const auto &dataidsData = dataids.getData();
    auto dataidsParsedData = dataids.getParsedData();

    assert(dataidsData[0] == "0,0,0,-1,-1,RESERVED,V,Main#<init>#()V#size=6");
    assert(dataidsParsedData[0].classid == 0);
    assert(dataidsParsedData[0].dataidx == 0);
    assert(dataidsParsedData[0].methodid == 0);
    assert(dataidsParsedData[0].linenumber == -1);
    assert(dataidsParsedData[0].ordernumber == -1);
    assert(dataidsParsedData[0].eventtype == "RESERVED");
    assert(dataidsParsedData[0].descriptor == "V");
    assert(dataidsParsedData[0].eventinfo["Main#<init>#()V#size"] == "6");
    assert(dataidsData.size() == dataidsParsedData.size());

    std::cout << "===================== DATAID PARSER TEST PASSED ====================" << std::endl;

    std::cout << "===================== SELOGGER PARSER TEST ====================" << std::endl;

    assert(seloggerParser.getDirName() == inputDir);
    const auto &seloggerLogData = seloggerParser.getData();
    assert(seloggerLogData[0] == "EventId=0,EventType=METHOD_ENTRY,ThreadId=0,DataId=13,Value=0,method:0,1,Main,main,([Ljava/lang/String;)V,9,Main.java,fe1fe704c569eaf77bb10120a2a4698035803860,Main:main,Main.java:0:0");

    std::cout << "===================== SELOGGER PARSER TEST PASSED ====================" << std::endl;

    std::cout << "========================= MetafileParser TEST is succeed =========================\n";
}