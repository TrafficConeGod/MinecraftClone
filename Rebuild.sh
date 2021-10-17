g++-11 -Wall -I ./Library -I ./Source -I ./Source/Entities -I ./Source/Entities/Subclasses -I ./Source/Entities/Subclasses/Actors -I ./Source/Entities/Subclasses/Actors/Subclasses -I ./Source/Entities/Subclasses/Threads -I ./Source/Entities/Subclasses/Threads/Subclasses -I ./Source/Entities/Subclasses/Graphics -I ./Source/Entities/Subclasses/Elements -I ./Source/Entities/Subclasses/Elements/Subclasses -I ./Source/Entities/Subclasses/BlockEntities -I ./Source/Entities/Subclasses/BlockHandlers -I ./Source/Entities/Subclasses/BlockHandlers/Subclasses -c $(find Source -name '*.cpp') $1
g++-11 $(find -name '*.o') -lGL -lglut -lGLEW -lglfw -lm -lX11 -lpthread -lXi -lXrandr -ldl -o App.elf $1