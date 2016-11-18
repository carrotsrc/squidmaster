#include "client.hpp"

using namespace squid::jack;

client::client(std::string name)
    : _name(name), _jstatus(JackStatus::JackInvalidOption)
{ }

bool client::open(JackOptions options) {
    _client = jack_client_open(_name.c_str(), options, &_jstatus);
    
    if(!_client) {
        _cstatus = status::error_client_open;
        return false;
    }
    
    if(_jstatus & JackNameNotUnique) {
        _name = std::string(jack_get_client_name(_client));
    }

    _sinfo.sample_rate = jack_get_sample_rate(_client);

    _input = jack_port_register(_client, "input", JACK_DEFAULT_AUDIO_TYPE,
                                JackPortIsInput,0);
    _output = jack_port_register(_client, "output", JACK_DEFAULT_AUDIO_TYPE,
                                 JackPortIsOutput, 0);
    
    if( !_input || !_output) {
        _cstatus = status::error_port_register;
        return false;
    }
    
    _cstatus = status::ok;
    return true;
}

JackStatus const& client::jack_status() const {
    return _jstatus;
}

client::status const& client::client_status() const {
    return _cstatus;
}

squid::stream_info const& client::info() const {
    return _sinfo;
}

std::string const& client::name() const {
    return _name;
}
