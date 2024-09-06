pushd ..
mkdir bin
mkdir int
popd
g++ -DUNICODE -D_UNICODE -O2 -funroll-loops -finline-functions -c Window.cpp -o ../int/renderer.o -I../Libraries/GLAD/include/
iconv -f UTF-16LE -t UTF-8 ../Renderer.rc > ../resource.rc
windres ../resource.rc -o ../int/resource.o
g++ ../int/renderer.o ../int/resource.o -o ../bin/Renderer.exe -Wl,--subsystem,windows -luser32 -lkernel32 -lgdi32 -lopengl32
pause