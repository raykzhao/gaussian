CC=gcc
ifeq "$(CC)" "gcc"
    COMPILER=gcc
else ifeq "$(CC)" "clang"
    COMPILER=clang
endif

ARCHITECTURE=_AMD64_
ifeq "$(ARCH)" "x64"
    ARCHITECTURE=_AMD64_
endif

AVX2=-D _AVX2_ -mavx2

AR=ar rcs
RANLIB=ranlib

CFLAGS = -std=gnu11 -O3 -D $(ARCHITECTURE) $(AVX2) -D __LINUX__ -fomit-frame-pointer
LDFLAGS=-lm -L/usr/lib/ -lssl -lcrypto 

ifeq "$(CC)" "gcc"
    CFLAGS+= -march=native
endif

DFLAG=
ifeq "$(DEBUG)" "TRUE"
    DFLAG=-DDEBUG
endif

OBJECTS_ASM_III = objs_III/s_consts.o objs/poly_mul1024.o objs/sparse_mul16.o
OBJECTS_EXTRAS = objs/fips202x4.o objs/KeccakP-1600-times4-SIMD256.o objs/fastrandombytes.o

OBJECTS_III = objs_III/sign.o objs_III/pack.o objs_III/sample.o objs_III/gauss.o objs_III/poly.o objs_III/consts.o $(OBJECTS_ASM_III) objs/fips202.o objs/random.o $(OBJECTS_EXTRAS)
SOURCE_TEST = tests/cpucycles.c tests/test_qtesla.c
SOURCE_KATS_GEN  = tests/rng.c tests/PQCgenKAT_sign.c
SOURCE_KATS_TEST = tests/rng.c tests/PQCtestKAT_sign.c

all: lib_III tests

objs_III/%.o: %.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) $(DFLAG) -D _qTESLA_III_ $< -o $@

objs/random.o: random/random.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) random/random.c -o objs/random.o

objs/fips202.o: sha3/fips202.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) sha3/fips202.c -o objs/fips202.o
	
objs/fips202x4.o: sha3/fips202x4.c
	$(CC) -c $(CFLAGS) sha3/fips202x4.c -o objs/fips202x4.o

objs/KeccakP-1600-times4-SIMD256.o: sha3/keccak4x/KeccakP-1600-times4-SIMD256.c
	$(CC) -c $(CFLAGS) sha3/keccak4x/KeccakP-1600-times4-SIMD256.c -o objs/KeccakP-1600-times4-SIMD256.o

objs/fastrandombytes.o: fastrandombytes.c
	$(CC) -c $(CFLAGS) fastrandombytes.c -o objs/fastrandombytes.o

objs/poly_mul1024.o: poly_mul1024.S
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) poly_mul1024.S -o objs/poly_mul1024.o

objs/sparse_mul16.o: sparse_mul16.S
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) sparse_mul16.S -o objs/sparse_mul16.o

lib_III: $(OBJECTS_III)
	rm -rf lib_III
	mkdir lib_III
	$(AR) lib_III/libqtesla.a $^
	$(RANLIB) lib_III/libqtesla.a

tests: lib_III
	$(CC) $(CFLAGS) -L./lib_III -D _qTESLA_III_ $(SOURCE_TEST) $(DFLAG) -lqtesla $(LDFLAGS) -o test_qtesla-III $(ARM_SETTING)
	$(CC) $(CFLAGS) -L./lib_III -D _qTESLA_III_ $(SOURCE_KATS_GEN) $(DFLAG) -lqtesla $(LDFLAGS) -o PQCgenKAT_sign-III $(ARM_SETTING)
	$(CC) $(CFLAGS) -L./lib_III -D _qTESLA_III_ $(SOURCE_KATS_TEST) $(DFLAG) -lqtesla $(LDFLAGS) -o PQCtestKAT_sign-III $(ARM_SETTING)

.PHONY: clean

clean:
	rm -rf objs* lib* test_qtesla-* PQCgenKAT_sign-* PQCtestKAT_sign-* PQCsignKAT_qTesla*
