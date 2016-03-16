INSTALL_PATH=~cs537-1/ta/tests/1a-contest
PYTHON_PATH=python

all: runtests gen1 gen2 gen3

runtests:
	echo "#!/bin/bash" > $@
	echo "base=$(INSTALL_PATH)" >> $@
	echo "$(PYTHON_PATH) \$$base/project1a-contest.py --test-path \$$base \$$@"\
		" | tee runtests_contest.log" >> $@
	echo "exit \$$?" >> $@
	chmod a+x runtests

install: runtests generate
	mkdir -p $(INSTALL_PATH)
	cp -r * $(INSTALL_PATH)
	afs_rseta $(INSTALL_PATH) system:anyuser read

check:
	pychecker --only *.py

clean:
	rm -f runtests
	rm -f *.pyc
	rm -f gen1
	rm -f gen2
	rm -f gen3

gen1: gen1.cpp
	g++ -O -Wall gen1.cpp -o gen1 -std=c++11

gen2: gen2.cpp
	g++ -O -Wall gen2.cpp -o gen2 -std=c++11

gen3: gen3.cpp
	g++ -O -Wall gen3.cpp -o gen3 -std=c++11
