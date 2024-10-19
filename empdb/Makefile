TARGET = bin/dbview
# List all files matching the pattern
SRC = $(wildcard src/*.c)
# Substring all the files provided(3) and convert using pattern provided (1) 
# into (2)
OBJ = $(patsubst src/%.c,obj/%.o, $(SRC))

default: $(TARGET)
	@echo "TODO:"
	@echo "ALL DONE :-)"

feed:
	./$(TARGET) -f ./mynewdb.db -a "Gof 1, 12/105 xxaa, 80"
	./$(TARGET) -f ./mynewdb.db -a "Gof 2, 12/105 xxaa, 80"
	./$(TARGET) -f ./mynewdb.db -a "Gof 3, 12/105 xxaa, 80"
	./$(TARGET) -f ./mynewdb.db -a "Gof 4, 12/105 xxaa, 80"
	./$(TARGET) -f ./mynewdb.db -a "Gof 5, 12/105 xxaa, 80"

add:
	./$(TARGET) -f ./mynewdb.db -a "Timmy H.,123 Sheshire Ln.,120"

new: default
	./$(TARGET) -f ./mynewdb.db -n

run: clean new 


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
	rm -f *.db


