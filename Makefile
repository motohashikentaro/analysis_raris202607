CXX = g++

CXXFLAGS = -std=c++17 -Wall \
           $(shell root-config --cflags) \
           -I./include

LDFLAGS = $(shell root-config --libs)


TARGET = ./bin/bm_hit_matching


SRCS = ./app/bm_hit_matching.cpp \
       ./src/bm_hit_matching.cpp


OBJS = ./build/main.o \
       ./build/bm_hit_matching.o


all: $(TARGET)


$(TARGET): $(OBJS)
	@mkdir -p ./bin
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)


./build/main.o: ./app/bm_hit_matching.cpp
	@mkdir -p ./build
	$(CXX) $(CXXFLAGS) -c $< -o $@


./build/bm_hit_matching.o: ./src/bm_hit_matching.cpp
	@mkdir -p ./build
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	rm -rf ./build/*.o ./bin/bm_hit_matching