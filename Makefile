
INSTALL_DIR ?= /usr/local

ICON_DIR=share/icons/hicolor/scalable
SHARE_DIR=share/schim
MAN_DIR=share/man/man1

# This target will create a directory tree within _build/release/dest
# that can be moved to /usr/ or /usr/local using `make install`
app:
	mkdir -p _build/release
	cd _build/release; qmake ../../src/schim.pro; make
	@####
	cd _build/release; rm -rf dest/; \
	mkdir -p dest/bin \
		dest/${ICON_DIR}/apps \
		dest/${SHARE_DIR}/symb; \
	cp schim dest/bin/; \
	cp ../../img/icon.svg dest/${ICON_DIR}/apps/schim.svg; \
	cp -r ../../symb/* dest/${SHARE_DIR}/symb/

docs: Makefile
	cd docs; make all

all: app docs

install: app docs
	umask 022
	mkdir -p ${INSTALL_DIR}
	mkdir -p "${INSTALL_DIR}/${MAN_DIR}"
	cp -r _build/release/dest/* "${INSTALL_DIR}"
	cp -r docs/_build/man/* "${INSTALL_DIR}/${MAN_DIR}"

uninstall:
	rm -rf "${INSTALL_DIR}/bin/schim" \
		"${INSTALL_DIR}/${ICON_DIR}/apps/schim.svg" \
		"${INSTALL_DIR}/${SHARE_DIR}" \
		"${INSTALL_DIR}/${MAN_DIR}"

# TODO remove this?
package: app
	cd _build/release; \
	tar --transform 's_^dest_usr/local_' -cvf schim.pkg.tar.zst dest/

clean:
	rm -rf _build/*
