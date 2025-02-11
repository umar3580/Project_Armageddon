import logging_c
import subprocess

def returnResponse(inputCmd="true", respType="str"):
    output = subprocess.check_output(inputCmd, shell=True)
    logging_c.log.debug(f"CMD: {inputCmd} Output: {output}")
    response = output
    
    if respType == "str":
        response = output.decode(encoding="utf-8")

    return response

def ttyFinder(cmdResponse, lineType="tty"):
    sortedResponse = cmdResponse.split("\n")
    logging_c.log.debug(f"Recieved Data Sorted: {sortedResponse}")
    
    availableLines = []
    chosenLine = None

    for i in sortedResponse:
        if lineType in i:
            availableLines.append(i)


    logging_c.log.debug(f"Current Available Lines: {availableLines}")
    noAvailableLines = len(availableLines)

    if noAvailableLines < 1:
        logging_c.log.critical(f"USB not detected - imminent program death")
    
    elif noAvailableLines > 1:
        logging_c.log.info("Multiple tty lines detected, user must select appropriate line")
        for i in availableLines:
            answ = input(f"Confirm if the printed tty line is correct (y/n) > {i}:")
            
            if answ == "y":
                chosenLine = i
                break
    else:
        logging_c.log.info("Automagically selecting tty line")
        chosenLine = availableLines[0]

    if chosenLine is None:
        raise Exception("No Chosen/Available lines")
    
    logging_c.log.debug(f"Returning Line: {chosenLine}")
    return chosenLine