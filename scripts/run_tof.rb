require 'orocos'
require 'vizkit'
require 'roby/vizkit'

include Orocos
Orocos.initialize

Orocos.run 'tofcamera_mesasr::Task' => 'tofcamera_mesasr' do
    # Set up logging
    #Orocos.log_all

    swissranger = Orocos.name_service.get 'tofcamera_mesasr'

#    swissranger.acquisition_mode = 'AM_COR_FIX_PTRN|AM_DENOISE_ANF|AM_CONF_MAP|AM_MEDIAN'
    swissranger.timeout = 3000
    swissranger.integration_time = 255
    swissranger.modulation_frequency = :MF_15MHz
    swissranger.min_int_time = 255
    swissranger.amplitude_threshold = 50
    swissranger.confidence_threshold = 0
    swissranger.remove_zero_points = false
    swissranger.color_interval = 1
    swissranger.configure

    swissranger.start

    Vizkit.display swissranger
    Vizkit.display swissranger.ir_frame
#    Vizkit.display converter 
    Vizkit.exec
	Readline::readline("Press ENTER to exit\n")do
	end
end
