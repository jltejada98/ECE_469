default:
	cd makeprocs; make
	cd hello_world; make
	cd part1; make

clean:
	cd makeprocs; make clean
	cd hello_world; make clean
	cd part1; make clean


run1:
	cd ../../bin; dlxsim -x os.dlx.obj -a -u makeprocs.dlx.obj 1; ee469_fixterminal

runh:
	cd ../../bin; dlxsim -x os.dlx.obj -a -u makeprocs.dlx.obj -1; ee469_fixterminal
