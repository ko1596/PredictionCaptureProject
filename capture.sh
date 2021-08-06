#!/bin/bash
v4l2-ctl --set-parm=30
v4l2-ctl --set-fmt-video=width=1920,height=1080,pixelformat=MJPG
v4l2-ctl --stream-mmap --stream-count=1 --stream-to=/home/root/0526001.jpeg

