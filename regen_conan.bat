IF EXIST .\build cd build & conan install .. --profile ../vs2017 &cmake ..
IF NOT EXIST .\build mkdir build & conan install .. --profile ../vs2017 & cmake ..