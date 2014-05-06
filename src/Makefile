SRCS = main.cpp app.cpp cube.cpp tile.cpp
OBJS = $(SRCS:%.cpp=../obj/%.o)
LDFLAGS =-framework sfml-graphics -framework sfml-window -framework sfml-system 
CXXFLAGS = -c -Wall -Wextra -std=c++11

TARGET = ../bin/photo-cube

$(TARGET): dirs $(OBJS)
	g++ $(LDFLAGS) $(OBJS) -o $@

../obj/%.o: %.cpp
	g++ $(CXXFLAGS) $< -o $@

ALLFILES = $(shell gfind -type f -name "*.*pp")

todolist:
	-@for file in $(ALLFILES:Makefile=); do fgrep -H -e TODO -e FIXME $$file; done; true

play: $(TARGET)
	$(TARGET)

.PHONY: dirs
dirs:
	@mkdir -p ../obj ../bin 

.PHONY: clean
clean:
	-rm -r ../obj ../bin 
