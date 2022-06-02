#include <Parser/Parser_Common.h>


namespace POLDAM
{
    class ReadFileStream
    {
        public:

        ReadFileStream(const std::string& fileName);

        ///@ref read next stream. if it reached file end, return false
        void nextStream();

        ///@ref 
        
        
        
    }
}