#!/bin/bash

  make clean
  make

  echo "__________ __________ __________ __________"
  scp ./Ta5320G_TIMER root@192.168.1.105://home/root/a53m0
  ls -l ./Ta5320G_TIMER
  #echo ""
  date

exit 0
