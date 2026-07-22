# # CXX = g++

# # CXXFLAGS = -std=c++17 -Wall \
# #            $(shell root-config --cflags) \
# #            -I./include

# # LDFLAGS = $(shell root-config --libs)


# # TARGET = ./bin/bm_hit_matching


# # SRCS = ./app/bm_hit_matching.cpp \
# #        ./src/bm_hit_matching.cpp \
# # 	   ./app/bm_correlation.cpp \


# # OBJS = ./build/main.o \
# #        ./build/bm_hit_matching.o \
# #        ./build/bm_correlation.o


# # all: $(TARGET)


# # $(TARGET): $(OBJS)
# # 	@mkdir -p ./bin
# # 	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)


# # ./build/main.o: ./app/bm_hit_matching.cpp
# # 	@mkdir -p ./build
# # 	$(CXX) $(CXXFLAGS) -c $< -o $@


# # ./build/bm_hit_matching.o: ./src/bm_hit_matching.cpp
# # 	@mkdir -p ./build
# # 	$(CXX) $(CXXFLAGS) -c $< -o $@


# # clean:
# # # 	rm -rf ./build/*.o ./bin/bm_hit_matching

# CXX = g++

# CXXFLAGS = -std=c++20 -Wall \
#            $(shell root-config --cflags) \
#            -I./include

# LDFLAGS = $(shell root-config --libs)


# BIN_DIR = bin
# BUILD_DIR = build


# DIRS = $(BIN_DIR) $(BUILD_DIR)


# # 共通ソース
# SRC = \

# # SRCから自動生成
# OBJ = $(SRC:src/%.cpp=$(BUILD_DIR)/%.o)


# # 実行ファイル
# APPS = \
# 	bm_hit_matching 

# TARGETS = $(addprefix $(BIN_DIR)/,$(APPS))


# all: dirs $(TARGETS)


# dirs:
# 	mkdir -p $(DIRS)


# # src/*.cpp -> build/*.o
# $(BUILD_DIR)/%.o: src/%.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@


# # app/*.cpp + 対応するobjでリンク
# $(BIN_DIR)/%: app/%.cpp $(OBJ)
# 	$(CXX) $(CXXFLAGS) $< $(OBJ) -o $@ $(LDFLAGS)


# clean:
# 	rm -rf $(BUILD_DIR)
# 	rm -f $(TARGETS)


# .PHONY: all clean dirs

CXX = g++

CXXFLAGS = -std=c++20 -Wall \
           $(shell root-config --cflags) \
           -I./include

LDFLAGS = $(shell root-config --libs)


BIN_DIR = bin
BUILD_DIR = build


DIRS = $(BIN_DIR) $(BUILD_DIR)



# src以下のソース
SRCS = $(wildcard src/*.cpp)

# src/*.cpp -> build/*.o
OBJS = $(SRCS:src/%.cpp=$(BUILD_DIR)/%.o)



# app以下の実行ファイル
APPS = $(patsubst app/%.cpp,%,$(wildcard app/*.cpp))


TARGETS = $(addprefix $(BIN_DIR)/,$(APPS))



all: dirs $(TARGETS)



dirs:
	mkdir -p $(DIRS)



# src/*.cpp -> build/*.o
$(BUILD_DIR)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@



# app/*.cpp + src/*.o -> bin/*
$(BIN_DIR)/%: app/%.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) $< $(OBJS) -o $@ $(LDFLAGS)



clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TARGETS)



.PHONY: all clean dirs