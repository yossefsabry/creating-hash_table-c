
main:
	@gcc main.c src/hash_table.c src/prime.c -lm -o main -g 
	@./main

clean:
	find . -maxdepth 1 -name "core*" -exec rm {} + 
	rm -rf core* *.o *.out OUT main


