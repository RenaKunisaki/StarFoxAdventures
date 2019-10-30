TARGET = Geometry

MAJOR = 0
MINOR = 0
POINT = 23

VERSION = ${MAJOR}.${MINOR}.${POINT}
QVERSION = "'${VERSION}'"
VERSION_FILE = VERSION

NEWPOINT = `expr $(POINT) + 1`
NEWMINOR = `expr $(MINOR) + 1`
NEWMAJOR = `expr $(MAJOR) + 1`

UPDTINIT = 's/__version__.*=.*/__version__ = ${QVERSION}/'
UPDTRDME = 's/Version: .*/Version: ${VERSION}/'

GIT = git
SED = sed
RM = rm
MV = mv

MAKEFILE= Makefile

PYTHON = python3
SETUP = setup.py
PYSETUP = ${PYTHON} ${SETUP}

PYPI= testpypi

PKG_ROOT = ${TARGET}
PKG_INIT = ${PKG_ROOT}/__init__.py
README = README.md

TMPFILES= VERSION dist build ${TARGET}.egg-info cover

NOSE = nosetests
NOSEFLAGS = --with-coverage --cover-tests --cover-html

AUTOPEP8 = autopep8
AUTOPEP8_FLAGS = -ria

FLAKE8 = flake8
FLAKE8_IGN = F401,F403
# F401 - imported but unused
# F403 - import * used, unable to detect undefined names

PIP = pip3
PIP_FLAGS := --verbose
PIP_FLAGS := ${PIP_FLAGS} --index-url http://${PYPI}.python.org/pypi
PIP_FLAGS := ${PIP_FLAGS} --trusted-host ${PYPI}.python.org
PIP_FLAGS := ${PIP_FLAGS} --proxy=$(HTTPS_PROXY)


all:
	@echo "make sdist         - creates a source distribution"
	@echo "make bdist         - creates a binary distribution"
	@echo "make wheel         - creates a bdist_wheel distribution"
	@echo "make bump_major    - increment version major number MAJOR=${MAJOR}"
	@echo "make bump_minor    - increment version minor number MINOR=${MINOR}"
	@echo "make bump_point    - increment version point number POINT=${POINT}"
	@echo "make update        - updates the version VERSION=${VERSION}"
	@echo "make upload        - uploads bdist_wheel to PYPI=${PYPI}"
	@echo "make clean         - removes all derived files and directories"
	@echo ""		  
	@echo "make test-install  - pip install from PYPI=${PYPI}"
	@echo "make test-upgrade  - pip upgrade from PYPI=${PYPI}"
	@echo ""
	@echo "make test          - run unit tests"
	@echo "make coverage      - run unit tests with code coverage"
	@echo "make autopep8      - run autopep8 on source, modifies in-place"
	@echo "make flake8        - run flake8 on source, report only"
	@echo ""
	@echo "Update workflow:"
	@echo "----------------"
	@echo "make bump_"
	@echo "make update"
	@echo "make tag"
	@echo "make commit"
	@echo "make upload"
	@echo ""


bump_major:
	@${SED} "s/^MAJOR[ \t]*=[ \t]*[0-9]*/MAJOR=$(NEWMAJOR)/" \
	  ${MAKEFILE} > ${MAKEFILE}.tmp
	@${MV} ${MAKEFILE}.tmp ${MAKEFILE}

bump_minor:
	@${SED} "s/^MINOR[ \t]*=[ \t]*[0-9]*/MINOR=$(NEWMINOR)/" \
	  ${MAKEFILE} > ${MAKEFILE}.tmp
	@${MV} ${MAKEFILE}.tmp ${MAKEFILE}

bump_point:
	@${SED} "s/^POINT[ \t]*=[ \t]*[0-9]*/POINT=$(NEWPOINT)/" \
	  ${MAKEFILE} > ${MAKEFILE}.tmp
		@${MV} ${MAKEFILE}.tmp ${MAKEFILE}

update:
	@echo ${VERSION} > ${VERSION_FILE}
	@${SED} -e ${UPDTINIT} ${PKG_INIT} > ${PKG_INIT}.tmp
	@${MV} ${PKG_INIT}.tmp ${PKG_INIT}
	@${SED} -e ${UPDTRDME} ${README} > ${README}.tmp
	@${MV} ${README}.tmp ${README}

tag:
	${GIT} tag ${VERSION}
	${GIT} push origin ${VERSION}

commit:
	@${GIT} add .
	@${GIT} commit -m ${VERSION}

sdist:
	${PYSETUP} build sdist

wheel:
	${PYSETUP} build bdist_wheel

bdist:
	${PYSETUP} build bdist

test:
	${PYSETUP} test -q

coverage:
	${NOSE} ${NOSEFLAGS}

autopep8:
	@${AUTOPEP8} ${AUTOPEP8_FLAGS} .

flake8:
	${FLAKE8} --ignore ${FLAKE8_IGN} ${PKG_ROOT}

register:
	$(PYSETUP) register -r ${PYPI}

# switch to twine?
upload:
	$(PYSETUP) bdist_wheel upload -r ${PYPI}

test-install:
	@echo "Testing install..."
	${PIP} install ${PIP_FLAGS} ${TARGET}

test-upgrade:
	@echo "Testing upgrade..."
	${PIP} install --upgrade ${PIP_FLAGS} ${TARGET}


clean:
	@${PYSETUP} clean
	@${RM} -rf ${TMPFILES}


