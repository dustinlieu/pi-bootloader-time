bootloader_time: bootloader_time.c
        gcc -o bootloader_time bootloader_time.c -lbcm_host

clean:
        rm bootloader_time