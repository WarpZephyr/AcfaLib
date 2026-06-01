# AcfaLib
An experimental SPRX library for the PS3 version of *Armored Core: For Answer*.  
This library is primarily an educational tool for learning how to do SPRX hooking.

## Setup
This library is designed to be loaded by a modified game `EBOOT.BIN`.  

### Toolchain Requirements
* **Toolchain**: You must provide your own PS3 toolchain configured to build PPU C++ projects.
* **Automatic Deployment (Optional)**: For automatic deployment to `/dev_hdd0/tmp/` in `RPCS3`, you can choose to define a system environment variable named `RPCS3_ROOT` pointing to your `RPCS3` folder.

### Game Steps
1. Decrypt the game's original `EBOOT.BIN`; Using the `RPCS3` `Utilities` -> `Decrypt PS3 Binaries` tool is preferred.  
2. Patch the decrypted executable using [SPRXPatcher](https://github.com/NotNite/SPRXPatcher), passing `/dev_hdd0/tmp/AcfaLib.sprx` as the library path.  
3. Ensure the `EBOOT.BIN` is placed back in the game folder it is loaded from.

### Build Steps
1. Build the library using your PS3 toolchain.  
2. Ensure the library build is present in the `RPCS3` `/dev_hdd0/tmp/` folder.  
3. Run the game.

## License
See the [COPYING](COPYING) file for details.