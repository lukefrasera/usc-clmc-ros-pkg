all: bspline

PKGVERSION = eol-bspline-1.5
TARBALL = $(PKGVERSION).zip
TARBALL_URL = http://www.eol.ucar.edu/homes/granger/bspline/$(PKGVERSION).zip
SOURCE_DIR = build/$(PKGVERSION)
MD5SUM_FILE = $(PKGVERSION).zip.md5sum
UNPACK_CMD = unzip


#PATCH = eol-bspline-1.5.patch

$(SOURCE_DIR)/unpacked: $(TARBALL_PATCH)
	-mkdir -p build
	touch -c $(TARBALL)
	rm -rf $(SOURCE_DIR)
	cd build; $(UNPACK_CMD) ../$(TARBALL)
#	mv $(INITIAL_DIR) $(SOURCE_DIR)
	touch $(SOURCE_DIR)/unpacked


bspline: $(SOURCE_DIR)/unpacked
	cd $(SOURCE_DIR) && scons && cd `rospack find bspline` && rm -rf lib include && mkdir include && mkdir include/bspline &&	cp $(SOURCE_DIR)/BSpline/BSpline.h include/bspline/ && cp $(SOURCE_DIR)/BSpline/BSplineBase.h include/bspline/ && mkdir lib && cp $(SOURCE_DIR)/BSpline/libbspline.a lib/ && touch bspline

clean:
	rm -rf build lib include
	rm -f bspline

wipe: clean
