CXX = g++
CXXFLAGS = -std=c++17 -I$(BOOST_HOME)/include
LDFLAGS = -L$(BOOST_HOME)/lib
LDLIBS = -lboost_graph

TARGET = jumping_jim

all: $(TARGET)
	@echo "Compilation successful. Executable generated: $(TARGET)"

$(TARGET): jumping_jim.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) jumping_jim.cpp -o $@ $(LDLIBS)

clean:
	rm -f $(TARGET)
