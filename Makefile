PARSEC_DIR = /home/joseph/parsec/build/install
PTG = $(PARSEC_DIR)/bin/parsec-ptgpp
CC = mpicc
CFLAGS = -g 
LDFLAGS = -lm 
INCS = -I. -I$(PARSEC_DIR)/include/
LIBS = -L$(PARSEC_DIR)/lib -lparsec #-L/usr/lib/x86_64-linux-gnu/ -lblas 

JDFFILES = \
	cache

CFILES = \

JDFFILES_CFILES = $(addsuffix .c,$(JDFFILES))
JDFFILES_HFILES = $(addsuffix .h,$(JDFFILES))
JDFFILES_OFILES = $(addsuffix .o,$(JDFFILES))
CFILES_OFILES   = $(addsuffix .o,$(CFILES))


all: $(CFILES_OFILES)  $(JDFFILES) $(JDFFILES_CFILES) $(JDFFILES_OFILES) $(JDFFILES_HFILES) 

%.c : %.jdf
	$(PTG) -E -i $< -o $(basename $<) -f $(basename $<) 

%.o: %.c
	$(CC) $(CFLAGS) $(INCS) -o $@ -c $< $(INCS)  $(LIBS) $(LDFLAGS)

cache : cache.o
	$(CC)  cache.o -o cache $(LIBS) $(LDFLAGS)



.PHONY : clean
clean: 
	rm -rf $(JDFFILES_CFILES) $(JDFFILES_HFILES) $(JDFFILES) *.o
