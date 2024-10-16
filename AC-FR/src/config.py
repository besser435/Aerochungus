try:
    from typing import List, Tuple, Dict, Any, Union
except ImportError:
    pass

from globals import log


"""
ACFR configuration file. Values should only match the type hints.
"""

# Update rates. How often to iterate through the main loop in Hz.
flight_loop_update_rate: int = 100  
descent_loop_update_rate: int = 10  # slower as to save flash memory


enable_logging: bool = True  # Enable printing to the console for debugging. Should be disabled for flight.


# Logging level. Should be set to logging.ERROR for flight.