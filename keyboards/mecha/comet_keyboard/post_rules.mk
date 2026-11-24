# Pivot Pip pointing stick (ADS1115 on I2C0). To build without it, and without
# QMK's pointing device feature: qmk compile -e PIVOT_PIP_ENABLE=no
PIVOT_PIP_ENABLE ?= yes
ifeq ($(strip $(PIVOT_PIP_ENABLE)), no)
    POINTING_DEVICE_ENABLE = no
endif

ifeq ($(strip $(POINTING_DEVICE_ENABLE)), yes)
    POINTING_DEVICE_DRIVER = custom
    I2C_DRIVER_REQUIRED = yes
    SRC += pivot_pip.c lib/pip_core/pip_core.c
endif
