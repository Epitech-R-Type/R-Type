
BIN = bin
BUILD = build

build: normdir conan
	cmake -S . -B $(BUILD);
	cmake --build $(BUILD)

test: testdir conan
	cmake -S . -B $(BUILD) -DTESTMODE=1;
	cmake --build $(BUILD);

testdir:
	$(eval BUILD="test/build")

normdir:
	$(eval BUILD="build")

conan:
	conan install conanfile.txt -if $(BUILD) --build=missing;

cleanbuild: normdir clean

cleantest: testdir clean

clean:
	rm -rf $(BUILD);
	mkdir $(BUILD);
	rm -rf $(BIN)/*

rebuild: cleanbuild
	make build;

retest: testdir
	make test;