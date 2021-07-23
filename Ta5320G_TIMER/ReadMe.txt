



//********************************************************************
//  SDK     -   Ta5320G_TIMER
//********************************************************************
//  FileZillea :: 	==================================================
D:\Green_Project\Prj_Parking\iMX8mini-SDK\SDK-M0\
/home/user/yocto_imx8/SDK/d88sdk-M0


//	docker 		========================================================
//	git-u1664_M4-01 	================================================
docker ps -a

advt@git-u1664:~$ docker ps -a
CONTAINER ID        IMAGE                     COMMAND             CREATED             STATUS                     PORTS               NAMES
3742ca706c83        ubuntu:18.04              "/bin/bash"         13 days ago         Exited (0) 13 days ago                         hardcore_elbakyan
ded14a49880c        advrisc/u18.04-imx8lbv1   "/bin/bash"         6 weeks ago         Up 11 days                                     imx8mm-sdk
507bbace5351        advrisc/u18.04-imx8lbv1   "/bin/bash"         6 weeks ago         Up 2 weeks                                     imx8mm-image
1c25897e25d6        advrisc/u16.04-imx8lbv1   "/bin/bash"         12 months ago       Up 2 weeks                                     greenidea-imx8lbv1-sdk
3d37c0891304        advrisc/u16.04-imx8lbv1   "/bin/bash"         12 months ago       Exited (0) 12 months ago                       imx8mm02
advt@git-u1664:~$ 



//--------------------------------------------------------------------
//	20210531
//	 		SDK  Start 		imx8mm-sdk 			--------------------------------
docker start imx8mm-sdk
docker exec -it imx8mm-sdk /bin/bash

cd /home/adv/adv-release-bsp
source ./build_sdk/tmp/work/aarch64-poky-linux/meta-toolchain/1.0-r7/sdk/image/opt/fsl-imx-xwayland/4.14-sumo/environment-setup-aarch64-poky-linux

export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig
export ARCH=arm64
export CROSS_COMPILE=/usr/bin/aarch64-linux-gnu-gcc
export JSON_C_DIR=/usr/local/

cd /home/adv/adv-release-bsp/D88SDK-M0/Ta5320G_TIMER

//	Local PC 	========================================================
cd D:\Green_Project\Prj_Parking\iMX8mini-SDK\SDK-M0\Ta5320G_TIMER
cd /home/user/yocto_imx8/sdk/D88SDK-M0/Ta5320G_TIMER
//	linux
cd /home/user/yocto_imx8/sdk/D88SDK-M0/Ta5320G_TIMER


//	${CC} 		********************************************************
##${CC} -o Ta5320G_TIMER Ta5320G_TIMER.c Ta53M0.c m0Uart_rx.c m0Uart_tx.c getDcuParam.c TstA53M0.c -lpthread

make clean
make

scp ./Ta5320G_TIMER root@192.168.1.104://home/root/a53m0
scp ./Ta5320G_TIMER root@192.168.1.104://home/root/D88

/home/root/a53m0/Ta5320G_TIMER 1
./Ta5320G_TIMER 1


ls -l --time-style='+%y-%m-%d %H:%M:%S'


ps -aux | grep Ta5320G_TIMER
top p PID

//	Samuel RUN
ps -aux | grep grids_main


//		Old Ver. 		----------------------------------------------------
//	 SDK  Start 		--------------------------------------------------
//--------------------------------------------------------------------
docker start greenidea-imx8lbv1-sdk
docker exec -it imx8mm-sdk /bin/bash

cd /home/adv/adv-release-bsp/d88sdk-M0
source environment-setup-aarch-64-poky-linux
cd /home/adv/adv-release-bsp/d88sdk-M0/Ta5320G_TIMER

//	Local PC 	========================================================
cd D:\Green_Project\Prj_Parking\iMX8mini-SDK\SDK-M0\Ta5320G_TIMER
//	linux
cd /home/user/yocto_imx8/SDK/d88sdk-M0



//********************************************************************
//	ROM-5721  ==>  json-c

root@imx8mmrom5721a1:~/a53m0# ls -l | grep libjson-c
-rwxr-xr-x 1 root root 619726 Jun 16 08:47 libjson-c.a
-rwxr-xr-x 1 root root 255800 Jun 16 08:47 libjson-c.so
-rwxr-xr-x 1 root root 255800 Jun 16 08:47 libjson-c.so.5
-rwxr-xr-x 1 root root 255800 Jun 16 08:47 libjson-c.so.5.1.0

cp libjson-c.* /usr/lib/ 

