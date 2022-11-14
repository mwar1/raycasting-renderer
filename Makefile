#OBJS specifies which files to compile as part of the project
OBJS = src/main.c

#Header files
HDRS = src/vector.c src/ray.c

#CC specifies which compiler we're using
CC = gcc

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lm

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = main

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(HDRS) -c
	$(CC) $(OBJS) *.o $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

exec : all
	./$(OBJ_NAME)

clean :
	rm -rf $(OBJ_NAME) *.o

run : exec clean
