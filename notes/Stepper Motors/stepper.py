# stepper.py
# Rotates stepper motors clockwise/counterclockwise by user input degrees.
# Input 0 degrees to turn on sleep. Enter another angle to wake it up.

import Adafruit_BBIO.GPIO as GPIO
import time

print("hello")

STEP_1 = "P8_7"
DIR_1 = "P8_9"
STEP_2 = "P8_8"
DIR_2 = "P8_10"
SLEEP = "P8_11"
    
def setup():
    GPIO.setup(STEP_1, GPIO.OUT)
    GPIO.setup(DIR_1, GPIO.OUT)
    GPIO.setup(STEP_2, GPIO.OUT)
    GPIO.setup(DIR_2, GPIO.OUT)
    GPIO.setup(SLEEP, GPIO.OUT)
    GPIO.setup("USR1", GPIO.OUT)
    GPIO.output(DIR_1, GPIO.HIGH)
    GPIO.output(DIR_2, GPIO.HIGH)
    GPIO.output(SLEEP, GPIO.HIGH)
    
def rotate(stepper, degChange):
    
    GPIO.output(SLEEP, GPIO.HIGH)
    
    if degChange == 0:
        GPIO.output(SLEEP, GPIO.LOW)
    
    if stepper == 0:
        STEP = STEP_1
        DIR = DIR_1
    elif stepper == 1:
        STEP = STEP_2
        DIR = DIR_2
    
    if degChange < 0:
        degChange = -degChange
        GPIO.output(DIR, GPIO.LOW)

    for i in range(int(degChange/0.45)): #0.45 degrees per step
        GPIO.output(STEP, GPIO.HIGH)
        GPIO.output("USR1", GPIO.HIGH)
        time.sleep(0.0001)
        GPIO.output(STEP, GPIO.LOW)
        GPIO.output("USR1", GPIO.LOW)
        time.sleep(0.0001)
        
    GPIO.output(DIR, GPIO.HIGH)

setup()

while 1:
    #stepper = input("Choose stepper to rotate (1/2): ")
    degChange = input("Degrees to rotate: ")
    
    #if stepper in [1,2]:
    rotate(0, degChange)
    #else:
    #    print("Select stepper 1 or 2")