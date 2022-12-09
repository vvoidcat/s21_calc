UNAME = $(shell uname -s)
PROJECTNAME = s21_calc
FLAGS_MAIN = "noflags"
FLAGS_TEST = "noflags"
CC = gcc -Wall -Werror -Wextra -std=c11 -g
SOURCES_C = src/main.c src/s21_calc_parser.c src/s21_calc_core.c src/s21_calc_math.c src/s21_calc_graphs.c src/s21_calc_other.c src/s21_calc.h src/s21_stack.c src/s21_stack.h
SOURCES_COMPILED = tmp/s21_tests_calc_core.o tmp/s21_tests_calc_math.o tmp/s21_tests_calc_graphs.o tmp/s21_tests_calc_other.o tmp/s21_tests_stack.o
SOURCES_TESTS = unit-tests/s21_tests_main.c unit-tests/s21_tests_lib.c unit-tests/s21_tests.h unit-tests/s21_testcases.h
TMPDIR = tmp
COVDIR = unit-tests/coverage
BUILDDIR = build
BUILDDIR_RELEASE = build/$(PROJECTNAME)-build-release
BUILDDIR_TESTS = build/$(PROJECTNAME)-build-tests
OUTNAME = $(PROJECTNAME)
OUTNAME_TESTS = s21_tests
OUTNAME_TAR = $(PROJECTNAME)-build-release-v1.tar.gz
PROJECT_MANUAL = "manual.html"

ifeq ($(UNAME),Darwin)
	FLAGS_MAIN = -lm -lpthread
	FLAGS_TEST = -lcheck -lm -lpthread -fprofile-arcs
endif
ifeq ($(UNAME),Linux)
	FLAGS_MAIN = -lsubunit -lrt -lm -lpthread
	FLAGS_TEST = -lcheck -lsubunit -lrt -lm -lpthread -fprofile-arcs
endif

all: cleanall install dist dvi
.PHONY : all

start:
	if ! [ -d "$(BUILDDIR)" ]; then mkdir $(BUILDDIR); fi
	if ! [ -d "$(TMPDIR)" ]; then mkdir $(TMPDIR); fi
.PHONY : start

build: start $(SOURCES_C)
	rm -rf $(OUTNAME)
	gcc $(SOURCES_C) $(FLAGS_MAIN) -o $(OUTNAME)
.PHONY : build

install: build
	if ! [ -d "$(BUILDDIR_RELEASE)" ]; then mkdir $(BUILDDIR_RELEASE); fi
	mv $(OUTNAME) $(BUILDDIR_RELEASE)/$(OUTNAME)
.PHONY : install

dist: $(BUILDDIR_RELEASE)/$(OUTNAME)
	tar -zcvf build/$(OUTNAME_TAR) $(BUILDDIR_RELEASE)
.PHONY : dist

dvi: dvi_clean misc/$(PROJECTNAME)_manual.texi
	makeinfo misc/$(PROJECTNAME)_manual.texi --html
	mv manual/index.html $(PROJECT_MANUAL)
	rm -rf manual
	open $(PROJECT_MANUAL)
.PHONY : dvi

launch: $(BUILDDIR_RELEASE)/$(OUTNAME)
	./$(BUILDDIR_RELEASE)/$(OUTNAME)
.PHONY : launch

tests: clean start $(SOURCES_TESTS) $(SOURCES_C)
	rm -rf $(TMPDIR)/s21_tests*
	if ! [ -d "$(BUILDDIR_TESTS)" ]; then mkdir $(BUILDDIR_TESTS); fi
	$(CC) -c --coverage src/s21_calc_core.c -o $(TMPDIR)/s21_tests_calc_core.o
	$(CC) -c --coverage src/s21_calc_math.c -o $(TMPDIR)/s21_tests_calc_math.o
	$(CC) -c --coverage src/s21_calc_graphs.c -o $(TMPDIR)/s21_tests_calc_graphs.o
	$(CC) -c src/s21_calc_other.c -o $(TMPDIR)/s21_tests_calc_other.o
	$(CC) -c src/s21_stack.c -o $(TMPDIR)/s21_tests_stack.o
	$(CC) $(SOURCES_TESTS) $(SOURCES_COMPILED) $(FLAGS_TEST) -o $(BUILDDIR_TESTS)/$(OUTNAME_TESTS)
	@echo "-------------------------------------------------"
	./$(BUILDDIR_TESTS)/$(OUTNAME_TESTS)
	@echo "-------------------------------------------------"
	mv ./$(BUILDDIR_TESTS)/*gcda $(TMPDIR)
.PHONY : tests

gcov_report: $(BUILDDIR_TESTS)/$(OUTNAME_TESTS)
	rm -rf $(COVDIR) && mkdir $(COVDIR)
	@echo ""
	gcov -b -l -p -c $(TMPDIR)/*.gcno
	gcovr -g -k -r . --html --html-details -o $(COVDIR)/coverage_report.html
	mv *.gcov $(TMPDIR)
	open $(COVDIR)/coverage_report.html
.PHONY : gcov_report

style: $(SOURCES_C)
	cp utils/".clang-format" ".clang-format"
	clang-format -i src/*.c
	clang-format -i src/*.h
	clang-format -n src/*.c
	clang-format -n src/*.h
	rm -rf ".clang-format"
.PHONY : style

valgrind: $(BUILDDIR_RELEASE)/$(OUTNAME) $(BUILDDIR_TESTS)/$(OUTNAME_TESTS)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(BUILDDIR_RELEASE)/$(OUTNAME)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(BUILDDIR_TESTS)/$(OUTNAME_TESTS)
.PHONY : valgrind

uninstall:
	rm -rf $(BUILDDIR_RELEASE)
.PHONY : uninstall

dvi_clean:
	rm -rf manual $(PROJECT_MANUAL)
.PHONY : dvi_clean

clean:
	rm -rf *.o *.gcno *.gcda *.gcov *.dSYM .DS_Store build/.DS_Store a.out ".clang-format"
	rm -rf $(TMPDIR)
.PHONY : clean

cleanall: clean dvi_clean uninstall
	rm -rf .qt/ build/CMakeCache.txt build/cmake_install.cmake build/.qt/ build/CMakeFiles build/Makefile
	rm -rf build/build-release build/build-debug
	rm -rf $(OUTNAME) build/$(OUTNAME) build/$(OUTNAME)_autogen/ $(OUTNAME_TESTS) build/$(OUTNAME_TESTS) *.a *.so
	rm -rf build/$(OUTNAME_TAR)
	rm -rf $(BUILDDIR_TESTS)
	rm -rf $(COVDIR)
.PHONY : cleanall
