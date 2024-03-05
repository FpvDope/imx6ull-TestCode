test001:A.o B.o
	gcc -o test001 A.o B.o
  
A.o:A.c
	gcc -c -o A.o A.c
  
B.o:B.c
	gcc -c -o B.o B.c

clean:
	rm -rf *.o test001
