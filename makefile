make: *.cpp
	g++ -std=c++17 -o game *.cpp -lSDl2
run: make
	./game