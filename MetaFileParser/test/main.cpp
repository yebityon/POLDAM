#pragma once
#include "../src/factory.h"
#include <iostream>
#include <cassert>

void test_dataids_1()
{
    const std::string inputDir = "./Data/Main_LOG";
    POLDAM::metafileFactory factory(inputDir);
    auto dataids = factory.createInstance<POLDAM::dataidsParser>(inputDir);

    auto dataidsData = dataids.getData();
    auto dataidsParsedData = dataids.getParsedData();

    // FIXME: In the wsl on windows, this test case is failed due to CRLF
    const std::string base = "0,0,0,-1,-1,RESERVED,V,Main#<init>#()V#size=6";
    assert(dataidsData[0] == base);
    assert(dataidsParsedData[0].classid == 0);
    assert(dataidsParsedData[0].dataidx == 0);
    assert(dataidsParsedData[0].methodid == 0);
    assert(dataidsParsedData[0].line == -1);
    assert(dataidsParsedData[0].instructionid == -1);
    assert(dataidsParsedData[0].eventtype == "RESERVED");
    assert(dataidsParsedData[0].valuedesc == "V");
    assert(dataidsParsedData[0].attr["Main#<init>#()V#size"] == "6");
    assert(dataidsData.size() == dataidsParsedData.size());

    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "dataids_test_1 passed!" << std::endl;
}

void test_dataids_2()
{
    const std::string inputDir = "./Data/java8/src/test/origin/selogger_out";
    POLDAM::metafileFactory factory(inputDir);
    auto dataids = factory.createInstance<POLDAM::dataidsParser>(inputDir, true);

    const std::string base = "97,1,4,12,0,LABEL,I,\"null\"";

    auto dataidsData = dataids.getData();
    auto dataidsParsedData = dataids.getParsedData();

    auto stringData = dataidsData[97];
    auto d = dataidsParsedData[97];

    assert(stringData == base);
    assert(d.dataidx == 97);
    assert(d.classid == 1);
    assert(d.methodid == 4);
    assert(d.line == 12);
    assert(d.instructionid == 0);
    assert(d.eventtype == "LABEL");
    assert(d.valuedesc == "I");
    // std::cout << d.attr["null"] << std::endl;
    // std::cout << d.attr["\"null\""] << std::endl;
    assert(d.attr["null"] == "null");

    const std::string base2 = "82,1,3,7,0,METHOD_ENTRY,Ljava/lang/Object;,\"methodtype=instance,index=0\"";

    stringData = dataidsData[82];
    d = dataidsParsedData[82];

    assert(stringData == base2);
    assert(d.dataidx == 82);
    assert(d.classid == 1);
    assert(d.methodid == 3);
    assert(d.line == 7);
    assert(d.instructionid == 0);
    assert(d.eventtype == "METHOD_ENTRY");
    assert(d.valuedesc == "Ljava/lang/Object;");
    assert(d.attr["methodtype"] == "instance");
    assert(d.attr["index"] == "0");

    stringData = dataidsData[74];
    d = dataidsParsedData[74];

    assert(d.attr["opcode"] == "INVOKESPECIAL");
    assert(d.attr["owner"] == "java/lang/Object");
    assert(d.attr["name"] == "<init>");
    assert(d.attr["desc"] == "()V");
    assert(d.attr["methodtype"] == "constructor");

    std::cout
        << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "dataids_test_2 passed!" << std::endl;
}
void test_seloggerLogParser_1()
{
    const std::string inputDir = "./Data/Main_LOG";
    POLDAM::metafileFactory factory(inputDir);
    auto seloggerParser = factory.createInstance<POLDAM::seloggerLogParser>(inputDir);
    assert(seloggerParser.getDirName() == inputDir);
    const auto &seloggerLogData = seloggerParser.getData();
    assert(seloggerLogData[0] == "EventId=0,EventType=METHOD_ENTRY,ThreadId=0,DataId=13,Value=0,method:0,1,Main,main,([Ljava/lang/String;)V,9,Main.java,fe1fe704c569eaf77bb10120a2a4698035803860,Main:main,Main.java:0:0");
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "test_2 passed!" << std::endl;
}

void test_ObjecetFileParser()
{
    const std::string inputDir = "./Data/Main_LOG";
    POLDAM::metafileFactory factory(inputDir);
    auto objParser = factory.createInstance<POLDAM::ObjectfileParser>(inputDir);
    auto &&data = objParser.getParsedData();

    assert(data.size() == 6);

    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "test_ObjectFileParser1 is Passed!" << std::endl;
}

int main()
{
    test_dataids_1();
    test_dataids_2();
    test_seloggerLogParser_1();
    test_ObjecetFileParser();
}