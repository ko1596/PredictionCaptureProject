#!/bin/bash
i=0
while [ $i != 5]
do
    NOW=$(data +"%Y%m%d%H%M%S")
    FILE0="/home/root/$NOW"

    echo $FILE0
    v4l2-ctl -d /dev/video1 --set-parm=30 --set-fmt-video=width=1920,height=1080,pixelformat=YUYV --stream-mmap --stream-count=1 --stream-to=$FILE0.raw
    i=$(($i+1))
    sleep 1
done