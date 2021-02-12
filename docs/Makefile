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
ifdef (${OS},UNIX)
	mkdir -p _build/man
	@for file in man/*.rst; do \
		outname="$$(echo "$$file" | sed 's:\.rst::g')"; \
		text="rst2man \"$$file\" > \"_build/$$outname.1.gz\""; \
		echo $$text; \
		eval $$text; \
	done
endif

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
	@echo _build/doxygen/html/class*__inherit__graph.svg
ifdef (${OS},UNIX)
	@for file in _build/doxygen/html/class*__inherit__graph.svg; do \
		mv "$$file" "$$(echo "$$file" | sed 's/inherit__graph/inh/')"; \
	done
	@scripts/rename _build/doxygen/html
endif
