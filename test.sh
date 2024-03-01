cd src
cmake
make
rm -r CMakeFiles
cd ..
mv src/main main
./Robot_gui ./main  -m maps/1.txt