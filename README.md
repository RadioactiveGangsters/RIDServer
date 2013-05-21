#IRI

industrial automation

##Installation
If you have windows, get MinGW before continuing.

To build the server, run `make` or `make all`. This will build the `ReaktorServerCLI` program which can be run normally. (See [Usage](#usage))

You can also `make install` to install this unfinished, useless, buggy piece of software to your `/usr/local/bin/` at your own peril. This probably requires some heightened priviledges.

##Usage
The software is infantile, it cannot do anything at all yet.

###Sensors
The server currently looks for a `sensor.ini` on `.` to determine what to do with the sensors, it currently supports only simulated sensors. The ini file should have a `[sensor]` section for the relevant keys. The file is probably case-sensitive, so keep everything but your names lowercase.

* `typecount`: How many sensor types to poll for, type instances are numbered from `typecount` down, so the types range from `type1` to `typeN` for a `typecount` of `N`. When this key is zero, negative (or possibly not a number), sensors will be turned off and the rest of the section may be omitted.
* `typeXname`: The human-readable name for this type of sensor. `X` is replaced by a positive integer, effectively limited by `typecount`.
* `typeXcount`: The amount of instances of sensors of type `typeX`, instances are numbered from 0 up, so the sensors range from `XXX0` to `XXX(N-1)` for a type of `XXX` and a `typeXcount` of `N`.
* `typeXtype`: The type of sensor, can either be `integer` or `binary`. Different sensortypes are handled differently from here on. (Default is `integer`)

####binary
Binary sensors can be on or off, on means it's giving off an alarm.

* `typeXalarm`: The message to display when this alarm goes off. (Default is `Alarm!`)

####integer
Integer sensors give off an alarm when out of a certian range.


* `typeXlbound`: The lowest value the sensor may register without giving off an alarm. (Default is `0`)
* `typeXubound`: The highest value the sensor may register without giving off an alarm. (Default is `100`)
* `typeXlalarm`: The message to display when the sensor crosses the lower bound. (Default is `lower bound Alarm!`)
* `typeXualarm`: The message to display when the sensor crosses the upper bound. (Default is `upper bound Alarm!`)

Try the example [`sensor.ini`](https://github.com/RadioactiveGangsters/RIDServer/blob/master/sensor.ini).

##Documentation
Server class diagram:
![Class Diagram](http://oege.ie.hva.nl/~alb001/iri/docs/reaktorserver.png)
