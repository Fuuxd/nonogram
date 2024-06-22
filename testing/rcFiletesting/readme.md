compiled using windres.exe (in mingw64)

windres -i "PATH TO FILE.rc" --input-format=rc -o "OUTPUT PATH AND NAME.res" -O coff

After further reading cmake can do this compiling itself- see the main CMakeLists.txt for how this is done