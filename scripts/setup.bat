@echo off

pushd %~dp0\..

git submodule update --init --recursive

call scripts\build.bat
call pip install -r utils\requirements.txt

popd
