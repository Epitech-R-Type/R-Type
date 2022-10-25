
all:
	conan install conanfile.txt -if build --build=missing;
	cmake -S . -B build;
	cmake --build build

re:
	rm -rf build;
	mkdir build;
	make;