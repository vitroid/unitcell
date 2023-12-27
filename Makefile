# use gmake
BIN		= unitcell unitrepeat
EXECUTABLES	= iceIcunit.pl iceIhunit.pl ice3unit.pl $(BIN)
CFLAGS  = -Wall -Werror -O
LDFLAGS = -lm
all: unitcell unitrepeat

clean:
	-rm $(BIN) *~ *.o @*
tarball: clean
	tar -h -C .. -zcf /tmp/unitcell.tar.gz UnitCell/Makefile UnitCell/unitcell.c UnitCell/iceIcunit.pl UnitCell/unitrepeat.c UnitCell/iceIhunit.pl UnitCell/ice3unit.pl UnitCell/xx

#通常とは違う向きのIce Icを作る。
@ice96c.ar3a::
	iceIcunit.pl 0.92 | unitrepeat 1 1 1   -2 1 1   0 -2 2 > $@
@iceIhfilm.ar3a:
	iceIhunit.pl 0.92 | unitrepeat 0 0 1   -2 -2 0   -3 1 0 > $@
#非常に薄いIceIhのフィルムを作成する。(6員環方向をz軸とする)
@iceIh531.ar3a:
	iceIhunit.pl 0.92 | unitrepeat 5 0 0    0 3 0    0 0 1 > $@
#立方体に近い構造を得る。
@iceIh535.ar3a:
	iceIhunit.pl 0.92 | unitrepeat 5 0 0    0 3 0    0 0 3 > $@
