cd ..
mkdir build
cd build

conan install ../Deco/tests
conan install ../Deco/tests -s build_type=Debug
cmake ../Deco/tests -G "Visual Studio 15 Win64"