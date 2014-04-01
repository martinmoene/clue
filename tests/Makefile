# Copyright 2014 by Martin Moene
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

CXXFLAGS = -Wall -Wextra -Weffc++ -std=c++03 -I..

vpath %.hpp ..

all: test_clue

test_clue: test_clue.cpp test_clue_part2.cpp clue.hpp
	$(CXX) $(CXXFLAGS) -o test_clue test_clue.cpp test_clue_part2.cpp 
	./test_clue

clean:
	rm test_clue

