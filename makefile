build/swim: swim.c
	clang -framework Carbon -Wno-deprecated -o build/swim swim.c
