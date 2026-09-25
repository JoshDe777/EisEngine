# GameLab Project Repository

|  General Info  | |
| ---|---|
| Working Title | `Ocean Outlaws!` & `Eis-Engine` |
| Final Title | `Ocean Outlaws!` & `Eis-Engine` |
| Student | `Joshua O'Donnell`, joshua.odonnell@stud-mail.uni-wuerzburg.de, `s411288` |
| Target Platform(s) | `PC` |
| Start Date | 17.10.2023 |
| Study Program | Games Engineering B.Sc.|
| Engine Version | 1.0 |

### Abstract

This Repository will contain my GameLab III project, consisting of a handmade engine of bundled libraries and own classes using OpenGL, and at least one game made using this engine.

Eis-Engine will aim to provide a multipurpose code-base for all kinds of 2D games. Being primarily conceived for my first envisioned game, "Ocean Outlaws!", Eis-Engine will initially be focussed on making this game possible before veering towards more general functions. It builds on several external libraries and API's, such as OpenGL for rendering, or Box2D for the physics system, whilst including many native features such as the comprehensive scene graph and many utility classes with highly integrated Vector classes or debugging and math functions.

Ocean Outlaws! is a top-down 2D shooter game, in which the player, captain of a pirate crew having just escaped a naval prison with their crew, attempts to keep their ship afloat for as long as they can, collecting supplies and treasure along the way. The longer the player survives, the more resources the navy will allocate in an attempt to thwart the player's ambitions of establishing themselves in pirate folklore.

## Software and Hardware requirements

EisEngine is a C++ game engine, and should be usable in any IDE upon importation. 

Ocean Outlaws! is playable exclusively on computers supporting the GLEW implementation of OpenGL, so platforms running on Windows, Linux, Mac OS X, FreeBSD, Irix, and Solaris.
The keys are classic keyboard keys, a mouse is not required for gameplay.

The following external libraries should come with the download when downloading the ~/code/extern folder or using the engine from the repository, but as requirements I'll list them up here just in case:
Box2D, GLAD, GLFW, GLM, Tinygltf, stb_image, SoLoud, FastNoise.

The engine requires a minimum CMake version of 3.21.

No additional software or hardware should be required.

## How to play Ocean Outlaws!?

This can be done simply by downloading the zip folder in the `builds` folder (directly at root), unzipping it, then running the Game.exe file.

## How can I use EisEngine?

Import the code folder for the project (~/code/engine), as well as the prerequesite external libraries in ~/code/extern. 
Make sure the project and dependencies are included in your project, ideally with CMake (a CMakeLists.txt file is to be found in the code/engine folder), then you can simply use EisEngine by including the file `EisEngine.h`. 
Every feature at your disposal is available from this header file alone. For more detailed implementation and modifications of EisEngine code, it is also possible to include individual files in the corresponding sub-folders.

It is a known issue that CMake compilation fails when building Engine code for the first time, however simply reloading CMake immediately afterwards seems to solve the issue.

# Relevant materials for grading:

- Project poster (available in the ~/poster folder).
- Project trailer (available in the ~/trailer folder, or on youtube from the following link: [link](https://youtu.be/3JW6ng6Qhmk?si=17ndqkpXYIKcT156)).
- Project report (available in the ~/report folder).
- Readme file (you are reading it right now :D)
- Documentation (available in the folder ~/documentation, open the file out/html/index.html and you can browse through the engine API from there) - important note, the QuickStart.md file contains an updated link to the repository, which was not updated in the documentation unfortunately.
- Code (available in the ~/code folder. Engine code is to be found under code/engine, game code under code/game).
- Binaries (available in the ~/builds folder. For use refer to the sub-section `How to play Ocean Outlaws!?`).

Additional notes: All external packages and libraries used for EisEngine are to be found in the ~/code/extern folder. 