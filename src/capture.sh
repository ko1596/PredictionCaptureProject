#!/bin/bash
i=0
PATHI=/home/root/pic/$3/
if [ "$2" = "0" ]
then
	sleep 2
	DEVICE=/dev/video0
else
	DEVICE=/dev/video1
fi

mkdir $PATHI
echo $PATHI

while [ $i != "$1" ]
do	
	echo _S$(($i+1)).jpg
	v4l2-ctl -d $DEVICE --set-fmt-video=width=1920,height=1080,pixelformat=MJPG --stream-mmap --stream-count=1 --stream-to=$PATHI$3"_S"$(($i+1)).jpg
	
	i=$(($i+1))
	sleep 1
done
