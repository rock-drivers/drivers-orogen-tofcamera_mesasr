require 'orocos'
require 'vizkit'
require 'roby/vizkit'

include Orocos
Orocos.initialize

Orocos.run 'tofcamera_mesa_swissranger::Task' => 'tofcamera_mesa_swissranger',
            'tofcamera_mesa_swissranger::FilterTask' => 'filter_task' do
    # Set up logging
    Orocos.log_all

    swissranger = TaskContext.get 'tofcamera_mesa_swissranger'
    filters = TaskContext.get 'filter_task'

    swissranger.acquisition_mode = 'AM_COR_FIX_PTRN|AM_DENOISE_ANF|AM_CONF_MAP|AM_MEDIAN|AM_MEDIANCROSS'
    swissranger.timeout = 3000
    swissranger.integration_time = 30
    swissranger.amplitude_threshold = 5
    swissranger.modulation_frequency = :MF_15MHz
    swissranger.min_int_time = 255
    swissranger.configure

    filters.min_confidence = 25; #filters.min_confidence = 30000;
    filters.min_amplitude = 50; #filters.min_amplitude = 200;
    filters.configure

    swissranger.tofscan.connect_to filters.tofscan

    swissranger.start
    filters.start

    Vizkit.exec
end
