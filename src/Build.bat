pushd ..
mkdir bin
mkdir int
popd
g++ -DUNICODE -D_UNICODE -O3 -march=native -funroll-loops -c Window.cpp -o ../int/renderer.o
iconv -f UTF-16LE -t UTF-8 ../Renderer.rc > ../resource.rc
windres ../resource.rc -o ../int/resource.o
g++ -flto ../int/renderer.o ../int/resource.o -o ../bin/Renderer.exe -Wl,--subsystem,windows -luser32 -lkernel32 -lgdi32
"../bin/Renderer.exe"