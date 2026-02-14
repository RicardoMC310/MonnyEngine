# Detecção do Sistema Operacional
ifeq ($(OS),Windows_NT)
	# Windows
	RM=del /Q /S
	EXT=.exe
	PATH_SEP=\\
	LIB_EXT=.dll
	CC=gcc
	CXX=g++
	LDFLAGS=
	LIBS=
	MKDIR=mkdir
	MKDIR_CMD=mkdir
	RMDIR=rmdir /S /Q
	SO_EXT=dll
else
	# Linux/Unix
	RM=rm -f
	EXT=
	PATH_SEP=/
	LIB_EXT=.so
	CC=gcc
	CXX=g++
	LDFLAGS=
	LIBS=
	MKDIR=mkdir -p
	MKDIR_CMD=mkdir -p
	RMDIR=rm -rf
	SO_EXT=so
endif

# Nome do Executável
TARGET=engine$(EXT)

# Diretórios
SRCDIR=src
INCDIR=include
BINDIR=bin
BUILDDIR=build
LIBDIR=$(BINDIR)/lib

# Diretórios específicos para C e CPP
CDIR=$(SRCDIR)/Linux/C
CPPDIR=$(SRCDIR)/Linux/CPP
CBUILDDIR=$(BUILDDIR)/C
CPPBUILDDIR=$(BUILDDIR)/CPP

# Arquivos Fontes e Objetos
C_SOURCES=$(wildcard $(CDIR)/*.c)
CPP_SOURCES=$(wildcard $(CPPDIR)/*.cpp)

# Objetos C (compilação normal)
C_OBJECTS=$(patsubst $(CDIR)/%.c,$(CBUILDDIR)/%.o,$(C_SOURCES))

# Bibliotecas compartilhadas C++ (cada CPP vira um .so)
CPP_LIBS=$(patsubst $(CPPDIR)/%.cpp,$(LIBDIR)/lib%.$(SO_EXT),$(CPP_SOURCES))
CPP_OBJECTS=$(patsubst $(CPPDIR)/%.cpp,$(CPPBUILDDIR)/%.o,$(CPP_SOURCES))

# Flags de Compilação
CFLAGS=-Wall -Wextra -I$(INCDIR)
CXXFLAGS=-Wall -Wextra -I$(INCDIR) -fPIC

# Detecção Específica do OS
ifeq ($(OS),Windows_NT)
	ifeq ($(shell where gcc 2>NUL),)
		$(warning GCC não encontrado no PATH. Teste usar MinGW ou WSL.)
	endif
	CFLAGS+=-D_WIN32 -DD_WIN32
	CXXFLAGS+=-D_WIN32 -DD_WIN32
	LIBS+=-lws2_32
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		CFLAGS+=-D_LINUX -DD_LINUX -D__linux__
		CXXFLAGS+=-D_LINUX -DD_LINUX -D__linux__
	else ifeq ($(UNAME_S),Darwin)
		CFLAGS+=-D_DARWIN -DD_DARWIN
		CXXFLAGS+=-D_DARWIN -DD_DARWIN
	else
		CFLAGS+=-D_UNIX -DD_UNIX
		CXXFLAGS+=-D_UNIX -DD_UNIX
	endif
endif

# Flags de linkagem para bibliotecas
LDFLAGS += -L$(LIBDIR)
# Usa o caminho absoluto completo
LDFLAGS += -Wl,-rpath,$(abspath $(LIBDIR))
LDFLAGS += -Wl,--enable-new-dtags

# CORREÇÃO: LIBS com as bibliotecas (usando -l)
LIBS += $(patsubst $(LIBDIR)/lib%.$(SO_EXT),-l%,$(CPP_LIBS))
LIBS += -lpthread

# Targets principais
all: $(BINDIR) $(LIBDIR) $(CBUILDDIR) $(CPPBUILDDIR) $(CPP_LIBS) $(TARGET)

# Criação dos diretórios
$(BINDIR):
	$(MKDIR_CMD) $(BINDIR)

$(LIBDIR):
	$(MKDIR_CMD) $(LIBDIR)

$(CBUILDDIR):
	$(MKDIR_CMD) $(CBUILDDIR)

$(CPPBUILDDIR):
	$(MKDIR_CMD) $(CPPBUILDDIR)

# Compilação C (objetos para o executável)
$(CBUILDDIR)/%.o: $(CDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compilação C++ (objetos para .so)
$(CPPBUILDDIR)/%.o: $(CPPDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Criação das bibliotecas compartilhadas (.so)
$(LIBDIR)/lib%.$(SO_EXT): $(CPPBUILDDIR)/%.o
	$(CXX) -shared $< -o $@

# Linkagem do executável principal - CORRIGIDO! 🎯
# NÃO usa $(CPP_LIBS) como dependência, apenas $(LIBS) com -l
$(TARGET): $(C_OBJECTS) | $(CPP_LIBS)
	$(CC) $(C_OBJECTS) -o $(BINDIR)/$(TARGET) $(LDFLAGS) $(LIBS)
	@echo "✅ Linkagem concluída!"
	@echo "   RPATH: $(abspath $(LIBDIR))"
	@echo "   LIBS: $(LIBS)"

# Limpeza
clean:
	$(RM) $(BINDIR)/$(TARGET)
	$(RMDIR) $(BUILDDIR)
	$(RMDIR) $(LIBDIR)
	$(RMDIR) $(BINDIR)

# Run - AGORA FUNCIONA! 🚀
run: all
ifeq ($(OS),Windows_NT)
	cd $(BINDIR) && $(TARGET)
else
	$(BINDIR)/$(TARGET)
endif

# Debug
debug: CFLAGS+=-g -O0
debug: CXXFLAGS+=-g -O0
debug: all

# Release
release: CFLAGS += -O3
release: CXXFLAGS += -O3
release: all

# Phony targets
.PHONY: all clean install run debug release info

# Informações do sistema
info:
	@echo "========================================"
	@echo "Informações do Sistema e Compilação"
	@echo "========================================"
	@echo "Sistema Operacional: $(OS)"
	@echo "Detalhe: $(UNAME_S)"
	@echo "Compilador C: $(CC)"
	@echo "Compilador C++: $(CXX)"
	@echo "Executável: $(TARGET)"
	@echo "========================================"
	@echo "Diretórios:"
	@echo "  Build: $(BUILDDIR)"
	@echo "  Bin: $(BINDIR)"
	@echo "  Lib: $(LIBDIR)"
	@echo "  Lib Abs: $(abspath $(LIBDIR))"
	@echo "========================================"
	@echo "Bibliotecas geradas:"
	@echo "  $(CPP_LIBS)"
	@echo "========================================"
	@echo "Flags Link: $(LDFLAGS)"
	@echo "Bibliotecas: $(LIBS)"
	@echo "========================================"