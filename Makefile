#Thanks to Claude

# Configuration
TINKY_NAME = tinky.exe
WINKEY_NAME = winkey.exe
TINKY_INCL = /I tinky\include
COMPILER = cl.exe
CFLAGS = /nologo /Wall /WX /EHsc /wd4668 /wd4865 /wd5039 /wd5045 /wd4820
OBJDIR = obj
LIBS = Advapi32.lib psapi.lib user32.lib

# Sources
TINKY_SRC = tinky\src\*.cpp
WINKEY_SRC = winkey\winkey.cpp

# Objets
TINKY_OBJ = $(OBJDIR)\main.obj \
            $(OBJDIR)\utils.obj \
            $(OBJDIR)\cmdLineManaged.obj \
            $(OBJDIR)\delete.obj \
            $(OBJDIR)\impersonate.obj \
            $(OBJDIR)\install.obj \
            $(OBJDIR)\serviceControlManaged.obj \
            $(OBJDIR)\start.obj \
            $(OBJDIR)\stop.obj

WINKEY_OBJ = $(OBJDIR)\winkey.obj

# Règle par défaut
all: $(OBJDIR) $(TINKY_NAME) $(WINKEY_NAME)

# Créer dossier obj
$(OBJDIR):
	@if not exist $(OBJDIR) mkdir $(OBJDIR)

# Linker les exécutables
$(TINKY_NAME): $(TINKY_OBJ)
	$(COMPILER) $(CFLAGS) /Fe$@ $(TINKY_OBJ) $(LIBS)

$(WINKEY_NAME): $(WINKEY_OBJ)
	$(COMPILER) $(CFLAGS) /Fe$@ $(WINKEY_OBJ) $(LIBS)

# Règle d'inférence pour tinky
{tinky\src}.cpp{$(OBJDIR)}.obj:
	$(COMPILER) $(CFLAGS) $(TINKY_INCL) /c $< /Fo$@

# Règle d'inférence pour winkey
{winkey}.cpp{$(OBJDIR)}.obj:
	$(COMPILER) $(CFLAGS) /c $< /Fo$@

# Nettoyage
clean:
	if exist $(OBJDIR)\*.obj del /Q $(OBJDIR)\*.obj
	if exist *.exe del /Q *.exe
	if exist *.pdb del /Q *.pdb

distclean: clean
	@if exist $(OBJDIR) rmdir /Q $(OBJDIR)

.PHONY: all clean distclean