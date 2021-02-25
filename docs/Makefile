
SPHINXOPTS    ?=
SPHINXBUILD   ?= sphinx-build
SOURCEDIR     = .
BUILDDIR      = _build

# Should be /bin/bash for ReadTheDocs
SHELL = '/bin/bash'

# Put it first so that "make" without argument is like "make help".
help:
	@${SPHINXBUILD} -M help "${SOURCEDIR}" "${BUILDDIR}" ${SPHINXOPTS} ${O}

.PHONY: help Makefile man html qthelp pdf latex

################################################################################
#                       Targets that build documentation                       #
################################################################################

all: man html qthelp pdf prepare-man graphs

html: prepare-man doxygen
	@echo "Generate directory tree file."
	@scripts/gen-tree.sh
	@${SPHINXBUILD} -M $@ "${SOURCEDIR}" "${BUILDDIR}" ${SPHINXOPTS}
	@"${MAKE}" graphs

latex:
	@${SPHINXBUILD} -M $@ "${SOURCEDIR}/manual" "${BUILDDIR}" ${SPHINXOPTS}

# TODO Build man here?
qthelp:
	@${SPHINXBUILD} -M $@ "${SOURCEDIR}/manual" "${BUILDDIR}" ${SPHINXOPTS}
	cd _build/qthelp && qhelpgenerator Schim.qhcp

# Build manpages using Sphinx, configured in the man/ directory
man:
ifneq (${OS},Windows_NT)
	@${SPHINXBUILD} -M $@ "${SOURCEDIR}/man" "${BUILDDIR}" ${SPHINXOPTS}
endif

pdf: latex
	cd _build/latex/ && pdflatex -interaction=nonstopmode schim.tex

################################################################################
#                            Intermediate targets                              #
################################################################################

doxygen:
	@mkdir -p _build/{html/doxygen,doxygen/xml}
	@doxygen

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

graphs:
	@echo "Creating DOT graphs..."
	@PREFIX="$$(pwd)"; \
	shopt -s globstar nullglob; \
	for f in $$(find manual/ dev/ -name "*.dot" -type f); do \
	    DEST="_build/html/$${f/.dot/}.svg"; \
	    mkdir -p "$$(dirname "$$DEST")" && \
	    dot -Tsvg "$$f" -o "$$DEST"; \
	done
	@echo "Done creating graphs."

################################################################################

clean:
	rm -rf _build/
	rm -rf _intermediate/

