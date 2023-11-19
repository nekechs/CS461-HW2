# CS461-HW2
first homework for ML principles

Compilation and running
-----------------------

Library needed: GSL (GNU Scientific Library). This can be found on most Linux distributions, not available on iLab unfortunately :(

Solution, then is to use either a virtual machine or a local machine that can have GSL installed and on the include path decided by the system (like me! My Fedora 36 machine is able to compile using GSL just fine).

To compile the project, go to the root directory of this project (the one that contains the README), and make a build folder by running `$ mkdir build`. Then move to the build folder and run `$ cmake ..`. Ensure that the project is able to build. Then, run `$ make -j4` to compile the project and generate the executables.

Then from build folder, you have multiple choices of executables to choose from. Choose the one relevant to you.

General Project Notes
---------------------

The top level src folder has all the binaries (to be run) directly under src. Then, src has some subdirectories (core and models) that serve as libraries, giving the runner programs functionality. All the header files are included in src/include.

If something has gone wrong, go to the TOP LEVEL CMakeLists.txt and uncomment the `add_compile_options(...)` and `add_link_options(...)`. This will help potentially see what's wrong with the code.
