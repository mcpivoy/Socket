all: s c
.PHONY : all

s:
	cc -o s s.c -lpthread

c:
	cc -o c c.c -lpthread

.PHONY : clean
clean :
	rm c s 
	
