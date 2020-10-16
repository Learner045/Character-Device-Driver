Execution steps:

make
sudo insmod cDevice.ko
./testcDevice
dmesg (optional)
sudo rmmod cDevice


cDevice.c is kernel file
testcDevice.c is Userprogram file that calls the character device read function

