compiler = g++
flags = -g -Wall -std=c++11
compile = $(compiler) $(flags)

scheduling: scheduling.cpp
	$(compile) scheduling.cpp -o scheduling

.PHONY: clean
clean:
	rm -rf *.o scheduling
