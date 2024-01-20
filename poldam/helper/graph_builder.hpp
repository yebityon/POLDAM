#include <poldam/util/arg_config.hpp>
#include <poldam/selogger_log_parser/common.hpp>
#include <poldam/selogger_log_parser/factory.hpp>
#include <poldam/graph/graph.hpp>

namespace POLDAM
{
    class GraphBuilder
    {
        public:
            PoldamGraph build();
            void parseLogs();

            GraphBuilder(
                poldamConfig _config,
                SeloggerLogParser &_seloggerParser,
                DataIdsParser &_dataids,
                ObjectfileParser &_objectFileParser,
                MethodDataParser &_methodParser,
                ClassesDataParser &_classesParser) : config(_config),
                                                     selogger(_seloggerParser),
                                                     dataids(_dataids),
                                                     objectFile(_objectFileParser),
                                                     method(_methodParser),
                                                     classes(_classesParser){};
        private:
            poldamConfig config;
            SeloggerLogParser selogger;
            DataIdsParser dataids;
            ObjectfileParser objectFile;
            MethodDataParser method;
            ClassesDataParser classes;

            std::vector<std::string> omniLog;
            std::vector<DataId> parsedDataIds;
            std::vector<MethodsData> parsedMethodsData;
            std::vector<ClassesData> parsedClassesData;
            std::vector<ObjectData> parsedObjectData;

            std::string shapeLogString(const DataId d, const MethodsData, const ClassesData);
    };
    
} // namespace 
