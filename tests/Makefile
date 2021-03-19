
# TODO remove probably
#SCHIM_EXECUTABLE ?= 
#ifeq (${SCHIM_EXECUTABLE},)
#  ifneq (${MAKECMDGOALS},clean)
#    $(error Variable SCHIM_EXECUTABLE must be specified!)
#  endif
#endif
JOBS = $(shell echo ${MAKEFLAGS} | sed -n 's_.*\(-j\|--jobs=\) *\([0-9][0-9]*\).*_\2_p')

MINIMAL_XML = ../res/examples/minimal.schim

export QT_QPA_PLATFORMTHEME := qt

.PHONY: tests

tests: 
	@echo "Building unittests executable"
	@mkdir -p _build/cov
	@cd _build && cmake .. && cmake --build . -j ${JOBS}

# TODO The rest of the file shall be moved somewhere else

cli: cli-info cli-export

cli-info:
	"${SCHIM_EXECUTABLE}" -h
	"${SCHIM_EXECUTABLE}" --help
	"${SCHIM_EXECUTABLE}" -v
	"${SCHIM_EXECUTABLE}" --version
	"${SCHIM_EXECUTABLE}" --help --version

cli-export: 
	mkdir -p _build/
	"${SCHIM_EXECUTABLE}" export --help
	"${SCHIM_EXECUTABLE}" export ${MINIMAL_XML} -o _build/minimal_autofmt.pdf
	"${SCHIM_EXECUTABLE}" export ${MINIMAL_XML} --format=pdf -o _build/minimal_manualfmt.pdf
	cat ${MINIMAL_XML} | "${SCHIM_EXECUTABLE}" export - -o _build/minimal_cat_o.pdf
	"${SCHIM_EXECUTABLE}" export ${MINIMAL_XML} -f pdf > _build/minimal_redirect.pdf
	cat ${MINIMAL_XML} | "${SCHIM_EXECUTABLE}" export --format pdf - > _build/minimal_cat_redirect.pdf

clean:
	rm -rf _build/
