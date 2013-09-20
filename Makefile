

CXX      := g++
CXXFLAGS += -Wall -g
INCLUDES := -Iinclude
LDFLAGS  +=
LIBS     :=

RM       := rm -f
MKDIR    := mkdir -p

SRCDIR   := src
SRCS     := $(wildcard $(SRCDIR)/*.cpp)

OBJDIR   := obj
OBJS     := $(patsubst $(SRCDIR)%, $(OBJDIR)%, $(patsubst %.cpp, %.o, $(SRCS)))

DEPDIR   := dep
DEPS     := $(patsubst $(SRCDIR)%, $(DEPDIR)%, $(patsubst %.cpp, %.d, $(SRCS)))
SUFFIXES += .d

MAIN     := rogue

all:    $(MAIN)

$(OBJDIR) $(DEPDIR):
	$(MKDIR) $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(DEPDIR)/%.d | $(OBJDIR)
	@echo [$(CXX)] $@
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(DEPDIR)/%.d: $(SRCDIR)/%.cpp | $(DEPDIR)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MM -MT '$(patsubst $(SRCDIR)%,$(OBJDIR)%,$(patsubst %.cpp,%.o,$<))' $< > $@

.PHONY: clean


$(MAIN): $(OBJS)
	@echo [$(CXX)] rogue
	@$(CXX) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

clean:
	$(RM) $(OBJS) $(MAIN) $(DEPS)


-include $(DEPS)
