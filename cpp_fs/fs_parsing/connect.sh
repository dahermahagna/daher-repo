mkdir /tmp/ramdisk
modprobe brd rd_nr=1 rd_size=102400 max_part=1
/sbin/mkfs.ext2 /dev/ram0
mount /dev/ram0 /tmp/ramdisk
chmod 777 /dev/ram0
chmod 777 /tmp/ramdisk
touch /tmp/ramdisk/name.txt
touch /tmp/ramdisk/lastname.txt
echo daher >> /tmp/ramdisk/name.txt
echo mahagna >> /tmp/ramdisk/lastname.txt
chmod 777 /tmp/ramdisk/name.txt
chmod 777 /tmp/ramdisk/lastname.txt