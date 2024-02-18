"""
Resistor Ladder Helper
"""

RESISTANCE = 1000
NUM_BUTTONS = 4

def voltage_divider(vs:float, r1: float, r2: float):
    return (vs * r2) / (r1 + r2)


if __name__ == "__main__":
    for button in range(0, NUM_BUTTONS):
        print(f"Button {button + 1}: {voltage_divider(vs=3.3, r1=RESISTANCE, r2=RESISTANCE * button)}V")