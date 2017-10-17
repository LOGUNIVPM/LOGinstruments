
SOURCES = $(wildcard src/*.cpp)
SOURCES += $(wildcard src/*.c)

include ../../plugin.mk

#LDFLAGS += -l:/home/leopard/Apps/Rack/plugins/Leonardo/src/kiss_fft130/kiss_fft.so

dist: all
	mkdir -p dist/Leonardo
	cp LICENSE* dist/Leonardo/
	cp plugin.* dist/Leonardo/
	cp -R res dist/Leonardo/
