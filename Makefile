# Compiler name
CC=g++
INCLUDE=include
TARGET=target/obj
SRCDIR=src/main/cpp
BINDIR=bin

#O.S. dependences (fix)
#Shell=cmd.exe
MPATH=ve/usb/ci/anise
CPATH=ve.usb.ci.anise

LD_LP=~/dev/mosek/6/tools/platform/linux32x86/bin:

ifdef LD_LIBRARY_PATH
#	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):~/.m2/repository/com/mosek/mosek/6.0/mosek-6.0.jar
else
	LD_LIBRARY_PATH=$(LD_LP)
endif


# Flags for debugging
# CFLAGS=-c -g -m64 -I $(INCLUDE)
CFLAGS=-c -g -I $(INCLUDE)

# Linker Flag
LFLAGS=-L $(TARGET)

# Test Executing enviroment
ifndef JVM_MEM_SIZE
JVM_MEM_SIZE=3840m
endif

MAVEN_OPTS=-Xmx$(JVM_MEM_SIZE) -Xms64m 

ifndef ERRORLOG
ERRORLOG=error.log
endif

ifndef OUTLOG
OUTLOG=out.log
endif

######################
# Sources
SRC=volume.cpp tmap.cpp tissue.cpp bbox.cpp anntxt.cpp
Prog=$(BINDIR)/fmtr $(BINDIR)/infer

SRCOBJS=$(TARGET)/$(SRC:.cpp=.o)
OBJDEP=$(addprefix $(TARGET)/,$(SRC:.cpp=.o))

.SUFFIXES:.cpp .o

all:$(PROG)

$(BINDIR)/fmtr: $(OBJDEP) $(TARGET)/fmtr.o
	$(CC) $(LFLAGS) -o $@ $(OBJDEP) $(TARGET)/fmtr.o

$(BINDIR)/infer: $(OBJDEP) $(TARGET)/infer.o
	$(CC) $(LFLAGS) -o $@ $(OBJDEP) $(TARGET)/infer.o

#$(PROG):$(OBJDEP) 
#	$(CC) $(LFLAGS) -o $@ $(OBJDEP) 

#.cpp.o:
#	$(CC) $(CFLAGS) $(SRCDIR)/$< -o $(TARGET)/$@

$(TARGET)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $< -o $@

#$(TARGET)/.cpp.o:
#	$(CC) $(CFLAGS) $(SRCDIR)/$< -o $@

dep.env: 
	mvn dependency:build-classpath -Dmdep.outputFile=dep.env

run: 	dep.env 
#	echo "running with " $(MAVEN_OPTS) 
	mvn exec:java -Dexec.mainClass=$(CPATH).arm

run2:	dep.env
	date >$(OUTLOG)
	java -Xmx$(JVM_MEM_SIZE) -cp ./target/classes:`cat dep.env` $(CPATH).arm 2> $(ERRORLOG) >>$(OUTLOG)
	date >>$(OUTLOG)

compile:
	mvn compile

infer: $(BINDIR)/infer

ske1f: $(BINDIR)/infer
	$(BINDIR)/infer -f data/skewed_head.dat -h 6 -x 184 -y 256 -z 170 -t 3 -a 1 -o data/skewed_head_1f.dat

ske: $(BINDIR)/infer
	$(BINDIR)\infer -f data\skewed_head.dat -h 6 -x 184 -y 256 -z 170 -t 2 -a 0 -o data\skewed_head_0.dat

ske1: $(BINDIR)/infer
	$(BINDIR)/infer -f data/skewed_head.dat -h 6 -x 184 -y 256 -z 170 -t 2 -a 1 -o data/skewed_head_1.dat
	$(BINDIR)/infer -f data/skewed_head.dat -h 6 -x 184 -y 256 -z 170 -t 3 -a 1 -o data/skewed_head_1f.dat
	$(BINDIR)/infer -f data/skewed_head.dat -h 6 -x 184 -y 256 -z 170 -t 1 -a 1 -o data/skewed_head_1.ani

ske2: $(BINDIR)/infer
	$(BINDIR)/infer -f data/skewed_head.dat -h 6 -x 184 -y 256 -z 170 -t 2 -a 2 -o data/skewed_head_2.dat

ske2f: $(BINDIR)/infer
	$(BINDIR)/infer -f data/skewed_head.dat -h 6 -x 184 -y 256 -z 170 -t 3 -a 2 -o data/skewed_head_2f.dat

ske3: $(BINDIR)/infer
	$(BINDIR)/infer -f data/skewed_head.dat -h 6 -x 184 -y 256 -z 170 -t 2 -a 1 -a 2 -o data/skewed_head_3.dat

