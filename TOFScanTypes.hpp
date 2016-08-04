#ifndef TOFSCAN_HPP
#define TOFSCAN_HPP

#include <base/Time.hpp>
#include <base/Eigen.hpp>
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

        std::vector<uint8_t> distance_image;
        std::vector<uint8_t> amplitude_image;
        std::vector<uint8_t> confidence_image;
        std::vector<base::Vector3d> coordinates_3D;
    };
}

#endif // TOFSCAN_HPP