====>
root@imx8mmrom5721a1:/usr/lib# ls -l | grep libjson
-rwxr-xr-x  1 root root   619726 Jun 16 08:50 libjson-c.a
-rwxr-xr-x  1 root root   255800 Jun 16 08:50 libjson-c.so
lrwxrwxrwx  1 root root       18 Apr 14 11:58 libjson-c.so.3 -> libjson-c.so.3.0.1
-rwxr-xr-x  1 root root    51616 Apr 14 11:59 libjson-c.so.3.0.1
-rwxr-xr-x  1 root root   255800 Jun 16 08:50 libjson-c.so.5
-rwxr-xr-x  1 root root   255800 Jun 16 08:50 libjson-c.so.5.1.0
lrwxrwxrwx  1 root root       27 Apr 14 11:55 libjson-glib-1.0.so.0 -> libjson-glib-1.0.so.0.400.2
-rwxr-xr-x  1 root root   145160 Apr 14 11:59 libjson-glib-1.0.so.0.400.2


****   ln -s libjson-c.so.5 libjson-c.so
root@imx8mmrom5721a1:/usr/lib# rm libjson-c.so

root@imx8mmrom5721a1:/usr/lib# ln -s libjson-c.so.5 libjson-c.so
root@imx8mmrom5721a1:/usr/lib# ls -l | grep libjson
-rwxr-xr-x  1 root root   619726 Jun 16 08:50 libjson-c.a
lrwxrwxrwx  1 root root       14 Jun 16 08:56 libjson-c.so -> libjson-c.so.5
lrwxrwxrwx  1 root root       18 Apr 14 11:58 libjson-c.so.3 -> libjson-c.so.3.0.1
-rwxr-xr-x  1 root root    51616 Apr 14 11:59 libjson-c.so.3.0.1
-rwxr-xr-x  1 root root   255800 Jun 16 08:50 libjson-c.so.5
-rwxr-xr-x  1 root root   255800 Jun 16 08:50 libjson-c.so.5.1.0
lrwxrwxrwx  1 root root       27 Apr 14 11:55 libjson-glib-1.0.so.0 -> libjson-glib-1.0.so.0.400.2
-rwxr-xr-x  1 root root   145160 Apr 14 11:59 libjson-glib-1.0.so.0.400.2






//********************************************************************
//		TL-SAM
//********************************************************************

//		加密16 bytes. 總回應bytes數是 256+(16+16）+256 = 544 bytes


