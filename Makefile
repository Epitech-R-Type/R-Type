
all:
	conan install conanfile.txt -if build
	cmake -S . -B build;
	cmake --build build

re:
	rm -rf build;
	mkdir build;
	make;