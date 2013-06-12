EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:arduino_shieldsNCL
LIBS:colorimeter_shield-cache
EELAYER 24 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date "8 jun 2013"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Label 650  2500 0    60   ~ 0
POT
Wire Wire Line
	1150 2500 650  2500
Wire Wire Line
	9600 4400 9600 4700
Wire Wire Line
	1150 2400 650  2400
Wire Wire Line
	5600 2850 6100 2850
Wire Wire Line
	9150 1400 9750 1400
Wire Wire Line
	3050 1500 3550 1500
Wire Wire Line
	7000 1550 7000 1800
Wire Wire Line
	3050 2200 3550 2200
Wire Wire Line
	1150 2200 650  2200
Wire Wire Line
	6050 1400 5500 1400
Wire Wire Line
	6050 1200 5500 1200
Wire Wire Line
	6700 4600 7200 4600
Wire Wire Line
	6700 4400 7200 4400
Wire Wire Line
	5900 4700 5400 4700
Wire Wire Line
	5900 4500 5400 4500
Wire Wire Line
	5900 4300 5400 4300
Wire Wire Line
	3050 2400 3550 2400
Wire Wire Line
	3050 1700 3550 1700
Wire Wire Line
	3050 1900 3550 1900
Wire Wire Line
	1150 2000 650  2000
Wire Wire Line
	1150 1900 650  1900
Wire Wire Line
	3050 2000 3550 2000
Wire Wire Line
	3050 1800 3550 1800
Wire Wire Line
	3050 2700 3550 2700
Wire Wire Line
	3050 2500 3550 2500
Wire Wire Line
	5900 4400 5400 4400
Wire Wire Line
	5900 4600 5400 4600
Wire Wire Line
	6700 4300 7200 4300
Wire Wire Line
	6700 4500 7200 4500
Wire Wire Line
	6700 4700 7200 4700
Wire Wire Line
	6050 1300 5500 1300
Wire Wire Line
	6050 1500 5500 1500
Wire Wire Line
	3050 2300 3550 2300
Wire Wire Line
	7000 1150 7000 900 
Wire Wire Line
	3050 1600 3550 1600
Wire Wire Line
	9150 1200 9750 1200
Wire Wire Line
	5600 2750 6100 2750
Wire Wire Line
	5600 2950 6100 2950
Wire Wire Line
	9150 4400 9150 4700
Wire Wire Line
	10000 4400 10000 4700
Text Label 10000 4700 0    60   ~ 0
5V
Text Label 9600 4700 0    60   ~ 0
VIN
$Comp
L PWR_FLAG #FLG01
U 1 1 4F04D294
P 10000 4400
F 0 "#FLG01" H 10000 4670 30  0001 C CNN
F 1 "PWR_FLAG" H 10000 4630 30  0000 C CNN
F 2 "" H 10000 4400 60  0001 C CNN
F 3 "" H 10000 4400 60  0001 C CNN
	1    10000 4400
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG02
U 1 1 4F04D28F
P 9600 4400
F 0 "#FLG02" H 9600 4670 30  0001 C CNN
F 1 "PWR_FLAG" H 9600 4630 30  0000 C CNN
F 2 "" H 9600 4400 60  0001 C CNN
F 3 "" H 9600 4400 60  0001 C CNN
	1    9600 4400
	1    0    0    -1  
$EndComp
Text Label 9150 4700 0    60   ~ 0
GND
$Comp
L GND #PWR03
U 1 1 4F04D249
P 9150 4700
F 0 "#PWR03" H 9150 4700 30  0001 C CNN
F 1 "GND" H 9150 4630 30  0001 C CNN
F 2 "" H 9150 4700 60  0001 C CNN
F 3 "" H 9150 4700 60  0001 C CNN
	1    9150 4700
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG04
U 1 1 4F04D23F
P 9150 4400
F 0 "#FLG04" H 9150 4670 30  0001 C CNN
F 1 "PWR_FLAG" H 9150 4630 30  0000 C CNN
F 2 "" H 9150 4400 60  0001 C CNN
F 3 "" H 9150 4400 60  0001 C CNN
	1    9150 4400
	1    0    0    -1  
$EndComp
Text Notes 5650 3350 0    60   ~ 0
display brightness
Text Notes 5350 3200 0    60   ~ 0
enclosure mount potentiometer
Text Label 5600 2950 0    60   ~ 0
GND
Text Label 5600 2750 0    60   ~ 0
5V
$Comp
L CONN_3 K1
U 1 1 4F04D008
P 6450 2850
F 0 "K1" V 6400 2850 50  0000 C CNN
F 1 "CONN_3" V 6500 2850 40  0000 C CNN
F 2 "" H 6450 2850 60  0001 C CNN
F 3 "" H 6450 2850 60  0001 C CNN
	1    6450 2850
	1    0    0    -1  
$EndComp
Text Notes 9100 1650 0    60   ~ 0
enclosure mount button
Text Label 9150 1400 0    60   ~ 0
GND
Text Label 9150 1200 0    60   ~ 0
BUTTON
$Comp
L CONN_2 P4
U 1 1 4F04AF56
P 10100 1300
F 0 "P4" V 10050 1300 40  0000 C CNN
F 1 "CONN_2" V 10150 1300 40  0000 C CNN
F 2 "" H 10100 1300 60  0001 C CNN
F 3 "" H 10100 1300 60  0001 C CNN
	1    10100 1300
	1    0    0    -1  
