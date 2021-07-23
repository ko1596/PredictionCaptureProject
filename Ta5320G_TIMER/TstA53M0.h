

//	for 	Test		****************************************************

#ifndef _TEST_A53M0_H
#define _TEST_A53M0_H


//********************************************************************
//	define
/*
	root@imx8mmrom5721a1:~# cat /proc/bus/input/devices
	I: Bus=0019 Vendor=0000 Product=0000 Version=0000
	N: Name="30370000.snvs:snvs-powerkey"
	P: Phys=snvs-pwrkey/input0
	S: Sysfs=/devices/platform/30370000.snvs/30370000.snvs:snvs-powerkey/input/input0
	U: Uniq=
	H: Handlers=kbd event0 
	B: PROP=0
	B: EV=3
	B: KEY=10000000000000 0

	I: Bus=0019 Vendor=0001 Product=0001 Version=0100
	N: Name="gpio-keys"
	P: Phys=gpio-keys/input0
	S: Sysfs=/devices/platform/gpio-keys/input/input1
	U: Uniq=
	H: Handlers=kbd event1 
	B: PROP=0
	B: EV=3
	B: KEY=10000000000000 0

	root@imx8mmrom5721a1:~# ls -l /dev/input/
	total 0
	drwxr-xr-x 2 root root      80 May 27 03:20 by-path
	crw-rw---- 1 root input 13, 64 May 27 03:20 event0
	crw-rw---- 1 root input 13, 65 May 27 03:20 event1
*/
//#define INPUT_KEYBOARD 	"/dev/input/event1"


//********************************************************************
//	Global
extern 	pthread_t thread_Keyboard_input;

extern 	UINT32 	Bat_v;
extern 	UINT32 	Bat_i;



//********************************************************************
//  Function
void  Keyboard_input_thread( void );

void 	TstA53M0_test( void );

void 	radar_param_down ( void );
void 	radar_param_set ( void );

void 	Tst_JSON_Fun( void );


#endif    //_TEST_A53M0_H

