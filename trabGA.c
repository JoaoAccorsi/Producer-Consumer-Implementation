#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include "queue.h"

pthread_t threads[4];
Queue sorting_belt;

sem_t semEmpty;
sem_t semFull;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    char* category;
    char* name;
    int range;
    int maximum_weight;
}Cargo_transport;

Cargo_transport plane;
Cargo_transport ship;
Cargo_transport truck;
Cargo_transport cargo;

Cargo_transport low_weight[1000];
Cargo_transport high_weight[1000];

int counter_low_weight = 1, counter_high_weight = 1, planes_produced = 0, ships_produced = 0, trucks_produced = 0, random_time = 0;

void* classifier_1 (void* arg){

    sem_wait(&semFull);
    pthread_mutex_lock(&mutex);

    dequeue(&sorting_belt, &cargo);

    real_classifier(1, cargo);

    pthread_mutex_unlock(&mutex);
    sem_post(&semEmpty);

    return NULL;
}

void* classifier_2 (void* arg){

    sem_wait(&semFull);
    pthread_mutex_lock(&mutex);
    
    dequeue(&sorting_belt, &cargo);

    real_classifier(2, cargo);

    pthread_mutex_unlock(&mutex);
    sem_post(&semEmpty);

    return NULL;
}

void real_classifier(int number, Cargo_transport cargo_classifier){

    if (cargo.maximum_weight < 200000){
        low_weight[counter_low_weight].category = cargo_classifier.category;
        low_weight[counter_low_weight].name = cargo_classifier.name;
        low_weight[counter_low_weight].range = cargo_classifier.range;
        low_weight[counter_low_weight].maximum_weight = cargo_classifier.maximum_weight;
        printf("Cargo Classified by Classifier %d: %s: {Name: %s - Range: %dkm - Maximum Weight: %dkg}\n", number, low_weight[counter_low_weight].category, low_weight[counter_low_weight].name, low_weight[counter_low_weight].range, low_weight[counter_low_weight].maximum_weight);      
        counter_low_weight++;
    } else {
        high_weight[counter_high_weight].category = cargo_classifier.category;
        high_weight[counter_high_weight].name = cargo_classifier.name;
        high_weight[counter_high_weight].range = cargo_classifier.range;
        high_weight[counter_high_weight].maximum_weight = cargo_classifier.maximum_weight;
        printf("Cargo Classified by Classifier %d: %s: {Name: %s - Range: %dkm - Maximum Weight: %dkg}\n", number, high_weight[counter_high_weight].category, high_weight[counter_high_weight].name, high_weight[counter_high_weight].range, high_weight[counter_high_weight].maximum_weight);
        counter_high_weight++;
    }

    if (cargo.category == "Plane")
        planes_produced++;
    else if (cargo.category == "Ship")
        ships_produced++;
    else
        trucks_produced++;
}

void* plane_producer(void* arg){

    char name[20][200] = {"Airbus A220", "Airbus A320", "Boing 747", "Boing 777", "Airbus A330", "Airbus A380", "Embraer 190", "Airbus Beluga", "Boing 737", "Embraer 170", "Embraer 195", "Airbus A300"};  

    sem_wait(&semEmpty);
    pthread_mutex_lock(&mutex);

    plane.category = "Plane";
    plane.name = name[random() % 11];
    plane.range = random() % 20000 + 1000; // 1k to 20k
    plane.maximum_weight = random() % 500000 + 20000; // 20k to 500k

    enqueue(&sorting_belt, &plane);
    printf("Plane Produced: %s: {Name: %s - Range: %dkm - Maximum Weight: %dkg}\n", plane.category, plane.name, plane.range, plane.maximum_weight);

    pthread_mutex_unlock(&mutex);
    sem_post(&semFull);

    return NULL;
}

void* ship_producer(void* arg){

    char name[20][200] = {"Bulk Ship", "Tanker Ship", "Oil Ship", "Gas Ship", "Container Ship", "General Cargo Ship", "Ro-Ro Ship"};

    sem_wait(&semEmpty);
    pthread_mutex_lock(&mutex);

    ship.category = "Ship";
    ship.name = name[random() % 6];
    ship.range = random() % 20000 + 100; // 100 to 20k
    ship.maximum_weight = random() % 400000 + 5000; // 5k to 400k

    enqueue(&sorting_belt, &ship);
    printf("Ship Produced: %s: {Name: %s - Range: %dkm - Maximum Weight: %dkg}\n", ship.category, ship.name, ship.range, ship.maximum_weight);

    pthread_mutex_unlock(&mutex);
    sem_post(&semFull);

    return NULL;
}

