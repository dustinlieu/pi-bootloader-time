# pi-bootloader-time
A tool to find the amount of time a Raspberry Pi spends in the bootloader.

## Compilling
```
$ make
```

## Usage
```
$ sudo ./bootloader_time
SoC uptime: 954.480248s
Linux uptime: 947.800000s
Time spent in bootloader: 6.680248s
```

## How it works
The Broadcom SoCs used on Raspberry Pis have a system timer peripheral which starts counting on power up. If we subtract the Linux uptime from this, then it gives us the amount of time spent in the bootloader.

## Compatibility
This should work on all Raspberry Pis. All [Raspberry Pi SoCs](https://www.raspberrypi.com/documentation/computers/processors.html) have the system timer peripheral at the same offset from the peripheral base address. The peripheral base address is found dynamically through `bcm_host_get_peripheral_address()`.

## License
MIT
