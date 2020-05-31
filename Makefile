run: project.exe
	g++ -std=c++11 -o project.exe ./src/main.cpp
	./project.exe character1.txt monster1.txt 0

dev:
	g++ -std=c++11 -o project.exe ./src/main.cpp
	./project.exe character1.txt monster1.txt 1

clean:
	rm *.o || exit 0
	rm *.exe || exit 0
