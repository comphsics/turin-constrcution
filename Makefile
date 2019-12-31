CXX:=g++
CXXFLAG:=-g
HEADER:=-I include -I lib
turin:turin.cpp
	@$(CXX)  $(CXXFLAG) $(HEADER) $^ -o $@
