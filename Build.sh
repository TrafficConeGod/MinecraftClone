g++-11 -Wall -I ./ -I ./Library -I ./Source -c $(find Source -name '*.cpp') $1
g++-11 $(find -name '*.o') -lGL -lglut -lGLEW -lglfw -lm -lX11 -lpthread -lXi -lXrandr -ldl -o App.elf