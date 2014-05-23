# Change the following three lines to match your FOX installation
FOXINCLUDEDIR=/usr/local/include/fox-1.4
FOXLIBDIR=/usr/local/lib
FOXLIB=FOX-1.4
# Under Windows 98, you may have to use something like:
# FOXINCLUDEDIR=c:\\msys\\1.0\\local\\include\\fox-1.4
# FOXLIBDIR=c:\\msys\\1.0\\local\\lib
# This is not necessary under Windows XP

# Note for Windows users: The code compiles and runs with Cygwin (from Bash),
# but at the moment I'm only interested in problems when compiling with MinGW.

.PHONY: all clean

GCC_WIN=0
ifeq ($(TERM),cygwin)                   # OSTYPE is not exported on Cygwin
GCC_WIN=1
endif
ifeq ($(OSTYPE),msys)
GCC_WIN=1
endif

ifeq ($(GCC_WIN),0)
#FORK_IPC=1
endif

CC=g++
CFLAGS=-I. -I$(FOXINCLUDEDIR) -Wall -W
# -O3
ifeq ($(GCC_WIN),1)
# -DWIN32 is only really necessary (for FOX) on Cygwin (MinGW defines it)
CFLAGS+=-DWIN32
endif
ifeq ($(findstring darwin,$(OSTYPE)),darwin) # for example "darwin7.0"
CFLAGS+=-D__unix__ -DDISABLE_COPIER
endif
LDFLAGS=-L$(FOXLIBDIR) -l$(FOXLIB)
# -s
ifeq ($(GCC_WIN),1)
LDFLAGS+=-mwindows -e _mainCRTStartup -lwsock32 -lm
endif

ifdef FORK_IPC
CFLAGS+=-DFORK_IPC
#LDFLAGS+=-lrt
endif

OBJECTS=main.o Frontend.o MainWindow.o OptionsWindow.o TextFieldOption.o \
        CheckButtonOption.o RegistryEntry.o CommandLineHandler.o Process.o \
        ProgressDialog.o OutputChecker.o FileSelector.o FileDialog.o \
        ComboBox.o EditTextWindow.o MultiGameFileSelector.o \
        SNESControllerSettings.o snes.o genesis.o gb.o gba.o n64.o nes.o sms.o \
        pce.o ngp.o misc.o icons.o
ifdef FORK_IPC
OBJECTS+=ProcessIPC.o
endif
ICONS=icon16.gif icon32.gif icon64.gif

TARGET=uf
ifeq ($(GCC_WIN),1)
TARGET:=$(addsuffix .exe,$(TARGET))
endif


all: $(TARGET)


clean:
	rm -f $(TARGET) $(OBJECTS) icons.cpp icons.h stdout stderr process_errors.txt


.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@


$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@


icons.h: $(ICONS)
	reswrap -o $@ $(ICONS)


icons.cpp: $(ICONS)
	reswrap -e -o $@ $(ICONS)


# Dependencies

TAB_STD_H=MainWindow.h CommandLineHandler.h TextFieldOption.h ComboBox.h \
          FileDialog.h Frontend.h

main.o: main.h Frontend.h
Frontend.o: Frontend.h MainWindow.h icons.h
MainWindow.o: MainWindow.h Frontend.h FileSelector.h OptionsWindow.h \
              CommandLineHandler.h FileDialog.h snes.h genesis.h gb.h gba.h \
              n64.h nes.h sms.h pce.h ngp.h misc.h
OptionsWindow.o: OptionsWindow.h MainWindow.h TextFieldOption.h \
                 CheckButtonOption.h EditTextWindow.h CommandLineHandler.h
TextFieldOption.o: TextFieldOption.h RegistryEntry.h
CheckButtonOption.o: CheckButtonOption.h RegistryEntry.h
RegistryEntry.o: RegistryEntry.h
CommandLineHandler.o: CommandLineHandler.h MainWindow.h Process.h ProcessIPC.h \
                      OutputChecker.h ProgressDialog.h OptionsWindow.h \
                      FileDialog.h
EditTextWindow.o: EditTextWindow.h
Process.o: Process.h
ProcessIPC.o: ProcessIPC.h Process.h
ProgressDialog.o: ProgressDialog.h
OutputChecker.o: OutputChecker.h Process.h ProcessIPC.h ProgressDialog.h \
                 MainWindow.h
FileSelector.o: FileSelector.h
FileDialog.o: FileDialog.h
ComboBox.o: ComboBox.h
MultiGameFileSelector.o: MultiGameFileSelector.h FileSelector.h Frontend.h
SNESControllerSettings.o: SNESControllerSettings.h ComboBox.h
snes.o: snes.h $(TAB_STD_H) MultiGameFileSelector.h SNESControllerSettings.h
genesis.o: genesis.h $(TAB_STD_H) MultiGameFileSelector.h
gb.o: gb.h $(TAB_STD_H)
gba.o: gba.h $(TAB_STD_H) MultiGameFileSelector.h
n64.o: n64.h $(TAB_STD_H)
nes.o: nes.h $(TAB_STD_H)
sms.o: sms.h $(TAB_STD_H) MultiGameFileSelector.h
pce.o: pce.h $(TAB_STD_H) MultiGameFileSelector.h
ngp.o: ngp.h $(TAB_STD_H)
misc.o: misc.h $(TAB_STD_H)
icons.o: icons.h
