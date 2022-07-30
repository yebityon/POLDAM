#pragma once

#include "metafileHandlerCommon.h"
#include "../../Util/include/src/poldamUtil.h"

namespace POLDAM
{
    struct ObjectData
    {
        // recored in  LOG$ObjectTypes
        unsigned int dataidx;
        unsigned int objectid;
        // typenum is recored in LOG$Types.txt
        int typenum1;
        int typenum2;
        std::string loadclasss;
        std::string objecttype;
        //  if type is sstring, concreate value is available
        std::string value;
    };

    class ObjectfileParser : fileReader
    {
    public:
        ObjectfileParser(std::string inputDir){};

        std::vector<std::string> getData()
        {
            return this->objecttypedata;
        };

    private:
        void readFile() override;
        void parseReadlines() override;
        void dirTraversal(std::string fileName) override;

        void AccumulateObjectFile(){};

        std::string objectfileName;

        std::string objecttypepath

            std::vector<std::string>
                objecttypedata;
        std::vector<std::string> stringdata;
        std::vector<std::string> logtypedata;
    };
}
