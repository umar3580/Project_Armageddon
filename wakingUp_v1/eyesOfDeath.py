import RPi.GPIO as gp
import time
import sys
import traceback
import serial
import argparse

def createListInts(arg):
    return list(map(int, arg.split(',')))

parser = argparse.ArgumentParser(prog="PROJECT ARMAGEDDON - Eyes of Death", description="Use at your risk", epilog="BE WARNED!")

parser.add_argument("-ey", "--eyes", type=createListInts, help="List connected gpio  pins")
args = parser.parse_args()


#------- SIG
def cleanUp(type, value, tb):
    traceback_details = "\n".join(traceback.extract_tb(tb).format())
    msg = f"\ncaller: {' '.join(sys.argv)}\n{type}: {value}\n{traceback_details}"

    print(msg)
    gp.cleanup()
    
    print("HOW DARE YOU! I gave you an honoured position, when I return I shall make you pay!")
    sys.exit()


sys.excepthook = cleanUp
#-------- END


#GLOBAL
eyes = [11,13]
#armour = serial.Serial('COM')

if args.eyes:
    eyes = args.eyes


def settingUp():
    gp.setmode(gp.BOARD)

    for i in eyes:
        gp.setup(i, gp.IN, pull_up_down=gp.PUD_DOWN)
    
    print("And I wake from slumber to one day rule the world where you shall be my personal right hand bish")


def stateDetect():
    
    for i in eyes:
        gp.add_event_detect(i, gp.FALLING, callback=objDetect, bouncetime=100)


def objDetect(channel):

    print(f"{channel}")

if __name__ == '__main__':
    settingUp()
    stateDetect()

    while True:
        print("Ah - Such a peaceful existence... None shall stand before me")
        time.sleep(10)


