#ifndef BASIC_ANALYSIS_HPP
#define BASIC_ANALYSIS_HPP


#include "waveform.hpp"
namespace squid { namespace analysis {
    /**
     * Analysis pass for printing frames to stdout
     * @param wf The waveform to print
     */
    template<typename T, typename Alloca>
    void echo_frames(waveform<T,Alloca> & wf) {
        #include <iostream>
        for(auto frame : wf) {
            std::cout << frame.left << "  " << frame.right << std::endl;
        }
    }
    
    template<typename T, typename Alloca = std::allocator<T>>
    class averager {
    public:
        averager(T& v) : _avg(v) { }

        void operator()(waveform<T,Alloca> & wf) {
            auto count = 0u;
            for(auto frame : wf) {
                _avg += frame.left;
                count++;
            }
            
            _avg /= count;
        }
        
        std::function<void(waveform<T,Alloca>&)> func() {
            return std::bind(&averager::operator(), this, std::placeholders::_1);
        }
    private:
            T& _avg;
    };
    
}}
#endif /* BASIC_ANALYSIS_HPP */

