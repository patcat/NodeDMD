var serialPort = require("serialport"),
	PNG = require('png-js'),
	SerialPort = serialPort.SerialPort,
	serialPort = new SerialPort("/dev/tty.usbmodem1431", {
		baudrate: 57600
	}),
	serialMessage = '';

serialPort.on("open", function() {
	console.log('Serial port open');
	serialPort.on("data", function (data) {
		console.log('Data received: ' + data);
	});

	PNG.decode('sitepointlogo-withsmile.png', function(data) {
		serialMessage = '';

		for (i = 0; i < data.length; i+=4) {
			var red = data[i],
                  green = data[i+1],
                  blue = data[i+2],
                  luminance = ((red * 299) + (green * 587) + (blue * 114)) / 1000;

            if (luminance > 150) {
            	serialMessage += '0';
          	} else {
	            serialMessage += '1';
          	}
	    }
	    serialPort.write(serialMessage);

	    setTimeout(function() {
			serialPort.write(serialMessage);
		}, 1000);
	});

	
});