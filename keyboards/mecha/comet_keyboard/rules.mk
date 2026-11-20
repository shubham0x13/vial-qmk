# Pivot Pip pointing stick (ADS1115 on I2C0)
POINTING_DEVICE_DRIVER = custom
I2C_DRIVER_REQUIRED = yes
SRC += pivot_pip.c pip_core.c
