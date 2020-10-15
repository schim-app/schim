
app:
	mkdir -p _build/release
	cd _build/release; qmake ../../src/schim.pro; make

docs:
	cd docs; make html

all: app docs

clean:
	rm -rf _build/*
