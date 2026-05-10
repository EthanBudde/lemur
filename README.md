# lemur

open source, low-overhead GPS bike computer built on adafruit sense nRF microcontroller series

# component list
	current functionality reccomends...
		- 1x Adafruit sense nRF##### microcontroller
		- 1x GT-U7 NEO-6M GPS sensor
			- 1x associated antenna
		- - gfx output device

# gfx output
	device (will) support a range of visual output devices
		current support list
			headless operation
			SSD1306 OLED dot matrix
		future support
			standard 1602 LCD modules
			LCD screens and higher-resolution I2C screens
			...

# indev feature list
	live information about 
		moving speed, top speed
		temperature / humidity
		current time
	trip tracking: press a button, and track:
		trip distance, time spent
		elevation (raw up/down and aggregate) 
		average mile time
		periodic gps coordinates
	cardinal directionality
	battery life readout and differental  power mode support
	protoboarded design with a 3d printed case

# rev 2 features (likely RaspPi)
	wifi interconnectability
	GPS
		longitude/latitude readout
		simple map gfx, basic lineart mapping  
		data output via csv
	low form factor pcb with breakout board support
