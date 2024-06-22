### About

This folder holds a Imguibased GUI. This is ultimately the way I decided to go 

Future plans are to change from openGL to directx but that would take away the option to move to linux in the future and this is more compatible both ways, so while UI and user convenience do not become a priority it will stay in this format

For details about the actual nonogram solving refer to nonogram.cpp


## Usage
I try to statically link and embed everything so that only using the .exe will suffice for most windows users.

However, obtaining a trusted signature to bypass the Windows defender unrecognized app screen is costly to my knowledge,
so you can build it yourself:

The process itself should consist of traversing to the root directory (cd Path), and afterwards:
rmdir /S /Q build
mkdir build
cd build
cmake ..
cmake --build .      

Alternatively, install cmake extensions for vscode and do >cmake clean rebuild

### Benchmarking
Uncomment the first lines in main.cpp in the main function, and rebuild, then:
.\Nonogram.exe > benchmarkSheet.csv

Sending me these csv files and similar data or suggestions would be greatly appreciated!
Feel free to open issues on this repository.