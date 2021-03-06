XKILL - Heartaches by the Cycle
========

### Backstory

In a forgotten store-room rests an ancient single-core processor. In this harsh landscape, before multicore-technology, fierce gladiator tournaments are fought in digital arenas by processes struggling to be granted processor execution time (cycles) by the scheduler. These tournaments consist of deadly combat as the processes battle to destroy one another. The incentive for battle is that only one process at a time can execute on the processor.

### Basics

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

### Key Features

* Unique gameplay (invariant enjoyment) inspired by operating system process scheduling.
* Up to 9 players split screen multiplayer (9 Xbox 360 gamepads (or equivalent) is required. Batteries not included).
* Customizable weaponry.
* 5 different power ups (referred to as hacks).
* Ludicrously over(X)kill randomized nickname selector, close to 2000 unique names! (for example: "G�ran Fries")

### Video

[Trailer](https://www.youtube.com/watch?v=b_UdIwGUgd0)

[Extended](https://www.youtube.com/watch?v=95hpfTjSltI)

[Nine-player split-screen](https://www.youtube.com/watch?v=hhSkEy9xeMQ)

[Extra](https://www.youtube.com/watch?v=t3NxIC61-HQ)

### Compile instructions
* Get environment and assets repositories:
 * https://github.com/L0mion/xkill-project
 * https://github.com/CaterHatterPillar/xkill-resources
* Install Visual Studio 2012 (tested with "Visual Studio 2012 Ultimate", x86)
* Install Qt Visual Studio Add-in 1.2.0 (http://download.qt-project.org/official_releases/vsaddin/qt-vs-addin-1.2.0-opensource.exe)
* Download Qt 4.8.4 SDK (http://download.qt-project.org/archive/qt/4.8/4.8.4/qt-everywhere-opensource-src-4.8.4.zip)
 * In Visual Studio, navigate to Qt Options (QT5 ->QT Options)
 * Add path to downloaded Qt SDKn, e.g. "C:\Qt\qt-everywhere-opensource-src-4.8.4" 
 * Set Qt Version as "Default Qt version"
* Open the file "xkill-project\xkill\xkill.sln" with Visual Studio 2012.
* The solution should contain 9 projects:
 * xkill
 * xkill-architecture
 * xkill-input
 * xkill-io
 * xkill-physics
 * xkill-renderer
 * xkill-sound
 * xkill-utilities
 * xkill-utilities-levelexporter (Visual Studio 2010)
* Set "xkill" as startup project (Right click -> "Set as StartUp Project").
Compile (F7 key). Tested on Microsoft Windows 7 and Microsoft Windows 8, with the setting "Win32" (Solution Platforms), next to the "Release" setting (Solution Configurations).
* Nota bene: XKILL will crash if compiled with the "Debug" setting, when the in-game null process starts executing. Don't ask.
* Move "xkill-project\xkill-dependencies\DLL move to build" to "xkill-build\bin-Release" (or to "xkill-build\bin-Debug" if compiling with the DEBUG flag set).
* To run XKILL, press F5 from within Visual Studio or run the "xkill-build\bin-Release\xkill.exe" file.

If it does not work, try downloading "Visual C++ Redistributable for Visual Studio 2012" ("vcredist_x86.exe" or "vcredist_x64.exe")
Perhaps through this link: http://www.microsoft.com/en-us/download/details.aspx?id=30679

---

### Download

* [2013-03-26] (https://github.com/L0mion/xkill-source/releases/tag/v1.1)
* [2013-03-21](https://github.com/L0mion/xkill-source/releases/tag/v1.0)

### Website
www.xkill.se
