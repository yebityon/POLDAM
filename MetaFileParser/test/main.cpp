#pragma once
#include "../src/factory.h"
#include <iostream>
#include <cassert>

const std::string inputDir = "./Data/Main_LOG";

POLDAM::metafileFactory factory(inputDir);

void test_dataids_1()
{
    auto dataids = factory.createInstance<POLDAM::dataidsParser>(inputDir);

    auto dataidsData = dataids.getData();
    auto dataidsParsedData = dataids.getParsedData();

    // FIXME: In the wsl on windows, this test case is failed due to CRLF
    const std::string base = "0,0,0,-1,-1,RESERVED,V,Main#<init>#()V#size=6";
    assert(dataidsData[0] == base);
    assert(dataidsParsedData[0].classid == 0);
    assert(dataidsParsedData[0].dataidx == 0);
    assert(dataidsParsedData[0].methodid == 0);
    assert(dataidsParsedData[0].linenumber == -1);
    assert(dataidsParsedData[0].ordernumber == -1);
    assert(dataidsParsedData[0].eventtype == "RESERVED");
    assert(dataidsParsedData[0].descriptor == "V");
    assert(dataidsParsedData[0].eventinfo["Main#<init>#()V#size"] == "6");
    assert(dataidsData.size() == dataidsParsedData.size());

    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "test_1 passed!" << std::endl;
}

void test_seloggerLogParser_1()
{

    auto seloggerParser = factory.createInstance<POLDAM::seloggerLogParser>(inputDir);
    assert(seloggerParser.getDirName() == inputDir);
    const auto &seloggerLogData = seloggerParser.getData();
    assert(seloggerLogData[0] == "EventId=0,EventType=METHOD_ENTRY,ThreadId=0,DataId=13,Value=0,method:0,1,Main,main,([Ljava/lang/String;)V,9,Main.java,fe1fe704c569eaf77bb10120a2a4698035803860,Main:main,Main.java:0:0");
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "test_2 passed!" << std::endl;
}

void test_ObjecetFileParser()
{
    auto objParser = factory.createInstance<POLDAM::ObjectfileParser>(inputDir);
    auto &&data = objParser.getParsedData();

    assert(data.size() == 6);
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "test_ObjectFileParser1 is Passed!" << std::endl;
}

int main()
{
    test_dataids_1();
    test_seloggerLogParser_1();
    test_ObjecetFileParser();
}