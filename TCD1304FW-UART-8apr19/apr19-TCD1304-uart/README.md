The TCD1304-driver firmware for STM32F401re.

Documentation can be found at:
https://tcd1304.wordpress.com/


The firmware outputs driving-pulses on the following GPIOs:
	fM: PB1, SH: PB10, ICG: PA1
	CCD-output: PC3


To interface with firmware send a 12-byte string over UART with the following format:
Format of the pc's txbuffer:
	txbuffer[0-1] = 'ER'
	txbuffer[2-5] = SH (20 - 2³²)
	txbuffer[6-9] = ICG (14776 - 2³²)
	txbuffer[10] = Ready_flag (not used anymore)
	txbuffer[11] = Integrations_to_avg (1 - 15)
Look in the source code for details.


Licenses:
	Everything, compiled or in source code, is under the FreeBSD-license.
	To cite the project write:
		Esben Rossel, (2017). The linear CCD-module [Firmware]
