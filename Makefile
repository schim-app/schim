
TYPE ?= Release
SHELL = '/usr/bin/bash'

# Directories
INSTALL_DIR ?= /usr/local
BUILD_DIR 	 = _build/$(shell [ "${TYPE}" = Release ] && echo release || echo debug)
ICON_DIR     = share/icons/hicolor/scalable
SHARE_DIR    = share/schim
MAN_DIR	     = share/man/man1

JOBS = $(shell echo ${MAKEFLAGS} | sed -n 's_.*\(-j\|--jobs=\) *\([0-9][0-9]*\).*_\2_p')

.PHONY: app qthelp man install uninstall

################
# CMAKE CONFIG #
################

ifeq (${OS},Windows_NT)
    CMAKE = cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=${TYPE} ../../src/
else
    CMAKE = cmake -DCMAKE_BUILD_TYPE=${TYPE} ../../src/
endif
CMAKE_BUILD = cmake --build ./ -j ${JOBS}

###########
# COMPILE #
###########

# This target will create a directory tree under ${BUILD_DIR}/dest
# that can be moved to /usr/ or /usr/local using `make install`
app: qthelp
	@if [ -z "$$(ls src/dxflib/)" ]; then { \
	echo -e "\033[0;31m" \
	        "ERROR: Directory src/dxflib is empty." \
	        "Have you cloned with --recurse-submodules?" "\e[0m"; \
	         false; }; fi
	mkdir -p "${BUILD_DIR}"
	cd "${BUILD_DIR}"; ${CMAKE} && ${CMAKE_BUILD}
	@# Setup directory structure
	cd "${BUILD_DIR}"; \
	mkdir -p dest/bin \
		"dest/${ICON_DIR}/apps" \
		"dest/${SHARE_DIR}/symb"; \
	cp schim dest/bin/
	@# Create launcher and copy resources
	cp misc/schim.sh "${BUILD_DIR}/"; \
	cp res/img/icon.svg "${BUILD_DIR}/dest/${ICON_DIR}/apps/schim.svg"; \
	cp -r res/symb/* "${BUILD_DIR}/dest/${SHARE_DIR}/symb/"
ifeq (${OS},Windows_NT)
	windeployqt "${BUILD_DIR}"/dest/bin/schim.exe
endif

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
	umask 022
	mkdir -p "${INSTALL_DIR}"
	mkdir -p "${INSTALL_DIR}/${MAN_DIR}"
	cp -r "${BUILD_DIR}"/dest/* "${INSTALL_DIR}"
	cp -r docs/_build/man/*.1* "${INSTALL_DIR}/${MAN_DIR}"

uninstall:
	rm -rf \
		"${INSTALL_DIR}/bin/schim" \
		"${INSTALL_DIR}/${ICON_DIR}/apps/schim.svg" \
		"${INSTALL_DIR}/${SHARE_DIR}" \
		"${INSTALL_DIR}/${MAN_DIR}"

clean:
	rm -rf _build/

clean-all: clean
	cd docs && "${MAKE}" clean
