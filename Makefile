

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

DEPDIR = dep
DEPS := $(patsubst $(SRCDIR)%, $(DEPDIR)%, $(patsubst %.cpp, %.d, $(SRCS)))
SUFFIXES += .d

MAIN = rogue

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(DEPDIR)/%.d
	@echo [$(CXX)] $@
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(DEPDIR)/%.d: $(SRCDIR)/%.cpp
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MM -MT '$(patsubst $(SRCDIR)%,$(OBJDIR)%,$(patsubst %.cpp,%.o,$<))' $< > $@

.PHONY: clean

all:    $(MAIN)

$(MAIN): $(OBJS)
	@echo [$(CXX)] rogue
	@$(CXX) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

clean:
	$(RM) $(OBJS) $(MAIN) $(DEPS)


-include $(DEPS)