$EndComp
Text Label 3550 2200 2    60   ~ 0
S3
Text Label 3550 2300 2    60   ~ 0
S2
Text Label 3550 1500 2    60   ~ 0
STX
Text Label 3550 1600 2    60   ~ 0
SRX
Text Label 7000 900  3    60   ~ 0
VIN
Text Label 7000 1800 0    60   ~ 0
GND
$Comp
L GND #PWR05
U 1 1 4F04AA6D
P 7000 1800
F 0 "#PWR05" H 7000 1800 30  0001 C CNN
F 1 "GND" H 7000 1730 30  0001 C CNN
F 2 "" H 7000 1800 60  0001 C CNN
F 3 "" H 7000 1800 60  0001 C CNN
	1    7000 1800
	1    0    0    -1  
$EndComp
Text Label 650  2200 0    60   ~ 0
VIN
Text Notes 5650 4950 0    60   ~ 0
header to color sensor board
Text Notes 5450 1750 0    60   ~ 0
software serial (display)
Text Label 5500 1500 0    60   ~ 0
STX
Text Label 5500 1400 0    60   ~ 0
SRX
Text Label 5500 1300 0    60   ~ 0
GND
Text Label 5500 1200 0    60   ~ 0
VIN
$Comp
L CONN_4 P1
U 1 1 4F04A60F
P 6400 1350
F 0 "P1" V 6350 1350 50  0000 C CNN
F 1 "CONN_4" V 6450 1350 50  0000 C CNN
F 2 "" H 6400 1350 60  0001 C CNN
F 3 "" H 6400 1350 60  0001 C CNN
	1    6400 1350
	1    0    0    -1  
$EndComp
Text Label 7200 4300 2    60   ~ 0
SRED
Text Label 7200 4400 2    60   ~ 0
SGRN
Text Label 7200 4500 2    60   ~ 0
SBLU
Text Label 7200 4600 2    60   ~ 0
FREQ
Text Label 7200 4700 2    60   ~ 0
S3
Text Label 5400 4700 0    60   ~ 0
S2
Text Label 5400 4600 0    60   ~ 0
S1
Text Label 5400 4500 0    60   ~ 0
S0
Text Label 5400 4400 0    60   ~ 0
GND
Text Label 5400 4300 0    60   ~ 0
5V
NoConn ~ 1150 1700
NoConn ~ 1150 1800
NoConn ~ 1150 2100
NoConn ~ 1150 2600
NoConn ~ 1150 2700
NoConn ~ 1150 2800
NoConn ~ 1150 2900
NoConn ~ 3050 2900
NoConn ~ 3050 2800
NoConn ~ 3050 1400
NoConn ~ 3050 1300
Text Label 650  2000 0    60   ~ 0
GND
Text Label 650  1900 0    60   ~ 0
5V
Text Label 3550 1700 2    60   ~ 0
SRED
Text Label 3550 1800 2    60   ~ 0
SGRN
Text Label 3550 1900 2    60   ~ 0
SBLU
Text Label 3550 2000 2    60   ~ 0
FREQ
Text Label 3550 2400 2    60   ~ 0
S1
Text Label 3550 2500 2    60   ~ 0
S0
Text Label 3550 2700 2    60   ~ 0
BUTTON
$Comp
L ARDUINO_SHIELD SHIELD1
U 1 1 4F03B33E
P 2100 2100
F 0 "SHIELD1" H 1750 3050 60  0000 C CNN
F 1 "ARDUINO_SHIELD" H 2150 1150 60  0000 C CNN
F 2 "" H 2100 2100 60  0001 C CNN
F 3 "" H 2100 2100 60  0001 C CNN
	1    2100 2100
	1    0    0    -1  
$EndComp
$Comp
L CONN_5X2 P2
U 1 1 4F03AC98
P 6300 4500
F 0 "P2" H 6300 4800 60  0000 C CNN
F 1 "CONN_5X2" V 6300 4500 50  0000 C CNN
F 2 "" H 6300 4500 60  0001 C CNN
F 3 "" H 6300 4500 60  0001 C CNN
	1    6300 4500
	1    0    0    -1  
$EndComp
Text Label 5600 2850 0    60   ~ 0
POT
Wire Wire Line
	3050 2600 3550 2600
Text Label 3550 2600 2    60   ~ 0
IRED
Text Label 650  2400 0    60   ~ 0
IGRN
$Comp
L CONN_3 K2
U 1 1 51B8E61C
P 9950 2750
F 0 "K2" V 9900 2750 50  0000 C CNN
F 1 "CONN_3" V 10000 2750 40  0000 C CNN
F 2 "" H 9950 2750 60  0000 C CNN
F 3 "" H 9950 2750 60  0000 C CNN
	1    9950 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	9100 2850 9600 2850
Wire Wire Line
	9100 2750 9600 2750
Wire Wire Line
	9100 2650 9600 2650
Text Notes 9300 3100 0    60   ~ 0
indicator leds
Text Label 9100 2850 0    60   ~ 0
GND
Text Label 9100 2750 0    60   ~ 0
IGRN
Text Label 9100 2650 0    60   ~ 0
IRED
$Comp
L C C1
U 1 1 51B8E8C5
P 7000 1350
F 0 "C1" H 7050 1450 50  0000 L CNN
F 1 "C" H 7050 1250 50  0000 L CNN
F 2 "" H 7000 1350 60  0000 C CNN
F 3 "" H 7000 1350 60  0000 C CNN
	1    7000 1350
	1    0    0    -1  
$EndComp
$EndSCHEMATC
