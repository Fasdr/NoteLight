all: clean build compile run

build:
	cmake -B build

compile:
	cmake --build build

run:
	./build/src/NoteLight

clean:
	rm -rf ./build


