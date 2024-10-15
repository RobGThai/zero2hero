# Leaks tutorial
# Valgrind equivalent on Mac
# https://www.youtube.com/watch?v=bhhDRm926qA
debug:
	gcc -o hello hello.c
	leaks --atExit -- ./hello

# Commands for enable/disable malloc stack. Doesn't seem to work from Makefile yet.
# otool -L can be used to diagnose loader ala ldd.
setStack:
	export MallocStackLogging=1

unsetStack:
	unset MallocStackLogging
