# PM-Board

## Embedded development board with ATmega324P

<img src="https://github.com/dantudose/PM-Board/blob/main/Images/PM_board.png" height="300"/>

This is a development board supporting the ATmega324P microcontroller. It is fully Arduino-compatible and it comes with the following features:
* LSM9DS0 9-axis inertial sensor (accelerometer, gyroscope and magnetometer)
* MPL3115 baro-altimeter sensor
* Ambient temperature and light sensors
* 128x160 ST7735 color LCD display with SD card reader 
* RGB LED
* 16x02 character LCD display
* GPIO LEDs and pushbuttons
* JTAG and ISP connectors
* USB programming and data interface

<div id="cover">
  <img src="https://github.com/dantudose/PM-Board/blob/main/Images/PM_board_top.png" height="250"/>
  <img src="https://github.com/dantudose/PM-Board/blob/main/Images/PM_board_bottom.png" height="250"/> 
</div>



## Repository Contents

* **/Hardware** - Eagle design files (.brd, .sch)
* **/Images** - Board images
* **/bin** - Binaries (board bootloader)

## Programming

### avr-gcc & Makefile

#### Bootloader
The board comes pre-loaded with a bootloader. If you need to re-write it, connect an AVR-ISP or JTAG to the corresponding port and burn the <a href="https://github.com/dantudose/PM-Board/tree/main/bin">bootloader</a> .hex file with _atmega324p_ as a target and the following fuse settings:

<table>
<thead>
  <tr>
    <th>Fuse</th>
    <th>Value</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td>HIGH</td>
    <td>0xDA</td>
  </tr>
  <tr>
    <td>LOW</td>
    <td>0xD7</td>
  </tr>
  <tr>
    <td>EXT</td>
    <td>0XFD</td>
  </tr>
</tbody>
</table>

#### Writing a simple Hello World

Install avr-gcc on your machine. There are various tutorials on how to do this. Here are some examples, depending on your OS type:
* <a href="https://tinusaur.com/guides/avr-gcc-toolchain/">Windows</a>
* <a href="https://baremetalmicro.com/tutorial_avr_toolchain/03-Install-on-Ubuntu-Linux.html">Linux</a>
* <a href="https://wellys.com/posts/avr_c_macos/">Mac</a>

Copy this code into your favourite code editor and save the file as "hello.c":

```cpp
#include <avr/io.h>
#include <util/delay.h>
 
int main() {
	/* Setting PC0 as output */
	DDRC |= (1 << PC0);
 
	while(1) {
                /* Toggle pin state */
		PORTC ^= (1 << PC0);
 
                _delay_ms(500);
	}
 
	return 0;
}

```
To build and upload, you can use the following Makefile. Edit the port to correspond with your OS and serial port name:

```Makefile
# Linux
PORT ?= /dev/ttyUSB0
# Windows
# PORT ?= COM1
 
all: hello.hex
 
hello.hex: hello.elf
	avr-objcopy  -j .text -j .data -O ihex $^ $@
	avr-size hello.elf
 
hello.elf: hello.c
	avr-g++ -mmcu=atmega324p -DF_CPU=12000000 -Os -Wall -o $@ $^
 
upload: hello.hex
	avrdude -c urclock -P $(PORT) -b 57600 -p atmega324p -D -xnometadata -U flash:w:$<:a
 
clean:
	rm -rf hello.elf hello.hex
```

From your command line call _> make all_ (generates hello.hex) and then _> make upload_.

### Arduino

The board can be programmed using <a href="https://github.com/MCUdude/MightyCore">Mighty-Core</a>. 

Follow the "How to install" guide on their github page and use the following settings when burning the bootloader and uploading a sketch:

<img src="https://github.com/dantudose/PM-Board/blob/main/Images/mightyCore.png" />

## Board Pinout

#### LCD Displays

<table>
<thead>
  <tr>
    <th>Character LCD</th>
    <th>GPIO Pin</th>
    <th>Arduino Pin</th>
    <th></th>
    <th>Graphical LCD</th>
    <th>GPIO Pin</th>
    <th>Arduino Pin</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td>RS</td>
    <td>PC0</td>
    <td>16</td>
    <td></td>
    <td>CS</td>
    <td>PB0</td>
    <td>0</td>
  </tr>
  <tr>
    <td>RW</td>
    <td>PC1</td>
    <td>17</td>
    <td></td>
    <td>RST</td>
    <td>PB1</td>
    <td>1</td>
  </tr>
  <tr>
    <td>EN</td>
    <td>PC2</td>
    <td>18</td>
    <td></td>
    <td>A0</td>
    <td>PB4</td>
    <td>4</td>
  </tr>
  <tr>
    <td>D4</td>
    <td>PC3</td>
    <td>19</td>
    <td></td>
    <td>SDA</td>
    <td>PB5</td>
    <td>5</td>
  </tr>
  <tr>
    <td>D5</td>
    <td>PC4</td>
    <td>20</td>
    <td></td>
    <td>SCK</td>
    <td>PB7</td>
    <td>7</td>
  </tr>
  <tr>
    <td>D6</td>
    <td>PC5</td>
    <td>21</td>
    <td></td>
    <td>Backlight</td>
    <td>PA7</td>
    <td>31</td>
  </tr>
  <tr>
    <td>D7</td>
    <td>PC6</td>
    <td>22</td>
    <td></td>
    <td></td>
    <td></td>
    <td></td>
  </tr>
  <tr>
    <td>Backlight</td>
    <td>PC7</td>
    <td>23</td>
    <td></td>
    <td></td>
    <td></td>
    <td></td>
  </tr>
