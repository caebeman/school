INSTALL_PATH:=~cs537-1/ta/tests/1a
PYTHON_PATH=python

#all: runtests dupdata generate
all: runtests

runtests:
	echo "#!/bin/bash" > $@
	echo "base=$(INSTALL_PATH)" >> $@
	echo "$(PYTHON_PATH) \$$base/project1a.py --test-path \$$base \$$@"\
		" | tee runtests_1a.log" >> $@
	echo "exit \$$?" >> $@
	chmod a+x runtests

install: runtests dupdata generate
	mkdir -p $(INSTALL_PATH)
	cp -r * $(INSTALL_PATH)
	afs_rseta $(INSTALL_PATH) system:anyuser read

check:
	pychecker --only *.py

clean:
	rm -f runtests
	rm -f *.pyc