//	Test 	adlk00026 		==============================================
scp /home/root/TL_SAM/*.tlsam user@192.168.1.104://home/user/TL_SAM








//********************************************************************
//
ttymxc0 	CN15 	下(SER2/UART1): /dev/ttymxc0
ttymxc1 	COM1 	(A53 debug/UART2): /dev/ttymxc1
ttymxc2 	CN15	上(SER0/UART3): /dev/ttymxc2
ttymxc3 	COM3 	(M4 debug/UART4): /dev/ttymxc3



//********************************************************************
//  UTC
//********************************************************************
#timedatectl list-timezones
Asia/Samarkand
Asia/Seoul
Asia/Shanghai
Asia/Singapore
Asia/Srednekolymsk
Asia/Taipei
Asia/Tashkent
Asia/Tbilisi
Asia/Tehran
Asia/Thimphu
Asia/Tokyo
Asia/Tomsk
Asia/Ulaanbaatar
Asia/Urumqi

#timedatectl set-timezone Asia/Taipei




//********************************************************************
//  A53 Sleep
//********************************************************************
root@imx8mmrom5721a1:~# cat /sys/power/state
freeze mem


echo mem > /sys/power/state
==> 	0.10A
[  174.984246] PM: suspend entry (deep)
[  174.987890] PM: Syncing filesystems ... done.
[  175.011427] Freezing user space processes ... (elapsed 0.054 seconds) done.
[  175.072664] OOM killer disabled.
[  175.075943] Freezing remaining freezable tasks ... (elapsed 0.001 seconds) done.
[  175.084672] Suspending console(s) (use no_console_suspend to debug)


echo freeze > /sys/power/state
==> 	0.24A
[  143.566466] PM: suspend entry (s2idle)
[  143.570279] PM: Syncing filesystems ... done.
[  143.640868] Freezing user space processes ... (elapsed 0.001 seconds) done.
[  143.649693] OOM killer disabled.
[  143.652954] Freezing remaining freezable tasks ... (elapsed 0.000 seconds) done.
[  143.661370] Suspending console(s) (use no_console_suspend to debug)





//********************************************************************
//  Radar Test
//********************************************************************
ftp.green-ideas.com.tw
user: [parkmeter]
passwd:[gitpark]

ftp :		/home/ftp/parkmeter/Tst_radar




wput ./getParam.txt ftp://parkmeter:gitpark@ftp.green-ideas.com.tw/Tst_radar/
wget ftp://parkmeter:gitpark@ftp.green-ideas.com.tw/Tst_radar/getParam.txt




//********************************************************************
//  ADC Test
//********************************************************************
8.4V / 3 = 2.8V
33 / 28 = 4095 / 3474.545454545455		3474

84 / 3474  = 62 / 2564.142857142857



//********************************************************************
//  GPIO A53
//********************************************************************
P108  	 GPIO0		409
P109  	 GPIO1 		GPIO5_IO06 	(5-1)*32+6=134
P110  	 GPIO2		497
P111  	 GPIO3
P112  	 GPIO4		498
P113  	 GPIO5 		133
P114  	 GPIO6 		499
P115  	 GPIO7 		500
P116  	 GPIO8 		501
P117  	 GPIO9 		502
P118  	 GPIO10 	503
P119  	 GPIO11 	131
 
P109: GPIO1: MX8MM_IOMUXC_ECSPI1_SCLK_GPIO5_IO06: 134
P111: GPIO3: MX8MM_IOMUXC_GPIO1_IO08_GPIO1_IO08: 8

S107 		GPIO4_IO27	123
//	GPIO4_IO27 	......................................................
echo 123 > /sys/class/gpio/export


//	GPIO0 	496 	----------------------------------------------------
//	TEST.		P113  	 GPIO5 		133
root@imx8mmrom5721a1:~# echo 133 > /sys/class/gpio/export
root@imx8mmrom5721a1:~# echo in > /sys/class/gpio/gpio1/direction
root@imx8mmrom5721a1:~# cat /sys/class/gpio/gpio1/value
1
root@imx8mmrom5721a1:~# echo 133 > /sys/class/gpio/unexport

//	TEST.		P119  	 GPIO11 	131
root@imx8mmrom5721a1:~# echo 131 > /sys/class/gpio/export
root@imx8mmrom5721a1:~# echo in > /sys/class/gpio/gpio1/direction
root@imx8mmrom5721a1:~# cat /sys/class/gpio/gpio1/value
1
root@imx8mmrom5721a1:~# echo 131 > /sys/class/gpio/unexport


//	TEST.		P134 	GPIO5_IO29		157
echo 157 > /sys/class/gpio/export
0
//	TEST.		P135 	GPIO5_IO28		156
echo 156 > /sys/class/gpio/export
0

//	TEST.		P57 	GPIO5_IO23		151
//	TEST.		P58 	GPIO5_IO24		152


//====================================================================
//--------------------------------------------------------------------
//	unsigned char   FwVer_Ta53201_UART_HS[] = "iMX8mm Ta5320G_TIMER  20210512-209d6";
//	Tx:	sleep(50ms)
//	Rx:	sleep(50ms)
//	uartA53M0_Req_Dumming_Test() 		75ms
RxM0  -xxxCRCxxx-[D75D][3FCB]  crcError[000000000021]		9978
 Tx[00010001]  Rx[00009957]				:	0.9956004399560044

RxM0  -xxxCRCxxx-[DF54][3FCB]  crcError[000000000030]		9971
 Tx[00010001]  Rx[00009941]				:	0.994000599940006









回復來看看



// 	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

RxM0 Radar-A [21-06-10 03:24:39] FW03000000 radar[0]  CC[0] Parking[0] LR[0]   type[5] xyz[16-6-12] [30,30,370,180,115] [65_0]
RxM0 Radar-B [21-06-10 03:24:40] FW03000000 radar[0]  CC[0] Parking[0] LR[1]   type[5] xyz[20-6-9] [30,30,370,180,115] [65_0]


// 	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig
export ARCH=arm64
export CROSS_COMPILE=/usr/bin/aarch64-linux-gnu-gcc
export JSON_C_DIR=/usr/local/

//====================================================================

export PKG_CONFIG_PATH+=/usr/local/lib/pkgconfig
export ARCH+=arm64
export CROSS_COMPILE+=/usr/bin/aarch64-linux-gnu-gcc
export JSON_C_DIR+=/usr/local/

PWD = $(shell pwd)
##SRC = testjsonc.c

TARGET = Ta5320G_TIMER

CC=aarch64-linux-gnu-gcc
#CC=gcc

CFLAGS += $(shell pkg-config --cflags json-c)
LDFLAGS += $(shell pkg-config --libs json-c)
LDFLAGS += -pthread


OBJS = 	./Ta5320G_TIMER.o \
				./Ta53M0.o \
				./m0Uart_rx.o \
				./m0Uart_tx.o \
				./getDcuParam.o \
				./TstA53M0.o


all: $(OBJS) $(TARGET)

$(TARGET): 
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

romfs:
	$(ROMFSINST) -d /cz/$(EXEC)
	$(ROMFSINST) -d /cz/html

clean:
	rm -f $(TARGET) *.elf *.gdb *.o
	rm -f $(OBJS)



//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//	Samuel RUN
ps -aux | grep grids_main

scp ./Ta5320G_TIMER root@192.168.1.104://home/root/a53m0
scp ./Ta5320G_TIMER root@192.168.1.104://home/root/D88


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


F/W Update :

A53-API F/W :  [iMX8mm Ta5320G_TIMER  20210623-20F33]

M0 SAML22 F/W :  [Prj20C_17ADC 210621-020Cf2Tradar]

scp ./getParam.txt root@192.168.1.108://home/root/a53m0

json_object_put()


ps -aux | grep Ta5320G_TIMER

/home/root/a53m0/Ta5320E_JSON
/home/root/a53m0/Ta5320G_TIMER 1



 setserial -a /dev/ttymxc2


stty -F /dev/ttymxc2 -echo
