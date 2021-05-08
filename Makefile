
TYPE ?= Release
VERSION = 0.0.1
SHELL = bash

# Directories
PREFIX        ?= /usr/local
DEPLOY_TARGET ?= linux-x86_64
BUILD_DIR      = _build/$(shell [ "${TYPE}" = Release ] && echo release || echo debug)
ICON_DIR       = share/icons/hicolor/scalable
SHARE_DIR      = share/schim
SCHIM_ICON_DIR = share/schim/icons
MAN_DIR	       = share/man/man1
DOC_DIR	       = share/doc/schim

JOBS = $(shell echo ${MAKEFLAGS} | sed -n 's_.*\(-j\|--jobs=\) *\([0-9][0-9]*\).*_\2_p')

.PHONY: app qthelp man install uninstall

# ┏━━━━━━━━━━━━━━┓
# ┃ CMAKE CONFIG ┃
# ┗━━━━━━━━━━━━━━┛
ifeq (${OS},Windows_NT)
    CMAKE = VERSION=${VERSION} cmake -G "Unix Makefiles" \
		-DCMAKE_BUILD_TYPE=${TYPE} ../../src/
else
    CMAKE = VERSION=${VERSION} cmake -DCMAKE_BUILD_TYPE=${TYPE} ../../src/
endif
CMAKE_BUILD = cmake --build ./ -j ${JOBS}

# ┏━━━━━━━━━┓
# ┃ COMPILE ┃
# ┗━━━━━━━━━┛

# This target will create a directory tree under ${BUILD_DIR}/dest
# that can be moved to /usr/ or /usr/local using `make install`
app:# qthelp
	@if [ -z "$$(ls src/dxflib/)" ]; then { \
	echo -e "\e[0;31m" \
	        "ERROR: Directory src/dxflib is empty." \
	        "Have you cloned with --recurse-submodules?" "\e[0m"; \
	         false; }; fi
	@mkdir -p "${BUILD_DIR}"
	@echo -e "\e[1;34m Starting CMake\e[0m"
	@cd "${BUILD_DIR}"; ${CMAKE} && ${CMAKE_BUILD}
	@echo -e "Copying binary to destination..."
	@cd "${BUILD_DIR}"; \
	mkdir -p dest/bin \
		"dest/${ICON_DIR}"/apps \
		"dest/${SHARE_DIR}/symb" \
		"dest/${SCHIM_ICON_DIR}/actions" \
		"dest/${DOC_DIR}"; \
		mv schim dest/bin/
	@# Create launcher and copy resources
	@echo -e "Copying resources to destination..."
	@umask 022
	@cp misc/schim.sh "${BUILD_DIR}/"                                        # Launch script
	@cp res/img/icon.svg "${BUILD_DIR}/dest/${ICON_DIR}/apps/schim.svg"      # Icon
	@cp res/img/actions/*.svg "${BUILD_DIR}/dest/${SCHIM_ICON_DIR}/actions/" # Action icons
	@cp -r res/symb/* "${BUILD_DIR}/dest/${SHARE_DIR}/symb/"                 # Symbols
	@umask 222
	@cp res/ATTRIBUTION.md "${BUILD_DIR}/dest/${DOC_DIR}/"                   # ATTRIBUTION.md
ifeq (${OS},Windows_NT)
	@echo "Bundling required libraries..."
	windeployqt "${BUILD_DIR}"/dest/bin/schim.exe
endif
	@echo "Done."

qthelp:
	mkdir -p "${BUILD_DIR}/dest/${SHARE_DIR}"
	cd docs && "${MAKE}" qthelp
	@echo "Copying help files to destination directory..."
	@cp docs/_build/qthelp/Schim.{qhc,qch} "${BUILD_DIR}/dest/${SHARE_DIR}/"
	@echo "Help files successfully generated."

man:
	cd docs/ && "${MAKE}" man

# ┏━━━━━━━━━━━━┓
# ┃ DEPLOYMENT ┃
# ┗━━━━━━━━━━━━┛

install: man
	@echo -e "\033[1;35mInstallation directory is: ${DESTDIR}\e[0m"
	@echo "Copying files to installation directory..."
	@umask 022
	@mkdir -p "${DESTDIR}/${PREFIX}/${MAN_DIR}" \
			  "${DESTDIR}/${PREFIX}/${DOC_DIR}"
	@cp -r "${BUILD_DIR}"/dest/** "${DESTDIR}/${PREFIX}" 2>/dev/null || true
	@cp docs/_build/man/*.1* "${DESTDIR}/${PREFIX}/${MAN_DIR}" 				# Manpages
	@cp -r res/examples "${DESTDIR}/${PREFIX}/${SHARE_DIR}"    				# Examples
	@cp -r res/*.conf "${DESTDIR}/${PREFIX}/${SHARE_DIR}/"     				# Config files
	@umask 222 # License file should be read-only
	@cp LICENSE "${DESTDIR}/${PREFIX}/${DOC_DIR}"           				# LICENSE

uninstall:
	rm -rf \
		"${DESTDIR}/${PREFIX}/bin/schim" \
		"${DESTDIR}/${PREFIX}/${ICON_DIR}/apps/schim.svg" \
		"${DESTDIR}/${PREFIX}/${SHARE_DIR}" \
		"${DESTDIR}/${PREFIX}/${DOC_DIR}" \
		"${DESTDIR}/${PREFIX}/${MAN_DIR}"/schim*.1

package:
	@echo "Removing previous archive..."
	@rm -rf "${BUILD_DIR}/package"
	@rm -rf _build/schim-"${VERSION}"-"${DEPLOY_TARGET}".tar.gz 
	@"${MAKE}" install DESTDIR="${BUILD_DIR}/package"
	@echo "Creating tar archive..."
	@DEST="${BUILD_DIR}/package"; \
	tar --transform "s:$$DEST/::" \
		-cf _build/schim-"${VERSION}"-"${DEPLOY_TARGET}".tar.gz \
		"$$DEST"/*
	@echo -e "\033[1;32mDone. Output is in _build.\e[0m"

clean:
	rm -rf _build/

clean-all: clean
	cd docs && "${MAKE}" clean
