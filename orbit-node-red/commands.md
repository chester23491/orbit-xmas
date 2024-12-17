commands


http://192.168.100.101:1880/mode?mode=auto




 2341:0043 

udevadm info -a -n /dev/ttyACM0
sudo nano /etc/udev/rules.d/99-usb-serial.rules
SUBSYSTEM=="tty", ATTRS{idVendor}=="2341", ATTRS{idProduct}=="0043", ATTRS{serial}=="758343439393515140F0", SYMLINK+="lift0"

SUBSYSTEM=="tty", ATTRS{idVendor}=="2341", ATTRS{idProduct}=="0043", ATTRS{serial}=="85735313333351608261", SYMLINK+="light0"


sudo udevadm control --reload-rules
sudo udevadm trigger




sudo chmod 666 /dev/light0



 001 Device 004: ID 2341:0043 Arduino SA Uno R3 (CDC ACM)

 ATTRS{serial}=="85735313333351608261"
