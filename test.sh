cd src
cmake
make
rm -r CMakeFiles
cd ..
mv src/main main
./Robot ./main -f -m maps/1.txt
./Robot ./main -f -m maps/2.txt
./Robot ./main -f -m maps/3.txt
./Robot ./main -f -m maps/4.txt