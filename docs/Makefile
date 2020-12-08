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

.PHONY: help Makefile

clean:
	rm -rf _build/

all: Makefile man html

man: Makefile
	mkdir -p _build/man
	@for file in man/*.rst; do \
		outname="$$(echo "$$file" | sed 's:\.rst::g')"; \
		text="rst2man \"$$file\" > \"_build/$$outname.1.gz\""; \
		echo $$text; \
		eval $$text; \
	done

# TODO the catch-all catches 'all' target as well
# TODO change back to catch-all once I figure it out

# Catch-all target: route all unknown targets to Sphinx using the new
# "make mode" option.  $(O) is meant as a shortcut for $(SPHINXOPTS).
html: Makefile doxygen
	@scripts/gen-tree
	@$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

doxygen:
	@mkdir -p _build/doxygen
	@doxygen
	@scripts/rename _build/doxygen/html
