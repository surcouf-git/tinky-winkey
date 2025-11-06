SERVICE_NAME = svc.exe
KEYLOGGER_NAME = winkey.exe
SVC_INCL = /I tinky\include
KELOGGER_INCL = /I winkey\include

COMPILER = cl.exe
CFLAGS = /nologo /Wall /WX /EHsc /wd5039 /wd5045

SVC_SRC =	tinky/src/main.cpp \
			tinky/src/utils.cpp \
			tinky/src/createService.cpp

service: $(SVC_SRC:.cpp=.obj)

%.obj: %.cpp
	$(COMPILER) $(CFLAGS) $(SVC_INCL) /c $< /Fo$@

#cl.exe /nologo /Wall /WX /EHsc /wd5039 /wd5045