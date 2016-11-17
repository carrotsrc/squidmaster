#ifndef WAVEFORM_TYPES_HPP
#define WAVEFORM_TYPES_HPP

 namespace squid {

// alias + functions U16
template <typename Alloca = global_allocator_u16>
using waveform_u16 = waveform<sample_u16, Alloca>;

using passop_u16 = typename waveform<sample_u16>::passop;

using passop_generic_u16 = typename waveform<sample_u16>::passop_generic;

template<passop_generic_u16 F>
inline passop_u16 free_functor_u16(){ return passop_u16(F); }


// alias + function U32
template <typename Alloca = global_allocator_f32>
using waveform_f32= waveform<sample_f32, Alloca>;

using passop_f32 = typename waveform<sample_f32>::passop;
using passop_generic_f32 = typename waveform<sample_f32>::passop_generic;

template<passop_generic_f32 F>
inline passop_f32 free_functor_f32(){ return passop_f32(F); }

// Unique ptrs
template <typename Alloca = global_allocator_f32>
using waveu16_uptr = std::unique_ptr<waveform_u16<Alloca>>;

template <typename Alloca = global_allocator_f32>
using wavef32_uptr = std::unique_ptr<waveform_f32<Alloca>>;
   
}
#endif /* WAVEFORM_TYPES_HPP */

