import adafruit_logging as logging

from config import *

"""
The place for global events and variables.
This is probably bad practice, but I dont care!

Generally, you shouldn't have to touch this file.
"""










# General Variables
state: int = 0 


iteration_start: float = 0.0
iteration_end: float = 0.0





# Logging
log = logging.getLogger("ACFR")
if enable_logging:
    log.setLevel(logging.NOTSET)
else:
    log.setLevel(logging.ERROR)

# def log(msg: str, level: int = logging.INFO):
#     log.log(level, msg)

