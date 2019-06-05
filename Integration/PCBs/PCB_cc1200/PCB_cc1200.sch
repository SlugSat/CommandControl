EESchema Schematic File Version 4
LIBS:PCB_cc1200-cache
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
Wire Wire Line
	5550 4900 5550 3900
Wire Wire Line
	5650 4900 5650 3900
$Comp
L Connector:Conn_01x07_Male J?
U 1 1 5CE78548
P 5350 3700
F 0 "J?" V 5185 3678 50  0000 C CNN
F 1 "Conn_01x07_Male" V 5276 3678 50  0000 C CNN
F 2 "" H 5350 3700 50  0001 C CNN
F 3 "~" H 5350 3700 50  0001 C CNN
	1    5350 3700
	0    1    1    0   
$EndComp
Wire Wire Line
	5450 3900 5450 4400
Wire Wire Line
	5350 3900 5350 4400
Wire Wire Line
	5250 3900 5250 4400
Wire Wire Line
	5150 3900 5150 4400
Wire Wire Line
	5050 3900 5050 4400
$Comp
L Connector:Conn_01x05_Male J?
U 1 1 5CE79B72
P 5250 4600
F 0 "J?" V 5404 4312 50  0000 R CNN
F 1 "Conn_01x05_Male" V 5313 4312 50  0000 R CNN
F 2 "" H 5250 4600 50  0001 C CNN
F 3 "~" H 5250 4600 50  0001 C CNN
	1    5250 4600
	0    -1   -1   0   
$EndComp
Connection ~ 5050 4400
Wire Wire Line
	5050 4400 5050 4900
Connection ~ 5150 4400
Wire Wire Line
	5150 4400 5150 4900
Connection ~ 5250 4400
Wire Wire Line
	5250 4400 5250 4900
Connection ~ 5350 4400
Wire Wire Line
	5350 4400 5350 4900
Connection ~ 5450 4400
Wire Wire Line
	5450 4400 5450 4900
$Comp
L Connector:Conn_01x07_Male J?
U 1 1 5CE7B6B1
P 5350 5100
F 0 "J?" V 5277 5078 50  0000 C CNN
F 1 "Conn_01x07_Male" V 5186 5078 50  0000 C CNN
F 2 "" H 5350 5100 50  0001 C CNN
F 3 "~" H 5350 5100 50  0001 C CNN
	1    5350 5100
	0    -1   -1   0   
$EndComp
$Comp
L Connector:Conn_01x05_Male J?
U 1 1 5CE7CEE0
P 6800 3600
F 0 "J?" V 6862 3844 50  0000 L CNN
F 1 "Conn_01x05_Male" V 6953 3844 50  0000 L CNN
F 2 "" H 6800 3600 50  0001 C CNN
F 3 "~" H 6800 3600 50  0001 C CNN
	1    6800 3600
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x05_Male J?
U 1 1 5CE7E31D
P 6800 5100
F 0 "J?" V 6954 4812 50  0000 R CNN
F 1 "Conn_01x05_Male" V 6863 4812 50  0000 R CNN
F 2 "" H 6800 5100 50  0001 C CNN
F 3 "~" H 6800 5100 50  0001 C CNN
	1    6800 5100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6600 3800 6600 4900
Wire Wire Line
	6700 4900 6700 3800
Wire Wire Line
	6800 3800 6800 4900
Wire Wire Line
	6900 4900 6900 3800
Wire Wire Line
	7000 3800 7000 4900
Text Notes 5200 3400 0    50   ~ 0
CC1200\n
Text Notes 6550 3450 0    50   ~ 0
SCIENCE PAYLAOD\n
Text Notes 4600 4450 0    50   ~ 0
AD2 PINS\n
$EndSCHEMATC
