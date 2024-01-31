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

## Board Pinout

### LCD Displays

<table>
<tbody>
<tr>
<td>
  <table style="float: left;">
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
</td>
<td>
  <table style="float: left;">
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

</td>
</tr>
</tbody>
</table>




### I2C Sensors & Bus Addresses

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

### SD Card

<table style="float: left;">
<tbody>
<tr>
<td>&nbsp;<b>SD Card</b>&nbsp;</td>
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

### RGB LED

<table class="tg">
<thead>
  <tr>
    <th class="tg-amwm">RGB LED</th>
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
    <td class="tg-0lax">Buttons (BTN1-6)</td>
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
