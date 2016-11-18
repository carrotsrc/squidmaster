#include <cstdlib>
#include <iostream>
#include "waveform_loader.hpp"
#include "squidjack/client.hpp"
#include "basic_transform.hpp"
#include "basic_analysis.hpp"

using namespace std;

using namespace squid;
int main(int argc, char** argv) {
    std::cout << "FLAC Transforms test\n";

    squid::waveform_loader wfl;
    auto wf_orig = wfl.loadf32_file(std::string("../flac/442Hz-1s.flac"));
    auto& wf = *wf_orig;
    
    auto window = wf.window(10,4);

    std::vector<passop_f32> v;
    v.push_back(free_functor_f32<transforms::invert_left_channel>());
    v.push_back(free_functor_f32<transforms::reverse_right_channel>());
    v.push_back(free_functor_f32<analysis::echo_frames>());
    sample_f32 avg = 0;
    auto avgf = analysis::averager<sample_f32>(avg);
    
    v.push_back(avgf);
    for(auto& func : v) {
        window->pass(func);
    }
    std::cout << "average: "<<avg << std::endl;
    wf.pass(avgf);
    std::cout << "average: "<< avg << std::endl;
    squid::jack::client c("Fooman chu");
    
    
    
    //c.open();
    //int i = 0;
    //std::cin >> i;
    return 0;
}

