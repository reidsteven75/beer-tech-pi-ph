from flask import Flask
import serial
import requests
import threading
from time import sleep
app = Flask(__name__)

webserver = 'https://beer-tech-web-prod.herokuapp.com'
# webserver = 'https://beer-tech-web-qa.herokuapp.com'
postServerInterval = 2
portArduino = '/dev/ttyUSB0'
portAduinoBaudrate = 9600

phTag = "[PH]"
phString = ""
tempTag = "[TEMP]"
tempString = ""

# posts data to server every 5 seconds
def postData():

	threading.Timer(postServerInterval, postData).start()

	if (phString or tempString):
		print('posting...')
		print(phString)
		print(tempString)
		url = webserver + '/sensor'

		data = {'data': phString}
		r = requests.post(url, data)

		data = {'data': tempString}
		r = requests.post(url, data)


# reads arduino serial data
def streamSerialUsb():

	global phString
	global tempString

	try:
		print("opening port {}".format(portArduino))
		ser = serial.Serial(portArduino)
		ser.baudrate = portAduinoBaudrate
		ser.timeout = 1
		
		while True:
			try:
				output = " "
				while output != "":
					output = ser.readline()
					string = output.decode()
					if (phTag in string):
						phString = output
					if (tempTag in string):
						tempString = output

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

		postData()

		streamSerialUsb()
		# app.run(host='0.0.0.0', port=80)


