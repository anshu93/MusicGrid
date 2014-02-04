# MusicGrid
=========

Code for Arduino powered LED grid equalizer.
Project and code based off "http://www.instructables.com/id/Yet-Another-Daft-Punk-Coffee-TableDisplay/" by lincomatic.
The grid was a 5x5 LED matrix embedded in a plank of wood and powered by an Arduino. The sound was filtered by frequency by a cascaded low pass filter and hence responded mostly to bass as intended.

# Videos

I unfortunately did not take decent quality videos before dismantling the project for the summer except the ones below, but they do the job:

1. http://www.youtube.com/watch?v=xHWVR887L0U
2. http://www.youtube.com/watch?v=IcQo9Z91nZw

## Technical Details

The restrictions on the board were caused by the arduino as it can only output enough current to light up two LEDs at a time. Thus it needed to cycle through a given frame at a speed faster than the persistence of vision to give the illusion of a fully lit board. 
Additionally, this could not be done with the typical digitalwrite or analogwrite commands as these were translated into too many machine instructions to achieve the desired speed. Thus individual registers had to be manipulated so as to achieve the intended illusion.
