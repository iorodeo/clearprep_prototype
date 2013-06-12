EESchema Schematic File Version 2  date Sat 08 Jun 2013 03:27:31 PM PDT
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
LIBS:tcs3200
LIBS:colorimeter_shield-cache
EELAYER 27 0
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
Text Label 2650 2300 0    60   ~ 0
POT2
Wire Wire Line
	3150 2300 2650 2300
Wire Wire Line
	1150 750  1150 1050
Wire Wire Line
	3150 2200 2650 2200
Wire Wire Line
	3700 3950 4200 3950
Wire Wire Line
	2850 5700 3100 5700
Wire Wire Line
	7700 4450 8300 4450
Wire Wire Line
	5050 1300 5550 1300
Wire Wire Line
	8800 3300 8800 3550
Wire Wire Line
	5050 2000 5550 2000
Wire Wire Line
	3150 2000 2650 2000
Wire Wire Line
	7850 3150 7300 3150
Wire Wire Line
	7850 2950 7300 2950
Wire Wire Line
	8550 1750 9050 1750
Wire Wire Line
	8550 1550 9050 1550
Wire Wire Line
	7750 1850 7250 1850
Wire Wire Line
	7750 1650 7250 1650
Wire Wire Line
	7750 1450 7250 1450
Wire Wire Line
	5050 2200 5550 2200
Wire Wire Line
	5050 1500 5550 1500
Wire Wire Line
	5050 1700 5550 1700
Wire Wire Line
	3150 1800 2650 1800
Wire Wire Line
	3150 1700 2650 1700
Wire Wire Line
	5050 1800 5550 1800
Wire Wire Line
	5050 1600 5550 1600
Wire Wire Line
	5050 2500 5550 2500
Wire Wire Line
	5050 2300 5550 2300
Wire Wire Line
	7750 1550 7250 1550
Wire Wire Line
	7750 1750 7250 1750
Wire Wire Line
	8550 1450 9050 1450
Wire Wire Line
	8550 1650 9050 1650
Wire Wire Line
	8550 1850 9050 1850
Wire Wire Line
	7850 3050 7300 3050
Wire Wire Line
	7850 3250 7300 3250
Wire Wire Line
	5050 2100 5550 2100
Wire Wire Line
	8800 2900 8800 2650
Wire Wire Line
	5050 1400 5550 1400
Wire Wire Line
	7700 4250 8300 4250
Wire Wire Line
	2700 5450 2700 5100
Wire Wire Line
	2700 5950 2700 6200
Wire Wire Line
	3700 3850 4200 3850
Wire Wire Line
	3700 4050 4200 4050
Wire Wire Line
	700  750  700  1050
Wire Wire Line
	1550 750  1550 1050
Text Label 1550 1050 0    60   ~ 0
5V
Text Label 1150 1050 0    60   ~ 0
VIN
$Comp
L PWR_FLAG #FLG01
U 1 1 4F04D294
P 1550 750
F 0 "#FLG01" H 1550 1020 30  0001 C CNN
F 1 "PWR_FLAG" H 1550 980 30  0000 C CNN
F 2 "" H 1550 750 60  0001 C CNN
F 3 "" H 1550 750 60  0001 C CNN
	1    1550 750 
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG02
U 1 1 4F04D28F
P 1150 750
F 0 "#FLG02" H 1150 1020 30  0001 C CNN
F 1 "PWR_FLAG" H 1150 980 30  0000 C CNN
F 2 "" H 1150 750 60  0001 C CNN
F 3 "" H 1150 750 60  0001 C CNN
	1    1150 750 
	1    0    0    -1  
$EndComp
Text Label 700  1050 0    60   ~ 0
GND
$Comp
L GND #PWR03
U 1 1 4F04D249
P 700 1050
F 0 "#PWR03" H 700 1050 30  0001 C CNN
F 1 "GND" H 700 980 30  0001 C CNN
F 2 "" H 700 1050 60  0001 C CNN
F 3 "" H 700 1050 60  0001 C CNN
	1    700  1050
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG04
U 1 1 4F04D23F
P 700 750
F 0 "#FLG04" H 700 1020 30  0001 C CNN
F 1 "PWR_FLAG" H 700 980 30  0000 C CNN
F 2 "" H 700 750 60  0001 C CNN
F 3 "" H 700 750 60  0001 C CNN
	1    700  750 
	1    0    0    -1  
$EndComp
Text Label 2650 2200 0    60   ~ 0
POT1
Text Notes 3750 4550 0    60   ~ 0
display brightness
Text Notes 3450 4400 0    60   ~ 0
enclosure mount potentiometer
Text Notes 2100 6450 0    60   ~ 0
throughhole potentiometer
Text Label 3700 4050 0    60   ~ 0
GND
Text Label 3700 3950 0    60   ~ 0
POT2
Text Label 3700 3850 0    60   ~ 0
5V
$Comp
L CONN_3 K1
U 1 1 4F04D008
P 4550 3950
F 0 "K1" V 4500 3950 50  0000 C CNN
F 1 "CONN_3" V 4600 3950 40  0000 C CNN
F 2 "" H 4550 3950 60  0001 C CNN
F 3 "" H 4550 3950 60  0001 C CNN
	1    4550 3950
	1    0    0    -1  
$EndComp
Text Label 3100 5700 2    60   ~ 0
POT1
Text Label 2700 5100 0    60   ~ 0
5V
Text Label 2700 6200 0    60   ~ 0
GND
$Comp
L GND #PWR05
U 1 1 4F04C4CB
P 2700 6200
F 0 "#PWR05" H 2700 6200 30  0001 C CNN
F 1 "GND" H 2700 6130 30  0001 C CNN
F 2 "" H 2700 6200 60  0001 C CNN
F 3 "" H 2700 6200 60  0001 C CNN
	1    2700 6200
	1    0    0    -1  
