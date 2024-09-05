g++ -DUNICODE -D_UNICODE -o ../bin/Renderer Window.cpp -Wl,--subsystem,windows -luser32 -lkernel32 -lgdi32 -lopengl32 -I../Libraries/GLAD/include/
pause