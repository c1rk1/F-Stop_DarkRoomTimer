# F-Stop_DarkRoomTimer
Simple to build, simple to use "F-Stop" DarkRoom Timer with develop timer. 

F- stop printing was originally made popular by the award winning printer Gene Nocon in 1987. 

Menu options:
*  En.-t. : Enlarger timer
*  dE.-t. : Develop timer
*  SEt.t. : Set time
*  F-St.  : f-stop set
*  Stri.  : Strip

En.-t. : Enlarger timer - set the desired value with the rotary encoder (the steps can be set in "F-St." menu) START/Pause with the "Pedal button". Sound will be played on lamp ON and lamp OFF. Default value after power up 10 sec. (you can change it in "SEt.t." menu).

dE.-t. : Develop timer - a simple count down timer in sec. from max 900 to 0 (default after power up 180sec/ 3min) Use rotary encoder to change the value, push long (1sec) to toggle between 1x/10x. Sound will play on begin and end. Start/pause with pedal button.

SEt.t. : Set time - of the enlarger timer start/begening value (1-10) default is 10 sec. Use the rotary encoder to change the value, short press to select and go back. This will have effect in "En.-t. : Enlarger timer" menu.

F-St.  : f-stop set - set the steps what you want to use (1/1, 1/2, 1/3, 1/6, 1/12 This will have effect in "En.-t. : Enlarger timer" and "Stri.  : Strip" menu

Stri.  : Strip - an easy way to create test strip. First set the estimated exprosure time in "En.-t. : Enlarger timer" menu then in the "Stri.  : Strip" menu select the count of the strips you want (3, 5, 7, 9). It will display ex. "S.-5" from Strip-5. To procced, long press the rotary encoder and the display will show blinking number 1. If you are ready press pedal/START button and exprosure will be made, continue while all strip is done - according to the selected count.
So for example 5. you will have 2 under exprosure and 2 over exprosure and in the midle the estimated value in  "En.-t. : Enlarger timer" menu. Ex: you estimated 10sec in  "En.-t. : Enlarger timer" menu and selected 1/2 F-Stops in "F-St.  : f-stop" menu and S.-5 (five strips). This will be:
1. strip = 5.0sec
2. strip = 7.1sec (+2.1sec to previous)
3. strip = 10.0sec - the midle (+2.9sec to previous)
4. strip = 14.1sec (+4.1sec to previous)
5. strip = 20.0sec (+5.9sec to previous)
After developing the paper (use dE.-t. : Develop timer) select the best exprosure time what you can set it in "En.-t. : Enlarger timer" menu.

In the schematics I used big 1" 7-segment RED display with the 74HC595 shift registers connected to arduino 5,6,7 pins useing the ShiftDisplay library from Github. You can use any 7segment 4 digit red display what is compatible with the ShiftDisplay library.
* Rotary encoder connet to pins 2,3,4 and GND (A,B, button)
* START/PAUSE pedal/button connect to pin 8 and GND - simple push button or pedal or both
* Lamp controll relay board connect to pin 9 and GND - simple 5v relay board to controll the enlarger
* Piezo buzzer connect to pin 10 and GND on the arduino board.

TO DO list:
1. expand dE.-t. : Develop timer to can select minutes with seconds or seconds only and add selectable tick sound for 10,20,30 sec to use for film developing and tick sound for flip inversion remainder.
