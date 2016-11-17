#ifndef COMMON_HPP
#define COMMON_HPP
#include <cstdint>
#include <vector>
#include <cmath>
#include <string>
#include <memory>

#include "throwables.hpp"

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
    
    
    using global_allocator_f32 = std::allocator<sample_f32>;
    using global_allocator_u16 = std::allocator<sample_u16>;
}


#endif /* COMMON_HPP */

