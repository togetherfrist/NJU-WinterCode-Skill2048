cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE --no-warn-unused-cli -S . -B ./build
cmake --build ./build --config Debug --target all --
copy ".\build\skill2048.exe"
.\skill2048.exe
