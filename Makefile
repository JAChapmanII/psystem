SDIR=src
BDIR=.
ODIR=obj

OBJS=
BIN=seuler

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

all: $(BDIR)/$(BIN)

$(BDIR)/$(BIN): $(ODIR)/$(BIN).o
	$(CXX) $(LDFLAGS) -o $@ $^

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

clean:
	rm -rf $(ODIR)/*.o $(BIN)

