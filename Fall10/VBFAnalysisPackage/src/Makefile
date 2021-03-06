HDR      = ../interface/
OBJ      = ../obj/
LIB      = ../lib/
BIN      =  ../bin/
PRG      =  ../test/

HdrSuf  =  .h
SrcSuf  =  .cc
ObjSuf  =  .o
PrgSuf  =  .cpp
BinSuf  =  .exe
LibSuf  =  .so

HDRS      =  $(wildcard $(HDR)*$(HdrSuf)) $(wildcard $(HDR)/*/*$(HdrSuf))
SRCS      =  $(wildcard *$(SrcSuf)) $(wildcard */*$(SrcSuf))
_OBJS     =  $(patsubst %$(SrcSuf), %$(ObjSuf), $(SRCS))
OBJS      =  $(addprefix $(OBJ),$(_OBJS))
PRGS      =  $(wildcard $(PRG)*$(PrgSuf))
_BINS     =  $(wildcard $(PRG)*$(PrgSuf))
__BINS    =  $(_BINS:$(PrgSuf)=$(BinSuf))
___BINS   =  $(notdir $(__BINS))
BINS	  =  $(addprefix $(BIN),${___BINS})
LINKDEF   =  $(wildcard ${HDR}*LinkDef.h)
DICTHDRS  =  $(patsubst $(LINKDEF),,$(HDRS)) $(LINKDEF)



ROOTCFLAGS    = $(shell root-config --cflags)
ROOTGLIBS     = $(shell root-config --glibs)

NTUPLEPACKAGEFLAGS = $(shell ntuplepkg-config --include)
NTUPLEPACKAGELIBS = $(shell ntuplepkg-config --libs)

KINFITTERPACKAGEFLAGS = $(shell kinfitpkg-config --include)
KINFITTERPACKAGELIBS = $(shell kinfitpkg-config --libs)



ARCHL = -m64

CXX  =  c++
CXXFLAGS  =  -Wall -O -fPIC -I$(HDR) $(ROOTCFLAGS) $(NTUPLEPACKAGEFLAGS) $(KINFITTERPACKAGEFLAGS)

CPP  =  c++
CPPFLAGS  = -Wall -I$(HDR) $(ROOTCFLAGS) $(NTUPLEPACKAGEFLAGS) $(KINFITTERPACKAGEFLAGS)

LD       =  c++
LDFLAGS  =  -rdynamic -shared $(ARCHL)
SONAME	 =  libAnalysisPackage.so
SOFLAGS  =  -Wl,-soname,$(SONAME)

GLIBS    =  $(ROOTGLIBS) -lGenVector -lFoam -lMinuit -lRooFitCore -lRooFit -lMLP -lXMLIO -lTMVA -lTreePlayer $(NTUPLEPACKAGELIBS) $(KINFITTERPACKAGELIBS)


#################################################
#if mac
ARCH  =  $(shell root-config --arch)
ifeq ($(ARCH),macosx)
LibSuf  =  .dylib

ARCHL = -m32

CPPFLAGS  =  -Wall -W -Woverloaded-virtual -O2 -pipe -I$(HDR) $(ROOTCFLAGS) $(NTUPLEPACKAGEFLAGS) $(KINFITTERPACKAGEFLAGS)

CXXFLAGS  =  -Wall -W -Woverloaded-virtual -O2 -pipe -I$(HDR) $(ROOTCFLAGS) $(NTUPLEPACKAGEFLAGS) $(KINFITTERPACKAGEFLAGS)

LDFLAGS  =  -dynamiclib -shared -single_module -undefined dynamic_lookup $(ARCHL)
SONAME	 =  libAnalysisPackage.dylib
SOFLAGS  =
endif
#################################################

#################################################
#if mac 64
ARCH  =  $(shell root-config --arch)
ifeq ($(ARCH),macosx64)
LibSuf  =  .dylib

ARCHL = -m64

CPPFLAGS  =  -Wall -W -Woverloaded-virtual -O2 -pipe -I$(HDR) $(ROOTCFLAGS) $(NTUPLEPACKAGEFLAGS) $(KINFITTERPACKAGEFLAGS)

CXXFLAGS  =  -Wall -W -Woverloaded-virtual -O2 -pipe -I$(HDR) $(ROOTCFLAGS) $(NTUPLEPACKAGEFLAGS) $(KINFITTERPACKAGEFLAGS)

LDFLAGS  =  -dynamiclib -shared -single_module -undefined dynamic_lookup $(ARCHL)
SONAME	 =  libAnalysisPackage.dylib
SOFLAGS  =
endif
#################################################





.PHONY: all exe test clean


all: $(LIB)$(SONAME)


exe: $(BINS) 


test:
	@echo ""
	@echo "HDRS = $(HDRS)"
	@echo ""
	@echo "DICTHDRS = $(DICTHDRS)"
	@echo ""
	@echo "SRCS = $(SRCS)"
	@echo ""
	@echo "OBJS = $(OBJS)"
	@echo ""
	@echo "PRGS = $(PRGS)"
	@echo ""
	@echo "BINS = $(BINS)"
	@echo ""


$(OBJ)%$(ObjSuf): %$(SrcSuf) $(HDRS)
	$(CXX) -c $(CXXFLAGS) -o $@ $< 


$(OBJ)mydict.o: $(OBJ)mydict.cc $(OBJ)mydict.h
	$(CXX) -c $(CXXFLAGS) -o $@ $<


$(OBJ)mydict.cc: $(DICTHDRS)
	@echo "Generating dictionary for  ..."
	rootcint -f $(OBJ)mydict.cc -c -p ${CXXFLAGS} $(DICTHDRS)

$(LIB)$(SONAME): $(OBJS) $(OBJ)mydict.o
	@echo "Linking $(SONAME):"
	$(LD) $(LDFLAGS) $(OBJS) $(OBJ)mydict.o $(SOFLAGS) -o $(LIB)$(SONAME)

$(BIN)%$(BinSuf): $(PRG)%$(PrgSuf) $(HDRS) $(LIB)$(SONAME)
	$(CPP) $(CPPFLAGS) $(GLIBS) -L$(LIB) -lAnalysisPackage  -o $@ $<


clean:
	rm -f $(OBJ)*$(ObjSuf) $(LIB)*$(LibSuf) $(OBJ)mydict* $(BIN)*$(BinSuf)
