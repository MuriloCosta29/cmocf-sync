#include "sync_car.h"

void station_init(struct station *station) {
  pthread_mutex_init(&station->lock, NULL); 
  pthread_cond_init(&station->car_arrive, NULL);
  pthread_cond_init(&station->all_boarded, NULL);
  station->waiting = 0;
  station->free_seats = 0;
  station->boarding = 0;
}
