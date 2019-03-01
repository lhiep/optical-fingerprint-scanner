# Mid Term Project - Optical Fingerprint Scanner
This project explores the capability of a fingerprint scanner and its interface with an Arduino Uno.

Click [here](https://github.com/lhiep/optical-fingerprint-scanner/wiki) to visit the wiki!

The example below demonstrates the majority of the functions of the fingerprint scanner. For simplicity, this example only uses the fingerprint scanner, an arduino, and LED paired with a 300 Ohm resistor, some jumper wires, and a breadboard. Inputs and outputs of this example will be communicated through the serial terminal provided by the Arduino IDE. The example sketch can be found above.

This sketch will run in an infinite loop, giving the user 4 options and continuously waiting for an input. The options are:  
[1] Enrolling a new fingerprint  
[2] Delete a fingerprint  
[3] View the number of fingerprints in the database  
[4] Lock the device

When enrolling a new fingerprint, the system will first ask for an index at which the fingerprint will be stored at. The range of this index is from 1 to 127. The system will then ask the user to put their finger on the scanner and remove after the scanner has finished reading, twice. If both images match, the fingerprint will be stored. Otherwise, the user will need to rescan their fingerprints.

When deleting a fingerprint, the system will ask the user for the index at which the fingerprint will be deleted. An error message will occur if the entered index is out of bound or invalid. If the index is withing range (1-127) and valid, the fingerprint at that index will be deleted.

Viewing the number of fingerprints in the database is exactly how it sounds. The system will loop through every slot of memory and count how many fingerprints are stored.

The "Lock the device" option simulates a real life situation of using the fingerprint scanner to lock something. In this case, the lock is simulated by an LED on pin 5. When the device is locked, the LED will be on and the fingerprint scanner will continuously scanning new fingerprints. If any fingerprint matches with existing fingerprints in the database, the system will be unlocked and the LED will be turned on. The whole system will then reset to the initial state, displaying the 4 options listed above again.

A demo can be view here.
