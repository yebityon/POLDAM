#include <poldam/helper/graph_builder.hpp>

namespace POLDAM
{
    void GraphBuilder::parseLogs()
    {
        omniLog = selogger.getData();
        dataids.getData();
        parsedDataIds = dataids.getParsedData();
        method.getData();
        parsedMethodsData = method.getParsedData();        
        classes.getData();
        parsedClassesData = classes.getParsedData();
        parsedObjectData = objectFile.getParsedData();
    }
    PoldamGraph GraphBuilder::build()
    {
        //Parserからデータを取得する
        PoldamGraph poldamGraph{config};

        for(const SeloggerData log : selogger.getParserdData())
        {
            DataId  d =  parsedDataIds[log.dataId];
            const MethodsData m = parsedMethodsData[d.methodId];
            const ClassesData c = parsedClassesData[d.classId];

            if (d.eventType == POLDAM::SELOGGER_EVENT_TYPE::METHOD_ENTRY)
            {
                POLDAM::GraphVertex v;
                const unsigned int classId = d.classId;
                v.classStr = m.className;
                v.methodStr = m.methodName;
                v.methodHash = std::to_string(std::hash<std::string>()(m.className + m.methodName));
                v.outputFormat = m.className + ":" + m.methodName;

                bool result = poldamGraph.addOmniVertex(v, log.threadId);
            }
            else if (d.eventType == POLDAM::SELOGGER_EVENT_TYPE::METHOD_PARAM)
            {
                const std::string argValue = log.value;
            }
            else if (d.eventType == POLDAM::SELOGGER_EVENT_TYPE::METHOD_NORMAL_EXIT)
            {
                poldamGraph.computeHash(log.threadId);
                poldamGraph.popStackVertex(log.threadId);
           }
            else if (d.eventType == POLDAM::SELOGGER_EVENT_TYPE::CALL_PARAM)
            {
                // Use these values to compute param hash.
                const unsigned int paramIdx = static_cast<unsigned int>(std::stoi(d.attr["index"]));
                const std::string paramType = d.attr["type"];
                std::string paramConcreateValue = "";

                if (paramType.find("String") != std::string::npos)
                {
                    // see also: https://github.com/takashi-ishio/selogger/blob/924f3d961344bf32cddc93709ed9609f27905191/src/selogger/logging/util/StringContentFile.java#L32
                    // fixed for 0-index
                    const int argValueIdx = std::stoi(log.value) - 1;
                    if (argValueIdx < 0)
                    {
                        if (config.isDebugMode)
                        {
                            std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << d.raw_value + " is Invalid Data, Skitp to compute hash\n";
                        }
                        continue;
                    }
                    const POLDAM::ObjectData o = parsedObjectData[argValueIdx];
                    poldamGraph.updateStackTopVertexParamInfo(o.stringValue, log.threadId);
                }
                // branch for paramType that paramValue is directly recored in SELogger.
                else if (paramType.find("Ljava") == std::string::npos)
                {
                    poldamGraph.updateStackTopVertexParamInfo(log.value, log.threadId);
                }
                else
                {
                    const int argValueIdx = std::stoi(log.value) - 1;
                    if (argValueIdx < 0)
                    {
                        if (config.isDebugMode)
                        {
                            std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << d.raw_value + " is Invalid Data, Skitp to compute hash\n";
                        }
                        continue;
                    }
                    // FIXME: 
                    const POLDAM::ObjectData o = parsedObjectData[argValueIdx];
                }
            }
            else
            {
                const std::string logString = shapeLogString(d, m, c);
                poldamGraph.updateStackTopVertex(logString, log.threadId);
            }
        }
        return poldamGraph;
    }
    std::string GraphBuilder::shapeLogString(const DataId d, const MethodsData m, const ClassesData c)
    {
        std::string res;
        res += std::to_string(static_cast<int>(d.eventType)) + d.valueDesc;
        for (const auto itr : d.attr)
        {
            res += itr.first + itr.second;
        }

        res += m.className + m.methodName + m.methodDesc;
        res += c.className + c.fileName;
        return res;
    }
}
