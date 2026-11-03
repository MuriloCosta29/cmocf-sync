#include "sync_car.h"
#include <pthread.h>

void station_init(struct station *station) {
  pthread_mutex_init(&station->lock, NULL);
  pthread_cond_init(&station->car_arrive, NULL);
  pthread_cond_init(&station->all_boarded, NULL);
  station->waiting = 0;
  station->free_seats = 0;
  station->boarding = 0;
}

void station_load_car(struct station *station, int count) {
  pthread_mutex_lock(&station->lock);

  if (count > 0 && station->waiting > 0) {
    station->free_seats = count;
    pthread_cond_broadcast(&station->car_arrive);

    while (station->boarding > 0 ||
           (station->free_seats > 0 && station->waiting > 0))
      pthread_cond_wait(&station->all_boarded, &station->lock);
  }

  station->free_seats = 0;
  pthread_mutex_unlock(&station->lock);
}

void station_wait_for_car(struct station *station) {
  pthread_mutex_lock(&station->lock);

  station->waiting++;
  while (station->free_seats == 0)
    pthread_cond_wait(&station->car_arrive, &station->lock);

  station->free_seats--;
  station->waiting--;
  station->boarding++;

  pthread_mutex_unlock(&station->lock);
}

void station_on_board(struct station *station) {
  pthread_mutex_lock(&station->lock);

  station->boarding--;
  if (station->boarding == 0)
    pthread_cond_signal(&station->all_boarded);

  pthread_mutex_unlock(&station->lock);
}
