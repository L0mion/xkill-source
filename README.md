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

### Compile instructions
* Install Visual Studio 2012 (x86)
* Install Qt (Swedish instructions below)
glossary:
men = but
n�r = when
du = you
har = have/has
f�ljande = the following
den = it
och = and
g�r = do/does
s�kv�g = path
till = to
l�gg till = add
installerat = installed
s�tt = set
som = as
[2013-01-10 12:00:22] Mattias A: men n�r du har den och n�r du har Qt Add-in installerat s� g�r du f�ljade i Visual Studio
QT5 ->QT Options -> (L�gg till s�kv�g till Qt SDKn, e.g. "C:\Qt\qt-everywhere-opensource-src-4.8.4" och s�tt den som "Deafult Qt version")
* Run the file "xkill-project\xkill\xkill.sln" from Visual Studio 2012.
* The solution should contain 9 projects:

xkill
xkill-architecture
xkill-input
xkill-io
xkill-physics
xkill-renderer
xkill-sound
xkill-utilities
xkill-utilities-levelexporter (Visual Studio 2010)

Set "xkill" as startup project (Right click -> "Set as StartUp Project").
Compile (F5 key).

Tested on Microsoft Windows 7 and Microsoft Windows 8, with the setting "Win32", next to the "Release"-setting.
Move "xkill-project\xkill-dependencies\DLL move to build" to "xkill-build\bin-Release" (or to "xkill-build\bin-Debug" if compiling with the DEBUG flag set)
To run XKILL: run the "xkill-build\bin-Release\xkill.exe" file.

If it does not work, try downloading "Visual C++ Redistributable for Visual Studio 2012" ("vcredist_x86.exe" or "vcredist_x64.exe")
Perhaps through this link: http://www.microsoft.com/en-us/download/details.aspx?id=30679

### Download

[2013-03-21](https://github.com/L0mion/xkill-source/releases)
[2013-03-26] (https://drive.google.com/file/d/0B6c_718zVy6QZW5EcEpxOUhrSDA/edit?usp=sharing)

### Website
www.xkill.se
