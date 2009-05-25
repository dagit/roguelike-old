

CXX = g++
CXXFLAGS = -Wall -g
INCLUDES = -Iinclude
LDFLAGS =
LIBS =

RM = rm -f

SRCDIR = src
SRCS := $(wildcard $(SRCDIR)/*.cpp)

OBJDIR = obj
OBJS := $(patsubst $(SRCDIR)%, $(OBJDIR)%, $(patsubst %.cpp, %.o, $(SRCS)))

MAIN = rogue

.PHONY: .depend clean

all:    $(MAIN)

$(MAIN): $(OBJS)
	@echo [$(CXX)] rogue
	@$(CXX) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo [$(CXX)] $@
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJS) $(MAIN)

.depend: $(SRCS)
	@echo Finding dependencies
	@$(RM) .depend
	@makedepend $(INCLUDES) -f- -- $(CXXFLAGS) -- $^ > .depend

include .depend