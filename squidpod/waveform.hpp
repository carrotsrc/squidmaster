#ifndef WAVEFORM_HPP
#define WAVEFORM_HPP
#include "common.hpp"

namespace squid {

template<typename T, typename Alloca> 
class waveform; // Forward declaration
    
template<typename T, typename Alloca>
using transform_waveform_op = void(*)(waveform<T,Alloca>&);

template<typename T, typename Alloca>
using analysis_waveform_op = void(*)(waveform<T,Alloca>&);


template<typename T, typename Alloca = std::allocator<T>> 
class waveform {
    /*
     * Type definitions
     */
public:
    
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;

    typedef std::vector<pointer> channel_container;
    typedef typename channel_container::size_type size_type;
    typedef Alloca allocator_type;
    
    
    using passop_generic = void(*)(waveform<T,Alloca>&);
    using passop = std::function<void(waveform<T,Alloca>&)>;
    
    /*
     * Iterator definitions
     */
public:
    struct frame {
        
        frame(reference l, reference r):left(l),right(r){};
        reference left;
        reference right;
    };
    class frame_iterator {
    public:
        typedef frame_iterator self_type;
        typedef T value_type;
        typedef T& reference;
        typedef T* pointer;
        typedef std::forward_iterator_tag iterator_category;
        typedef int difference_type;
        
        frame_iterator(pointer l, pointer r) : _l(l), _r(r) { }
        self_type& operator++() {  _l++; _r++; return *this; }
        self_type& operator++(int) { _l++; _r++; return *this; }
        
        self_type& operator--() { _l--; _r--; return *this; }
        self_type& operator--(int) { _l--; _r--; return *this; }
        
        bool operator==(self_type& other) { return _l == other._l; }
        bool operator!=(self_type& other) { return _l != other._l; }
        
        value_type left() { return *_l; }
        void left(value_type v) { *_l = v; }
        
        value_type right() { return *_r; }
        void right(value_type v) { *_r = v; }
        

        void set(frame f) { *_l = f.left; *_r = f.right; }
        void set(value_type l, value_type r) { *_l = l; *_r = r; }
                           
        frame get(){ return {*_l, *_r}; }
        
        value_type get(audio_channel channel)
        { return channel == audio_channel::stereo_right ? *_r:*_l; } 
        
        void set(audio_channel channel, value_type v)
        { channel == audio_channel::stereo_right ? *_r = v : *_l = v; } 

        
        frame operator*() { return {*_l, *_r}; }
    private:
        pointer _l, _r;
    };
    
    class const_frame_iterator {
    public:
        typedef const_frame_iterator self_type;
        typedef T value_type;
        typedef const T& const_reference;
        typedef const T* const_pointer;
        typedef std::forward_iterator_tag iterator_category;
        typedef int difference_type;
        
        const_frame_iterator(pointer l, pointer r) : _l(l), _r(r) { }
        self_type operator++() { self_type i = *this; _l++; _r++; return i; }
        self_type operator++(int) { _l++; _r++; return *this; }
        
        self_type operator--() { self_type i = *this; _l--; _r--; return i; }
        self_type operator--(int) { _l--; _r--; return *this; }
        
        bool operator==(self_type& other) { return _l == other._l; }
        bool operator!=(self_type& other) { return _l != other._l; }
        
        value_type left() { return *_l; }
        value_type right() { return *_r; }
        
        const frame get(){ return frame(*_l, *_r); }
        const frame operator*() { return frame(*_l, *_r); }
    private:
        pointer _l, _r;
    };
    
    typedef frame_iterator iterator;
    typedef const_frame_iterator const_iterator;
    
    
    /*
     * Member Functions
     */
public:
    waveform();
    waveform(pointer raw, frame_count nframes, channel_count nchannels);
    waveform(channel_container channels, frame_count nframes, channel_count nchannels);

    waveform(const waveform& orig);
    waveform(const waveform&& orig);
    
    virtual ~waveform();

    sample_count num_samples() const;
    frame_count num_frames() const;
    channel_count num_channels() const;
    
    channel_container& channels();
    pointer channel(audio_channel id);
    
    void update_stats();
    
    pointer operator[](audio_channel id) {
        return this->channel(id);
    }
    
    frame_iterator begin();    
    frame_iterator end();
    
    const_frame_iterator begin() const;
    const_frame_iterator end() const;
    
    waveform<T,Alloca>* window(frame_count start, frame_count amount);
    template<passop_generic F>
    inline void transform() {
        F(*this);
    };
    
    template<passop_generic F>
    inline void pass() {
        F(*this);
    };
    
    inline void pass(passop f) {
        f(*this);
    }
    
private:
    channel_container   _channels;
    
    frame_count         _nframes;
    sample_count        _nsamples;
    channel_count       _nchannels;
    
    int                 _ninternal;
    
    allocator_type      _allocator;
    
    bool                _window;
    
