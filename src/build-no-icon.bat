pushd ..
mkdir bin
mkdir int
popd
g++ -DUNICODE -D_UNICODE -O2 -funroll-loops -finline-functions -std=c++17 *.cpp -o ../bin/Renderer.exe -Wl,--subsystem,windows -luser32 -lkernel32 -lgdi32 
"../bin/Renderer.exe"
pause