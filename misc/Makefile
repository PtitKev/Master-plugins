BASE_DIR=..
NAME=Misc
OBJS_DIR=$(BASE_DIR)/bin/objs
DEST_DIR=$(BASE_DIR)/bin
INC_DIR=$(BASE_DIR)/includes

CXX=g++
CFLAGS=-c -g -O2 -fPIC -Wall -DRASPBERRY -std=gnu++0x -fpermissive -I$(INC_DIR)

LDFLAGS= -shared


SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp,$(OBJS_DIR)/%.o,$(SOURCES))
SO=$(DEST_DIR)/lib$(NAME).so

all: mk_dir $(SOURCES) $(SO)

mk_dir:
	@mkdir -p $(OBJS_DIR)
h:
	@echo OBJECTS=$(OBJECTS)
	@echo SOURCES=$(SOURCES)
	@echo SO=$(SO)


$(SO): $(OBJECTS) 
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

$(OBJS_DIR)/%.o: %.cpp $(INC_DIR)/Kernel.h
	$(CXX) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS)
	rm -rf $(SO)

new : clean all

