#include <string>
#include <exception>

#ifndef THROWABLES_HPP
#define THROWABLES_HPP

namespace squid {
    
    class unsupported_format : public std::exception {
    public:
        unsupported_format() { }
        virtual const char* what() const throw() { 
            return "Unsupported format";
        }
        
    private:
        std::string message;
    };
    
}

#endif /* THROWABLES_HPP */

