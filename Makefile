SDIR=src
BDIR=.
ODIR=obj

OBJS=$(ODIR)/particle.o $(ODIR)/particlesystem.o $(ODIR)/util.o
MAIN=psystem
BIN=$(MAIN)

CXXFLAGS=-std=c++0x
LDFLAGS=-lsfml-graphics -lsfml-window

ifndef release
CXXFLAGS+=-g
else
CXXFLAGS+=-O3 -Os
endif

ifndef nowall
CXXFLAGS+=-Wall -Wextra -pedantic -Wmain -Weffc++ -Wswitch-default -Wswitch-enum
CXXFLAGS+=-Wmissing-include-dirs -Wmissing-declarations -Wunreachable-code
CXXFLAGS+=-Winline -Wfloat-equal -Wundef -Wcast-align -Wredundant-decls
CXXFLAGS+=-Winit-self -Wshadow
endif

ifdef werror
CXXFLAGS+=-Werror
endif

ifdef profile
CXXFLAGS+=-pg
LDFLAGS+=-pg
endif

all: $(BDIR)/$(BIN)

$(BDIR)/$(BIN): $(ODIR)/$(MAIN).o $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

clean:
	rm -rf $(ODIR)/*.o $(BIN)

