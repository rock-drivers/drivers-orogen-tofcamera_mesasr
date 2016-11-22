#ifndef TOFSCAN_HPP
#define TOFSCAN_HPP

#include <base/time.h>
#include <base/eigen.h>
#include <base/samples/Frame.hpp>
#include <base/samples/Pointcloud.hpp>
#include <vector>

namespace tofcamera_mesasr
{
    class TOFScan
    {
    public:
        base::Time time;

        uint32_t rows;
        uint32_t cols;

        uint32_t data_depth;

        std::vector<uint16_t> distance_image;
        std::vector<uint16_t> amplitude_image;
        std::vector<uint16_t> confidence_image;
        std::vector<base::Vector3d> coordinates_3D;
    };
}

#endif // TOFSCAN_HPP
