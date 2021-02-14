# Minimal makefile for Sphinx documentation
#

# You can set these variables from the command line, and also
# from the environment for the first two.
SPHINXOPTS    ?=
SPHINXBUILD   ?= sphinx-build
SOURCEDIR     = .
BUILDDIR      = _build

# Put it first so that "make" without argument is like "make help".
help:
	@$(SPHINXBUILD) -M help "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

.PHONY: help Makefile man html

all: man html qthelp pdf

html:
	@scripts/gen-tree
	@$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS)

latex: doxygen
	@scripts/gen-tree
	@$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS)

qthelp:
	@${SPHINXBUILD} -M $@ "${SOURCEDIR}/manual" "${BUILDDIR}" ${SPHINXOPTS}
	cd _build/qthelp && qhelpgenerator Schim.qhcp

man:
ifneq (${OS},Windows_NT)
	mkdir -p _build/man
	@for file in man/*.rst; do \
		outname="$$(echo "$$file" | sed 's:\.rst::g')"; \
		text="rst2man \"$$file\" > \"_build/$$outname.1.gz\""; \
		echo $$text; \
		eval $$text; \
	done
endif

# This target is will be built from conf.py
doxygen:
	@mkdir -p _build/doxygen
	@doxygen
ifneq (${OS},Windows_NT) # TODO reinstate this again on Windows
	@for file in _build/doxygen/html/class*__inherit__graph.svg; do \
		cp "$$file" "$$(echo "$$file" | sed 's/inherit__graph/inh/')"; \
	done
endif

pdf: latex
	cd _build/latex/ && pdflatex -interaction=nonstopmode schim.tex

clean:
	rm -rf _build/

