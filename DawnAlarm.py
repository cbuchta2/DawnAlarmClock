import gpiozero
import time
from datetime import datetime, timedelta

#configure IO
doEnabledLED = gpiozero.LED(21)
doDisabledLED = gpiozero.LED(20)
doLightRelay = gpiozero.OutputDevice(4,active_high=False)
diInputBtn = gpiozero.Button(26)

#define Alarm Class
class Alarm:
    def __init__(self):
        self.enabled = 1
        self.setTime = None
        self.presetMin = 0
        self.prevEdge = 0
        
    #returns true if rising edge of alarm (current time == alarm time)
    def alarmRisingEdge(self):
        currTime = datetime.now()
        alarmActive = self.setTime.hour == currTime.hour and alarm.setTime.minute == currTime.minute
        if alarmActive and not self.prevEdge:
            self.prevEdge = alarmActive
            return True
        else:
            self.prevEdge = alarmActive
            return False

#define function to toggle enable on button press
def toggleEnable(alarm):
    alarm.enabled =  not alarm.enabled
    return

#declare class instance and initialize IO
alarm = Alarm()
diInputBtn.when_pressed = lambda: toggleEnable(alarm)

#set alarm time via command line input 
while True:
    timeInput = input("Please enter wakeup time in HH:MM format (24 hour): ")
    try:
        alarm.setTime = datetime.strptime(timeInput, "%H:%M")
        print("Waking up at: " + datetime.strftime(alarm.setTime, "%H:%M"))
        break

    except ValueError:
        print("Incorrect entry please try again.")

#set light preset time (time before alarm that the light will turn on)
while True:
    lightTime = input("Please enter amount of time light should be on before alarm (minutes):  ")
    try:
        alarm.presetTime = int(lightTime)
        alarm.setTime =  alarm.setTime - timedelta(minutes=alarm.presetTime)
        print("Light will turn on: %d minutes before alarm at: " % alarm.presetTime, datetime.strftime(alarm.setTime, "%H:%M"))
        break
    except ValueError:
        print("Incorrect entry please try again.")

#Continuously check for rising edge of alarm to turn light on
while True:
    if alarm.enabled:
        doEnabledLED.on()
        doDisabledLED.off()
        if alarm.alarmRisingEdge():
            doLightRelay.on()
    else:
        doLightRelay.off()
        doEnabledLED.off()
        doDisabledLED.on()