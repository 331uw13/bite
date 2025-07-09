FLAGS = -O2 -Wall -Wextra
CXX = g++

TARGET_NAME = bite


SRC  = $(shell find ./src -type f -name *.cpp)

LIBS = -lcursesw
OBJS = $(SRC:.cpp=.o)

all: $(TARGET_NAME)


%.o: %.cpp
	@echo "compile"
	$(CXX) $(FLAGS) -c $< -o $@ 

$(TARGET_NAME): $(OBJS)
	@echo "link"
	$(CXX) $(OBJS) -o $@ $(LIBS) $(FLAGS)

clean:
	rm $(OBJS) $(TARGET_NAME)

.PHONY: all clean

