require 'orocos'
require 'vizkit'

include Orocos
Orocos.initialize

Orocos.run 'tofcamera_mesa_swissranger::Task' => 'tofcamera_mesa_swissranger' do

	swissranger = TaskContext.get 'tofcamera_mesa_swissranger'

	swissranger.acquisition_mode = 'AM_COR_FIX_PTRN|AM_CONV_GRAY|AM_DENOISE_ANF|AM_CONF_MAP'

	swissranger.configure
	swissranger.start

	# start the vizkit gui interface
	#widget = Vizkit.default_loader.create_widget("vizkit::Vizkit3DWidget")
	#vizkit_tof = widget.createPlugin("tofcamera_mesa_swissranger", "TOFVisualization")
	vizkit_tof = Vizkit.default_loader.TOFVisualization

	# collect data from the output port	
	swissranger.distance_cartesian_image.connect_to do |data, name|
	vizkit_tof.updateData data if data
	    data
	end

	Vizkit.display swissranger.distance_cartesian_image
	Vizkit.exec

end
