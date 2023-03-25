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

It also includes an edited 01-01 and 01-06 that removes old unused sprite settings from Nintendo set
on the Rolling Hills that, now would crash NSMBWer. They are different flags for the Rolling Hill recolors
that don't exist in base Newer. (I plan to fix this as to not keep slightly modified vanilla files included with the mod)

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
* Porting of some Newer Wii sprites to new slots. (currently broken, certain sprites despawn unless placed near the level enterance)

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
* Hammer Suit (BUGGED: Accessing the item menu on the map currently crashes the game, and Mario's hat still shows, so it's disabled.)
* LH Compression
* Line God
* Magic Platform
* Message Box
* Meteor
* Music Slots
* Mushroom Platform Slant Mod
* RYOM
* S/E Spawner
* Shy Guys, and the giant ones (BUGGED: will not spawn unless placed near level enterance)
* Size Hacks
* Special Event
* Sprite Retextures
* Tile Randomization (BUGGED: Currently also looks for LevelInfo.bin, couldn't get rid of it. Doesn't need to be valid though)
* Thundercloud
* Tile God
* Topman
* Universal Tileset Slot Mod
* Spine Coaster Platforms
* Fake Star Coin (BUGGED: will not spawn unless placed near level enterance.)

#### Other Stuff
Other Stuff includes, well, other stuff:
* Riivolution XML (files load from the NSMBwer folder)
* ReggiePatch (a Reggie patch based on the NewerSMBW one, updated to work with Reggie! Next 4.8 and 4.9)

#### TODO
* Fix more sprites issues (custom sprites will despawn unless near level enterance)
* Move all Newer sprites to new slots
* Koopatlas and Pirahnatlas support(?)
* Fix NSMBW bugs
* Fix mispositoned Reggie! Next sprite images
* Fix hammer suit bugs
* Fix worldmap crash
