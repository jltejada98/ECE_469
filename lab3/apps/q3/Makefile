default:
	cd makeprocs; make
	cd nothing_proc; make

clean:
	cd makeprocs; make clean
	cd nothing_proc; make clean

run:
	cd ../../bin; dlxsim -x os.dlx.obj -a -u makeprocs.dlx.obj 3 5; ee469_fixterminal
