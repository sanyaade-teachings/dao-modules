# Generated by DaoMake: DO NOT EDIT!
# Targeting platform macosx.

all: libffi.dylib libffi.a

.PHONY: test
test: libffi.dylib libffi.a subtest 


DAOMAKE = /Users/min/Projects/dao/tools/daomake/daomake


HEADERS_F179 = include/ffi.h include/fficonfig.h include/ffi_common.h src/x86/ffitarget.h

CFLAGS_B91A =-O2 -Iinclude/ -Isrc/x86/ 


LFLAGS_666A =-dynamiclib -install_name @rpath/libffi.dylib -undefined dynamic_lookup


DaoMake.Objs/closures.c.27AA.o: src/closures.c $(HEADERS_F179)
	$(CC) $(CFLAGS_B91A) -c src/closures.c -o DaoMake.Objs/closures.c.27AA.o

DaoMake.Objs/darwin64.S.0F0E.o: src/x86/darwin64.S $(HEADERS_F179)
	$(AS) src/x86/darwin64.S -o DaoMake.Objs/darwin64.S.0F0E.o

DaoMake.Objs/debug.c.30E7.o: src/debug.c $(HEADERS_F179)
	$(CC) $(CFLAGS_B91A) -c src/debug.c -o DaoMake.Objs/debug.c.30E7.o

DaoMake.Objs/dlmalloc.c.A0C5.o: src/dlmalloc.c $(HEADERS_F179)
	$(CC) $(CFLAGS_B91A) -c src/dlmalloc.c -o DaoMake.Objs/dlmalloc.c.A0C5.o

DaoMake.Objs/ffi64.c.F769.o: src/x86/ffi64.c $(HEADERS_F179)
	$(CC) $(CFLAGS_B91A) -c src/x86/ffi64.c -o DaoMake.Objs/ffi64.c.F769.o

DaoMake.Objs/java_raw_api.c.C6DD.o: src/java_raw_api.c $(HEADERS_F179)
	$(CC) $(CFLAGS_B91A) -c src/java_raw_api.c -o DaoMake.Objs/java_raw_api.c.C6DD.o

DaoMake.Objs/prep_cif.c.065F.o: src/prep_cif.c $(HEADERS_F179)
	$(CC) $(CFLAGS_B91A) -c src/prep_cif.c -o DaoMake.Objs/prep_cif.c.065F.o

DaoMake.Objs/raw_api.c.71D8.o: src/raw_api.c $(HEADERS_F179)
	$(CC) $(CFLAGS_B91A) -c src/raw_api.c -o DaoMake.Objs/raw_api.c.71D8.o

DaoMake.Objs/types.c.EF86.o: src/types.c $(HEADERS_F179)
	$(CC) $(CFLAGS_B91A) -c src/types.c -o DaoMake.Objs/types.c.EF86.o


OBJECTS_3FCD =DaoMake.Objs/closures.c.27AA.o DaoMake.Objs/dlmalloc.c.A0C5.o DaoMake.Objs/prep_cif.c.065F.o DaoMake.Objs/types.c.EF86.o DaoMake.Objs/debug.c.30E7.o DaoMake.Objs/java_raw_api.c.C6DD.o DaoMake.Objs/raw_api.c.71D8.o DaoMake.Objs/darwin64.S.0F0E.o DaoMake.Objs/ffi64.c.F769.o


libffi.dylib: $(OBJECTS_3FCD)
	$(CC) $(OBJECTS_3FCD) $(LFLAGS_666A)  -o libffi.dylib

libffi.a: $(OBJECTS_3FCD)
	-@$(DAOMAKE) remove libffi.a
	ar rcs libffi.a $(OBJECTS_3FCD)

subtest:

.PHONY: subtest

TESTSUM =/Users/min/Projects/dao/modules/clinker/daotest_result_summary.txt

testsum:

.PHONY: testsum

install:


uninstall:


.PHONY: install uninstall

clean:
	$(DAOMAKE) remove $(OBJECTS_3FCD)

.PHONY: clean

distclean:
	$(DAOMAKE) remove DaoMake.Objs

.PHONY: distclean

