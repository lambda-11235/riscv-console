
## Demo

The demo is a simple application that use multithreading, synchronous
variables, and I/O.

### Controls

- CMD: Switch between text and graphics mode.
- Arrow keys: Control large sprite location.

### Explanation

- The demo spawns four threads named A, B, C, and G.
  It also attaches to upcall handlers for video and CMD interrupts.

- A-C each have their own counters, shown on screen when in text mode.

- A and B increment their counters at regular, but different, intervals.

- C uses a condition variable to check when its counter is less than A's.
  When this occurs C set its counter to twice the value of A's.
  A will notify C when A's counter overtakes C's.
  
- G monitors user input and controls a large sprites location on screen.

- The video interrupt upcall handles displaying everything on screen.

- The CMD upcall changes video modes between text and graphics.


### Known issues

Currently there are two known kernel issues.

1. Thread join semantics are ill defined. Currently, it is impossible
   to join a thread that has already exited. While this technically
   doesn't go against the API spec, as the spec doesn't specify a
   behavior if the thread has exited, it is still suboptimal.

2. There is a bug with mutexs and congestion variables that can lead
   to a deadlock. It was fixed by adding a dummy variable to the mutex
   structure, which may indicate allocation issues. Didn't have time
   to debug.
