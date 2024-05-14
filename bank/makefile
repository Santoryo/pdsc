sources = account.h account.c filehandler.c filehandler.h logger.h logger.c main.c

executable = banking

$(executable): $(headers) $(sources)
	gcc -g -Wall -pedantic $(sources) -o $(executable)

test: $(headers) $(sources) $(executable)
	gcc -g -Wall -pedantic $(sources) -o $(executable)
	./$(executable)

.PHONY: clean
clean:
	rm $(executable)

.PHONY: check
check: $(executable)
	valgrind --leak-check=full --track-origins=yes ./$(executable)