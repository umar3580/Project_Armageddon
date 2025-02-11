import time
import logging_c


def donningArmour(armour):
    readCourtesy = 0
    wisdomOut = []
    
    while readCourtesy < 2:
        
        if armour.in_waiting > 0:
            wordsOfWisdom = armour.readline().decode('utf-8').rstrip()
            wisdomOut.append(wordsOfWisdom)
            logging_c.log.info(f"Donning Armour Wisdom: {wordsOfWisdom}")
            readCourtesy -= 1

        else:
            readCourtesy += 1

    return wisdomOut

def forgingArms(directions, armour):
    armour.write(bytes(directions, 'utf-8'))


def armLingo(serve, action, data=None):
    armTranslation = {
        "read": 'r',
        "write": 'w',
        "attach": 'a',
        "detach": 'd'
    }
    
    if data is None:
        return f"<{serve}-{armTranslation[action]}>"

    return f"<{serve}-{armTranslation[action]}{data}>"


def translatingWisdom(request, data):
    searchField = {
        "read": "Read Position: "
    }

    reqData = False

    logging_c.log.info(f"transWis Searching for: {searchField[request]}" )
    for i in data:
        if searchField[request] in i:
            reqData = int(i.split(searchField[request])[1])
            break

    return reqData
