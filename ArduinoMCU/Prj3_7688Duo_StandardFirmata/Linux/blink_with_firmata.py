from pyfirmata import Arduino, util
from time import sleep

board = Arduino('/dev/ttyS0')
print "Start blinking D13"
while True:

	board.digital[13].write(1)
	print "LED D13 ON"
	sleep(0.5)
	board.digital[13].write(0)
	print "LED D13 OFF"
	sleep(0.5)