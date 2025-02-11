import serial
import time
import argparse
import sys
import traceback



parser = argparse.ArgumentParser(prog="PROJECT ARMAGEDDON - Communicating Destruction", description="Use at your risk", epilog="BE WARNED!")

parser.add_argument("-cp", "--comPort", help="List available com ports")
args = parser.parse_args()

#------- SIG
def cleanUp(type, value, tb):
    traceback_details = "\n".join(traceback.extract_tb(tb).format())
    msg = f"\ncaller: {' '.join(sys.argv)}\n{type}: {value}\n{traceback_details}"

    print(msg)
    armour.close()

    print("No words more can be said to such a looser")
    sys.exit()


sys.excepthook = cleanUp
#-------- END

com = 'USB1'


if args.comPort:
    com = args.comPort


armour = serial.Serial(f'/dev/tty{com}', 9600, timeout=1)
armour.reset_input_buffer()



def donningArmour():
    readCourtesy = 0
    
    while readCourtesy < 2:
        
        if armour.in_waiting > 0:
            wordsOfWisdom = armour.readline().decode('utf-8').rstrip()
            print(f"Words Of Wisdom: {wordsOfWisdom}")
            readCourtesy -= 1

        else:
            readCourtesy += 1


while True:
    time.sleep(0.3)
    donningArmour()
    
    wordsToSend = input("Information to report: ")
    armour.write(bytes(wordsToSend, 'utf-8'))
    



