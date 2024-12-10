commands


http://192.168.100.101:1880/mode?mode=auto




udevadm info -a -n /dev/ttyUSB0
sudo nano /etc/udev/rules.d/99-usb-serial.rules
SUBSYSTEM=="tty", ATTRS{idVendor}=="1234", ATTRS{idProduct}=="5678", SYMLINK+="my-serial-port"

sudo udevadm control --reload-rules
sudo udevadm trigger




sudo chmod 666 /dev/ttyUSB0