#!/usr/bin/env bash

cleanup() {
    echo "Cleaning up..."
    find -name "*.gcov" -delete
}

# Generate all .gcov files
find ./test/build/CMakeFiles/ -name "*.gcda" -exec gcov -bf {} \;

# Check that gcovr is installed or return
if ! command -v gcovr &> /dev/null
then
    echo "\nPlease install gcovr to continue..."
    echo "\tManjaro: yay -S gcovr"
    echo "\tUbuntu: sudo apt-get install gcovr"

    cleanup

    exit
fi

#Output gcovr to html
gcovr --html-details report.html --html-self-contained -e "test/build/"

# Run cleanup
cleanup