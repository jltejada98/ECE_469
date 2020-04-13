os:
	cd os ; make clean; make; cd ..

helloworld:
	cd apps/example/helloworld; make clean; make; make run

all:
	git reset --hard; git pull
	cd os ; make clean; make; cd ..
	cd apps/example/helloworld; make clean; make; make run
