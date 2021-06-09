#include <iostream>
using namespace std;
#include "MovieBookingSystem.h"
int main(){
ReservationList RL;
RL.insert(0, 7);
RL.insert(0, 5);
RL.insert(0, 1);
RL.linkedListInsert(RL.head, 2);

cout << RL.head->Code;
cout << RL.head->next->Code;

return 0;
}
