
all: librm.a  app myapp

librm.a:  rm.c
	gcc -Wall -c rm.c
	ar -cvq librm.a rm.o
	ranlib librm.a

app: app.c
	gcc -Wall -o app app.c -L. -lrm -lpthread -ggdb3

myapp: myapp.c
	gcc -Wall -o myapp myapp.c -L. -lrm -lpthread -ggdb3

clean: 
	rm -fr *.o *.a *~ a.out  app rm.o rm.a librm.a
