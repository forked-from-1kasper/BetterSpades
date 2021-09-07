![GPL v3](https://www.gnu.org/graphics/gplv3-127x51.png)

## ![](docs/icon_small.png) BetterSpades

* Replicate of the great game *Ace of Spades* (classic voxlap)
* runs on very old systems back to OpenGL 1.1 (OpenGL ES support too)
* shares similar if not even better performance to voxlap
* can run on *"embedded"* systems like a [Steam Link](https://store.steampowered.com/app/353380/Steam_Link/)

#### Why should I use this instead of ...?

* free of any Jagex code, they can't shut it down
* open for future expansion
* easy to use
* no hidden bugs

### Quick usage guide

You can either:
* use the client temporarily by extracting the downloaded zip into a new directory.
* extract all contents to your current Ace of Spades installation directory (normally found at `C:/Ace of Spades/`), effectively replacing the old voxlap version

## System requirements

| Type    | min. requirement                                     |
| ------- | ---------------------------------------------------- |
| OS      | Windows 98 or Linux                                  |
| CPU     | 1 GHz single core processor                          |
| GPU     | 64MB VRAM, Mobile Intel 945GM or equivalent          |
| RAM     | 256MB                                                |
| Display | 800x600px                                            |
| Others  | Keyboard and mouse<br />Dial up network connection   |


## Build requirements

This project uses the following libraries and files:

| Name         | License         | Usage                  | GitHub                                             |
| ------------ | --------------- | ---------------------- | :------------------------------------------------: |
| GLFW3        | *ZLib*          | OpenGL context         | [Link](https://github.com/glfw/glfw)               |
| OpenAL soft  | *LGPL-2.1*      | 3D sound environment   | [Link](https://github.com/kcat/openal-soft)        |
| inih         | *BSD-3.Clause*  | .INI file parser       | [Link](https://github.com/benhoyt/inih)            |
| stb_truetype | *Public domain* | TrueType font renderer | [Link](https://github.com/nothings/stb)            |
| dr_wav       | *Public domain* | wav support            | [Link](https://github.com/mackron/dr_libs/)        |
| http         | *Public domain* | http client library    | [Link](https://github.com/mattiasgustavsson/libs)  |
| LodePNG      | *MIT*           | png support            | [Link](https://github.com/lvandeve/lodepng)        |
| libdeflate   | *MIT*           | decompression of maps  | [Link](https://github.com/ebiggers/libdeflate)     |
| enet         | *MIT*           | networking library     | [Link](https://github.com/lsalzman/enet)           |
| parson       | *MIT*           | JSON parser            | [Link](https://github.com/kgabis/parson)           |
| log.c        | *MIT*           | logger                 | [Link](https://github.com/xtreme8000/log.c)        |
| GLEW         | *MIT*           | OpenGL extensions      | [Link](https://github.com/nigels-com/glew)         |
| hashtable    | *MIT*           | hashtable              | [Link](https://github.com/goldsborough/hashtable/) |
| libvxl       | *MIT*           | access VXL format      | [Link](https://github.com/xtreme8000/libvxl/)      |
| microui      | *MIT*           | user interface         | [Link](https://github.com/rxi/microui)             |

You will need to compile the following by yourself, or get hold of precompiled binaries:

* GLFW3
* GLEW
* OpenAL soft *(only needed on Windows)*
* libdeflate
* enet

Follow the instructions on their project page, then place produced static libraries in `deps/`.

All other requirements of the above list (like single file libs) will be downloaded by CMake automatically and **don't** need to be taken care of. Because state of copyright of 0.75 assets is unknown, CMake will also download additional assets from [*here*](http://aos.party/bsresources.zip) which are not part of this repository.

## Gallery

| <img src="/docs/pic01.png" width="250px"><br />*quite old* | <img src="/docs/pic02.png" width="250px"><br />hiesville | <img src="/docs/pic03.png" width="250px"> |
| :-: | :-: | :-: |
| <img src="/docs/pic04.png" width="250px"><br />*grenade fun* | <img src="/docs/pic05.png" width="250px"><br />*falling block animation* | <img src="/docs/pic06.png" width="250px"><br />*sniping on normandie* |
