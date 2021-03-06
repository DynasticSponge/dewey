APPNAME = dewey
STARTDIR = ${CURDIR}/
OUTPUTDIR = ${STARTDIR}output/
HEADERDIR = ${STARTDIR}headers/
OBJECTDIR = ${STARTDIR}object/
SOURCEDIR = ${STARTDIR}source/
STATICDIR = ${STARTDIR}libraries/
OBJECTS = $(wildcard ${OBJECTDIR}*.o)
SOURCES = $(wildcard ${SOURCEDIR}*.cpp)
STATICS = $(wildcard ${STATICSDIR}*.a)
LIBRARIES = -lpq -lpthread

compile: ${SOURCES}
	$(CXX) -std=c++17 -c ${SOURCES} -I ${HEADERDIR}
	mv ${STARTDIR}*.o ${OBJECTDIR}

dcompile: ${SOURCES}
	$(CXX) -std=c++17 -ggdb -c ${SOURCES} -I ${HEADERDIR}
	mv ${STARTDIR}*.o ${OBJECTDIR}

link: ${OBJECTS}
	$(CXX) -o ${OUTPUTDIR}${APPNAME} ${OBJECTS} -L${STATICDIR} ${LIBRARIES} 
	chmod 755 ${OUTPUTDIR}${APPNAME}

staticlib: ${OBJECTS}
	ar rs ${OUTPUTDIR}lib${APPNAME}.a ${OBJECTS}

postop:
	rm ${OBJECTDIR}*.*

clean:
	rm ${OBJECTDIR}*.*
	rm ${OUTPUTDIR}*.*

debug:
	$(MAKE) dcompile
	$(MAKE) link
	$(MAKE) postop

static: 
	$(MAKE) compile
	$(MAKE) staticlib
	$(MAKE) postop

all:
	$(MAKE) compile
	$(MAKE) link
	$(MAKE) postop