</tbody>
</table>


#### I2C Sensors & Bus Addresses

<table style="float: left; width: 584.312px;">
<tbody>
<tr>
<td style="width: 76px;">&nbsp;<strong>I2C</strong></td>
<td style="width: 103px;"><strong>&nbsp;GPIO Pin</strong>&nbsp;</td>
<td style="width: 94px;"><strong>&nbsp;Arduino Pin</strong>&nbsp;</td>
<td style="width: 35px;">&nbsp;</td>
<td style="width: 98px;"><strong>&nbsp;Chip</strong>&nbsp;</td>
<td style="width: 162.312px;"><strong>&nbsp;Address</strong>&nbsp;</td>
</tr>
<tr>
<td style="width: 76px;">&nbsp;I2C Enable&nbsp;</td>
<td style="width: 103px;">&nbsp;PA6</td>
<td style="width: 94px;">&nbsp;30</td>
<td style="width: 35px;">&nbsp;</td>
<td style="width: 98px;">&nbsp;LSM9DS0&nbsp;</td>
<td style="width: 162.312px;">&nbsp;0x1D (XM) 0x6B (G)</td>
</tr>
<tr>
<td style="width: 76px;">&nbsp;SCL&nbsp;</td>
<td style="width: 103px;">&nbsp;PC0&nbsp;</td>
<td style="width: 94px;">&nbsp;16</td>
<td style="width: 35px;">&nbsp;</td>
<td style="width: 98px;">&nbsp;MPL3115A2&nbsp;</td>
<td style="width: 162.312px;">&nbsp;0x60&nbsp;</td>
</tr>
<tr>
<td style="width: 76px;">&nbsp;SDA&nbsp;</td>
<td style="width: 103px;">&nbsp;PC1&nbsp;</td>
<td style="width: 94px;">&nbsp;17&nbsp;</td>
<td style="width: 35px;">&nbsp;</td>
<td style="width: 98px;">&nbsp;</td>
<td style="width: 162.312px;">&nbsp;</td>
</tr>
</tbody>
</table>
<!-- DivTable.com -->

#### SD Card

<table style="float: left;">
<tbody>
<tr>
<td>&nbsp;<b> </b>&nbsp;</td>
<td>&nbsp;<b>GPIO Pin</b>&nbsp;</td>
<td>&nbsp;<b>Arduino Pin</b>&nbsp;</td>
</tr>
<tr>
<td>&nbsp;CS</td>
<td>&nbsp;PA2</td>
<td>&nbsp;26</td>
</tr>
<tr>
<td>&nbsp;MOSI&nbsp;</td>
<td>&nbsp;PB5&nbsp;</td>
<td>&nbsp;5&nbsp;</td>
</tr>
<tr>
<td>&nbsp;MISO&nbsp;</td>
<td>&nbsp;PB6&nbsp;</td>
<td>&nbsp;6&nbsp;</td>
</tr>
<tr>
<td>&nbsp;SCK&nbsp;</td>
<td>&nbsp;PB7&nbsp;</td>
<td>&nbsp;7&nbsp;</td>
</tr>
</tbody>
</table>
<!-- DivTable.com -->

#### RGB LED

<table class="tg">
<thead>
  <tr>
    <th class="tg-amwm"> </th>
    <th class="tg-amwm">GPIO Pin</th>
    <th class="tg-amwm">Arduino Pin</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td class="tg-hmp3">Red</td>
    <td class="tg-hmp3">PD5</td>
    <td class="tg-hmp3">13</td>
  </tr>
  <tr>
    <td class="tg-0lax">Green</td>
    <td class="tg-0lax">PD7</td>
    <td class="tg-0lax">15</td>
  </tr>
  <tr>
    <td class="tg-hmp3">Blue</td>
    <td class="tg-hmp3">PB3</td>
    <td class="tg-hmp3">3</td>
  </tr>
</tbody>
</table>

### Miscellaneous

<table class="tg">
<thead>
  <tr>
    <th class="tg-0lax"></th>
    <th class="tg-amwm">GPIO Pin</th>
    <th class="tg-amwm">Arduino Pin</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td class="tg-0lax">Temperature Sensor</td>
    <td class="tg-0lax">PA0</td>
    <td class="tg-0lax">A0</td>
  </tr>
  <tr>
    <td class="tg-0lax">Ambient Light Sensor</td>
    <td class="tg-0lax">PA1</td>
    <td class="tg-0lax">A1</td>
  </tr>
  <tr>
    <td class="tg-0lax">Buttons (BTN1 - 6)</td>
    <td class="tg-0lax">PA5</td>
    <td class="tg-0lax">A5</td>
  </tr>
  <tr>
    <td class="tg-0lax">BTN 1</td>
    <td class="tg-0lax">PB2</td>
    <td class="tg-0lax">2</td>
  </tr>
  <tr>
    <td class="tg-0lax">BTN 2</td>
    <td class="tg-0lax">PD6</td>
    <td class="tg-0lax">14</td>
  </tr>
  <tr>
    <td class="tg-0lax">Speaker</td>
    <td class="tg-0lax">PD4</td>
    <td class="tg-0lax">12</td>
  </tr>
</tbody>
</table>

## License Information

This product is _**open source**_! 

Please review the LICENSE file for license information. 

Distributed as-is; no warranty is given.
