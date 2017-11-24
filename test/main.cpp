#include "../include/OCLRadixSort.hpp"
#include <random>

cl::Device getDevice( int pid, int did, cl_uint type = CL_DEVICE_TYPE_GPU ) {
    std::vector<cl::Platform> pls;
    cl::Platform::get(&pls);

    std::vector<cl::Device> dvs;
    pls[pid].getDevices(type, &dvs);

    for (auto & d : dvs) {

        std::cout << d.getInfo<CL_DEVICE_NAME>() << ' ' << d.getInfo<CL_DEVICE_VERSION>() << std::endl;

    }

    return dvs[did];
}

template<typename T, typename U>
bool checkPermutation(const std::vector<T> &keysAfterSort,
                      const std::vector<T> &keysBeforeSort,
                      const std::vector<U> &permutation) {
    for (size_t i = 0; i < keysAfterSort.size(); ++i) {
        if (keysBeforeSort[permutation[i]] != keysAfterSort[i]) {
            std::cout << i << " " << keysAfterSort[i] << " " << keysBeforeSort[i] << " " << permutation[i] << std::endl;
            return false;
        }
    }
    return true;
}

int main() {
    using T = unsigned long long int;
    using TI = T;
    using RadixSortT = RadixSort<8, 64, T, TI, 128, 4, true, 256, false, 32, true>;

    RadixSortT rs { getDevice( 1, 0 ) };

    std::cout << "generating keys...." << std::endl;

    std::vector<RadixSortT::KeyType > v ( 1 << 20 );

    std::minstd_rand rng{};
    std::generate( v.begin(), v.end(), [&rng, &rs](){ return rng() % ( rs.maxValue() - 1  ); } );

    std::vector<RadixSortT::KeyType > vCopy = v;

    try {
        rs.sort<true>(v.begin(), v.end());
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
