all: update wrest

update:
	cd wclasses && git submodule update --init --remote wondermacros/
	touch .flag

wrest: .flag
	cd wrest && make
	rm -f .flag