ske3f: $(BINDIR)/infer
	$(BINDIR)/infer -f data/skewed_head.dat -h 6 -x 184 -y 256 -z 170 -t 3 -a 1 -a 2 -o data/skewed_head_3f.dat

ske3i: $(BINDIR)/infer
	$(BINDIR)/infer -f data/skewed_head.dat -h 6 -x 184 -y 256 -z 170 -t 1 -a 1 -a 2 -o data/skewed_head_3i.dat

ske4: $(BINDIR)/infer
	$(BINDIR)\infer -f data\skewed_head.dat -h 6 -x 184 -y 256 -z 170 -t 2 -a 4 -o data\skewed_head_4.dat

ske4f: $(BINDIR)/infer
	$(BINDIR)\infer -f data\skewed_head.dat -h 6 -x 184 -y 256 -z 170 -t 3 -a 4 -o data\skewed_head_4f.dat

ske5: $(BINDIR)/infer
	$(BINDIR)\infer -f data\skewed_head.dat -h 6 -x 184 -y 256 -z 170 -t 2 -a 4 -a 1 -o data\skewed_head_5.dat

ske6: $(BINDIR)/infer
	$(BINDIR)\infer -f data\skewed_head.dat -h 6 -x 184 -y 256 -z 170 -t 2 -a 4 -a 2 -o data\skewed_head_6.dat

ske6f: $(BINDIR)/infer
	$(BINDIR)\infer -f data\skewed_head.dat -h 6 -x 184 -y 256 -z 170 -t 3 -a 4 -a 2 -o data\skewed_head_6f.dat

ske6i: $(BINDIR)/infer
	$(BINDIR)\infer -f data\skewed_head.dat -h 6 -x 184 -y 256 -z 170 -t 1 -a 4 -a 2 -o data\skewed_head_6i.dat

ske7: $(BINDIR)/infer
	$(BINDIR)\infer -f data\skewed_head.dat -h 6 -x 184 -y 256 -z 170 -t 2 -a 4 -a 2 -a 1 -o data\skewed_head_7.dat

ske7f: $(BINDIR)/infer
	$(BINDIR)\infer -f data\skewed_head.dat -h 6 -x 184 -y 256 -z 170 -t 3 -a 4 -a 2 -a 1 -o data\skewed_head_7f.dat

ske7i: $(BINDIR)/infer
	$(BINDIR)\infer -f data\skewed_head.dat -h 6 -x 184 -y 256 -z 170 -t 1 -a 4 -a 2 -a 1 -o data\skewed_head_7i.dat

ske8: $(BINDIR)/infer
	$(BINDIR)/infer -f data/skewed_head.dat -h 6 -x 184 -y 256 -z 170 -t 2 -a 2 -a 8 -o data/skewed_head_8.dat

ske9: $(BINDIR)/infer
	$(BINDIR)/infer -f data/skewed_head.dat -h 6 -x 184 -y 256 -z 170 -t 2 -a 1 -a 2 -a 8 -o data/skewed_head_9.dat

ske9f: $(BINDIR)/infer
	$(BINDIR)/infer -f data/skewed_head.dat -h 6 -x 184 -y 256 -z 170 -t 3 -a 1 -a 2 -a 8 -o data/skewed_head_9f.dat

ske15: $(BINDIR)/infer
	$(BINDIR)/infer -f data/skewed_head.dat -h 6 -x 184 -y 256 -z 170 -t 2 -a 1 -a 2 -a 4 -a 8 -o data/skewed_head_15.dat

ske15f: $(BINDIR)/infer
	$(BINDIR)/infer -f data/skewed_head.dat -h 6 -x 184 -y 256 -z 170 -t 3 -a 1 -a 2 -a 4 -a 8 -o data/skewed_head_15f.dat

####################
####  Visible
####################

vis: $(BINDIR)/infer
	$(BINDIR)/infer -f data\visible_head.dat -h 0 -x 512 -y 512 -z 245 -t 2 -a 0 -o data\visible_head_0.dat

vis1: $(BINDIR)/infer
	$(BINDIR)/infer -f data\visible_head.dat -h 0 -x 512 -y 512 -z 245 -t 2 -a 1 -o data\visible_head_1.dat
	$(BINDIR)/infer -f data\visible_head.dat -h 0 -x 512 -y 512 -z 245 -t 1 -a 1 -o data\visible_head_1.ani

vis1f: $(BINDIR)/infer
	$(BINDIR)\infer -f data\visible_head.dat -h 0 -x 512 -y 512 -z 245 -t 3 -a 1 -o data\visible_head_1f.dat

