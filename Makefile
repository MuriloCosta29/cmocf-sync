sync_car: sync_car.c sync_car-runner.c sync_car.h
	cc -Wall -pthread -o sync_car sync_car.c sync_car-runner.c

clean:
	rm -f sync_car

test: sync_car
	./sync_car
