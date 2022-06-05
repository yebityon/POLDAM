#include <Interpreter/InterpreterCommon.h>


namespace POLDAM 
{
    class ILogType {
        public:


        virtual std::string getLog() = 0;
        virtual EventType getEventType() = 0;
    
        

    };

    class ILogInterpreter{
        
        public:
        virtual void parseLog() = 0;
        virtual EventType getEventType() = 0;
        virtual std::string getLog() = 0;
    };

    template<typename T>
    class LogInterpreter : ILogInterpreter {

        public:
    
        override void parseLog(){};
        override EventType getEventType() { return EventType::UNDEFINED; }
        override std::string getLog() {return std::string(0);}
    };
    
};
