#include <string>
#include <iostream>
#include <Interpreter/LogEventType.h>

#include <Interpreter/ILogInterpreter.h>

namespace POLDAM {


    struct METHOD_ENTRY {
        std::string entryName;
    };

    class IMethodEntry : public ILogType {

    public:
     
    METHOD_ENTRY data;
    IMethodEntry(const std::string log_) : log(log_){};
    
    std::string getLog() override { return log; }
    EventType getEventType() override  { return EventType::METHOD_ENTRY; }
    
    private: 
    const std::string log;
    const EventType eventType = EventType::METHOD_ENTRY;
    };

    template<>
    class LogInterpreter<IMethodEntry> : ILogInterpreter {
        
        public:
        LogInterpreter(const std::string log_) : methodEntry(IMethodEntry(log_)){};
        
        METHOD_ENTRY getMethodEntry() {
            return methodEntry.data;
        }

        void parseLog () override {};
        
        EventType getEventType() override { return methodEntry.getEventType(); }
        std::string getLog() override { return methodEntry.getLog(); }

        private:
        IMethodEntry methodEntry;
        
    };
}