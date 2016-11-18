#ifndef CLIENT_HPP
#define CLIENT_HPP


#include <jack/jack.h>
#include "squidpod/common.hpp"

namespace squid { namespace jack {
  
    class client {
    public:
        enum class status {
            ok,
            error_client_open,
            error_port_register,
            error_client_activate
        };
    public:
        client(std::string name);
        bool open(JackOptions options = JackOptions::JackNullOption);

        JackStatus const& jack_status() const;
        status const& client_status() const;
        stream_info const& info() const;
        std::string const& name() const;

    protected:
        std::string     _name;
        jack_client_t*  _client;
        
        jack_port_t*    _input;
        jack_port_t*    _output;
        
        jack_status_t   _jstatus;
        status          _cstatus;
        
        stream_info     _sinfo;
    };
    
    
  
}}


#endif /* CLIENT_HPP */

