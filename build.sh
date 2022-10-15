# if [[ ! -d "build" ]]
# then
#     mkdir build;
# else
#     rm -rf build;
#     mkdir build;
# fi

cd build;

conan install .. --build=missing;

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    cmake .. -G "Unix Makefiles";
elif [[ "$OSTYPE" == "msys"* ]]; then
    cmake .. -G "Visual Studio 17";
else
    echo "Unable to detect OS";
    exit 84
fi
cmake --build .;

cd ..;

echo "Build finished successfully."