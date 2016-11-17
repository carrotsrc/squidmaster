#ifndef WAVEFORMLOADER_HPP
#define WAVEFORMLOADER_HPP
#include <type_traits>
#include <iostream>
#include <sndfile.h>

#include "waveform.hpp"

namespace squid {
    

    class waveform_loader {
    public:
        template<typename Alloca = std::allocator<sample_f32>>
        wavef32_uptr<Alloca> loadf32_file(std::string const& path);

        template<typename Alloca = std::allocator<sample_f32>>
        wavef32_uptr<Alloca> loadf32_memory(sample_f32* raw,
                                            frame_count nframes,
                                            channel_count nchannels = 2);


        template<typename Alloca = std::allocator<sample_u16>>
        waveu16_uptr<Alloca> loadu16_file(std::string const& path);
        
    private:
    };
    
    template<typename Alloca>
    wavef32_uptr<Alloca> waveform_loader::loadf32_file(std::string const& path) {
        SF_INFO info;
        info.format = 0;
        
        auto fp = sf_open(path.c_str(), SFM_READ, &info);

        auto nc = info.channels;
        auto nf = info.frames;
        auto ns = nc * nf;
        Alloca al = Alloca();
        auto buf = al.allocate(ns);
        sf_readf_float(fp, buf, nf);
        auto wf = wavef32_uptr<Alloca>(new waveform_f32<Alloca>(buf, nf, nc));
        delete[] buf;

        sf_close(fp);

        return std::move(wf); 
    }
    
    template<typename Alloca>
    waveu16_uptr<Alloca> waveform_loader::loadu16_file(std::string const& path) {
        SF_INFO info;
        info.format = 0;
        
        auto fp = sf_open(path.c_str(), SFM_READ, &info);

        auto nc = info.channels;
        auto nf = info.frames;
        auto ns = nc * nf;
        Alloca al = Alloca();
        auto buf = al.allocate(ns);
        sf_readf_short(fp, buf, nf);
        auto wf = waveu16_uptr<Alloca>(new waveform_u16<Alloca>(buf, nf, nc));
        delete[] buf;

        sf_close(fp);

        return std::move(wf);        
    }
    
    template<typename Alloca>
    wavef32_uptr<Alloca> waveform_loader::loadf32_memory(sample_f32* raw,
                                                         frame_count nframes,
                                                         channel_count nchannels) {
        return wavef32_uptr<Alloca>(new waveform_f32<Alloca>(raw, nframes, 2));
    }
}




#endif /* WAVEFORMLOADER_HPP */