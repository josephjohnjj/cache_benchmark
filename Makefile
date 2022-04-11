PARSEC_DIR = /home/joseph/parsec/build/install
DAGUEPP = $(PARSEC_DIR)/bin/parsec-ptgpp
CC = mpicc
CFLAGS = -g 
LDFLAGS = -lm 
INCS = -I. -I$(PARSEC_DIR)/include/
LIBS = -L$(PARSEC_DIR)/lib -lparsec -L/usr/lib/x86_64-linux-gnu/ -lblas 

JDFFILES = \
	Bal_SDSG \
	Bal_LDLG \
	Bal_LDSG \
	Bal_SDLG

CFILES = \
	bal_data

JDFFILES_CFILES = $(addsuffix .c,$(JDFFILES))
JDFFILES_HFILES = $(addsuffix .h,$(JDFFILES))
JDFFILES_OFILES = $(addsuffix .o,$(JDFFILES))
CFILES_OFILES   = $(addsuffix .o,$(CFILES))


all: $(CFILES_OFILES)  $(JDFFILES) $(JDFFILES_CFILES) $(JDFFILES_OFILES) $(JDFFILES_HFILES)  

%.c : %.jdf
	$(DAGUEPP) -E -i $< -o $(basename $<) -f $(basename $<)

%.o: %.c
	$(CC) $(CFLAGS) $(INCS) -o $@ -c $<  

Bal_SDSG : Bal_SDSG.o bal_data.o
	$(CC)  Bal_SDSG.o bal_data.o -o Bal_SDSG $(LIBS) $(LDFLAGS)

Bal_LDLG : Bal_LDLG.o bal_data.o
	$(CC)  Bal_LDLG.o bal_data.o -o Bal_LDLG $(LIBS) $(LDFLAGS)

Bal_LDSG : Bal_LDSG.o bal_data.o
	$(CC)  Bal_LDSG.o bal_data.o -o Bal_LDSG $(LIBS) $(LDFLAGS)

Bal_SDLG : Bal_SDLG.o bal_data.o
	$(CC)  Bal_SDLG.o bal_data.o -o Bal_SDLG $(LIBS) $(LDFLAGS)

.PHONY : clean
clean: 
	rm -rf $(JDFFILES_CFILES) $(JDFFILES_HFILES) $(JDFFILES) *.o
