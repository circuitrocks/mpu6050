# MPU6050 (GY-521) — Sample Code

Sample sketch for the **MPU6050 6-axis accelerometer + gyroscope** module, sold by [Circuitrocks](https://circuit.rocks/).

This example reads raw accelerometer, gyroscope, and temperature data and prints it to the Arduino Serial Monitor. It uses no external libraries — only the built-in `Wire` library — so you can see exactly how the chip is being talked to.

## What you need

- 1x MPU6050 module (GY-521 breakout)
- 1x Arduino Uno (or compatible 5V board)
- 4x jumper wires (female-to-male if you're using a breadboard-less setup)

## Wiring

| MPU6050 pin | Arduino Uno pin |
|-------------|-----------------|
| VCC         | 5V              |
| GND         | GND             |
| SCL         | A5              |
| SDA         | A4              |
| INT         | not connected   |

The GY-521 breakout has an onboard 3.3V regulator and level-shifters, so 5V on VCC is safe. If you're using a 3.3V board (ESP32, Pro Mini 3.3V, etc.), connect VCC to 3.3V instead and use that board's I2C pins.

## How to use

1. Open `mpu6050.ino` in the Arduino IDE.
2. Select **Tools → Board → Arduino Uno** and the correct COM port.
3. Click **Upload**.
4. Open **Tools → Serial Monitor** and set the baud rate to **9600**.
5. You should see a stream of values. Tilt the module — the accelerometer values will change. Rotate it — the gyroscope values will change.

## What the values mean

- **aX, aY, aZ** — acceleration in g (1 g ≈ 9.81 m/s²). When the module is sitting flat and still, one axis should read ~1.00 (gravity) and the others ~0.
- **gX, gY, gZ** — angular velocity in degrees per second. Should read near 0 when the module is still.
- **Temp** — internal die temperature in °C. This runs a few degrees above ambient.

## Troubleshooting

- **"MPU6050 not found"** — check VCC and GND first, then SDA/SCL. Swapping SDA and SCL is the most common mistake.
- **All values are 0 or -1** — usually a wiring issue or the module isn't getting power. Check that the small power LED on the GY-521 is lit.
- **Values look noisy** — that's normal for raw output. Real applications usually filter the data (e.g. complementary filter, Kalman filter) before using it.

## License

This sample code is released under the MIT License — see [LICENSE](LICENSE). You can use it in your own projects, commercial or otherwise, with no restrictions.

## Got stuck?

Drop by [circuit.rocks](https://circuit.rocks/) or message us — we're happy to help our customers get their modules working.
