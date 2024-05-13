.PHONY: all windows linux

ECHO = echo
MKDIR = mkdir

CC = gcc
CFLAGS = -Wall

INCLUDES = -I./include
WINDOWSLIBS = "SDL2.dll" "SDL2_image.dll" "SDL2_mixer.dll" "SDL2_ttf.dll"
LINUXLIBS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
SRC = src
BIN = bin
TARGET = $(BIN)/fifteen

all:
	@$(ECHO) ' * make win'
	@$(ECHO) ' * make lin'

win: $(TARGET).exe
$(TARGET).exe: $(SRC)/main.c | $(BIN)
	@$(ECHO) 'Building $(@F)'
	@$(CC) $(CFLAGS) $(INCLUDES) -Wl,-subsystem,windows $< -o $@ $(WINDOWSLIBS)

lin: $(TARGET)
$(TARGET): $(SRC)/main.c | $(BIN)
	@$(ECHO) 'Building $(@F)'
	@$(CC) $(CFLAGS) $(INCLUDES) $< -o $@ $(LINUXLIBS)
	
$(BIN):
	@$(ECHO) 'Creating $(BIN)'
	@$(MKDIR) $(BIN)
