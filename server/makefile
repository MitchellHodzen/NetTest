CC = g++
CLIBS = -lSDL2_image -lmingw32 -lSDL2main -lSDL2 -lwsock32
INCLUDES = -Iinclude -IC:/CppLibraries/include/SDL2
LIBPATH = -LC:/CppLibraries/lib
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