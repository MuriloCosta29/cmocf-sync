/*
 * Teste de casos limite e invalido para sync_car.c.
 * Compilar: cc -Wall -pthread -o teste_limites teste_limites.c sync_car.c
 * Executar: ./teste_limites
 */
#include "sync_car.h"
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static volatile int boarded = 0;
static int falhas = 0;

static void alarme(int sig) {
  (void)sig;
  fprintf(stderr, "FALHOU: travou (timeout de 2 s)\n");
  exit(1);
}

static void *passageiro(void *arg) {
  struct station *s = arg;
  station_wait_for_car(s);
  __sync_add_and_fetch(&boarded, 1);
  station_on_board(s);
  return NULL;
}

static void espera_passageiros(struct station *s, int n) {
  for (;;) {
    pthread_mutex_lock(&s->lock);
    int w = s->waiting;
    pthread_mutex_unlock(&s->lock);
    if (w == n)
      return;
    usleep(1000);
  }
}

static void confere(const char *nome, int ok) {
  printf("%s: %s\n", ok ? "OK    " : "FALHOU", nome);
  if (!ok)
    falhas++;
}

/* count (0 ou negativo) com 1 passageiro esperando: retorna na hora e
 * ninguem embarca. Depois libera o passageiro com um carro valido. */
static void caso_sem_assentos(const char *nome, int count) {
  struct station s;
  pthread_t t;
  boarded = 0;
  station_init(&s);
  pthread_create(&t, NULL, passageiro, &s);
  espera_passageiros(&s, 1);

  alarm(2);
  station_load_car(&s, count);
  alarm(0);
  usleep(200000);
  confere(nome, boarded == 0);

  station_load_car(&s, 1);
  pthread_join(t, NULL);
}

int main(void) {
  signal(SIGALRM, alarme);
  printf("== Casos limite ==\n");

  caso_sem_assentos("count == 0 com passageiro esperando: retorna e ninguem "
                    "embarca",
                    0);

  {
    struct station s;
    station_init(&s);
    alarm(2);
    station_load_car(&s, 5);
    alarm(0);
    confere("nenhum passageiro esperando: retorna imediatamente", 1);
  }

  {
    struct station s;
    pthread_t t[5];
    boarded = 0;
    station_init(&s);
    for (int i = 0; i < 5; i++)
      pthread_create(&t[i], NULL, passageiro, &s);
    espera_passageiros(&s, 5);

    alarm(2);
    station_load_car(&s, 2);
    alarm(0);
    confere("2 assentos e 5 passageiros: embarcam exatamente 2", boarded == 2);

    alarm(2);
    station_load_car(&s, 3);
    alarm(0);
    for (int i = 0; i < 5; i++)
      pthread_join(t[i], NULL);
    confere("segundo carro leva os 3 restantes", boarded == 5);
  }

  printf("== Caso invalido ==\n");
  caso_sem_assentos("count negativo (-3): retorna e ninguem embarca", -3);

  printf(falhas ? "\nRESULTADO: %d falha(s)\n" : "\nRESULTADO: tudo OK\n",
         falhas);
  return falhas != 0;
}
