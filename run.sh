rm test
g++ -w  test.cpp -o test -lglut -lGLU -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
./test