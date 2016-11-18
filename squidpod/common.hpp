#ifndef COMMON_HPP
#define COMMON_HPP
#include <cstdint>
#include <vector>
#include <cmath>
#include <string>
#include <memory>

#include "throwables.hpp"
#include "stream.hpp"
namespace squid {

    
    using alloc_f32 = std::allocator<sample_f32>;
    using alloc_u16 = std::allocator<sample_u16>;
}


#endif /* COMMON_HPP */

