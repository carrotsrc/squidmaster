#ifndef BASIC_TRANSFORMS_HPP
#define BASIC_TRANSFORMS_HPP

#include "waveform.hpp"


namespace squid { namespace transforms {
       
    /**
     * Block widget that swaps two frames
     * @param a Iterator to swap with B
     * @param b Iterator to swap with A
     */
    template<typename T, typename Alloca>
    inline void swap_frames(typename waveform<T,Alloca>::iterator& a, 
                     typename waveform<T,Alloca>::iterator& b) {
        auto l = b.left();
        auto r = b.right();
        b.set(*a);
        a.set(l, r);
    }
    
    /**
     * Block widget that swaps two iterators
     * @param channel The channel id
     * @param a Iterator to swap channel samples with b
     * @param b Iterator to swap channel samples with a
     */
    template<typename T, typename Alloca>
    inline void swap_samples( audio_channel channel,
                     typename waveform<T,Alloca>::iterator& a, 
                     typename waveform<T,Alloca>::iterator& b) {
        auto v = b.get(channel);
        b.set(channel, a.get(channel));
        a.set(channel, v);
    }

    /**
     * reverse all the frames in the waveform
     * @param wf Reference to waveform to perform transform
     */
    template<typename T, typename Alloca>
    void reverse_frames(waveform<T,Alloca> & wf) {
        auto f = wf.begin(); auto b = wf.end();
        
        while(f != b && f != --b) {
            swap_frames<T,Alloca>(f,b);
            f++;
        }
    }
    
    /**
     * Reverse a specific channel in the waveform
     * @param wf Reference to waveform to perform transform
     */
    template<typename T, typename Alloca, audio_channel C>
    void reverse_channel(waveform<T,Alloca> & wf) {
        auto f = wf.begin(); auto b = wf.end();
        
        while(f != b && f != --b) {
            swap_samples<T,Alloca>(C, f,b);
            f++;
        }
    }
    
   /**
     * Reverse the left channel in the waveform
     * @param wf Reference to waveform to perform transform
     */
    template<typename T, typename Alloca>
    inline void reverse_left_channel(waveform<T,Alloca> & wf) {
        reverse_channel<T,Alloca,audio_channel::stereo_left>(wf);
    }
    
   /**
     * Reverse the right channel in the waveform
     * @param wf Reference to waveform to perform transform
     */
    template<typename T, typename Alloca>
    inline void reverse_right_channel(waveform<T,Alloca> & wf) {
        reverse_channel<T,Alloca,audio_channel::stereo_right>(wf);
    }
    
    /**
     * Invert all the frames in the waveform
     * @param wf Reference to waveform to perform transform
     */
    template<typename T, typename Alloca>
    void invert_frames(waveform<T,Alloca> & wf) {
        for(auto frame : wf) {
            frame.left = frame.left * -1.0;
            frame.right = frame.right * -1.0;
        }
    }
    
     /**
     * Invert the specified channel of the waveform
     * @param wf Reference to waveform to perform transform
     */
    template<typename T, typename Alloca, audio_channel C>
    void invert_channel(waveform<T,Alloca> & wf) {
        auto f = wf.begin(); auto b = wf.end();

        for(auto frame : wf) {

            C == audio_channel::stereo_right
                ? frame.right = frame.right * -1.0
                : frame.left = frame.left * -1.0;
        }
    }  

    /**
    * Invert the right channel of the waveform
    * @param wf Reference to waveform to perform transform
    */
    template<typename T, typename Alloca>
    inline void invert_right_channel(waveform<T,Alloca> & wf) {
        invert_channel<T,Alloca,audio_channel::stereo_right>(wf);
    }

    /**
    * Invert the left channel of the waveform
    * @param wf Reference to waveform to perform transform
    */
    template<typename T, typename Alloca>
    inline void invert_left_channel(waveform<T,Alloca> & wf) {
        invert_channel<T,Alloca,audio_channel::stereo_left>(wf);
    }
} }
#endif /* BASIC_TRANSFORMS_HPP */

