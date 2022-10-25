
BUILD = build

all: conan
	cmake -S . -B $(BUILD);
	cmake --build $(BUILD)

test: clean conan
	cmake -S . -B $(BUILD) -DTESTMODE=1;
	cmake --build $(BUILD);

conan:
	conan install conanfile.txt -if $(BUILD) --build=missing;

clean:
	rm -rf build;
	rm -rf test/build;
	mkdir build;
	mkdir test/build;

re: clean
	make;