SLUG = LOGinstruments
VERSION = 0.5.4

SOURCES = $(wildcard src/*.cpp)
SOURCES += $(wildcard src/*.c)


include ../../plugin.mk


dist: all
	mkdir -p dist/$(SLUG)
	cp LICENSE* dist/$(SLUG)/
	cp $(TARGET) dist/$(SLUG)/
	cp -R res dist/$(SLUG)/
	cd dist && zip -5 -r $(SLUG)-$(VERSION)-$(ARCH).zip $(SLUG)
