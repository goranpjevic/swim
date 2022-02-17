build_flags = -Wno-deprecated
frameworks = -framework Carbon
build_path = ./build
swim_src = ./swim.c

.phony: all clean

all: $(build_path)/swim

clean:
	rm -rf $(build_path)

$(build_path)/swim: $(swim_src)
	mkdir -p $(build_path)
	clang $(build_flags) $(frameworks) -o $(build_path)/swim $(swim_src)
