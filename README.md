Tilley-Twinkle
==============

The Tilley Twinkle was a digital Christmas card sent to some of the Tiree community. This code cycles through the data of 23rd Dec 2012, and changes the colour of bi-colour LEDs accordingly. The Tilley Twinkle hardware consists of an Arduino Uno connected to a number of bi-colour LEDs in parallel, which where distributed throughout a card manifold. These LEDS are controled via PWM, and will change from red to green depending on the currently value.

Overall, the code executes in about 144 seconds, and then resets back to the beginning.

Requires
========

* Timer1 library
* Tilley Twinkle hardware
