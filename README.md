# Devilution
Diablo devolved - magic behind the 1996 computer game

**Note**, Devilution requires an original copy of `diabdat.mpq`. None of the Diablo 1 game assets are provided by this project. To get a legitimate copy of the game assets, please refer to the [GoG release of Diablo 1](https://www.gog.com/game/diablo).

# Introduction
This version of Devilution started out as a fork of [diasurgical's devilution](https://github.com/diasurgical/devilution). My intention was to just do some debugging to see how one of my favorite games of all time internally works. Unfortunately, I ran into some issues when migrating the projects to Visual Studio 2019 and before I knew it, I completely restructured the source code layout.
Given the fact the restructuring touched pretty much every file in the code base it is almost impossible to merge my changes back into [diasurgical's devilution](https://github.com/diasurgical/devilution) and so after issuing a huge PR accidentally to `upstream` I decided to "un-fork" this repo and continue it as a standalone project.
 
# Compiling
Diablo was developed on Windows 95 using Visual C++ 4.20 and later 5.10 and 6 for newer patches. However, the code restructuring was done with Visual Studio 2019 in mind and ties to older versions have been cut.

### Building with Visual Studio 2019
- Open the project solution `Diablo\build\vs2019\Diablo.sln`, choose `Debug` or `Release` and `x86` (x64) is not supported yet), and then `Build Diablo`.

# Installing
Once compiled, the Devilution binary will serve as a replacement for `Diablo.exe`. The following files from the original game patched to 1.09(b) need to be present: `DIABDAT.MPQ`, `DiabloUI.dll`, `SmackW32.dll`, `Standard.snp`, and `Storm.dll`.

# Credits
- All credits goes to the team at [diasurgical's devilution](https://github.com/diasurgical/devilution). I'm just a guy who plays around with the result of their hard work.

# Legal
Devilution is released to the Public Domain. The documentation and function provided by Devilution may only be utilized with assets provided by ownership of Diablo.

The source code in this repository is for non-commerical use only. If you use the source code you may not charge others for access to it or any derivative work thereof.

Battle.net(R) - Copyright (C) 1996 Blizzard Entertainment, Inc. All rights reserved. Battle.net and Blizzard Entertainment are trademarks or registered trademarks of Blizzard Entertainment, Inc. in the U.S. and/or other countries.

Diablo(R) - Copyright (C) 1996 Blizzard Entertainment, Inc. All rights reserved. Diablo and Blizzard Entertainment are trademarks or registered trademarks of Blizzard Entertainment, Inc. in the U.S. and/or other countries.

Devilution and any of its' maintainers are in no way associated with or endorsed by Blizzard Entertainment(R).
