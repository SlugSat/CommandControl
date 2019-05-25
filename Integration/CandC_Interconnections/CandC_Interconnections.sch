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
L Connector:Conn_01x03_Female J1
U 1 1 5CE71BBA
P 4300 3650
F 0 "J1" H 4328 3676 50  0000 L CNN
F 1 "Conn_01x03_Female" H 4328 3585 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 4300 3650 50  0001 C CNN
F 3 "~" H 4300 3650 50  0001 C CNN
	1    4300 3650
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x03_Female J2
U 1 1 5CE728AA
P 5850 2750
F 0 "J2" V 5696 2898 50  0000 L CNN
F 1 "Conn_01x03_Female" V 5787 2898 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 5850 2750 50  0001 C CNN
F 3 "~" H 5850 2750 50  0001 C CNN
	1    5850 2750
	0    -1   -1   0   
$EndComp
$Comp
L Connector:Conn_01x03_Female J3
U 1 1 5CE730F9
P 7800 3650
F 0 "J3" H 7692 3325 50  0000 C CNN
F 1 "Conn_01x03_Female" H 7692 3416 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 7800 3650 50  0001 C CNN
F 3 "~" H 7800 3650 50  0001 C CNN
	1    7800 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 3650 5750 3650
Wire Wire Line
	4500 3750 5950 3750
Wire Wire Line
	5750 2950 5750 3650
Connection ~ 5750 3650
Wire Wire Line
	5750 3650 7600 3650
Wire Wire Line
	4500 3550 5850 3550
Wire Wire Line
	5850 2950 5850 3550
Connection ~ 5850 3550
Wire Wire Line
	5850 3550 7600 3550
Wire Wire Line
	5950 2950 5950 3750
Connection ~ 5950 3750
Wire Wire Line
	5950 3750 7600 3750
$EndSCHEMATC
