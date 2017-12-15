#include "Common.hpp"

int main() {
    // TODO speed-compare more RadixSort configurations
    // TODO easier customizaton for running on different configurations

    SimpleRadixSort64bit rs { getDevice() };

    // TODO implement variety of sizes
    std::vector<SimpleRadixSort64bit::KeyType > v ( 1 << 25 );
    std::cout << "generating " << v.size() << " keys...." << std::endl;

    auto rng = makeRng( rs.maxValue() );
    std::generate( v.begin(), v.end(), rng );

    std::vector<SimpleRadixSort64bit::KeyType > vCopy = v;

    using Clock = std::chrono::high_resolution_clock;
    using ms = std::chrono::milliseconds;
    using std::chrono::duration_cast;

    auto t = Clock::now();
    try {
        rs.sort( v.begin(), v.end() );
    } catch (const cl::Error& err) {
        std::cerr << err.err() << ' '  << err.what() << std::endl;
    }
    auto radixSortTime = duration_cast<ms>( Clock::now() - t );

    auto rsKernel = rs.profilingInfo().total() * 1e-6;
    if (!std::is_sorted( v.begin(), v.end() )) {
        std::cerr << "Not sorted!";
    } else {
        std::cout << "Radix sort pure kernel time: " << rsKernel << "ms" << std::endl;
        std::cout << "Radix sort total time (with initializations, memory transfers, etc): " << radixSortTime.count()
                  << "ms" << std::endl;
        auto ef = (rsKernel / radixSortTime.count()) * 100.0;
        std::cout << "\tPercent of time taken by kernels: " << ef << std::endl;
    }

    std::cout << "Sorting on CPU, single thread..." << std::endl;
    
    auto tt = Clock::now();
    std::sort( vCopy.begin(), vCopy.end() );
    auto cpuSortTime = duration_cast<ms>( Clock::now() - tt );
    
    std::cout << "Sorted in " << cpuSortTime.count() << "ms" << std::endl;

    //  TODO add C++17 sort with std::execution::par

    std::cout << "Real speedup is " <<  (double)cpuSortTime.count() / radixSortTime.count()  << " times" << std::endl;
    std::cout << "Pure speedup is " <<  (double)cpuSortTime.count() / rsKernel << " times" << std::endl;
}
