require 'orocos'

include Orocos
Orocos.initialize

Orocos.run 'tofcamera_mesa_swissranger::Task' => 'tofcamera_mesa_swissranger' do

	swissranger = TaskContext.get 'tofcamera_mesa_swissranger'

	swissranger.acquisition_mode = 'AM_COR_FIX_PTRN|AM_CONV_GRAY|AM_DENOISE_ANF'
	swissranger.timeout = 3000
	swissranger.integration_time = 5
	swissranger.amplitude_threshold = 5	
	swissranger.modulation_frequency = :MF_30MHz
	swissranger.min_int_time = 255
	swissranger.coord_percision = :CP_DOUBLE

	swissranger.configure
	swissranger.start

	reader = swissranger.distance_image.reader

	while true
		if msg = reader.read_new
			puts "read new data"
		end
		sleep 0.1
	end

end
