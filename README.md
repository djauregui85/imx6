# imx6
Repository with setup steps documentation and working files for the TS-7970

# SD Card

1. Insertar la tarjeta SD

2. Verificar el nombre de la unidad con `lsblk`. Ejemplo: `/dev/mmcblk0p1`

3. Verificar que no este montada la unidad

```
sudo umount /dev/mmcblk0p1
```

4. Formater la unidad con el formato ext3
```
sudo mkfs.ext3 /dev/mmcblk0p1
```
5. Crear un carpeta donde se montara la unidad de la tarjeta SD
```
sudo mkdir /mnt/sd
sudo mount /dev/mmcblk0p1 /mnt/sd/
```
6. Descargar el OS y descomprimirlo en la SD
```
wget ftp://ftp.embeddedarm.com/ts-arm-sbc/ts-7990-linux/distributions/debian/debian-armhf-jessie-latest.tar.bz2
sudo tar --numeric-owner -xf ubuntu-armhf-20.04-latest.tar.bz2 -C /mnt/sd
sudo umount /mnt/sd
sync
```

# Conectar a la consola

Conectar el cable usb al puerto de la consola y a la computadora
```
sudo picocom -b 115200 /dev/ttyUSB0
```


sudo tar --numeric-owner -xjf ubuntu-armhf-20.04-latest.tar.bz2 -C /mnt/emmc

# Obtener internet en Ubuntu
```
dhclient eth0
echo "nameserver 8.8.8.8" > /etc/resolv.conf
```

# Configurar Red en eth0 automaticamente al iniciar


In /etc/systemd/network/eth.network
```
[Match]
Name=eth0

[Network]
DHCP=yes
```
Luego
```
systemctl enable systemd-networkd
```

# Gabrar el eMMC
```
mkfs.ext3 /dev/mmcblk2p1
mkdir /mnt/emmc
mount /dev/mmcblk2p1 /mnt/emmc
wget -qO- https://files.embeddedarm.com/ts-socket-macrocontrollers/ts-4900-linux/distributions/ubuntu/ubuntu-armhf-20.04-latest.tar.bz2 
tar --numeric-owner -xjf ubuntu-armhf-20.04-latest.tar.bz2 -C /mnt/emmc/
umount /mnt/emmc
sync
```

# Crear usuario para acceder por SSH

adduser cvr

# Clave de root

passwd root

# Instalar utilitarios

sudo apt install midori xfc4-terminal


# Install Arduino IDE
```
sudo apt install arduino
```

Give permission to the user to use the serial port.

## Upload a hex file
```
avrdude -C /usr/share/arduino/hardware/tools/avrdude.conf -v -p atmega328p -c arduino -P /dev/ttyACM1 -b 115200 -D -U flash:w:grbl_v1.1h.20190825.hex:i
```


# Download UGS for ARM

wget --no-check-certificate https://ugs.jfrog.io/ugs/UGS/v2.0.7/ugs-platform-app-pi.tar.gz
tar -xf ugs-platform-app-pi.tar.gz
apt install libxtst6


# Run Python3 script

Dependencia

sudo apt-get install python3-serial

## Instalar gstreamer

apt-get update
apt-get upgrade
apt install g++ cmake pkg-config
apt-get install libgstreamer1.0-0 gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-doc gstreamer1.0-tools gstreamer1.0-x


# Utils

apt-get install feh mousepad v4l-utils

# No organizado

Recompilar kernel rpi

https://engineering-diy.blogspot.com/2013/01/adding-7inch-display-with-touchscreen.html


env set cmdline_append "console=ttymxc0,115200 rootwait ro init=/sbin/init video=mxcfb0:dev=hdmi,800x480M@60,if=RGB24" 


root@ts-imx6:/etc/X11# cat xorg.conf 
Section "Device"
    Identifier  "i.MX Accelerated Framebuffer Device"
    Driver      "vivante"
    Option      "fbdev"     "/dev/fb0"
    Option      "vivante_fbdev" "/dev/fb0"
    Option      "HWcursor"  "false"
EndSection

Section "ServerFlags"
    Option "BlankTime"  "0"
    Option "StandbyTime"  "0"
    Option "SuspendTime"  "0"
    Option "OffTime"  "0"
EndSection
