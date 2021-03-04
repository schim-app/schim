
SPHINXOPTS    ?=
SPHINXBUILD   ?= sphinx-build
SOURCEDIR     = .
BUILDDIR      ?= _build

# Put it first so that "make" without argument is like "make help".
help:
	@${SPHINXBUILD} -M help "${SOURCEDIR}" "${BUILDDIR}" ${SPHINXOPTS} ${O}

.PHONY: help man html qthelp pdf latex

################################################################################
#                       Targets that build documentation                       #
################################################################################

all: man html qthelp pdf

html: prepare-man
	mkdir -p "${BUILDDIR}"/html
	@${SPHINXBUILD} -M $@ "${SOURCEDIR}" "${BUILDDIR}" ${SPHINXOPTS}

latex:
	echo BUILDDIR IS ${BUILDDIR}
	mkdir -p "${BUILDDIR}"/latex
	@${SPHINXBUILD} -M $@ "${SOURCEDIR}" "${BUILDDIR}" ${SPHINXOPTS}

qthelp:
	mkdir -p "${BUILDDIR}"/pdf
	@${SPHINXBUILD} -M $@ "${SOURCEDIR}" "${BUILDDIR}" ${SPHINXOPTS}
	cd "${BUILDDIR}"/qthelp && qhelpgenerator Schim.qhcp

# Build manpages using Sphinx, using its own man/conf.py
man:
ifneq (${OS},Windows_NT)
	@${SPHINXBUILD} -M $@ "${SOURCEDIR}/man" "${BUILDDIR}" ${SPHINXOPTS}
endif

pdf:
	@${SPHINXBUILD} -M latexpdf "${SOURCEDIR}" "${BUILDDIR}" ${SPHINXOPTS}

# Tweak manpage sources for inclusion in the HTML version of the docs.
# A NAME section containing the description is added to each file from man/.
# The reason why the NAME section is excluded from the source files in the first
# place is because `sphinx-build -M man` automatically creates that section.
prepare-man: man
	@echo Converting manpages back to rst...
	@mkdir -p _intermediate/man
	@for file in man/schim*.rst; do \
		scripts/prepare-man.sh "$$file" _intermediate/man; \
	done

################################################################################

clean:
	rm -rf "${BUILDDIR}"
	rm -rf _intermediate/

