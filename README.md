# NSMBWer+
## Source Code Public Release

### Introduction

This fork of the NSMBWer repo is a heavily modified version that aims to keep it up to date with current NSMBW modding while keeping everything from Vanilla intact.
Most notably, this version uses Abood's more sprites mod to implement every custom sprite and actor from Newer while keeping the ones they replaced.
I'm releasing this in a heavily unfinished state, I will keep updating the project until I see it as complete.

### Licensing

Like in the original repo, the source code and tools are released under the MIT license.
See the `LICENSE` file in this repository for full details.

Copyright (c) 2010-2013 Treeki, Tempus, megazig

### Requirements

#### For compiling the Newer game hacks:

- Windows (required for CodeWarrior to run)
- devkitPPC
- Python 3.x
- Python libraries: PyYAML, pyelftools

### What's Here

#### NSMBWer+
A build is inside this folder, including the SpriteTex files, an XML and the related folders. If you
want to create a mod, starting with this is suggested. Keep in mind this build doesn't include the
custom music or tilesets from Newer.

It also includes an edited 01-01, 01-06, and 06-01 that removes old unused sprite settings from Nintendo set
on several sprites that, now would crash NSMBWer. They are different flags for sprite recolors
that don't exist in base Newer.

#### Kamek

All the Newer hacks are inside the Kamek folder in this repository but, technically, Kamek is
really just a simple build system for calling the tools to compile/assemble
each source file and putting together an output file you can inject into your
game.

Unlike most typical build systems, though, there's a bunch of features
specific to game hacks like Newer:

- Kamek is configured through a game-specific project file; each project
  pulls in various modules, which can each contain C++ and assembly source
  files, and hooks
- Hooks are different types of patches that are used to inject things into the
  game: patch static code/data in RAM, overwrite pointers with a reference to
  a symbol exported from Kamek code, inject branch instructions, ...
- Memory addresses in the linker script and used by hooks are automatically
  converted to match all supported game versions - Kamek uses the original
  version of the PAL/European NSMBW release as a reference.
- The compiled output is converted to a specific format expected by the
  Newer loader.

You may learn how to compiler the NSMBWer+ source code on [This Website][horizoncodesetup]

Changes:
* General folder and YAML cleanup
* Changes that break vanilla levels (such as the newer chest mod) have been re-implemented. A branch that removes all these major changes will be added in the future.
* Every sprite mentioned in the section below has been moved to new sprite slots to avoid overriding any original game sprites to have compatibility with the original game.
* Added 2-Castle hardcode fixes from AnotherSMBW
* Added more effects to effect spawner
* Support for Yoshi Drums in music slots 200+
* Load custom music from the default BRSTM folder
* Porting of some Newer Wii sprites to new slots
* Ported fixes from RoadrunnerWMC's NSMBW-Updated project.

Supported mods:
* Actor Spawner
* Angry Sun
* Animated Tiles
* Chestnut
* Electric Line
* Event Block
* Fire Laser
* Flipblock
* Giga Goomba
* Hammer Suit
* Line God
* Magic Platform
* Message Box
* Meteor
* Music Slots
* Mushroom Platform Slant Mod
* RYOM
* S/E Spawner
* Shy Guys, and the giant ones
* Size Hacks
* Special Event
* Sprite Retextures
* Tile Randomization (BUGGED: Currently also looks for LevelInfo.bin, couldn't get rid of it. Doesn't need to be valid though)
* Thundercloud
* Tile God
* Topman
* Universal Tileset Slot Mod
* Spine Coaster Platforms
* Fake Star Coin
* Classic Controller Support

#### Upcoming Changes/Fixes
* Move all Newer sprites to new slots
* Pirahnatlas support(?)
* Fix NSMBW bugs
* Add Hammer Suit rendering on the worldmap
* Fix Boo Circles

### Credits
* Nin0 - Programming Assistance, General Assistance, Numerous bits of code, original NSMBWer
* Bupcraft, Lurker, Kirblue - Bug Reporting
* Abood - More Sprites code
* CLF78 - Original NSMBWer, Korean/Taiwanese support, additional More Sprites code
* EternityShack - Crash Reporter
* Luminyx - Programming assistance
* Danster - original NSMBWer
* Ninji, Tempus - original NewerSMBW
* RoadrunnerWMC - Reggie! Next spritedata, NSMBW Updated code, Korean/Taiwanese support
* G4L - Fixed NewerSMBW models

[horizoncodesetup]:
https://horizon.miraheze.org/wiki/Setting_Up_and_Compiling_the_Newer_Sources#Windows_(Newer_Super_Mario_All-Stars_Revived_and_NSMBWer_only)
