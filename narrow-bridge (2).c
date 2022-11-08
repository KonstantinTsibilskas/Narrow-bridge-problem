
/* File for 'narrow_bridge' task implementation.
   SPbSTU, IBKS, 2017 */

#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/thread.h"
#include "threads/synch.h"
#include "narrow-bridge.h"
#include "devices/timer.h"
enum direction{
  right,
  left
};
struct semaphore normal_left;
struct semaphore normal_right;
struct semaphore emergency_right;
struct semaphore emergency_left;
struct semaphore bridge;
int amount_normal_car_left;
int amount_normal_car_right;
int amount_emergency_car_left;
int amount_emergency_car_right;
int all_car;
int amount_total_emergency_car;
int amount_total_normal_car;
int cars_on_bridge;
int total;
enum direction direction= -1;
#define BROKEN_BRIDGE 3
void set_new_direction(void);
// Called before test. Can initialize some synchronization objects.
void narrow_bridge_init( unsigned int num_vehicles_left, unsigned int num_vehicles_right,
                         unsigned int num_emergency_left, unsigned int num_emergency_right)
{
  sema_init(&normal_left, 0);
  sema_init(&normal_right, 0);
  sema_init(&emergency_left,0);
  sema_init(&emergency_right,0);
  sema_init(&bridge,0);
  amount_normal_car_left = 0;
  amount_normal_car_right =  0;
  amount_emergency_car_left = 0;
  amount_emergency_car_right = 0;
  all_car =  num_vehicles_left + num_vehicles_right
             + num_emergency_left + num_emergency_right;
  amount_total_normal_car = num_vehicles_left+num_vehicles_right;
  amount_total_emergency_car = num_emergency_left + num_emergency_right;
  cars_on_bridge = 0;
  total = all_car;
  set_new_direction();
}

void open(){
  timer_sleep(5);
  exit_bridge(car_normal, dir_right);
}

/*BLOCKED ALL CARS BEFORE THEY CROSS BRIDGE*/
void arrive_bridge(enum car_priority prio , enum car_direction dir )
{

      if (amount_normal_car_left + amount_normal_car_right + amount_emergency_car_right + amount_emergency_car_left == all_car - 1) {
      char name[16];
      snprintf(name,sizeof(name),"%u",100);
      thread_create(name,PRI_DEFAULT,open,0);
    }

    if (prio == car_emergency && dir == dir_left) {

      amount_emergency_car_left++;
      sema_down(&emergency_left);
    }
    if(prio == car_emergency && dir == dir_right) {

    amount_emergency_car_right++;
      sema_down(&emergency_right);

    }
    if(prio == car_normal && dir == dir_left) {

       amount_normal_car_left++;
       sema_down(&normal_left);

    }
    if(prio == car_normal && dir == dir_right){

      amount_normal_car_right++;
       sema_down(&normal_right);
    }

}


int empty(int e){
  if(e == 0) return 1;
  else return 0;
}

void set_new_direction(void){
  if (!empty(amount_emergency_car_left) && empty(amount_emergency_car_right) || amount_emergency_car_left > amount_emergency_car_right) {
      direction = left;
  }

  else if ((!empty(amount_emergency_car_left) && !empty(amount_emergency_car_right)) && (amount_emergency_car_left == amount_emergency_car_right)) {
    direction = left;
  }
  else if (empty(amount_emergency_car_left) && !empty(amount_emergency_car_right) || amount_emergency_car_right > amount_emergency_car_left) {
      direction = right;
  }
  else if (amount_emergency_car_left < amount_normal_car_right ) {
      direction = right;
  }
  else if(amount_emergency_car_right < amount_normal_car_left){
      direction = left;
  }
  else if (empty(amount_emergency_car_left) && empty(amount_emergency_car_right) && amount_emergency_car_left && amount_normal_car_right
           && (amount_normal_car_left == amount_normal_car_right)) {
  direction = left;
}

}
void exit_bridge(enum car_priority prio , enum car_direction dir )
{

  if(cars_on_bridge > 0) cars_on_bridge--;
  set_new_direction();

  if (cars_on_bridge == 0) {
    /*UNBLOCK LEFT CARS*/

    if(direction == left){

    if (amount_emergency_car_left && amount_emergency_car_left != 1) { //when 2 emergency cars
      sema_up(&emergency_left);
      sema_up(&emergency_left);
      cars_on_bridge+=2;
      amount_emergency_car_left-=2;
    }
    else if (amount_emergency_car_left == 1) {
      sema_up(&emergency_left);
      cars_on_bridge++;
      amount_emergency_car_left--;

       if (amount_normal_car_left) { // when 1 emergency and 1 normal

          sema_up(&normal_left);
          amount_normal_car_left--;
          cars_on_bridge++;
       }
    }
  else  if (empty(amount_emergency_car_left)) {
        if (amount_normal_car_left && amount_normal_car_left != 1) {

          sema_up(&normal_left);
          sema_up(&normal_left);
          amount_normal_car_left-=2;
          cars_on_bridge += 2;

        }
        else if(amount_normal_car_left == 1) {

          sema_up(&normal_left);
          amount_normal_car_left--;
          cars_on_bridge++;

        }
    }
  }
  if(direction == right){
    if (amount_emergency_car_right && amount_emergency_car_right != 1) {

       sema_up(&emergency_right);
       sema_up(&emergency_right);
       amount_emergency_car_right -= 2;
       cars_on_bridge += 2;

    }
    else if (amount_emergency_car_right == 1) {

       sema_up(&emergency_right);
       amount_emergency_car_right--;
       cars_on_bridge++;

       if (amount_normal_car_right) {
          sema_up(&normal_right);
          amount_normal_car_right--;
          cars_on_bridge++;
       }
    }
  else  if (empty(amount_emergency_car_right)) {
        if (amount_normal_car_right && amount_normal_car_right != 1) {

            sema_up(&normal_right);
            sema_up(&normal_right);
            amount_normal_car_right-=2;
            cars_on_bridge+=2;
        }
        else if (amount_normal_car_right == 1) {
            sema_up(&normal_right);
            amount_normal_car_right--;
            cars_on_bridge++;
        }
    }

  }
    set_new_direction();
}
}
