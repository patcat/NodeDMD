var SerialPort = require('serialport').SerialPort,
    PNG = require('png-js'),
    serialPort = new SerialPort('/dev/tty.usbmodem1431', {
      baudrate: 57600
    }),
    serialMessage = '';

serialPort.on('open', function() {
  console.log('Serial port open');
  
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

    setTimeout(function() {
      serialPort.write(serialMessage);
    }, 2000);
  });
});