vis2: $(BINDIR)/infer
	$(BINDIR)\infer -f data\visible_head.dat -h 0 -x 512 -y 512 -z 245 -t 2 -a 2 -o data\visible_head_2.dat

vis2f: $(BINDIR)/infer
	$(BINDIR)\infer -f data\visible_head.dat -h 0 -x 512 -y 512 -z 245 -t 3 -a 2 -o data\visible_head_2f.dat

vis4: $(BINDIR)/infer
	$(BINDIR)\infer -f data\visible_head.dat -h 0 -x 512 -y 512 -z 245 -t 2 -a 4 -o data\visible_head_4.dat

vis4f: $(BINDIR)/infer
	$(BINDIR)\infer -f data\visible_head.dat -h 0 -x 512 -y 512 -z 245 -t 3 -a 4 -o data\visible_head_4f.dat

vis7: $(BINDIR)/infer
	$(BINDIR)\infer -f data\visible_head.dat -h 0 -x 512 -y 512 -z 245 -t 2 -a 1 -a 2 -a 4 -o data\visible_head_7.dat

vis7f: $(BINDIR)/infer
	$(BINDIR)\infer -f data\visible_head.dat -h 0 -x 512 -y 512 -z 245 -t 3 -a 1 -a 2 -a 4 -o data\visible_head_7f.dat

vis9: $(BINDIR)/infer
	$(BINDIR)\infer -f data\visible_head.dat -h 0 -x 512 -y 512 -z 245 -t 2 -a 1 -a 8 -o data\visible_head_9.dat


####################
####  CT Head
####################

cth: $(BINDIR)/infer
	$(BINDIR)\infer -f data\cthead.dat -h 0 -x 256 -y 256 -z 113 -t 2 -a 0 -o data\cthead_0.dat

cth1: $(BINDIR)/infer
	$(BINDIR)\infer -f data\cthead.dat -h 0 -x 256 -y 256 -z 113 -t 2 -a 1 -o data\cthead_1.dat
	$(BINDIR)\infer -f data\cthead.dat -h 0 -x 256 -y 256 -z 113 -t 1 -a 1 -o data\cthead_1.ani

cth1f: $(BINDIR)/infer
	$(BINDIR)\infer -f data\cthead.dat -h 0 -x 256 -y 256 -z 113 -t 3 -a 1 -o data\cthead_1f.dat

cth2: $(BINDIR)/infer
	$(BINDIR)\infer -f data\cthead.dat -h 0 -x 256 -y 256 -z 113 -t 2 -a 2 -o data\cthead_2.dat

cth4: $(BINDIR)/infer
	$(BINDIR)\infer -f data\cthead.dat -h 0 -x 256 -y 256 -z 113 -t 2 -a 4 -o data\cthead_4.dat

jaw1: $(BINDIR)/infer
	$(BINDIR)/infer -f data/skewed_head.dat -h 6 -x 184 -y 256 -z 170 -t 2 -a 1 -a2 -a3 -a 8 -o data/skewed_jaw_1.dat

jaw2: $(BINDIR)/infer
	$(BINDIR)/infer -f data/skewed_head_9f.dat -h 6 -x 184 -y 256 -z 170 -t 2 -a 2 -o data/skewed_jaw_2.dat

jaw3: $(BINDIR)/infer
	$(BINDIR)/infer -f data/skewed_head_9f.dat -h 6 -x 184 -y 256 -z 170 -t 2 -a 1 -a 2 -o data/skewed_jaw_3.dat

jaw4: $(BINDIR)/infer
	$(BINDIR)/infer -f data/skewed_head_9f.dat -h 6 -x 184 -y 256 -z 170 -t 2 -a 4 -o data/skewed_jaw_4.dat

jaw6f: $(BINDIR)/infer
	$(BINDIR)/infer -f data/skewed_head_9f.dat -h 6 -x 184 -y 256 -z 170 -t 3 -a 2 -a 4 -o data/skewed_jaw_6f.dat

jaw7f: $(BINDIR)/infer
	$(BINDIR)/infer -f data/skewed_head_9f.dat -h 6 -x 184 -y 256 -z 170 -t 3 -a 1 -a 2 -a 4 -o data/skewed_jaw_7f.dat

jaw1a: $(BINDIR)/infer
	$(BINDIR)/infer -f data/skewed_head_7f.dat -h 6 -x 184 -y 256 -z 170 -t 3 -a 1 -a 2 -a 4 -o data/skewed_jaw_1a.dat


clean:
	rm *.env
	rm -Rf target/classes/ve
	rm -f target/obj/*
	rm psldb*
