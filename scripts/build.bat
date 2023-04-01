@echo off

pushd %~dp0\..

if not exist build mkdir build

cd build

call cmake -G "Ninja" ..
call ninja

cd ..

if not exist executables mkdir executables

copy build\core\ground_station\ground_station.uf2 executables\ground_station.uf2
copy build\core\satellite\satellite.uf2 executables\satellite.uf2
copy build\core\prototyping\prototyping.uf2 executables\prototyping.uf2
copy build\tests\crypto_tests\crypto_tests.uf2 executables\crypto_tests.uf2
copy build\tests\kernel_tests\kernel_tests.uf2 executables\kernel_tests.uf2

popd