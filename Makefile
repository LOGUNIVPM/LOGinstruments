
SOURCES = $(wildcard src/*.cpp)
SOURCES += $(wildcard src/*.c)

include ../../plugin.mk

dist: all
	mkdir -p dist/LOGinstruments
	cp LICENSE* dist/LOGinstruments/
	cp plugin.* dist/LOGinstruments/
	cp -R res dist/LOGinstruments/
