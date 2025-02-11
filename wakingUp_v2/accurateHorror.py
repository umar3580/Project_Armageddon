import serial, time, argparse, sys, traceback, subprocess
import RPi.GPIO as gp
import communicatingDestruction as cd
import eyesOfDeath as ed
import stuff
import logging_c

def createListInts(arg):
    return list(map(int, arg.split(',')))


parser = argparse.ArgumentParser(prog="PROJECT ARMAGEDDON - Accurate Horror", description="Use at your risk", epilog="BE WARNED!")

parser.add_argument("-cp", "--comPort", help="List available com ports")
parser.add_argument("-ey", "--eyes", type=createListInts, help="List connected gpio  pins")
args = parser.parse_args()

#------- SIG
def cleanUp(type, value, tb):
    traceback_details = "\n".join(traceback.extract_tb(tb).format())
    msg = f"\ncaller: {' '.join(sys.argv)}\n{type}: {value}\n{traceback_details}"
    logging_c.log.critical(msg)

    ed.cleanMe()
    try:
        armour.close()
    except NameError:
        pass
    
    
    logging_c.log.info("No words more can be said to such a looser")
    sys.exit()


sys.excepthook = cleanUp
#-------- END


def zimmerFrame(age):
    cd.donningArmour(armour)
    cd.forgingArms(cd.armLingo(2, "read"), armour)

    readData = cd.translatingWisdom("read", cd.donningArmour(armour))
    zimmer = None

    if age == "young":
        if readData > 50:
            zimmer = 50

    elif age == "old":
        if readData < 130:
            zimmer = 130

    elif age == "middleAge":
        zimmer = 90

    cd.forgingArms(cd.armLingo(2, "write", zimmer), armour)
    cd.donningArmour(armour)




def decisionMaking(channel):
    logging_c.log.info(f"This sensor {channel} has detected advaseries.. Awaiting instuctions to exterminate...")
    
    
    cd.donningArmour(armour)
    cd.forgingArms(cd.armLingo(1, 'read'), armour)
    time.sleep(0.5)
    
    readData = cd.translatingWisdom("read", cd.donningArmour(armour))
    if not readData:
        readData = cd.translatingWisdom("read", cd.donningArmour(armour))
        if not readData:
            logging_c.log.error("READ ERROR")
            return
        
    logging_c.log.info(f"READ DATA: {readData}")
    instruction = readData
    zimmerFrameReq = False
    wheelChairReq = False
    
    if readData < 80:
        zimmerFrameReq = True
        
    elif readData > 110:
        wheelChairReq = True

    
    if channel == eyes[0]:
        instruction -= 20


    elif channel == eyes[1]:
        instruction += 20
  
    
    if zimmerFrameReq:
        if instruction > readData:
            zimmerFrame("young")

    elif wheelChairReq:
        if instruction < readData:
            zimmerFrame("old")
    

    
    cd.forgingArms(cd.armLingo(1, "write", instruction), armour)
    cd.donningArmour(armour)
    
    if 80 < instruction < 110:
        zimmerFrame("middleAge")
                


if __name__ == '__main__':
    # hint: use ls /dev/tty* to find usb - will make this automagically sort itself at some point
    com = ''
    eyes = [11,13]

    
    
    if args.comPort:
        com = f'/dev/tty{args.comPort}'
    else:
        com = stuff.ttyFinder(stuff.returnResponse("ls /dev/tty*"), "USB")
    
    if args.eyes:
        eyes = args.eyes

    armour = serial.Serial(com, 9600, timeout=1)
    armour.reset_input_buffer()

    ed.settingUp(eyes)
    ed.stateDetect(eyes, decisionMaking)


    while True:
        print("I travelled so far and wide...")
        time.sleep(10)



















