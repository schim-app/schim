
SCHIM_EXECUTABLE ?= 
ifeq (${SCHIM_EXECUTABLE},)
  ifneq (${MAKECMDGOALS},clean)
    $(error Variable SCHIM_EXECUTABLE must be specified!)
  endif
endif

MINIMAL_XML = ../res/examples/minimal.schim

.PHONY: cli-export

tests: cli-export

cli-export: 
	mkdir -p _build/
	"${SCHIM_EXECUTABLE}" export ${MINIMAL_XML} -o _build/minimal_autofmt.pdf
	"${SCHIM_EXECUTABLE}" export ${MINIMAL_XML} --format=pdf -o _build/minimal_manualfmt.pdf
	cat ${MINIMAL_XML} | "${SCHIM_EXECUTABLE}" export - -o _build/minimal_cat_o.pdf
	"${SCHIM_EXECUTABLE}" export ${MINIMAL_XML} -f pdf > _build/minimal_redirect.pdf
	cat ${MINIMAL_XML} | "${SCHIM_EXECUTABLE}" export --format pdf - > _build/minimal_cat_redirect.pdf

clean:
	rm -rf _build/
