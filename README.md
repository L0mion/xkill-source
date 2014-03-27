xkill
========

### Backstory

In a forgotten store-room rests an ancient single-core processor. In this harsh landscape, before multicore-technology, fierce gladiator tournaments are fought in digital arenas by processes struggling to be granted processor execution time (cycles) by the scheduler. These tournaments consist of deadly combat as the processes battle to destroy one another. The incentive for battle is that only one process at a time can execute on the processor.

#### Basics

You are playing as one the processes inside a single-core processor. Priority is awarded to a process when it terminates another process. The goal of XKILL is to have as many cycles as possible.

Cycles can be retrieved in two ways:
By being executed (refer to "Execution")
By cycle-stealing (refer to "Cyclehack")

The game will end when one process reaches the cycle limit, or when the time limit is up. The cycle limit, time limit and a bunch of other settings can be set in the customize menu before starting a match. The winner of a match is the process having the most cycles.

#### Execution

At regular intervals one process will be selected for execution by the scheduler. During execution priority is exchanged for cycles. 

The scheduler will select the process with the highest priority at the time of selection. Being executed is not as harsh as it sounds. The process being executed is granted a powerful laser weapon (Laser Automatic Sniper Execution Ray) for the duration of the execution. 

If the executing process is terminated during execution the laser weapon will be lost along with all the unexchanged priority. A process terminating the executing process will be given a priority reward.

![Execution](http://i.imgur.com/wQxX28U.png)

#### Null Process
If no process has the highest priority at the time of the scheduler's selection, the null process will run. The null process represents a sudden death mode where the world is falling apart and no process will be able to respawn when terminated or falling out of bounds.

When only one process remains, the null process will stop its execution. The last surviving process will be given a priority reward, all process will respawn, and the world will be reassembled.

### Video

[Trailer](https://www.youtube.com/watch?v=b_UdIwGUgd0)

[Extended](https://www.youtube.com/watch?v=95hpfTjSltI)

[Nine-player split-screen](https://www.youtube.com/watch?v=hhSkEy9xeMQ)

[Extra](https://www.youtube.com/watch?v=t3NxIC61-HQ)
