value = 6
for led in range(16):
    for reg_val in ["ON_L", "ON_H", "OFF_L", "OFF_H"]:
        print(f"#define PCA9685_REGISTER_LED{led}_{reg_val}\t{hex(value).upper()}U")
        value += 1