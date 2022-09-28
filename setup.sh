if [[ ! -d "build" ]]
then
    mkdir build;
else
    rm -rf build;
    mkdir build;
fi

cd build;

conan install .. --build=missing;
cmake .. -G "Unix Makefiles";
cmake --build .;

cd ..;

echo "Build finished successfully."
