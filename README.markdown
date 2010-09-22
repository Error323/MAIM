Modular Artificial Intelligence
===============================

Last revised: September 22, 2010

A modular approach to an Artificial Intelligence for the
[Spring RTS engine](http://springrts.com) using Lua and C++.

Installation (N/A)
------------------

If you want to make use of this AI do the following:

	git clone git://github.com/spring/spring.git
	cd spring
	git submodule update --init

Of course, if you already have a clone of Spring, just update.  To keep up to
date, cd into the submodule and fetch, merge (like any normal git repository).


Basic Idea
----------

Lua modules define specific behaviors for groups of units within the
RTS game ("mod") being played. These modules will be loaded by the AI
system and can query for information from the C++ side, such as:

* What is the current resource income?
* Where is the biggest cluster of enemy units located?
* Give me the closest upgradeable resource spot
* What type of factory should I build?

This allows the AI to support virtually any mod (game mechanics) and
to create very fine-grained behavior for each unit-type within a group.


Technical Details
-----------------

This part of the document is sort of a reminder of what it is we are
trying to achieve and how :). NOTE: INCOMPLETE AND SUBJECT TO CHANGE


### Groups

The central concept in MAI is a group of units. Each group (class AIGroup)
maintains a set of Lua modules (class LuaModule) that together define the
group's behavior. Whenever a group is created or changed at run-time, zero
or more LuaModule instances suitable for that group's unit composition are
added to it or removed.

A group always contains at least one unit. The group is removed when all of the
units in it are destroyed. Groups are *homogeneous*, meaning only unit-types
from a single class can exist in a group. This implies that for a certain group
only a single LuaModule can be active.

Each group contains its own instances of Lua modules, allowing the modules to
be stateful. Groups contain a module-stack sorted by module priority. Each frame,
the group's active modules are pushed onto this stack and then executed and popped
again.


### Group Manager

The group manager, as the name implies, deals with the creation and destruction
of groups. Basic workflow for the GroupManager:

1. Create group.
2. Assign unit from the callback `AIMain::UnitFinished()` to the appropreate group.
3. Assign Lua Module to the group.
4. The function `GroupManager::Update()` makes sure all `AIGroup::Update()` functions are called.


### Lua Modules

	<modname> is the mod's short name without version (some separate
	configuration Lua script, eg. <modname.lua>, should then define
	the minimum- and maximum-versions that the AI can support)

To load a new LuaModule instance, the AI library first scans the Lua
module scripts present in `data/modules/generic`. Next, mod-specific
modules are scanned in `data/modules/<modname>`. If a mod-specific
module exists, that is loaded instead of the default (default modules
will typically just be stubs). If no default module exists either, the
AI will terminate.

Each LuaModule represents a certain _class_ of unit-types. A class is
defined by four different parameters (bitmasks), which are determined
through an automated UnitDef categorization process on the C++ side.
Thus, multiple unit-TYPES (Spring UnitDef instances) can belong to the
same CLASS. Allowing these to be overriden by mod-specific scripts might
be a useful extension, but is complicated due to the potential for errors.

Every LuaModule stores four uint32 "mask" variables ("moduleTypeMasks",
"moduleTerrainMasks", "moduleWeaponMasks", "moduleMoveMasks"). Each of
these four variables is assigned a value from the following C++ enums:

* enum UnitDefTypeMasks {`MASK_BUILDER_MOBILE`, `MASK_ASSISTER_STATIC`, ...}
* enum UnitDefTerrainMasks {`MASK_LAND`, `MASK_WATER_SURFACE`, ...}
* enum UnitDefWeaponMasks {`MASK_NUKE`, `MASK_SHIELD`, ...}
* enum UnitDefClassMasks {`MASK_SCOUT`, `MASK_ASSAULT`, ...}

Lua module scripts define the values of these variables; they become
constants for the lifetime of the LuaModule (ie. for as long as it is
bound to a group).

For example, a hypothetical module intended to manage a group of scout
hovercraft would have a terrain-mask of `MASK_LAND | MASK_WATER_SURFACE`
(where `|` is the bitwise-OR operator). Any unit from the class of UnitDef
types with terrain mask `MASK_WATER_SUBMERGED` would not be able to join
this group.

Furthermore, each module is given a certain execution priority (high to low):

* `EMERGENCY` -- This type of module kicks in when, e.g. the unit is under attack
* `REACTIVE`  -- Kicks in when e.g. a builder detects wrecks which it can reclaim
* `PROACTIVE` -- Defines a unit's autonomous behavior when none of the above are active


### Tasks

Tasks are controlled by the modules themselves, the c++ `TaskHolder` merely
allows other modules to query tasks executed by others, allowing for e.g.
assisting of a task. For now, four tasks will be available:

* Build -- Keeps track of groups that are building, what they are building and where they are building it.
* AssistBuild -- Allows for assisting a build.
* Attack -- Keeps track of groups that are attacking, what they are attacking and where.
* AssistAttack -- Allows for assisting an attack.


### Globals

The directory `src/globals` contains classes with functions (callouts) which
are available to all the existing Lua modules. Examples of global classes are:

* EcoState -- for economy-related queries and actions
* DefenseMatrix -- for defense-related queries and actions
* Tasks -- for defining tasks and querying the types of tasks currently active


### Events

We implement the observer pattern to efficiently handle high-frequency events
such as UnitDamaged. Each event is sent only to those AI components that need
to know about it. This has the following consequences:

* The unit-handler (class AIUnitHandler) and possibly other AI components
become an observer for all unit-related types of events that they care about
(eg. UnitDestroyed, UnitIdle).
* For each type of game event, there exists an observer base-class (AObserver)
which is partially specialized (AUnitDestroyedObsever). Concrete observers
like AIUnitHandler extend these further. If an observer is interested in more
than one event-type, it MUST use multiple inheritance. (!)
* To prevent (eg.) a UnitDestroyed event from being broadcasted to every
AIGroup instance when at most one group needs to receive it, each unit
"attaches" the receiver(s) that it wants to inform about this event-type (eg.
AIUnitHandler and its own AIGroup) to a list of subject observers upon
creation, and later notifies these when it is destroyed. To accomplish this,
AIUnit must extend the AUnitDestroyedSubject class and other specialized
subclasses of ASubject.  (again using multiple inheritance)
