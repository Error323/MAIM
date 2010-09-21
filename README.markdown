Modular Artificial Intelligence
===============================

A modular approach to an Artificial Intelligence for the [Spring rts
engine](http://springrts.com) using lua and c++.

Installation (N/A)
------------------

If you want to make use of this AI do the following:

	git clone git://github.com/spring/spring.git
	cd spring
	git submodule update --init

Of course, if you already have a clone of spring, just update.  To keep up to
date, cd into the submodule and fetch,merge (like any normal git repo).

Basic Idea
----------

Lua modules define (mod) specific behaviors for units within the rts game.
These modules will be loaded by the AI system and have access to several
queries defined in c++ such as:

* What is the current metal income?
* Where is the biggest enemy cluster located?
* Give me the closest upgradable mexspot
* What type of factory should I build?

This allows the AI to support virtually any mod and create very fine-grained
behavior for each unit-type.

Technical Details
-----------------

This part of the document is sort of a reminder of what it is we are trying to
achieve and how :).

### Lua Modules

The program first scans the lua modules in `data/modules/generic`, next mod
specific modules are scanned in `data/modules/<modname>`. Each module
represents a certain _class_ of units. A class is defined by four different
enums:

* UnitDefTypeMasks -- `MASK_BUILDER_MOBILE`, `MASK_ASSISTER_STATIC`, ...
* UnitDefTerrainMasks -- `MASK_LAND`, `MASK_WATER_SURFACE`, ...
* UnitDefWeapons -- `MASK_NUKE`, `MASK_SHIELD`, ...
* UnitDefClassMasks -- `MASK_SCOUT`, `MASK_ASSAULT`, ...

These c++ enums represent the variables in a lua module. Each variable is
assigned with zero or more values from within the enum via the logical OR
operator. Furthermore each module is given a certain priority (high to low):

* `EMERGENCY` -- This type of module kicks in when, e.g. the unit is under attack
* `REACTIVE` -- Kicks in when e.g. a worker detects metalwrecks which it can suck
* `PROACTIVE` -- Defines a unit's autonomous behavior when none of the above are active


### Globals

The directory `src/globals` contains classes with functions which are reachable
for all the existing lua modules. Examples of global classes are:

* EcoState -- for economy related queries and actions
* DefenseMatrix -- for defense related queries and actions
* Tasks -- for defining tasks and querying the types of tasks currently active


### Groups

A group always contains atleast one unit. Groups can consist of a variety of
unit classes. This implies that for a certain group multiple module classes can
be loaded, potentially. Therefore the c++ group class contains a filter
function, filtering out units suitable for certain modules.

Each group contains its own instances of lua modules, allowing the modules to
be statefull. Groups contain a moduleStack sorted by module priority. These
modules receive the update command through `AIMain::Update()`.
