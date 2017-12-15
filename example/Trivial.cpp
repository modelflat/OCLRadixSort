#include "Common.hpp"

int main() {

    // create instance for device
    SimpleRadixSort64bit rs { getDevice() };

    std::vector<SimpleRadixSort64bit::KeyType > v ( 1 << 25 );
    std::cout << "generating " << v.size() << " keys...." << std::endl;

    auto rng = makeRng( rs.maxValue() );
    std::generate( v.begin(), v.end(), rng );

    // take a copy of v for permutation check
    std::vector<SimpleRadixSort64bit::KeyType > vCopy = v;

    try {
        // sort method is STL-style
        rs.sort( v.begin(), v.end() );
    } catch (const cl::Error& e) {
        std::cerr << "CL Error: " << e.err() << " " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Sorted: " << std::boolalpha << std::is_sorted( v.begin(), v.end() ) << std::endl;
    std::cout << "Permutation correct: " << std::boolalpha << checkPermutation(v, vCopy, rs.permutation()) << std::endl;

    const auto pi = rs.profilingInfo();
    std::cout << "Profiling: " << std::endl;
//    std::cout << "transpose:\t" << pi.transpose << "ns\n";
//    std::cout << "histogram:\t" << pi.histogram << "ns\n";
//    std::cout << "scan:\t\t" << pi.scan << "ns\n";
//    std::cout << "reorder:\t" << pi.reorder << "ns" << std::endl;

    std::cout << "total kernel execution time: " << pi.total() * 1e-6 << "ms" << std::endl;
}
