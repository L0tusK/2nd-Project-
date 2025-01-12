final : all

all : 
	gcc main.c catalog.h -o eshop 

clean : 
	rm *o.final
