# cd src
# cmake
# make
# rm -r CMakeFiles
# cd ..
# mv src/main main
# ./Robot_gui ./main  -m maps/1.txt


# 统一代码风格
# clang-format --assume-filename=.clang-format -i src/*.cpp
# clang-tidy --config-file=.clang-tidy src/*.cpp

cd build
cmake ../src
make
cd ../judger
./Robot_gui ../build/main -m maps/1.txt