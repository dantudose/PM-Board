# PM-Board

Embedded development board with ATmega324P
------------------------------------------

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



Repository Contents
-------------------

* **/Hardware** - Eagle design files (.brd, .sch)
* **/Images** - Board images

Board Pinout
------------

<table style="float: left;" border="-">
<tbody>
<tr>
<td style="text-align: left;">&nbsp;<b>16x2 LCD</b>&nbsp;</td>
<td style="text-align: left;">&nbsp;<b>GPIO Pin</b>&nbsp;</td>
<td style="text-align: left;">&nbsp;<b>Arduino Pin</b>&nbsp;</td>
</tr>
<tr>
<td>RS</td>
<td>&nbsp;PC0</td>
<td>&nbsp;16</td>
</tr>
<tr>
<td>RW&nbsp;</td>
<td>&nbsp;PC1&nbsp;</td>
<td>&nbsp;17</td>
</tr>
<tr>
<td>En&nbsp;</td>
<td>&nbsp;PC2&nbsp;</td>
<td>&nbsp;18&nbsp;</td>
</tr>
<tr>
<td>D4&nbsp;</td>
<td>&nbsp;PC3&nbsp;</td>
<td>&nbsp;19&nbsp;</td>
</tr>
<tr>
<td>D5&nbsp;</td>
<td>&nbsp;PC4&nbsp;</td>
<td>&nbsp;20&nbsp;</td>
</tr>
<tr>
<td>D6&nbsp;</td>
<td>&nbsp;PC5&nbsp;</td>
<td>&nbsp;21&nbsp;</td>
</tr>
<tr>
<td>D7&nbsp;</td>
<td>&nbsp;PC6&nbsp;</td>
<td>&nbsp;22&nbsp;</td>
</tr>
<tr>
<td>Backlight&nbsp;</td>
<td>&nbsp;PC7&nbsp;</td>
<td>&nbsp;23&nbsp;</td>
</tr>
</tbody>
</table>


<table style="float: left;" border="|">
<tbody>
<tr>
<td>&nbsp;<b>Graphical LCD</b>&nbsp;</td>
<td>&nbsp;<b>GPIO Pin</b>&nbsp;</td>
<td>&nbsp;<b>Arduino Pin</b>&nbsp;</td>
</tr>
<tr>
<td>&nbsp;CS</td>
<td>&nbsp;PB0</td>
<td>&nbsp;0</td>
</tr>
<tr>
<td>&nbsp;RST&nbsp;</td>
<td>&nbsp;PB1&nbsp;</td>
<td>&nbsp;1&nbsp;</td>
</tr>
<tr>
<td>&nbsp;A0&nbsp;</td>
<td>&nbsp;PB4&nbsp;</td>
<td>&nbsp;4&nbsp;</td>
</tr>
<tr>
<td>&nbsp;SDA&nbsp;</td>
<td>&nbsp;PB5&nbsp;</td>
<td>&nbsp;5&nbsp;</td>
</tr>
<tr>
<td>&nbsp;SCK&nbsp;</td>
<td>&nbsp;PB7&nbsp;</td>
<td>&nbsp;7&nbsp;</td>
</tr>
<tr>
<td>&nbsp;Backlight&nbsp;</td>
<td>&nbsp;PA7&nbsp;</td>
<td>&nbsp;31&nbsp;</td>
</tr>
</tbody>
</table>



License Information
-------------------

This product is _**open source**_! 

Please review the LICENSE file for license information. 

Distributed as-is; no warranty is given.
