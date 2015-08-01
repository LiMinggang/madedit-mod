# Make file for GCC compiler on Linux or compatible OS
# The license.html file describes the conditions under which this software may be distributed.

# list of source files
SRC = astyle_main.cpp \
      ASBeautifier.cpp \
      ASFormatter.cpp \
      ASEnhancer.cpp \
      ASLocalizer.cpp \
      ASResource.cpp

# source directories
vpath %.cpp ../../src
vpath %.h   ../../src

# NOTE for java compiles the environment variable $JAVA_HOME must be set
# example: export JAVA_HOME=/usr/lib/jvm/java-6-sun-1.6.0.00
ifndef JAVA_HOME
    JAVA_HOME = /usr/lib/jvm/default-java
endif

# set prefix if not defined on the command line
ifndef prefix
    prefix=/usr
endif
SYSCONF_PATH=$(prefix)/share/doc/astyle

# define macros
bindir = bin
objdir = obj
ipath=$(prefix)/bin
CBASEFLAGS = -Wall -Wextra -fno-rtti -fno-exceptions
JAVAINCS   = -I$(JAVA_HOME)/include
CXX = g++
INSTALL=install -o $(USER) -g $(USER)

##################################################

# define compile options for each build
ifdef CFLAGS
    CFLAGSr   = -DNDEBUG $(CBASEFLAGS) $(CFLAGS)
    CFLAGSd   = -g $(CBASEFLAGS) $(CFLAGS)
else
    CFLAGSr   = -DNDEBUG -O3 $(CBASEFLAGS)
    CFLAGSd   = -g $(CBASEFLAGS)
endif
CFLAGSs   = -DASTYLE_LIB -fPIC -fvisibility=hidden $(CFLAGSr)
CFLAGSsd  = -DASTYLE_LIB -fPIC -fvisibility=hidden $(CFLAGSd)
CFLAGSa   = -DASTYLE_LIB $(CFLAGSr)
CFLAGSad  = -DASTYLE_LIB $(CFLAGSd)
CFLAGSsj  = -DASTYLE_JNI -fPIC -fvisibility=hidden $(CFLAGSr) $(JAVAINCS)
CFLAGSsjd = -DASTYLE_JNI -fPIC -fvisibility=hidden $(CFLAGSd) $(JAVAINCS)

# define link options
ifdef LDFLAGS
    LDFLAGSr   = $(LDFLAGS)
    LDFLAGSd   = $(LDFLAGS)
else
    LDFLAGSr   = -s
    LDFLAGSd   =
endif

# object files are built from the source list $(SRC)
# a suffix is added for each build
OBJ   = $(patsubst %.cpp,$(objdir)/%.o,$(SRC))
OBJd  = $(patsubst %.cpp,$(objdir)/%_d.o,$(SRC))
OBJs  = $(patsubst %.cpp,$(objdir)/%_s.o,$(SRC))
OBJsd = $(patsubst %.cpp,$(objdir)/%_sd.o,$(SRC))
OBJa  = $(patsubst %.cpp,$(objdir)/%_a.o,$(SRC))
OBJad = $(patsubst %.cpp,$(objdir)/%_ad.o,$(SRC))
OBJsj  = $(patsubst %.cpp,$(objdir)/%_sj.o,$(SRC))
OBJsjd = $(patsubst %.cpp,$(objdir)/%_sjd.o,$(SRC))

# define object file rule (with the suffix) for each build

# OBJ
$(objdir)/%.o:  %.cpp  astyle.h  astyle_main.h
	@ mkdir -p $(objdir)
	$(CXX) $(CFLAGSr) -c $< -o $@

# OBJd
$(objdir)/%_d.o:  %.cpp  astyle.h  astyle_main.h
	@ mkdir -p $(objdir)
	$(CXX) $(CFLAGSd) -c $< -o $@

# OBJs
$(objdir)/%_s.o:  %.cpp  astyle.h
	@ mkdir -p $(objdir)
	$(CXX) $(CFLAGSs) -c $< -o $@

# OBJsd
$(objdir)/%_sd.o:  %.cpp  astyle.h
	@ mkdir -p $(objdir)
	$(CXX) $(CFLAGSsd) -c $< -o $@

# OBJa
$(objdir)/%_a.o:  %.cpp  astyle.h
	@ mkdir -p $(objdir)
	$(CXX) $(CFLAGSa) -c $< -o $@

# OBJad
$(objdir)/%_ad.o:  %.cpp  astyle.h
	@ mkdir -p $(objdir)
	$(CXX) $(CFLAGSad) -c $< -o $@

# OBJsj
$(objdir)/%_sj.o:  %.cpp  astyle.h
	@ mkdir -p $(objdir)
	$(CXX) $(CFLAGSsj) -c $< -o $@

# OBJsjd
$(objdir)/%_sjd.o:  %.cpp  astyle.h
	@ mkdir -p $(objdir)
	$(CXX) $(CFLAGSsjd) -c $< -o $@

##################################################
# define build dependencies for each command

release:  astyle
astyle:  $(OBJ)
	@ mkdir -p $(bindir)
	$(CXX) $(LDFLAGSr) -o $(bindir)/$@ $^
	@ echo

debug:  astyled
astyled:  $(OBJd)
	@ mkdir -p $(bindir)
	$(CXX) $(LDFLAGSd) -o $(bindir)/$@ $^
	@ echo

shared:  libastyle-2.06.so
libastyle-2.06.so:  $(OBJs)
	@ mkdir -p $(bindir)
	$(CXX) -shared $(LDFLAGSr) -o $(bindir)/$@ $^
	@ echo

shareddebug:  libastyle-2.06d.so
libastyle-2.06d.so:  $(OBJsd)
	@ mkdir -p $(bindir)
	$(CXX) -shared $(LDFLAGSd) -o $(bindir)/$@ $^
	@ echo

static:  libastyle.a
libastyle.a:  $(OBJa)
	@ mkdir -p $(bindir)
	ar crs $(bindir)/$@ $^
	@ echo

staticdebug:  libastyled.a
libastyled.a:  $(OBJad)
	@ mkdir -p $(bindir)
	ar crs $(bindir)/$@ $^
	@ echo

java:  libastyle-2.06j.so
libastyle-2.06j.so:  $(OBJsj)
	@ mkdir -p $(bindir)
	$(CXX) -shared $(LDFLAGSr) -o $(bindir)/$@ $^
	@ echo

javadebug:  libastyle-2.06jd.so
libastyle-2.06jd.so:  $(OBJsjd)
	@ mkdir -p $(bindir)
	$(CXX) -shared $(LDFLAGSd) -o $(bindir)/$@ $^
	@ echo

all:  release debug shared shareddebug static staticdebug

javaall:  java javadebug

clean:
	rm -f $(objdir)/*.o $(bindir)/*astyle*

cleanobj:
	rm -f $(objdir)/*.o

install:
	$(INSTALL) -m 755 -d $(ipath)
	@$(INSTALL) -m 755 $(bindir)/astyle  $(ipath)

	$(INSTALL) -m 755 -d $(SYSCONF_PATH)
	@mkdir -p $(SYSCONF_PATH)/html;
	@for files in ../../doc/*.html  ../../doc/*.css; \
	do \
		$(INSTALL)  -m 644  $$files  $(SYSCONF_PATH)/html; \
	done

uninstall:
	rm -f $(ipath)/astyle
	rm -rf $(SYSCONF_PATH)