void* truck_producer(void* arg){

    char name[20][200] = {"Concrete Transport Truck", "Tank Truck" ,"Tractor Unit Truck", "Refrigerator Truck", "Dump Truck", "Garbage Truck", "Log Carrier Truck", "Mobile Crane Truck"};

    sem_wait(&semEmpty);
    pthread_mutex_lock(&mutex);

    truck.category = "Truck";
    truck.name = name[random() % 7];
    truck.range = random() % 200 + 50; // 50 to 200
    truck.maximum_weight = random() % 15000 + 3000; // 3k to 15k

    enqueue(&sorting_belt, &truck);
    printf("Truck Produced: %s: {Name: %s - Range: %dkm - Maximum Weight: %dkg}\n", truck.category, truck.name, truck.range, truck.maximum_weight);

    pthread_mutex_unlock(&mutex);
    sem_post(&semFull);

    return NULL;
}

int main() {

    queueInit(&sorting_belt, sizeof(Cargo_transport));

    sem_init(&semEmpty, 0, 1000);
    sem_init(&semFull, 0, 0);

    srand (time(NULL));

    Cargo_transport aux;
    aux.category = "NULL";

    for (int j = 0; j < 1000; j ++){
        low_weight[j].category == aux.category;
        high_weight[j].category == aux.category;
    }

    int count = 0, limit = 0, rand = 0, producer = 0;

    random_time = random() % 10; // 0
    producer = random() % 20 + 1; // 100

    printf("Produce how many cargos? %d\n", producer);
    printf("Time to produce each cargo: %ds\n\n", random_time);
    
    while(count < producer){

        pthread_create(&threads[0], NULL, &classifier_1, NULL);
        pthread_create(&threads[1], NULL, &classifier_2, NULL);

        rand = (random() % 3);

        switch (rand){
            case 0:
                pthread_create(&threads[2], NULL, &plane_producer, NULL);
                sleep(random_time);
                break;
            case 1:
                pthread_create(&threads[3], NULL, &ship_producer, NULL);
                sleep(random_time);
                break;
            case 2:
                pthread_create(&threads[4], NULL, &truck_producer, NULL);
                sleep(random_time);
                break;
            default:
                break;
        }     

        count++;     
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&semEmpty);
    sem_destroy(&semFull);

    sleep(1);
    toString();

    return 0;
}

void toString(){   

    int i = 1;
    printf("\n\n");

    while (low_weight[i].category != NULL){
    
        if (i == 1){
            printf("\n++-------------------------------------------------------------------++\n");
            printf("++-------------------------Low Weigth Cargo--------------------------++\n");
            printf("++--------------------------Below 200000kg---------------------------++\n");
            printf("++-------------------------------------------------------------------++\n");
        }

        printf("%d. %s: {Name: %s - Range: %dkm - Maximum Weight: %dkg}\n", i, low_weight[i].category, low_weight[i].name, low_weight[i].range, low_weight[i].maximum_weight);
        i++;
    }

    i = 1;

    while (high_weight[i].category != NULL){

        if (i == 1){
            printf("\n++-------------------------------------------------------------------++\n");
            printf("++-----------------------High Weigth Cargo---------------------------++\n");
            printf("++------------------------Above 200000kg-----------------------------++\n");
            printf("++-------------------------------------------------------------------++\n");
        }

        printf("%d. %s: {Name: %s - Range: %dkm - Maximum Weight: %dkg}\n", i, high_weight[i].category, high_weight[i].name, high_weight[i].range, high_weight[i].maximum_weight);
        i++;
    }

    printf("\n\n++-------------------------++\n");
    printf("Total Cargos Produced: %d\n", planes_produced + ships_produced + trucks_produced);
    printf("Planes: %d\n", planes_produced);
    printf("Ships: %d\n", ships_produced);
    printf("Trucks: %d\n", trucks_produced);
    printf("++-------------------------++\n\n");
}
