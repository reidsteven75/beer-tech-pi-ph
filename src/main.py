from flask import Flask
import serial
import requests
app = Flask(__name__)

webserver = '192.168.1.79:3000/'
portArduinoPh = '/dev/ttyUSB0'
portAduinoPhBaudrate = 9600

def postData(rawData):
	url = webserver + 'sensor/ph'
	data = {'data': rawData}
	r = requests.post(url, data)
	print(r.text)

def streamSerialUsb():
	print("opening port {}".format(portArduinoPh))
	ser = serial.Serial(portArduinoPh)
	ser.baudrate = portAduinoPhBaudrate
	
	while True:
		output = " "
		print("----")
		while output != "":
			output = ser.readline()
			print(output)
			postData(output)

if __name__ == '__main__':

		print('====================')
		print('BEER TECH: PH Sensor')
		print('====================')

		streamSerialUsb()
		# app.run(host='0.0.0.0', port=80)


