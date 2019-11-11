cd ..
mkdir tests-build
cd tests-build

conan install ../Deco/tests
cmake ../Deco/tests -G "Visual Studio 16 2019"