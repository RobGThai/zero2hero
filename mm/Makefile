TARGET = bin/final
# List all files matching the pattern
SRC = $(wildcard src/*.c)
# Substring all the files provided(3) and convert using pattern provided (1) 
# into (2)
OBJ = $(patsubst src/%.c,obj/%.o, $(SRC))

default: $(TARGET)

# Understanding macro
# https://stackoverflow.com/questions/3220277/what-do-the-makefile-symbols-and-mean
# $@ = Left side
# $< = One input from right side
# $? = All inputs from right side

$(TARGET): $(OBJ)
	$(info Linking objects: [$(@)] [$(?)])
	gcc -Wall -o $@ $? -Iinclude

obj/%.o: src/%.c
	$(info Compiling: [$(<)] -> [$(@)])
	gcc -Wall -c $< -o $@ -Iinclude

# Clean all files
clean:
	rm -f obj/*.o
	rm -f bin/*
	rm -f *.a
	rm -f *.dylib

# Assembled using Archive object instead of plain object (.o)
archive:
	gcc -Wall -o obj/my_lib.o -c src/my_lib.c
	# Archive lib into artifact
	ar rcs my_lib.a obj/my_lib.o

so:
	gcc -Wall -o my_lib.so src/my_lib.c -shared

builddylib:
	gcc -Wall -o obj/my_lib.o -c src/my_lib.c
	gcc -Wall -dynamiclib -o my_lib.dylib obj/my_lib.o

linka:
	gcc -Wall -o bin/main src/main.c my_lib.a -Iinclude

linkdylib:
	# DYLD_LIBRARY_PATH
	# LD_LIBRARY_PATH
	gcc -Wall -o bin/maindy src/main.c -Iinclude my_lib.dylib

buildso:
	gcc -Wall -o obj/my_lib.o -c src/my_lib.c
	gcc -Wall -dynamiclib -o my_lib.so obj/my_lib.o

linkso:
	gcc -Wall -o bin/mainso src/main.c -Iinclude my_lib.so


