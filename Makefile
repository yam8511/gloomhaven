run:
	g++ -std=c++17 -o project.exe ./src/main.cpp
	./project.exe character3.txt monster3.txt 0

dev:
	g++ -std=c++17 -o project.exe ./src/main.cpp
	./project.exe character1.txt monster1.txt 1

make exam:
	g++ -std=c++17 -o project.exe ./src/main.cpp
	cat ./input3.txt | ./project.exe character3.txt monster3.txt 0

clean:
	rm *.o || exit 0
	rm *.exe || exit 0
