import analogio
import board
vbat_sense_pin = analogio.AnalogIn(board.IO)

def batt_voltage() -> float:
    # See: https://www.youtube.com/watch?v=FytOw9TRPcc
    ADC_VALUES = 2 ** 16 # 16 bit ADC

    # R1 is connected to VBAT, R2 is connected to GND, VBAT_SENSE is in between
    R1 = 1000               # 1k resistor
    vbat_sense = 65_535     # placeholder pin reading
    R2 = 806                # 806o resistor

    vout = vbat_sense / ADC_VALUES * 3.3
    vbat = vout / (R2 / (R1 + R2))

    return vbat