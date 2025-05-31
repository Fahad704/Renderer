pushd ..
mkdir bin
mkdir int
popd
iconv -f UTF-16LE -t UTF-8 ../Renderer.rc > ../resource.rc
windres ../resource.rc -o ../int/resource.o
g++ -DUNICODE -D_UNICODE -Os -DNDEBUG -march=native -mtune=native *.cpp -o ../bin/Renderer.exe -Wl,--subsystem,windows -luser32 -lkernel32 -lgdi32
cd ..
"bin/Renderer.exe"
pause