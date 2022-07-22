import spidev
import time
import threading
from numpy import interp
from Adafruit_IO import Client
import Adafruit_BMP.BMP085 as BMP085
import adafruit_mcp3xxx.mcp3008 as MCP
from adafruit_mcp3xxx.analog_in import AnalogIn
import busio
import digitalio
import board
import serial
import drivers
display = drivers.Lcd()
display.lcd_clear()
serial1 = serial.Serial('/dev/ttyUSB0', 115200)
spi = busio.SPI(clock=board.SCK, MISO=board.MISO, MOSI=board.MOSI)
cs = digitalio.DigitalInOut(board.D5)
mcp = MCP.MCP3008(spi, cs)
channel_0 = AnalogIn(mcp, MCP.P1)
spi = spidev.SpiDev()
spi.open(0,0)
bmp = BMP085.BMP085()
ADAFRUIT_IO_KEY = 'aio_ujhl41jqb7sV7LOxoUf6u0XJXncz'
ADAFRUIT_IO_USERNAME = 'Ahnaf12'
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)
def _range(x, in_min, in_max, out_min, out_max):
    return int((x - in_min) * (out_max - out_min) / (in_max - in_min)+ out_min)
def analogInput(channel):
  spi.max_speed_hz = 1350000
  adc = spi.xfer2([1,(8+channel)<<4,0])
  data = ((adc[1]&3) << 8) + adc[2]
  return data
def soil():
    while True:
        sensorValue = _range(channel_0.value, 0, 30000, 0, 1023)
        output = analogInput(0)
        output = interp(output, [0, 1023], [100, 0])
        output = int(output)
        water = analogInput(2)
        water = interp(water, [0, 1023], [0, 100])
        water = int(water)
        display.lcd_display_string("T:"+str(bmp.read_temperature())+"*C"+" S:"+str(output)+"%", 1)
        display.lcd_display_string("W:"+str(water)+"%"+"     A:"+str(sensorValue)+"ppm", 2)
        if (output<42):
            serial1.write(b'on')
        if (water<30):
            serial1.write(b'sendsms')
def iot():
    while True:
        sensorValue = _range(channel_0.value, 0, 30000, 0, 1023)
        output = analogInput(0)
        output = interp(output, [0, 1023], [100, 0])
        output = int(output)
        water = analogInput(2)
        water = interp(water,[0, 1023], [0, 100])
        water = int(water)
        airfeed = aio.feeds('air-quality')
        aio.send_data(airfeed.key, sensorValue)
        pressurefeed = aio.feeds('barometric-pressure')
        aio.send_data(pressurefeed.key, bmp.read_pressure())
        tempfeed = aio.feeds('temperature')
        aio.send_data(tempfeed.key, bmp.read_temperature())
        soilfeed = aio.feeds('soil-moisture')
        aio.send_data(soilfeed.key, output)
        waterfeed = aio.feeds('water-level')
        aio.send_data(waterfeed.key, water)
        time.sleep(10)
thread1 = threading.Thread(target=soil)
thread1.start()
thread2 = threading.Thread(target=iot)
thread2.start()
