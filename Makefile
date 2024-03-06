.ONESHELL:

tidy:
	clang-format --assume-filename=.clang-format -i src/*.cpp src/*.h
	clang-tidy --config-file=.clang-tidy src/*.cpp src/*.h

build:
	mkdir -p build
	cd build
	cmake ../src
	make
	cp main ../LinuxRelease

run1:
	make build
	cd LinuxRelease
	./Robot_gui ../build/main -m maps/1.txt

run2:
	make build
	cd LinuxRelease
	./Robot_gui ../build/main -m maps/2.txt

run3:
	make build
	cd LinuxRelease
	./Robot_gui ../build/main -m maps/3.txt

run4:
	make build
	cd LinuxRelease
	./Robot_gui ../build/main -m maps/4.txt

zip:
	./src/CodeCraft_zip.sh
	mv src/CodeCraft-2023.zip code.zip

replay:
	cd ./LinuxRelease/replay
	../replayer/CodeCraft_2023_replay.x86_64