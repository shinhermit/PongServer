inc=inc
src=src
obj=obj
cc=g++
flags=-Wall -Wextra
inc_path=-I$(inc)

cmd=$(cc) $(flags) $(inc_path) -c $@; mv $@.o $(obj)
cmd_exe=$(cc) $(flags) $(inc_path) $@ $(obj)/* -o $@.out

clean:
	rm -f *~ *# obj/* *.out
	cd inc; rm -f *~ *#
	cd src; rm -f *~ *#
