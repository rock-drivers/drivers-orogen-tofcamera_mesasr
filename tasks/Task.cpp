
/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Task.hpp"

#include <vizkit3d/ColorConversionHelper.hpp>

#include <tofcamera_mesasr/SwissRangerDriver.hpp>

using namespace tofcamera_mesasr;

Task::Task(std::string const& name)
    : TaskBase(name), m_driver(0)
{
}

Task::Task(std::string const& name, RTT::ExecutionEngine* engine)
    : TaskBase(name, engine), m_driver(0)
{
}

Task::~Task()
{
    if (m_driver != 0)
        delete m_driver;
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See Task.hpp for more detailed
// documentation about them.

bool Task::configureHook()
{
    if (! TaskBase::configureHook())
        return false;

    m_driver = new tofcamera_mesasr::SwissRangerDriver();

    // open camera
    // get interface type
    if ( _interface.value() == "USB" )
    {
        if( !m_driver->openUSB(0) )
        {
            RTT::log(RTT::Error) << "SwissRanger (USB) device could not be opened." << RTT::endlog();
            return false;
        }
    }
    else if  ( _interface.value() == "ETHERNET" )
    {
        if( !m_driver->openEthernet(_ip_address.value()) )
        {
            RTT::log(RTT::Error) << "SwissRanger (Ethernet) device could not be opened." << RTT::endlog();
            return false;
        }
    }
    else {
            RTT::log(RTT::Error) << "Interface type not supported or recognized. SwissRanger device could not be opened." << RTT::endlog();
            return false;
    }


    // set acquire mode
    std::string acquire_mode_str = _acquisition_mode.get();
    std::vector<std::string> acquire_modes_list;
    boost::split(acquire_modes_list, acquire_mode_str, boost::is_any_of("|"));

    int acquire_mode = 0;
    for (unsigned int i = 0; i < acquire_modes_list.size(); ++i)
    {
        if (acquire_modes_list.at(i).compare("AM_COR_FIX_PTRN") == 0)
            acquire_mode |= AM_COR_FIX_PTRN;
        else if (acquire_modes_list.at(i).compare("AM_MEDIAN") == 0)
            acquire_mode |= AM_MEDIAN;
        else if (acquire_modes_list.at(i).compare("AM_TOGGLE_FRQ") == 0)
            acquire_mode |= AM_TOGGLE_FRQ;
        else if (acquire_modes_list.at(i).compare("AM_CONV_GRAY") == 0)
            acquire_mode |= AM_CONV_GRAY;
        else if (acquire_modes_list.at(i).compare("AM_SW_ANF") == 0)
            acquire_mode |= AM_SW_ANF;
        else if (acquire_modes_list.at(i).compare("AM_RESERVED0") == 0)
            acquire_mode |= AM_RESERVED0;
        else if (acquire_modes_list.at(i).compare("AM_RESERVED1") == 0)
            acquire_mode |= AM_RESERVED1;
        else if (acquire_modes_list.at(i).compare("AM_CONF_MAP") == 0)
            acquire_mode |= AM_CONF_MAP;
        else if (acquire_modes_list.at(i).compare("AM_HW_TRIGGER") == 0)
            acquire_mode |= AM_HW_TRIGGER;
        else if (acquire_modes_list.at(i).compare("AM_SW_TRIGGER") == 0)
            acquire_mode |= AM_SW_TRIGGER;
        else if (acquire_modes_list.at(i).compare("AM_DENOISE_ANF") == 0)
            acquire_mode |= AM_DENOISE_ANF;
        else if (acquire_modes_list.at(i).compare("AM_MEDIANCROSS") == 0)
            acquire_mode |= AM_MEDIANCROSS;
    }

    if (!m_driver->setAcquisitionMode(acquire_mode))
    {
        RTT::log(RTT::Error) << "SwissRanger acquisition mode could not be set." << RTT::endlog();
        return false;
    }

    // set timeout
    m_driver->setTimeout(_timeout);

    // set integration time
    try
    {
        unsigned char integration_time_uchar = boost::numeric_cast<unsigned char>(_integration_time.get());

        if (!m_driver->setIntegrationTime(integration_time_uchar))
        {
            RTT::log(RTT::Error) << "SwissRanger integration time could not be set." << RTT::endlog();
            return false;
        }
    }
    catch (boost::numeric::bad_numeric_cast& e)
    {
        RTT::log(RTT::Error) << "SwissRanger integration time could not be set. "
                             << "The value should be in range from 0 to 255." << RTT::endlog();
        return false;
    }

    // set dual integration time
    m_driver->setDualIntegrationTime(_dual_integration_time.get());

    // modulation frequency
    if (!m_driver->setModulationFrequency(_modulation_frequency.get()))
    {
        RTT::log(RTT::Error) << "SwissRanger modulation frequency could not be set." << RTT::endlog();
        return false;
    }

    // set auto exposure parameter
    try
    {
        unsigned char min_int_time_uchar = boost::numeric_cast<unsigned char>(_min_int_time.get());
        unsigned char max_int_time_uchar = boost::numeric_cast<unsigned char>(_max_int_time.get());
        unsigned char percent_over_pos_uchar = boost::numeric_cast<unsigned char>(_percent_over_pos.get());
        unsigned char desired_pos_uchar = boost::numeric_cast<unsigned char>(_desired_pos.get());

        if (!m_driver->setAutoExposure(min_int_time_uchar, max_int_time_uchar, percent_over_pos_uchar, desired_pos_uchar))
        {
            RTT::log(RTT::Error) << "SwissRanger auto exposure could not be set." << RTT::endlog();
            return false;
        }
    }
    catch (boost::numeric::bad_numeric_cast& e)
    {
        RTT::log(RTT::Error) << "SwissRanger auto exposure could not be set. "
                             << "The value should be in range from 0 to 255." << RTT::endlog();
        return false;
    }

    // set amplitude threshold
    try
    {
        unsigned short amplitude_thresh_ushort = boost::numeric_cast<unsigned short>(_amplitude_threshold.get());

        if (!m_driver->setAmplitudeThreshold(amplitude_thresh_ushort))
        {
            RTT::log(RTT::Error) << "SwissRanger amplitude threshold could not be set." << RTT::endlog();
            return false;
        }
    }
    catch (boost::numeric::bad_numeric_cast& e)
    {
        RTT::log(RTT::Error) << "SwissRanger amplitude threshold could not be set. "
                             << "The value should be in range from 0 to 65535." << RTT::endlog();
        return false;
    }    

    // set confidence threshold
    try
    {
        unsigned short confidence_thresh_ushort = boost::numeric_cast<unsigned short>(_confidence_threshold.get());

        m_driver->setConfidenceThreshold(confidence_thresh_ushort);
    }
    catch (boost::numeric::bad_numeric_cast& e)
    {
        RTT::log(RTT::Error) << "SwissRanger confidence threshold could not be set. "
                             << "The value should be in range from 0 to 65535." << RTT::endlog();
        return false;
    }    

    // set remove zero points
    m_driver->turnRemoveZeroPoints(_remove_zero_points.get());

    color_interval_ = _color_interval;
    
    return true;
}
// bool Task::startHook()
// {
//     if (! TaskBase::startHook())
//         return false;
//     return true;
// }
void Task::updateHook()
{
    TaskBase::updateHook();

    int result = m_driver->acquire();

    base::Time capture_time = base::Time::now();

    if (result == true)
    {
        if (_tofscan.connected())
        {
            TOFScan scan;
            m_driver->getRows(scan.rows);
            m_driver->getCols(scan.cols);
            // TODO: the data depth can be changed later
            scan.data_depth = 16;       
            m_driver->getDistanceImage( (std::vector<uint16_t>*) &scan.distance_image );
            m_driver->getAmplitudeImage( (std::vector<uint16_t>*) &scan.amplitude_image );
            m_driver->getConfidenceImage( (std::vector<uint16_t>*) &scan.confidence_image );
            m_driver->getPointcloudDouble(scan.coordinates_3D);

            scan.time = capture_time;

            _tofscan.write(scan);           
        }

        // TODO: add float and ushort pointcloud to the ports

        if (_pointcloud.connected())
        {
            base::samples::Pointcloud pointcloud;
            m_driver->getPointcloudDouble(pointcloud.points);  

            // if color_range is set to some value except 0
            // than color the poincloud according to the point distance
            if (color_interval_ != 0)
            {
                // color the pointcloud
                pointcloud.colors.clear();  // TODO: remove clean?
                pointcloud.colors.reserve(pointcloud.points.size());

                float color_dist = 0.;
                float point_dist = 0.;
                float r, g, b;

                for (std::vector<base::Point>::iterator it = pointcloud.points.begin();
                        it != pointcloud.points.end(); ++it)
                {
                    // TODO: depends on the unit of the pointcloud (meter oder mm)
                    // the distance from the camera to the point
                    point_dist = it->norm();

                    color_dist = fmod(point_dist, (float)color_interval_) / color_interval_;

                    r = g = b = 0.;                
                    vizkit3d::hslToRgb(color_dist, 1.0, 0.5, r, g, b);

                    // r, g, b and alpha
                    pointcloud.colors.push_back(base::Vector4d(r, g, b, 1.0)); 
                } 
            }

            pointcloud.time = capture_time;

            _pointcloud.write(pointcloud);
        }

    } else {
        RTT::log(RTT::Error) << "Failed to acquire the camera image." << RTT::endlog();
    }  
}
// void Task::errorHook()
// {
//     TaskBase::errorHook();
// }
// void Task::stopHook()
// {
//     TaskBase::stopHook();
// }
void Task::cleanupHook()
{
    TaskBase::cleanupHook();

    if (m_driver != 0)
    {
        delete m_driver;
        m_driver = 0;
    }
}

