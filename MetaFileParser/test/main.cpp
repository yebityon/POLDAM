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
    auto dataids = factory.createInstance<POLDAM::dataidsParser>(inputDir, "dataids.txt", true);

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
    const std::string inputDir = "./Data/java8/src/test/origin/selogger_out";
    POLDAM::metafileFactory factory(inputDir);
    auto seloggerParser = factory.createInstance<POLDAM::seloggerLogParser>(inputDir, "log-00001.txt");

    seloggerParser.getData();
    std::vector<POLDAM::SeloggerData> parsedData = seloggerParser.getParserdData();

    auto &d = parsedData[0];

    assert(d.eventid == 0);
    assert(d.dataid == 12);
    assert(d.threadid == 0 && d.value == 0);

    d = parsedData[817];
    assert(d.eventid == 817);
    assert(d.dataid == 101);
    assert(d.threadid == 0 && d.value == 8388608);

    std::cout
        << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "test_2 passed!" << std::endl;
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