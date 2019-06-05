EESchema Schematic File Version 4
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector:Conn_01x09_Male J?
U 1 1 5CE7894A
P 2400 4150
F 0 "J?" H 2350 4750 50  0000 C CNN
F 1 "Conn_01x09_Male" H 2300 4650 50  0000 C CNN
F 2 "" H 2400 4150 50  0001 C CNN
F 3 "~" H 2400 4150 50  0001 C CNN
	1    2400 4150
	1    0    0    -1  
$EndComp
Text Notes 2150 3800 0    50   ~ 0
SDA\n
Text Notes 2150 3900 0    50   ~ 0
SCL\n
Text Notes 2000 4000 0    50   ~ 0
SCIENCE
Text Notes 2100 4100 0    50   ~ 0
MECH
Text Notes 2050 4200 0    50   ~ 0
POWER\n
Text Notes 2200 4300 0    50   ~ 0
LT
Text Notes 2100 4400 0    50   ~ 0
TELE
Text Notes 2150 4500 0    50   ~ 0
KILL\n
$Comp
L power:GND #PWR?
U 1 1 5CE797D8
P 2600 4550
F 0 "#PWR?" H 2600 4300 50  0001 C CNN
F 1 "GND" H 2605 4377 50  0000 C CNN
F 2 "" H 2600 4550 50  0001 C CNN
F 3 "" H 2600 4550 50  0001 C CNN
	1    2600 4550
	1    0    0    -1  
$EndComp
Text Notes 2150 4600 0    50   ~ 0
GND
$Comp
L Connector:Conn_01x12_Male J?
U 1 1 5CE7A67B
P 3750 3300
F 0 "J?" V 3585 3228 50  0000 C CNN
F 1 "Conn_01x12_Male" V 3676 3228 50  0000 C CNN
F 2 "" H 3750 3300 50  0001 C CNN
F 3 "~" H 3750 3300 50  0001 C CNN
	1    3750 3300
	0    1    1    0   
$EndComp
Wire Wire Line
	2600 3750 3150 3750
Wire Wire Line
	3150 3750 3150 3500
Wire Wire Line
	3150 3750 3350 3750
Wire Wire Line
	3350 3750 3350 3500
Connection ~ 3150 3750
Wire Wire Line
	3350 3750 3550 3750
Wire Wire Line
	3550 3750 3550 3500
Connection ~ 3350 3750
Wire Wire Line
	3550 3750 3750 3750
Wire Wire Line
	3750 3750 3750 3500
Connection ~ 3550 3750
Wire Wire Line
	3750 3750 3950 3750
Wire Wire Line
	3950 3750 3950 3500
Connection ~ 3750 3750
Wire Wire Line
	3950 3750 4150 3750
Wire Wire Line
	4150 3750 4150 3500
Connection ~ 3950 3750
Wire Wire Line
	2600 3850 3250 3850
Wire Wire Line
	3250 3850 3250 3500
Wire Wire Line
	3250 3850 3450 3850
Wire Wire Line
	3450 3850 3450 3500
Connection ~ 3250 3850
Wire Wire Line
	3450 3850 3650 3850
Wire Wire Line
	3650 3850 3650 3500
Connection ~ 3450 3850
Wire Wire Line
	3650 3850 3850 3850
Wire Wire Line
	3850 3850 3850 3500
Connection ~ 3650 3850
Wire Wire Line
	3850 3850 4050 3850
Wire Wire Line
	4050 3850 4050 3500
Connection ~ 3850 3850
Wire Wire Line
	4050 3850 4250 3850
Wire Wire Line
	4250 3850 4250 3500
Connection ~ 4050 3850
$Comp
L Connector:Conn_01x09_Male J?
U 1 1 5CE7E07D
P 4850 4150
F 0 "J?" H 4822 4082 50  0000 R CNN
F 1 "Conn_01x09_Male" H 4822 4173 50  0000 R CNN
F 2 "" H 4850 4150 50  0001 C CNN
F 3 "~" H 4850 4150 50  0001 C CNN
	1    4850 4150
	-1   0    0    1   
$EndComp
Wire Wire Line
	4650 3750 4150 3750
Connection ~ 4150 3750
Wire Wire Line
	4650 3850 4250 3850
Connection ~ 4250 3850
Wire Wire Line
	2600 3950 4650 3950
Wire Wire Line
	2600 4050 4650 4050
Wire Wire Line
	2600 4150 4650 4150
Wire Wire Line
	2600 4250 4650 4250
Wire Wire Line
	2600 4350 4650 4350
Wire Wire Line
	2600 4450 4650 4450
$Comp
L power:GND #PWR?
U 1 1 5CE84C68
P 4650 4550
F 0 "#PWR?" H 4650 4300 50  0001 C CNN
F 1 "GND" H 4655 4377 50  0000 C CNN
F 2 "" H 4650 4550 50  0001 C CNN
F 3 "" H 4650 4550 50  0001 C CNN
	1    4650 4550
	1    0    0    -1  
$EndComp
Text Notes 4900 3750 0    50   ~ 0
AD2SDA\n
Text Notes 4900 3850 0    50   ~ 0
AD2SCL
$EndSCHEMATC
