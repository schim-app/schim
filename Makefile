
TYPE ?= Release
VERSION = 0.0.1
SHELL = '/usr/bin/bash'

# Directories
INSTALL_DIR   ?= /usr/local
DEPLOY_TARGET ?= linux-x86_64
BUILD_DIR 	   = _build/$(shell [ "${TYPE}" = Release ] && echo release || echo debug)
ICON_DIR       = share/icons/hicolor/scalable
SHARE_DIR      = share/schim
SCHIM_ICON_DIR = share/schim/icons
MAN_DIR	       = share/man/man1
DOC_DIR	       = share/doc/schim

JOBS = $(shell echo ${MAKEFLAGS} | sed -n 's_.*\(-j\|--jobs=\) *\([0-9][0-9]*\).*_\2_p')

.PHONY: app qthelp man install uninstall

################
# CMAKE CONFIG #
################

ifeq (${OS},Windows_NT)
    CMAKE = VERSION=${VERSION} cmake -G "Unix Makefiles" \
		-DCMAKE_BUILD_TYPE=${TYPE} ../../src/
else
    CMAKE = VERSION=${VERSION} cmake -DCMAKE_BUILD_TYPE=${TYPE} ../../src/
endif
CMAKE_BUILD = cmake --build ./ -j ${JOBS}

###########
# COMPILE #
###########

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
	@cp misc/schim.sh "${BUILD_DIR}/"										 # Launch script
	@cp res/img/icon.svg "${BUILD_DIR}/dest/${ICON_DIR}/apps/schim.svg"		 # Icon
	@cp res/img/actions/*.svg "${BUILD_DIR}/dest/${SCHIM_ICON_DIR}/actions/" # Action icons
	@cp -r res/symb/* "${BUILD_DIR}/dest/${SHARE_DIR}/symb/"				 # Symbols
	@cp res/ATTRIBUTION.md "${BUILD_DIR}/dest/${DOC_DIR}/"					 # ATTRIBUTION.md
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
	mkdir -p "${BUILD_DIR}/dest/${SHARE_DIR}"
	cd docs/ && make man

##############
# DEPLOYMENT #
##############

install: app man
	@echo "Copying files to installation directory..."
	@umask 022
	@mkdir -p "${INSTALL_DIR}/${MAN_DIR}"
	@# INSTALL_DIR can be the same as BUILD_DIR
	@cp -r "${BUILD_DIR}"/dest/** "${INSTALL_DIR}" 2>/dev/null || true
	@cp docs/_build/man/*.1* "${INSTALL_DIR}/${MAN_DIR}" # Manpages
	@cp -r res/examples "${INSTALL_DIR}/${SHARE_DIR}"    # Examples
	@cp -r res/*.conf "${INSTALL_DIR}/${SHARE_DIR}/"     # Config files

uninstall:
	rm -rf \
		"${INSTALL_DIR}/bin/schim" \
		"${INSTALL_DIR}/${ICON_DIR}/apps/schim.svg" \
		"${INSTALL_DIR}/${DOC_DIR}" \
		"${INSTALL_DIR}/${SHARE_DIR}" \
		"${INSTALL_DIR}/${MAN_DIR}"

ifeq (${MAKECMDGOALS},package)
    INSTALL_DIR = "${BUILD_DIR}"/dest # Used with package target
endif

package: install
	@echo "Removing previous archive..."
	@rm -rf _build/schim-"${VERSION}"-"${DEPLOY_TARGET}".tar.gz 
	@echo "Creating tar archive..."
	@INSTALL_DIR="${INSTALL_DIR}"; \
	tar --transform "s:$$INSTALL_DIR/::" \
		-cf _build/schim-"${VERSION}"-"${DEPLOY_TARGET}".tar.gz \
		"${INSTALL_DIR}"/*
	@echo -e "\033[1;32mDone. Output is in _build.\e[0m"

clean:
	rm -rf _build/

clean-all: clean
	cd docs && "${MAKE}" clean
