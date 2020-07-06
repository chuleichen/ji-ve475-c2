c2: c2.o
	g++ -o c2 c2.o   /usr/lib/x86_64-linux-gnu/libgmp.a /usr/lib/x86_64-linux-gnu/libgmpxx.a
c2.o : c2.cpp
	g++ -Wall -fexceptions -g  -c c2.cpp -o c2.o
clean:
	rm c2.o -rf
