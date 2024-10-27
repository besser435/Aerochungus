import time




import ac_boot
import globals
from tick import sm_tick


"""Main state machine and loop for managing ACFR"""


state = globals.state


if state == 0:   # Boot
    import ac_boot

while True:
    try:
        globals.iteration_start = time.monotonic()
        if state == 1:
            import pad_idle

        elif state == 2:
            import boost

        elif state == 3:
            import apogee

        elif state == 4:
            import decent

        elif state == 5:
            import landed

        else:
            raise ValueError(f"Invalid state value: {state}")
        
        globals.iteration_end = time.monotonic()
        sm_tick()
    except Exception as e:
        logging.error(f"An error occurred in state {state}: {e}")
        raise e
