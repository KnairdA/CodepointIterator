CXX      = g++ 
CXXFLAGS = -std=c++11 -W -Wall -Wextra -pedantic

SRC  = src/utility.cc            \
       src/codepoint_iterator.cc \
       test.cc
OBJ  = $(subst .cc,.o,$(SRC))

all: test

test: ${OBJ}
	$(CXX) -o test -lgtest $(OBJ)
	./test

.PHONY: clean;
clean:
	rm -f $(OBJ)
	rm test

depend: .depend

.depend: $(SRC)
	$(CXX) -M $(CXXFLAGS) $< > $@

include .depend
