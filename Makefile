#Thanks to Claude

TINKY_NAME = svc.exe
WINKEY_NAME = winkey.exe
RSHELL_NAME = safe-shell.exe
TINKY_INCL = /I tinky\include
RSHELL_INCL = /I reverse-shell\include
COMPILER = cl.exe
CFLAGS = /nologo /Wall /WX /EHsc /wd4668 /wd4865 /wd4820 /wd5045 /wd5039
OBJDIR = obj
LIBS = Advapi32.lib psapi.lib user32.lib Ws2_32.lib Kernel32.lib

TINKY_SRC = tinky\src\*.cpp
WINKEY_SRC = winkey\winkey.cpp
RSHELL_SRC = reverse-shell\src\*.cpp

TINKY_OBJ = $(OBJDIR)\main.obj \
			$(OBJDIR)\utils.obj \
			$(OBJDIR)\cmdLineManaged.obj \
			$(OBJDIR)\delete.obj \
			$(OBJDIR)\impersonate.obj \
			$(OBJDIR)\install.obj \
			$(OBJDIR)\serviceControlManaged.obj \
			$(OBJDIR)\start.obj \
			$(OBJDIR)\stop.obj \
			$(OBJDIR)\processes.obj \
			$(OBJDIR)\sCleanUp.obj 


WINKEY_OBJ = $(OBJDIR)\winkey.obj

RSHELL_OBJ = $(OBJDIR)\init.obj \
			$(OBJDIR)\cleanUp.obj \
			$(OBJDIR)\networking.obj \
			$(OBJDIR)\run.obj \
			$(OBJDIR)\newSession.obj

all: $(OBJDIR) $(TINKY_NAME) $(WINKEY_NAME) $(RSHELL_NAME)

$(OBJDIR):
	if not exist $(OBJDIR) mkdir $(OBJDIR)

$(TINKY_NAME): $(TINKY_OBJ)
	$(COMPILER) $(CFLAGS) /Fe$@ $(TINKY_OBJ) $(LIBS)

$(WINKEY_NAME): $(WINKEY_OBJ)
	$(COMPILER) $(CFLAGS) /Fe$@ $(WINKEY_OBJ) $(LIBS)

$(RSHELL_NAME): $(RSHELL_OBJ)
	$(COMPILER) $(CFLAGS) /Fe$@ $(RSHELL_OBJ) $(LIBS)

{tinky\src}.cpp{$(OBJDIR)}.obj:
	$(COMPILER) $(CFLAGS) $(TINKY_INCL) /c $< /Fo$@

{winkey}.cpp{$(OBJDIR)}.obj:
	$(COMPILER) $(CFLAGS) /c $< /Fo$@

{reverse-shell\src}.cpp{$(OBJDIR)}.obj:
	$(COMPILER) $(CFLAGS) $(RSHELL_INCL) /c $< /Fo$@

clean:
	if exist $(OBJDIR)\*.obj del /Q $(OBJDIR)\*.obj
	if exist *.exe del /Q *.exe
	if exist *.pdb del /Q *.pdb

re: clean all

distclean: clean
	if exist $(OBJDIR) rmdir /Q $(OBJDIR)

.PHONY: all clean distclean