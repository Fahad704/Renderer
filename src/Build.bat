pushd ..
mkdir bin
mkdir int
popd
g++ -DUNICODE -D_UNICODE -Os -DNDEBUG -march=native -mtune=native -c Window.cpp -o ../int/renderer.o
g++ -Os -DNDEBUG -c Vector.cpp -o ../int/Vector.o
iconv -f UTF-16LE -t UTF-8 ../Renderer.rc > ../resource.rc
windres ../resource.rc -o ../int/resource.o
g++ ../int/renderer.o ../int/resource.o ../int/Vector.o -o ../bin/Renderer.exe -Wl,--subsystem,windows -luser32 -lkernel32 -lgdi32
"../bin/Renderer.exe"
pause