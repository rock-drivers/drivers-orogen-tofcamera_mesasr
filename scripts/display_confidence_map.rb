require 'orocos'
require 'vizkit'

include Orocos
Orocos.initialize

Orocos.run 'tofcamera_mesa_swissranger::Task' => 'tofcamera_mesa_swissranger' do

	swissranger = TaskContext.get 'tofcamera_mesa_swissranger'

	swissranger.acquisition_mode = 'AM_COR_FIX_PTRN|AM_CONV_GRAY|AM_DENOISE_ANF|AM_CONF_MAP'

	swissranger.configure
	swissranger.start

	Vizkit.display swissranger.confidence_map_image
	Vizkit.exec

end
