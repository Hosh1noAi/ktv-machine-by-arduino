0. because the limit in size, we mit the mp3 file of songs.
1.project overview
This project employs three UNO boards, interconnected through the I2C communication protocol. One board serves as the master, while the remaining two function as slaves. The master board controls the SV5W module to play music, utilizing SV5W's built-in commands for playing, pausing, skipping to the next or previous song, increasing or decreasing the volume, and selecting a specific song. The master UNO's six pins are externally connected to six buttons, each controlling one of these functions.

The first slave board controls an LCD1602 display screen to show lyrics and read the lyrics stored in the SD card. By reading the SV5W's play status variable from the master, it determines if the system is in a pause state. If so, an interrupt function triggers the LCD1602 to display "Song Pause". It queries the current track being played using SV5W's built-in command, and then sends this information to the first slave via I2C communication. The first slave then reads the corresponding lyrics from the SD card and displays them on the LCD1602. As most lyrics are quite long, they are displayed in a scrolling fashion on the LCD1602.

The second slave controls an OLED display and a sound sensor (KV-037). The sound feedback (which will be modified in the later stages) involves the sound sensor reading the volume decibel levels of the singer and displaying them on the OLED. Once the volume surpasses a certain threshold, the OLED displays "low voice". The OLED also shows whether the song is old or new (old/new) and the song's title, both of which are obtained from the master via the I2C protocol.

The ISD1820 can provide recording and playback functions, and only requires a power supply.

The lyrics in the SD card should begin with a time prefix, which is used to calculate the timing of the current lyrics displayed on the LCD1602.


2.Folder contents
The lib folder contains additional libraries that need to be downloaded. You can download them directly from Arduino or download the library files in lib.

The song folder contains MP3s of 20 songs.

The lyrics folder contains lyrics for 20 songs, stored in lrc format.

The source code folder is where the source code is stored, and under it there are three subfolders for the master, slave1, and slave2 source code to run with the Arduino IDE.


3.Conditions precedent
The Arduino IDE is needed to run this program, which can be downloaded from the Arduion official website. The materials and wiring instructions required by the project are detailed in the Appendix.


4.Execution notes
It needs to be compiled and uploaded to three uno boards respectively before functional operation can be carried out. slave1 and salve2 uno boards cannot run separately without master, and master can run separately.


5.Troubleshooting
If there is no response after the function button is pressed, there should be a time interval between the two function button presses.

If the lyrics are not displayed, there is a problem reading the SD card, please re-upload the code of slave1.