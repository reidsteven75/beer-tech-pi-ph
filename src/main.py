from flask import Flask
# import serial
app = Flask(__name__)

@app.route('/')
def hello_world():
    return 'Hello World!'

if __name__ == '__main__':
		print('====================')
		print('BEER TECH: PH Sensor')
		print('====================')
		app.run(host='0.0.0.0', port=80)

		# ser = serial.Serial('/dev/ttyUSB0')  # open serial port
		# print(ser.name)
