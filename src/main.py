from flask import Flask
import serial
import requests
from time import sleep
app = Flask(__name__)

webserver = 'https://beer-tech-web-prod.herokuapp.com'
# webserver = 'https://beer-tech-web-qa.herokuapp.com'
portArduinoPh = '/dev/ttyUSB0'
portAduinoPhBaudrate = 9600

def postData(rawData):
	url = webserver + '/sensorPh'
	data = {'data': rawData}
	r = requests.post(url, data)

def streamSerialUsb():
	try:
		print("opening port {}".format(portArduinoPh))
		ser = serial.Serial(portArduinoPh)
		ser.baudrate = portAduinoPhBaudrate
		
		while True:
			try:
				output = " "
				while output != "":
					output = ser.readline()
					print(output)
					postData(output)
			except Exception as e:
				print("USB stream error: " + str(e))
				break

		ser.close()
		retryUsb()
		
	except Exception as e:
		print("USB port open error: " + str(e))
		retryUsb()

def retryUsb():
	sleep(2)
	streamSerialUsb()

if __name__ == '__main__':

		print('====================')
		print('BEER TECH: PH Sensor')
		print('====================')

		streamSerialUsb()
		# app.run(host='0.0.0.0', port=80)


