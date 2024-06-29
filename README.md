### About

This folder holds a Imguibased GUI. The GUI is primarily a debugging feature and is not the focus of this branch. However, the branch "GUIMerge" focuses more on visualizing the complete solving process, if you are interested.

changing from openGL to directx would take away the option to move to linux in the future and this is more compatible both ways, so while UI and user convenience do not become a priority it will stay in this format

For details about the actual nonogram solving refer to nonogram.cpp / nonogram.md

## Usage
I try to statically link and embed everything so that only using the .exe [Nonogram.exe](./Nonogram.exe) will suffice for most windows users.

If you are a user interested solely on the visualization of the solving process, I recommend using the GUIMerge branch instead of main.

However, obtaining a trusted signature to bypass the Windows defender unrecognized app screen is costly to my knowledge,

Therefore, you are welcome to build it yourself: 

download and Install cmake, afterwards

    git clone https://github.com/Fuuxd/nonogram.git --recurse-submodules
    mkdir build
    cd build
    cmake ..
    cmake --build .      

Alternatively, install cmake extensions for vscode and do the equivalent

If you see errors related to dataSetNonograms.h, or Webpn. Delete all related lines, and try building again.

### Benchmarking
Uncomment the first lines in main.cpp in the main function, and rebuild, then:
.\Nonogram.exe > benchmarkSheet.csv

Sending me these csv files and similar data or suggestions would be greatly appreciated!
Feel free to open issues on this repository.
