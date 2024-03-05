test002:A.o B.o C.o
	gcc -o test002 $^
  
%.o:%.c
	gcc -c -o $@ $<

clean:
	rm -rf *.o test002
