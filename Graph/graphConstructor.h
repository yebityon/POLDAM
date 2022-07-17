#pragma once
#include <stdio.h>

namespace POLDAM
{

    class graphConstructor
    {
    public:
        graphConstructor(){};

        void setDataids();
        void setSeloggerLog();

        template <typename T>
        T &getGraph();
        void build();
    };

}