inc=headers
src=sources
dep = dependecies
obj=obj
cc=g++
flags=-Wall -Wextra

inc_path=-I$(inc)
cmd=$(cc) $(flags) $(inc_path) -c $@; mv $@.o $(obj)
cmd_exe=$(cc) $(flags) $(inc_path) $@ $(obj)/* -o $@.out

files=$(src)/GameStateWorker.cpp

mkdep:
	$(cc) $(inc_paths) -MM $(files) -MF $(dep)

#include $(dep)

clean:
	rm -f *~ *# obj/* *.out
	cd inc; rm -f *~ *#
	cd src; rm -f *~ *#
