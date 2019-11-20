CC = g++
CLIBS = -lwsock32
INCLUDES = -Iinclude -I../lib
LIBPATH =
OUTPUT = test
DEBUGOUTPUTDIR = bin/debug
RELEASEOUTPUTDIR = bin/release
OBJDIR = bin
SRCDIR = src
CPPVERSION = c++1z

SOURCES = $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/*/*.cpp)
POBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=%.o)
OBJECTS = $(addprefix $(OBJDIR)/, $(POBJECTS))

$(OUTPUT): $(OBJECTS)
	@mkdir -p bin/debug
	$(CC) $(OBJECTS) $(INCLUDES) $(LIBPATH) $(CLIBS) -std=$(CPPVERSION) -g -o $(DEBUGOUTPUTDIR)/$(OUTPUT)
	@echo "\n"Debug build completed\; Enter $(DEBUGOUTPUTDIR)/$(OUTPUT) to run."\n"

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(INCLUDES) $(LIBPATH) $(CLIBS) -std=$(CPPVERSION) -c $< -o $@

clean:
	rm $(OBJDIR)/*.o $(DEBUGOUTPUTDIR)/$(OUTPUT) #$(RELEASEOUTPUTDIR)/$(OUTPUT)