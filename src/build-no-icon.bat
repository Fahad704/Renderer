pushd ..
mkdir bin
mkdir int
popd
g++ -DUNICODE -D_UNICODE -O2 -funroll-loops -finline-functions Window.cpp -o ../bin/Renderer.exe -Wl,--subsystem,windows -luser32 -lkernel32 -lgdi32 
"../bin/Renderer.exe"