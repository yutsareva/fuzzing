
#-fsanitize=address would make findings a few times slower.

CXXFLAGS=-O3 -g -std=c++17 -stdlib=libc++

bfs: bfs.cpp
	clang++ bfs.cpp $(CXXFLAGS) -o bfs

bfs.fuzz: bfs.cpp
	afl-clang++ bfs.cpp $(CXXFLAGS) -o bfs.fuzz

str: str.cpp
	clang++ str.cpp $(CXXFLAGS) -o str

# Enable laf-intel to split large comparisons into multiple branches
# https://github.com/AFLplusplus/AFLplusplus/blob/stable/instrumentation/README.laf-intel.md
str.fuzz: str.cpp
	AFL_LLVM_LAF_SPLIT_SWITCHES=1 AFL_LLVM_LAF_TRANSFORM_COMPARES=1 AFL_LLVM_LAF_SPLIT_COMPARES=1 afl-clang++ str.cpp $(CXXFLAGS) -o str.fuzz

str.asm: str
	gdb --batch -ex "disassemble main" $< > str.asm

str.fuzz.asm: str.fuzz
	gdb --batch -ex "disassemble main" $< > str.fuzz.asm

fuzz-bfs: bfs.fuzz
	AFL_SKIP_CPUFREQ=1 afl-fuzz -i in_bfs -o out_bfs -- ./bfs.fuzz

fuzz-str: str.fuzz
	AFL_SKIP_CPUFREQ=1 afl-fuzz -i in_str -o out_str -- ./str.fuzz

clean:
	rm -rf out_* bfs bfs.fuzz str str.fuzz *.asm
