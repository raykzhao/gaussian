CC=gcc
ifeq "$(CC)" "gcc"
    COMPILER=gcc
else ifeq "$(CC)" "clang"
    COMPILER=clang
endif

ARCHITECTURE=_AMD64_
ifeq "$(ARCH)" "x64"
    ARCHITECTURE=_AMD64_
else ifeq "$(ARCH)" "x86"
    ARCHITECTURE=_X86_
else ifeq "$(ARCH)" "ARM"
    ARCHITECTURE=_ARM_
    ARM_SETTING=-lrt
else ifeq "$(ARCH)" "ARM64"
    ARCHITECTURE=_ARM64_
    ARM_SETTING=-lrt
endif

AR=ar rcs
RANLIB=ranlib

CFLAGS = -std=gnu11 -O3 -D $(ARCHITECTURE) -D __LINUX__ -fomit-frame-pointer
LDFLAGS=-lm -L/usr/lib/ -lssl -lcrypto 

ifeq "$(CC)" "gcc"
    CFLAGS+= -march=native
endif

DFLAG=
ifeq "$(DEBUG)" "TRUE"
    DFLAG=-DDEBUG
endif

OBJECTS_III_SPEED = objs_III_speed/sign.o objs_III_speed/pack.o objs_III_speed/sample.o objs_III_speed/poly.o objs_III_speed/consts.o objs_III_speed/fastrandombytes.o objs/fips202.o objs/random.o
SOURCE_TEST = tests/cpucycles.c tests/test_qtesla.c
SOURCE_KATS_GEN  = tests/rng.c tests/PQCgenKAT_sign.c
SOURCE_KATS_TEST = tests/rng.c tests/PQCtestKAT_sign.c

all: lib_III_speed tests

objs_III_speed/%.o: %.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) $(DFLAG) -D _qTESLA_III_speed_ $< -o $@

objs/random.o: random/random.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) random/random.c -o objs/random.o

objs/fips202.o: sha3/fips202.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) sha3/fips202.c -o objs/fips202.o

lib_III_speed: $(OBJECTS_III_SPEED)
	rm -rf lib_III_speed
	mkdir lib_III_speed
	$(AR) lib_III_speed/libqtesla.a $^
	$(RANLIB) lib_III_speed/libqtesla.a

tests: lib_III_speed
	$(CC) $(CFLAGS) -L./lib_III_speed -D _qTESLA_III_speed_ $(SOURCE_TEST) $(DFLAG) -lqtesla $(LDFLAGS) -o test_qtesla-III-speed $(ARM_SETTING)
	$(CC) $(CFLAGS) -L./lib_III_speed -D _qTESLA_III_speed_ $(SOURCE_KATS_GEN) $(DFLAG) -lqtesla $(LDFLAGS) -o PQCgenKAT_sign-III-speed $(ARM_SETTING)
	$(CC) $(CFLAGS) -L./lib_III_speed -D _qTESLA_III_speed_ $(SOURCE_KATS_TEST) $(DFLAG) -lqtesla $(LDFLAGS) -o PQCtestKAT_sign-III-speed $(ARM_SETTING)

.PHONY: clean

clean:
	rm -rf objs* lib* test_qtesla-* PQCgenKAT_sign-* PQCtestKAT_sign-* PQCsignKAT_qTesla*
