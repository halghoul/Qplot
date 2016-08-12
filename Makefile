#
#
#
# Makefile for PWA subsystem
#



CPP= g++ 
F77= gfortran

DEBUG = -g
COPT =  -O2
SYSLIB =	 -lm
FLIB = -L/lib -lgfortran -lstdc++ 

CERNLIBS =   -L$(CERNLIB)   -lpacklib -lmathlib -lkernlib 
FINCLUDE =  -Bstatic -fno-second-underscore


INCLUDE= -I. -I../include -DFUNCPROTO

all: Qplot 


QplotDict.o:QplotDict.cc
	g++  `root-config --cflags`  -c -o QplotDict.o QplotDict.cc 

QplotDict.cc:Qplot_Gui.h QplotLinkDef.h 
	rootcint -f QplotDict.cc -c Qplot_Gui.h QplotLinkDef.h 

Qplot:Qplot.o Qplot_Gui.o QplotDict.o
	g++  `root-config --glibs` -o Qplot Qplot.o Qplot_Gui.o QplotDict.o

Qplot_Gui.o : Qplot_Gui.h Qplot_Gui.cc
	g++ `root-config --cflags` $(DEBUG) $(INCLUDE)  -c Qplot_Gui.cc


Qplot.o : Qplot_Gui.h Qplot.cc
	 g++  $(INCLUDE) $(DEBUG)  `root-config --cflags` -c Qplot.cc
 




# clean up
clean:
	rm *.o 


# End of file

