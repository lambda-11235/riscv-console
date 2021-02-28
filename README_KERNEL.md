
## Demo

To run the demo application do the following steps.

1. Build and run the kernel found in `./kernel/`.

2. Build and insert the program found in `./program/`.

3. Press the left button to initiate a graphics demo.

4. The graphics demo will show a block bouncing around the screen.

5. Press the right button to return to the kernel. The return value
   will be the system time.
   
6. The user program uses syscalls for getting the current time,
   getting input, drawing text to screen, managing the graphics, and
   registering user upcalls.
   
   
At this point threads are the only planned feature that are
unimplemented. I plan on finishing them in phase 4.
