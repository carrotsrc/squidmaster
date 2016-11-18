#ifndef STREAM_HPP
#define STREAM_HPP
#include "common.hpp"

namespace squid {
    
    using sample_u16 = std::uint16_t;
    using sample_f32 = std::float_t;
    using frame_count = std::uint32_t;
    using sample_count = std::uint32_t;
    using channel_count = std::uint32_t;
    using channel_id = std::uint32_t;
    
    enum audio_channel {
        mono_front = 0,
        stereo_left = 0, stereo_right = 1
    };
    
    using hertz = std::uint32_t;
    using khertz = std::uint32_t;
    
    struct stream_info {
        frame_count sample_rate;
    };
    
}
#endif /* STREAM_HPP */

