import logging
import os
import sys

# logging.basicConfig(format="[%(asctime)s] %(levelname)s: %(filename)s:%(lineno)s | %(message)s", level=os.environ.get("LOGLEVEL", "DEBUG"))

log = logging.getLogger(__name__)
stdout = logging.StreamHandler(stream=sys.stdout)
fmtOption = logging.Formatter("%(levelname)s: %(asctime)s  | %(filename)s>%(lineno)s | %(message)s")

stdout.setLevel(logging.DEBUG)
stdout.setFormatter(fmtOption)

log.setLevel(logging.DEBUG)
log.addHandler(stdout)




# def log(data, out="DEBUG: "):
#     print(f"{out}{data}")