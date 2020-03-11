default:
	cd makeprocs; make
	cd producerS; make
	cd producerCO; make
	cd react1; make
	cd react2; make
	cd react3; make

clean:
	cd makeprocs; make clean

run:
	cd ../../bin; dlxsim -x os.dlx.obj -a -u makeprocs.dlx.obj 5 7; ee469_fixterminal
