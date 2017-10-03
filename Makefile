CC = gcc
LD = ld
RM = -rm
PERL = perl

CFLAGS = -g -m64

BANNER = @echo "\n=== $@\n"

MACH = $(shell uname -m)

AES = run_aes
AES_OBJS = aes.o aesp8-ppc.o

all: $(AES) $(CHACHA)

$(AES): $(AES_OBJS)
	$(BANNER)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(AES_OBJS) $(LFLAGS) $(LIBS)

%.S: %.pl
	$(BANNER)
	$(PERL) $< linux-$(MACH) > $@

%-ppc.o: %-ppc.s
	$(BANNER)
	$(CC) $(CFLAGS) $(INCLUDES) -c -mno-strict-align -nostdinc \
		-D__KERNEL__ -DHAVE_AS_ATHIGH=1 -D__ASSEMBLY__ -Iarch/powerpc \
		-mabi=elfv2 -Wa,-maltivec -Wa,-gdwarf-2  -DCC_HAVE_ASM_GOTO \
		$< -o $@

%.o: %.c
	$(BANNER)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(BANNER)
	$(RM) -f *.o *.s run_*

.PRECIOUS: %.s
