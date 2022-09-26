
if [[ ! -d build ]];
then
    mkdir build;
fi
cd build;
conan install ..;
cmake .. -G 'Unix Makefiles' -DCMAKE_BUILD_TYPE=Release;
echo $PWD;
cmake --build .;
