
BIN = bin
BUILD = build

build: normdir conan
	cmake -S . -B $(BUILD);
	cmake --build $(BUILD);

test: testdir conan
	cmake -S . -B $(BUILD) -DTESTMODE=1;
	cmake --build $(BUILD);

coverage: cleantest test
	./bin/Tests
	./outputCoverage.sh

testdir:
	$(eval BUILD="test/build")

normdir:
	$(eval BUILD="build")

conan:
	conan install conanfile.txt -if $(BUILD) --build=missing

cleanbuild: normdir clean

cleantest: testdir clean cleancoverage

cleancoverage:
	find . -name "*.gcno" -delete
	find . -name "*.gcda" -delete
	find . -name "*.gcov" -delete
	find . -name "*.html" -delete


clean:
	rm -rf $(BUILD);
	mkdir $(BUILD);
	rm -rf $(BIN)/*

rebuild: cleanbuild build

retest: testdir cleantest test