/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "FilterTask.hpp"

using namespace tofcamera_mesa_swissranger;

FilterTask::FilterTask(std::string const& name)
    : FilterTaskBase(name)
{
}

FilterTask::FilterTask(std::string const& name, RTT::ExecutionEngine* engine)
    : FilterTaskBase(name, engine)
{
}

FilterTask::~FilterTask()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See FilterTask.hpp for more detailed
// documentation about them.

 bool FilterTask::configureHook()
 {
     if (! FilterTaskBase::configureHook())
         return false;

     try
     {
         min_confidence = boost::numeric_cast<unsigned int>(_min_confidence.get());
     }
     catch (boost::numeric::bad_numeric_cast& e)
     {
         RTT::log(RTT::Error) <<  "The minimal confidence should be in range from 0 to 65.535." << RTT::endlog();
         return false;
     }

     try
     {
         min_amplitude = boost::numeric_cast<unsigned int>(_min_amplitude.get());
     }
     catch (boost::numeric::bad_numeric_cast& e)
     {
         RTT::log(RTT::Error) <<  "The minimal amplitude should be in range from 0 to 65.533." << RTT::endlog();
         return false;
     }

     try
     {
         max_amplitude = boost::numeric_cast<unsigned int>(_max_amplitude.get());
     }
     catch (boost::numeric::bad_numeric_cast& e)
     {
         RTT::log(RTT::Error) <<  "The maximal amplitude should be in range from 0 to 65.533." << RTT::endlog();
         return false;
     }

     return true;
 }
// bool FilterTask::startHook()
// {
//     if (! FilterTaskBase::startHook())
//         return false;
//     return true;
// }
 void FilterTask::updateHook()
 {
     FilterTaskBase::updateHook();

    base::Time time_start = base::Time::now();

     tofcamera_mesa_swissranger::TOFScan tofscan;

     if (_tofscan.read(tofscan) == RTT::NewData)
     {

         Filter::filteConfidence(tofscan, min_confidence);

         Filter::filteAmplitude(tofscan, min_amplitude, max_amplitude);

         if (_tofscan_filtered.connected() == true) {
             _tofscan_filtered.write(tofscan);
        }

     }


    base::Time time_end = base::Time::now();

    // std::cout << "filter time " << (time_end - time_start).toSeconds() << std::endl;     
 }
// void FilterTask::errorHook()
// {
//     FilterTaskBase::errorHook();
// }
// void FilterTask::stopHook()
// {
//     FilterTaskBase::stopHook();
// }
 void FilterTask::cleanupHook()
 {
     FilterTaskBase::cleanupHook();
 }