$EndComp
$Comp
L POT RV1
U 1 1 4F04C494
P 2700 5700
F 0 "RV1" H 2700 5600 50  0000 C CNN
F 1 "POT" H 2700 5700 50  0000 C CNN
F 2 "" H 2700 5700 60  0001 C CNN
F 3 "" H 2700 5700 60  0001 C CNN
	1    2700 5700
	0    1    1    0   
$EndComp
Text Notes 7650 4700 0    60   ~ 0
enclosure mount button
Text Label 7700 4450 0    60   ~ 0
GND
Text Label 7700 4250 0    60   ~ 0
BUTTON
$Comp
L CONN_2 P4
U 1 1 4F04AF56
P 8650 4350
F 0 "P4" V 8600 4350 40  0000 C CNN
F 1 "CONN_2" V 8700 4350 40  0000 C CNN
F 2 "" H 8650 4350 60  0001 C CNN
F 3 "" H 8650 4350 60  0001 C CNN
	1    8650 4350
	1    0    0    -1  
$EndComp
NoConn ~ 5050 2400
Text Label 5550 2000 2    60   ~ 0
S3
Text Label 5550 2100 2    60   ~ 0
S2
Text Label 5550 1300 2    60   ~ 0
STX
Text Label 5550 1400 2    60   ~ 0
SRX
Text Label 8800 2650 3    60   ~ 0
VIN
Text Label 8800 3550 0    60   ~ 0
GND
$Comp
L GND #PWR06
U 1 1 4F04AA6D
P 8800 3550
F 0 "#PWR06" H 8800 3550 30  0001 C CNN
F 1 "GND" H 8800 3480 30  0001 C CNN
F 2 "" H 8800 3550 60  0001 C CNN
F 3 "" H 8800 3550 60  0001 C CNN
	1    8800 3550
	1    0    0    -1  
$EndComp
$Comp
L CP1 C1
U 1 1 4F04AA4F
P 8800 3100
F 0 "C1" H 8850 3200 50  0000 L CNN
F 1 "CP1" H 8850 3000 50  0000 L CNN
F 2 "" H 8800 3100 60  0001 C CNN
F 3 "" H 8800 3100 60  0001 C CNN
	1    8800 3100
	1    0    0    -1  
$EndComp
Text Label 2650 2000 0    60   ~ 0
VIN
Text Notes 7550 1200 0    60   ~ 0
header to color sensor board
Text Notes 8100 2500 0    60   ~ 0
software serial (display)
Text Label 7300 3250 0    60   ~ 0
STX
Text Label 7300 3150 0    60   ~ 0
SRX
Text Label 7300 3050 0    60   ~ 0
GND
Text Label 7300 2950 0    60   ~ 0
VIN
$Comp
L CONN_4 P1
U 1 1 4F04A60F
P 8200 3100
F 0 "P1" V 8150 3100 50  0000 C CNN
F 1 "CONN_4" V 8250 3100 50  0000 C CNN
F 2 "" H 8200 3100 60  0001 C CNN
F 3 "" H 8200 3100 60  0001 C CNN
	1    8200 3100
	1    0    0    -1  
$EndComp
Text Label 9050 1450 2    60   ~ 0
RED
Text Label 9050 1550 2    60   ~ 0
GREEN
Text Label 9050 1650 2    60   ~ 0
BLUE
Text Label 9050 1750 2    60   ~ 0
FREQ
Text Label 9050 1850 2    60   ~ 0
S3
Text Label 7250 1850 0    60   ~ 0
S2
Text Label 7250 1750 0    60   ~ 0
S1
Text Label 7250 1650 0    60   ~ 0
S0
Text Label 7250 1550 0    60   ~ 0
GND
Text Label 7250 1450 0    60   ~ 0
5V
NoConn ~ 3150 1500
NoConn ~ 3150 1600
NoConn ~ 3150 1900
NoConn ~ 3150 2400
NoConn ~ 3150 2500
NoConn ~ 3150 2600
NoConn ~ 3150 2700
NoConn ~ 5050 2700
NoConn ~ 5050 2600
NoConn ~ 5050 1200
NoConn ~ 5050 1100
Text Label 2650 1800 0    60   ~ 0
GND
Text Label 2650 1700 0    60   ~ 0
5V
Text Label 5550 1500 2    60   ~ 0
RED
Text Label 5550 1600 2    60   ~ 0
GREEN
Text Label 5550 1700 2    60   ~ 0
BLUE
Text Label 5550 1800 2    60   ~ 0
FREQ
Text Label 5550 2200 2    60   ~ 0
S1
Text Label 5550 2300 2    60   ~ 0
S0
Text Label 5550 2500 2    60   ~ 0
BUTTON
$Comp
L ARDUINO_SHIELD SHIELD1
U 1 1 4F03B33E
P 4100 1900
F 0 "SHIELD1" H 3750 2850 60  0000 C CNN
F 1 "ARDUINO_SHIELD" H 4150 950 60  0000 C CNN
F 2 "" H 4100 1900 60  0001 C CNN
F 3 "" H 4100 1900 60  0001 C CNN
	1    4100 1900
	1    0    0    -1  
$EndComp
$Comp
L CONN_5X2 P2
U 1 1 4F03AC98
P 8150 1650
F 0 "P2" H 8150 1950 60  0000 C CNN
F 1 "CONN_5X2" V 8150 1650 50  0000 C CNN
F 2 "" H 8150 1650 60  0001 C CNN
F 3 "" H 8150 1650 60  0001 C CNN
	1    8150 1650
	1    0    0    -1  
$EndComp
$EndSCHEMATC
