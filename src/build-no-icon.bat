pushd ..
mkdir bin
mkdir int
popd
g++ -DUNICODE -D_UNICODE -O2 -funroll-loops -finline-functions -c Window.cpp -o ../int/renderer.o -I../Libraries/GLAD/include/
g++ ../int/renderer.o -o ../bin/Renderer.exe -Wl,--subsystem,windows -luser32 -lkernel32 -lgdi32 -lopengl32
"../bin/Renderer.exe"
pause