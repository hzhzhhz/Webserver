all: echo_test http_test
.PHONY:all
CXX=g++
CFLAGS=-g -std=c++11
LIBS=-pthread
SRCS=$(wildcard ./*.cc)
SRCSHTTP=$(wildcard ./http/*.cc)
echo_test:$(SRCS)
	$(CXX) -o echo_test ./echo/test.cc $(CFLAGS) $(SRCS) $(LIBS)
http_test:$(SRCS) $(SRCSHTTP)
	$(CXX) -o http_test $(CFLAGS) $(SRCS) $(SRCSHTTP) $(LIBS)
.PHONY:clean
clean:
	rm echo_test http_test