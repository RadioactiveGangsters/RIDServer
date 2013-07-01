#IRI

industrial automation

##Installation
If you have windows, get MinGW before continuing.

The software no longer requires [`iniparser`](http://ndevilla.free.fr/iniparser/) to be installed on your system, but rather bundles it to automatically create the needed resources.

To build the server, run `make` or `make all`. This will build the `ReaktorServerCLI` program which can be run normally. (See [Usage](#usage))

You can also `make install` to install this unfinished, useless, buggy piece of software to your `/usr/local/bin/` at your own peril. This probably requires some heightened priviledges.

##Usage
The software is infantile, it cannot do anything at all yet.

The server will look for a config file named `reactor.ini` and cannot start without it. Configuration options are described below. Entries in the file are case-sensitive.

###Network
The server will wait for operators to connect with their clients over `TCP/IP`, you can configure the port and the address to listen on in the `[network]` section with any of these options:

* `port`: Which port to listen to, if unspecified, the default port `61014` will be used.
* `address`: Which address to listen to, it is recommended to leave this unspecified unless you have multiple NICs or something fancy like that. You'll know what I mean when you truely need it.
* `clients`: The expected number of clients. Setting this makes sure at least `N` clients can initialize connections at the same time. If set too low, clients may get rejected during startup of your system. This option is not a limit, it will continue to accept clients indefinitely as long as it is in its power to do so. It is generally safe to ignore this option if you don't have a lot of clients connecting within a few milliseconds as may be the case in large automated networks. The default is `10`.

###Sensors
The server currently supports only simulated sensors. The ini file must have a `[sensor]` section for the relevant keys. The file is probably case-sensitive, so keep everything but your names lowercase.
Ther server will not start without sensor section, but may run without sensors if you're into that sort of thing.

* `typecount`: How many sensor types to poll for, type instances are numbered from `typecount` down, so the types range from `type1` to `typeN` for a `typecount` of `N`. When this key is zero, negative (or possibly not a number), sensors will be turned off and the rest of the section may be omitted.
* `typeXname`: The human-readable name for this type of sensor. `X` is replaced by a positive integer, effectively limited by `typecount`.
* `typeXcount`: The amount of instances of sensors of type `typeX`, instances are numbered from 0 up, so the sensors range from `XXX0` to `XXX(N-1)` for a type of `XXX` and a `typeXcount` of `N`.
* `typeXtype`: The type of sensor, can either be `integer` or `binary`. Different sensortypes are handled differently from here on. (Default is `integer`)
* `typeXinterval`: The minimum polling time of this type of sensor in milliseconds.

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