    inline void update_frame_count() {
        auto m = _nsamples % _nchannels; 
        if(m == 0) {
            _nframes = _nsamples / _nchannels;
        } else {
            _nframes = (_nsamples - m) / _nchannels;
        }        
    }
};


template<typename T, typename Alloca> 
waveform<T, Alloca>::waveform()
    : _nchannels(2), _channels()
{ }

template<typename T, typename Alloca> 
waveform<T,Alloca>::waveform(pointer raw, frame_count nframes, channel_count nchannels)
    : _nchannels(2), _nframes(nframes), _channels(), _window(false)
{ 
    if(nchannels > 2) { throw unsupported_format(); }
    
    _nsamples = _nframes * 2;
    _ninternal = _nframes + static_cast<std::uint32_t>(
                                    static_cast<float>(_nframes)/2.0f
                                );

    auto left = _allocator.allocate(_ninternal);
    auto right = _allocator.allocate(_ninternal);
    
    if(nchannels == 1) {
        for(auto sample = 0u; sample < _nframes; ++sample) {
            left[sample] = raw[sample];
            right[sample] = raw[sample];
        }
    } else {
        auto frame = 0u;
        for(auto sample = 0u; sample < _nsamples;) {
            left[frame] = raw[sample++];
            right[frame++] = raw[sample++];
        }
    }
    
    _channels.push_back(left);
    _channels.push_back(right);
}


template<typename T, typename Alloca> 
waveform<T,Alloca>::waveform(channel_container channels, frame_count nframes, channel_count nchannels)
    : _channels(channels),_nchannels(nchannels), _nframes(nframes), _nsamples(_nchannels*_nsamples), _window(true)
{ }


template<typename T, typename Alloca> 
waveform<T,Alloca>::~waveform()
{ }


template<typename T, typename Alloca>
waveform<T,Alloca>::waveform(const waveform<T,Alloca>& orig)
    : _nframes(orig._nframes) , _nsamples(orig._nsamples)
    , _nchannels(orig._nchannels), _window(false)
{
    _ninternal = _nframes + static_cast<std::uint32_t>(
                                    static_cast<float>(_nframes)/2.0f
                                );
    _channels.push_back(_allocator.allocate(_ninternal));
    _channels.push_back(_allocator.allocate(_ninternal));
    
    auto lptr = orig._channels[audio_channel::stereo_left];
    auto rptr = orig._channels[audio_channel::stereo_right];
    std::copy(lptr, lptr+_nframes, _channels[audio_channel::stereo_left]);
    std::copy(rptr, rptr+_nframes, _channels[audio_channel::stereo_right]);
    
}

template<typename T, typename Alloca>
waveform<T,Alloca>::waveform(const waveform<T,Alloca>&& orig)
    : _nframes(orig._nframes) , _nsamples(orig._nsamples)
    , _nchannels(orig._nchannels), _window(false)
    , _channels(orig._channels), _ninternal(orig._ninternal)
{ }

template<typename T, typename Alloca>
frame_count waveform<T,Alloca>::num_frames() const {
    return _nframes;
}

template<typename T, typename Alloca>
sample_count waveform<T,Alloca>::num_samples() const {
    return _nsamples;
}

template<typename T, typename Alloca>
channel_count waveform<T,Alloca>::num_channels() const {
    return _nchannels;
}

template<typename T, typename Alloca>
typename waveform<T,Alloca>::channel_container& waveform<T,Alloca>::channels() {
    return _channels;
}

template<typename T, typename Alloca>
void waveform<T,Alloca>::update_stats() {
    _nsamples = _channels.size();
    update_frame_count();
}


template<typename T, typename Alloca>
typename waveform<T,Alloca>::pointer waveform<T,Alloca>::channel(audio_channel id) {
     if(id > _nchannels) { return nullptr; }
        return _channels[id];
}

template<typename T, typename Alloca>
typename waveform<T,Alloca>::frame_iterator waveform<T,Alloca>::begin() {
    return frame_iterator(_channels[audio_channel::stereo_left],
                          _channels[audio_channel::stereo_right]
                        );
}

template<typename T, typename Alloca>
typename waveform<T,Alloca>::const_frame_iterator waveform<T,Alloca>::begin() const {
    return const_frame_iterator(_channels[audio_channel::stereo_left],
                          _channels[audio_channel::stereo_right]
                        );
}

template<typename T, typename Alloca>
typename waveform<T,Alloca>::frame_iterator waveform<T,Alloca>::end() {
    return frame_iterator(_channels[audio_channel::stereo_left] + _nframes,
                          _channels[audio_channel::stereo_right] + _nframes
                        );    
}

template<typename T, typename Alloca>
typename waveform<T,Alloca>::const_frame_iterator waveform<T,Alloca>::end() const {
    return const_frame_iterator(_channels[audio_channel::stereo_left] + _nframes,
                          _channels[audio_channel::stereo_right] + _nframes
                        );    
}

template<typename T, typename Alloca>
waveform<T,Alloca>* waveform<T,Alloca>::window(frame_count start, frame_count amount) {
    channel_container container;
    container.push_back(_channels[audio_channel::stereo_left] + start);
    container.push_back(_channels[audio_channel::stereo_right] + start);
    return new waveform<T,Alloca>(container,amount,_nchannels);
}

}

#include "waveform_types.hpp"

#endif /* WAVEFORM_HPP */

