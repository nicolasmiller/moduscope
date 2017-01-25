TEST_MAIN = test_moduscope.cpp
SOURCE = moduscope.cpp

test_moduscope:
	mkdir -p build
	g++ -o build/test_moduscope $(TEST_MAIN) $(SOURCE)

test: test_moduscope
	./build/test_moduscope

clean:
	rm -rf ./build
