import RPi.GPIO as gp
import time
import logging_c

def settingUp(eyes):
    gp.setmode(gp.BOARD)
    logging_c.log.debug(f"this is eyes from settingUP {eyes}")
    for i in eyes:
        gp.setup(i, gp.IN, pull_up_down=gp.PUD_DOWN)
    
    logging_c.log.info("And I wake from slumber to one day rule the world where you shall be my personal right hand bish")


def stateDetect(eyes, objctDecision):
    for i in eyes:
        gp.add_event_detect(i, gp.RISING, callback=objctDecision, bouncetime=100)


def objDetect(channel):
    logging_c.log.debug(f"objDetect channel: {channel}")


def cleanMe():
    gp.cleanup()
    