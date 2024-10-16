from time import *

from globals import state, iteration_start, iteration_end
from config import flight_loop_update_rate


def _fast_update():
    pass

def _slow_update():
    pass

def sm_tick():
    """Determines how often to iterate through the main loop according to set update rate in Hz."""
    # We update as fast as possible in pad_idle (1) state.
    # We update at the flight_loop_update_rate in boost and apogee (2, 3) states.
    # We slow down to the descent_loop_update_rate when we are in the descent (4) state.

    
    if state == 1:
        return
    
    if state in [2, 3]:
        _fast_update()

    if state == 4:
        _slow_update()

    
