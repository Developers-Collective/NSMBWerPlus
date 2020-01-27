# NSMBWer
## Source Code Public Release

### Introduction

This fork of the NewerSMBW repo is a slightly modified version which runs most of the sprite mods whilst
keeping the original game's map system intact and working. Among the notable changes, this version of
Kamek is based on CodeWarrior and supports Python 3. More details below.

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

#### Newer Hacks

Changes:
* General folder and YAML cleanup
* Removed some changes that have an impact on the original game (such as the Chest mod)
* RYOM moved over Mega Thwomp (11 -> 322) and Thundercloud moved over Super Topman (168 -> 251) to avoid overriding map actors
* Added 2-Castle hardcode fixes from AnotherSMBW
* Added more effects to effect spawner
* Support for Yoshi Drums in music slots 200+
* Load custom music from the default BRSTM folder

Supported mods:
* Actor Spawner
* Angry Sun
* Animated Tiles
* Chestnut
* Electric Line
* Event Block
* Event Looper
* Fire Laser
* Flipblock
* Giga Goomba
* Hammer Suit (BUGGED: Accessing the item menu on the map currently crashes the game, and Mario's hat still shows)
* LH Compression
* Line God
* Magic Platform
* Message Box
* Meteor
* Music Slots
* Mushroom Platform Slant Mod
* RYOM
* S/E Spawner
* Shy Guys (not the giant ones)
* Size Hacks
* Special Event
* Sprite Retextures
* Tile Randomization (BUGGED: Currently looks for LevelInfo.bin but doesn't need it to be valid or anything)
* Thundercloud
* Tile God
* Topman
* Universal Tileset Slot Mod

#### Misc Tools

There's a bunch of scripts in here which do... things. Most of them might not
be useful in their current state. Just look through and see what you can
gather, I guess.
