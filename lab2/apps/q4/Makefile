default:
	cd makeprocs; make
	cd producer; make
	cd consumer; make

clean:
	cd makeprocs; make clean

run:
	cd ../../bin; dlxsim -x os.dlx.obj -a -u makeprocs.dlx.obj 1; ee469_fixterminal
	cd ../../bin; dlxsim -x os.dlx.obj -a -u makeprocs.dlx.obj 2; ee469_fixterminal
	cd ../../bin; dlxsim -x os.dlx.obj -a -u makeprocs.dlx.obj 3; ee469_fixterminal
	cd ../../bin; dlxsim -x os.dlx.obj -a -u makeprocs.dlx.obj 4; ee469_fixterminal
	cd ../../bin; dlxsim -x os.dlx.obj -a -u makeprocs.dlx.obj 5; ee469_fixterminal
	cd ../../bin; dlxsim -x os.dlx.obj -a -u makeprocs.dlx.obj 10; ee469_fixterminal
