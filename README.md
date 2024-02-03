# LC meter
The LC meter in this project is based on a popular design that's found on numerous websites. I added an external GPSDO input to use as stable clock reference and a second input so the meter can be used as a frequency counter.

## Principle of operation


## Major components
- The display is a 1602A (ver 5.5) with an I2C PCF8574 expander from aliExpress.
- uC is AVR32DB28
