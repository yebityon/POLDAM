#include <Interpreter/InterpreterCommon.h>


namespace POLDAM 
{
    class LogInterPreter {

        public:
        
        struct METHOD_ENTRY {
            int functionName = 0;
            int otherInfo = 0;
        };
        
        ///@ref parseString and create struct information from given Program 
        METHOD_ENTRY interPreter(const std::string& str);
        
        
    }
    
}