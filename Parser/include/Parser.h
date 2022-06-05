#include <Parser/Parser_Common.h>

namespace POLDAM
{
    template<class F>
    class poldamLogSwitcher
    {
        public:
        void setSwitcher(const F  f) { condition = f; } ;
        const F& getSwitcher(){ return condition;}; 
        
        private:
        const F condition;
        
    };

    template<class F>
    class poldamLogFileManager
    {
        public:

        poldamLogFileManager(const std::string& fileName);
        ~poldamLogFileManager() = default;
    

        ///@ref read next stream. if it reached file end, return false
        bool readNextLine();
        
        ///@ref get fileName 
        const std::string getFileName();

        /// @ref get EventType
        POLDAM::EventType getEventType(const std::string log);
        
        F eventInterpreter(const std::string log);
    
        
        const std::string fileName;
        std::string log;    
    
        private:
        
        POLDAM::poldamLogSwitcher<F>& f;
    };

}