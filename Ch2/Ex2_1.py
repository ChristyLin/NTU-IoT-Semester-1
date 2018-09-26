#!/usr/bin/env python

import RPi.GPIO as GPIO
import time
import picamera
global PictureNum
BTN_PIN = 23
PictureNum = 1

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(BTN_PIN, GPIO.IN,pull_up_down = GPIO.PUD_UP)
##GPIO.setup(BTN_PIN, GPIO.IN)

previousStatus = None
previousTime = time.time()
currentTime = None
WAIT_TIME = 750

def TakePicture(BTN_PIN):
    global PictureNum
    print('Take a picture')
    with picamera.PiCamera() as camera:
        #camera.start_preview()
        #time.sleep(5)
        print('image'+str(PictureNum)+'.jpg saved')
        camera.capture('image'+str(PictureNum)+'.jpg')
        PictureNum += 1
        #camera.stop_preview()

try:
    GPIO.add_event_detect(BTN_PIN, GPIO.FALLING, callback=TakePicture, bouncetime = WAIT_TIME)
    while True:
        time.sleep(10)
##        input_state = GPIO.input(BTN_PIN)#GPIO25
##        currentTime = time.time()
##        print(currentTime - previousTime)
##        if input_state == False and previousStatus == True and (currentTime - previousTime) > WAIT_TIME:
##            print('Take a picture')
##            TakePicture(PictureNum)
##            PictureNum += 1
            
            
##        previousStatus =  input_state  
##        previousTime = currentTime
except KeyboardInterrupt:
    print('Exception: KeyboardInterrupt')
finally:
    GPIO.cleanup()