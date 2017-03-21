  #ifndef _ERROR_H
#define _ERROR_H

#include <string>

class Error {
public:
    struct IOError {
        std::string error;
        IOError(const std::string& s):
            error(s) {}
    };
    
    struct SyntaxError {
        std::string error;
        SyntaxError(const std::string& s):
            error(s) {}
    };
};

#endif
