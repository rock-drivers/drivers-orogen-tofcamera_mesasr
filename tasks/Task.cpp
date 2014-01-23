
/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Task.hpp"

#include "tofcamera_mesa_swissranger/SwissRangerDriver.hpp"

using namespace tofcamera_mesa_swissranger;

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

    m_driver = new tofcamera_mesa_swissranger::SwissRangerDriver();

    if( !m_driver->openUSB(0) )
    {
        RTT::log(RTT::Error) << "Device could not be opened." << RTT::endlog();
        return false;
    }

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
        RTT::log(RTT::Error) << "Acquisition mode could not be set." << RTT::endlog();
        return false;
    }

    m_driver->setTimeout(_timeout);

    try
    {
        unsigned char integration_time_uchar = boost::numeric_cast<unsigned char>(_integration_time.get());

        if (!m_driver->setIntegrationTime(integration_time_uchar))
        {
            RTT::log(RTT::Error) << "Integration time could not be set." << RTT::endlog();
            return false;
        }
    }
    catch (boost::numeric::bad_numeric_cast& e)
    {
        RTT::log(RTT::Error) << "Integration time could not be set. "
                             << "The value should be in range from 0 to 255." << RTT::endlog();
        return false;
    }

    try
    {
        unsigned short amplitude_thresh_int_ushort = boost::numeric_cast<unsigned short>(_amplitude_threshold.get());

        if (!m_driver->setAmplitudeThreshold(amplitude_thresh_int_ushort))
        {
            RTT::log(RTT::Error) << "Amplitude threshold could not be set." << RTT::endlog();
            return false;
        }
    }
    catch (boost::numeric::bad_numeric_cast& e)
    {
        RTT::log(RTT::Error) << "Amplitude threshold could not be set. "
                             << "The value should be in range from 0 to 65.535." << RTT::endlog();
        return false;
    }


    if (!m_driver->setModulationFrequency(_modulation_frequency.get()))
    {
        RTT::log(RTT::Error) << "Modulation frequency could not be set." << RTT::endlog();
        return false;
    }

    try
    {
        unsigned char min_int_time_uchar = boost::numeric_cast<unsigned char>(_min_int_time.get());
        unsigned char max_int_time_uchar = boost::numeric_cast<unsigned char>(_max_int_time.get());
        unsigned char percent_over_pos_uchar = boost::numeric_cast<unsigned char>(_percent_over_pos.get());
        unsigned char desired_pos_uchar = boost::numeric_cast<unsigned char>(_desired_pos.get());

        if (!m_driver->setAutoExposure(min_int_time_uchar, max_int_time_uchar, percent_over_pos_uchar, desired_pos_uchar))
        {
            RTT::log(RTT::Error) << "Auto Exposure could not be set." << RTT::endlog();
            return false;
        }
    }
    catch (boost::numeric::bad_numeric_cast& e)
    {
        RTT::log(RTT::Error) << "Auto exposure could not be set. "
                             << "The value should be in range from 0 to 255." << RTT::endlog();
        return false;
    }

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

    base::Time time_start = base::Time::now();

    int result = m_driver->acquire();
    if (result == true)
    {
        if (_tofscan.connected())
        {
            TOFScan scan;
            scan.time = base::Time::now();
            m_driver->getRows(scan.rows);
            m_driver->getCols(scan.cols);
            scan.data_depth = 16;
            m_driver->getDistanceImage( (std::vector<uint16_t>*) &scan.distance_image );
            m_driver->getAmplitudeImage( (std::vector<uint16_t>*) &scan.amplitude_image );
            m_driver->getConfidenceImage( (std::vector<uint16_t>*) &scan.confidence_image );
            m_driver->getPointcloudDouble(scan.coordinates_3D);
           _tofscan.write(scan);
        }

    } else {
        RTT::log(RTT::Error) << "Failed to acquire the camera image." << RTT::endlog();
    }

    base::Time time_end = base::Time::now();

    // std::cout << "acquire time " << (time_end - time_start).toSeconds() << std::endl;     
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

    delete m_driver;
    m_driver = 0;
}

