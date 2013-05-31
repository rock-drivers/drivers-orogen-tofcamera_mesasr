require 'orocos'
require 'vizkit'

include Orocos
Orocos.initialize

Orocos.run 'tofcamera_mesa_swissranger::Task' => 'tofcamera_mesa_swissranger' do
    # Set up logging
    #Orocos.log_all

    swissranger = TaskContext.get 'tofcamera_mesa_swissranger'

    swissranger.acquisition_mode = 'AM_COR_FIX_PTRN|AM_CONV_GRAY|AM_DENOISE_ANF|AM_CONF_MAP'
    swissranger.timeout = 3000
    swissranger.integration_time = 5
    swissranger.amplitude_threshold = 5
    swissranger.modulation_frequency = :MF_15MHz
    swissranger.min_int_time = 255
    swissranger.coord_percision = :CP_DOUBLE

    swissranger.configure
    swissranger.start

    # Vizkit.display swissranger.distance_cartesian_image
    # start the vizkit gui interface
    vizkit_tof = Vizkit.default_loader.TOFVisualization
    # collect data from the output port
    swissranger.coordinates_3D.connect_to do |data, name|
    vizkit_tof.updateData data if data
        data
    end

    #Vizkit.display swissranger.coordinates_3D
    Vizkit.display swissranger.confidence_image
    Vizkit.exec
end
