
SPHINXOPTS    ?=
SPHINXBUILD   ?= sphinx-build
SOURCEDIR     = .
BUILDDIR      = _build

# Put it first so that "make" without argument is like "make help".
help:
	@${SPHINXBUILD} -M help "${SOURCEDIR}" "${BUILDDIR}" ${SPHINXOPTS} ${O}

.PHONY: help Makefile man html

################################################################################
#                       Targets that build documentation                       #
################################################################################

all: man html qthelp pdf prepare-man

html: prepare-man doxygen
	@scripts/gen-tree
	@${SPHINXBUILD} -M $@ "${SOURCEDIR}" "${BUILDDIR}" ${SPHINXOPTS}

latex: doxygen
	@scripts/gen-tree
	@${SPHINXBUILD} -M $@ "${SOURCEDIR}" "${BUILDDIR}" ${SPHINXOPTS}

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
	@mkdir -p _build/doxygen
	@doxygen
ifneq (${OS},Windows_NT) # TODO reinstate this again on Windows
	@for file in _build/doxygen/html/class*__inherit__graph.svg; do \
		cp "$$file" "$$(echo "$$file" | sed 's/inherit__graph/inh/')"; \
	done
endif

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
	rm -rf _build/
	rm -rf _intermediate/

