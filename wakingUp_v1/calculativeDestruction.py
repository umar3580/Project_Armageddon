import gpiozero as gz
import sys
import time
import argparse
import traceback


parser = argparse.ArgumentParser(prog="PROJECT ARMAGEDDON - Calculative Destruction", description="Use at your risk", epilog="BE WARNED!")

parser.add_argument("-tr", "--trigger", required=True, help="BCM of trigger pin")
parser.add_argument("-ec", "--echo", required=True, help="BCM of echo pin")
args = parser.parse_args()

#------- SIG
def cleanUp(type, value, tb):
    traceback_details = "\n".join(traceback.extract_tb(tb).format())
    msg = f"\ncaller: {' '.join(sys.argv)}\n{type}: {value}\n{traceback_details}"
    sense.close()
    
    print(msg)
    print("No words more can be said to such a looser")
    sys.exit()


sys.excepthook = cleanUp
#-------- END



def setup(trig, ech):
    sensor = gz.DistanceSensor(echo=ech, trigger=trig, max_distance=3,  threshold_distance=0.2)
    return sensor

def sensingAllergies():
    print("OI - GET AWAY!")
    print(f"This is the distance I have detected {sense.distance*100}")


if __name__ == '__main__':
    sense = setup(args.trigger, args.echo)
    sense.when_in_range = sensingAllergies
    while True:
        print("IDK WHAT I'M DOING")
        time.sleep(10)

