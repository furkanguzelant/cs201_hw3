/**
 * @author Furkan Güzelant ID: 21901515
 * @version 03.05.2021

*/


#include "MovieBookingSystem.h"
#include <ctime>
#include <iostream>
#include <sstream>
using namespace std;


    MovieBookingSystem::MovieBookingSystem() {
        reservCode = 0;
    }

    MovieBookingSystem::~MovieBookingSystem() {

    }


    void MovieBookingSystem:: addMovie( const long movieID, const int audienceRadius ) {
        bool validId = true;
        int length = movies.getLength();
        for(int i = 0; i < length; i++) {
            Movie* m;
            movies.retrieve(i, m);
            if(movieID == m->getId()) {
                validId = false;
                m->displayMovieInfo();
                cout << "already exits" << endl;
                return;
            }
        }
        if(validId && audienceRadius >= 0 && audienceRadius < 8) {

            Movie m(movieID, audienceRadius);
            movies.insertTail(m);

            Movie* mov;
            movies.retrieve(movies.getLength() - 1, mov);

            mov->displayMovieInfo();
            cout << "has been added." << endl;
        }
        else {
            cout << "Invalid input! Audience radius should be in range [0,7]." << endl;
        }

    }



    void MovieBookingSystem::cancelMovie( const long movieID) {
        bool found = false;
        int length = movies.getLength();
        Movie* m;

        int index;
        for(index = 0; !found && index < length; index++) {
            movies.retrieve(index, m);
            if(movieID == m->getId()) {
                found = true;
            }
        }
        index--; //To undo the last increment and get index of movie

        if(found) {

            for(int i = 0; i < reservationCodes.getLength(); i++) {
                Movie* m;
                reservationCodes.retrieveMovie(i, m);
                if(movieID == m->getId()) {
                    reservationCodes.remove(i);
                    i--;
                }
            }

            m->displayMovieInfo();
            cout << "has been canceled." << endl;

            movies.remove(index);

         }

         else {
            time_t t = movieID;
            string time = ctime(&t);
            time = time.substr(0, time.size() - 1); //To delete the newline character
            cout << "Movie at " << time << " does not exist" << endl;
         }
    }



    void MovieBookingSystem::showMovie( const long movieID) const {
        bool found = false;
        int length = movies.getLength();
        Movie* m;
        int i = 0;
         while(!found && i < length) {
            movies.retrieve(i, m);
            long id = m->getId();
            if(movieID == id) {
                found = true;
            }
            i++;
         }
         if(found) {
            m->displayMovieSeats();
         }
         else {
            time_t t = movieID;
            string time = ctime(&t);
            time = time.substr(0, time.size() - 1) + " "; //To delete the newline character
            cout << "Movie at " << time << "does not exist" << endl;
         }
     }

     void MovieBookingSystem:: showAllMovies() const{
        int length = movies.getLength();
        if(length == 0) {
            cout << "No movie on show" << endl;
        }
        else {
            cout << "Movies on show: " << endl;
            for(int i = 0; i < length; i++) {
                Movie* m;
                movies.retrieve(i, m);
                m->displayMovieInfo();
                cout << "(" << m->getSeat() << " available seats)" << endl;
            }
        }
    }


    int MovieBookingSystem::makeReservation( const long movieID, const int row, const char col) {

        int r;
        Movie* m;

        bool found = false;
        int length = movies.getLength();
        int i = 0;
        while(!found && i < length) {
            movies.retrieve(i, m);
            long id = m->getId();
            if(movieID == id) {
                r = m->getRadius();
                found = true;
            }
            i++;
         }

         if(found) {

            if((col - 'A') % (r + 1) == 0 && (row - 1) % (r + 1) == 0) { //Made use of ASCII values

                if(m->occupySeat((row - 1) / (r + 1), (col - 'A') / (r + 1))) {

                    stringstream ss;
                    string s;

                    ss << col << row;
                    ss >> s;

                    int resLength = reservationCodes.getLength();
                    reservCode++;

                    reservationCodes.insert(resLength, reservCode);

                    reservationCodes.setReservInfo(reservCode, m, s);


                    cout << "Reservation done for " << s << " in ";
                    m->displayMovieInfo();
                    cout << endl;

                    return reservCode;
                }
                else {
                    cout << "Seat " << col << row << " is not available in ";
                    m->displayMovieInfo();
                    cout << endl;
                }
            }
            else {
            cout << "Seat " << col << row << " is not occupiable in ";
            m->displayMovieInfo();
            cout << endl;
            }
         }
         else {
            time_t t = movieID;
            string time = ctime(&t);
            time = time.substr(0, time.size() - 1); //To delete the newline character
            cout << "Movie at " << time << " does not exist" << endl;
            return -1;
         }
         return 0;
    }


    void MovieBookingSystem::cancelReservations(ReservationList resCode) {

        int numRes = resCode.getLength();
        int res = numRes;

        int resLength = reservationCodes.getLength();

        for(int i = 0; i < numRes; i++) {
            for(int j = 0; j < resLength; j++) {
                int code, cancelCode;

                reservationCodes.retrieve(j, code);
                resCode.retrieve(i, cancelCode);

                if(code == cancelCode)
                    res--;
            }
        }


        if(res == 0) {

            Movie* m;
            string seat;

            for(int i = 0; i < numRes; i++) {
               for(int j = 0; j < reservationCodes.getLength(); j++) {
                    int code, cancelCode;
                    reservationCodes.retrieve(j, code);
                    resCode.retrieve(i, cancelCode);
                    if(code == cancelCode) {
                        reservationCodes.retrieveMovie(j, m);
                        reservationCodes.retrieveSeat(j, seat);
                        char col = seat.at(0);

                        int row;

                        stringstream ss(seat.substr(1));
                        ss >> row;

                        int r = m->getRadius();

                        m->cancelSeat((row - 1) / (r + 1), (col - 'A') / (r + 1));

                        reservationCodes.remove(j);
                        j--;


                        cout << "Reservation on Code " << cancelCode << " is canceled " << "Seat: " << seat << " in ";
                        m->displayMovieInfo();
                        cout << endl;
                    }
               }
            }

        }
        else {
            cout << "Some codes do not exist. Cancelation is failed." << endl;
        }
    }


    void MovieBookingSystem::showReservation( const int resCode) const{
        int resLength = reservationCodes.getLength();
        bool found = false;
        int i = 0;
        int code;
        string seat;
        Movie* movie;

        while(!found && i < resLength) {
            reservationCodes.retrieve(i, code);
            if(resCode == code) {
                reservationCodes.retrieveSeat(i, seat);
                reservationCodes.retrieveMovie(i, movie);
                found = true;
            }
            i++;
        }
        if(found) {
            cout << "Reservation with Code " << code << ": Seat " << seat << " ";
            movie->displayMovieInfo();
            cout << endl;
        }
        else {
            cout << "No reservation with code " << resCode << endl;
        }

    }




// ------------------ Movie class --------------------


    Movie::Movie() {
        id = 0;
        radius = 0;
        head = NULL;
    }

    Movie::~Movie() {

        RowNode* curHead = head;
        while(curHead != NULL) {
            SeatNode* rowHead = curHead->rowHead;
            while(rowHead != NULL) {
                SeatNode* cur = rowHead;
                rowHead = rowHead->next;
                delete cur;
            }
            curHead = curHead->next;
        }

        while(head != NULL) {
            curHead = head;
            head = head->next;
            delete curHead;
        }

    }

    Movie& Movie::operator=( const Movie& right ) {
         if ( &right != this ) { // to avoid self-assignment

            id = right.id;
            seat = right.seat;
            time = right.time;
            radius = right.radius;

            RowNode* newRow;
            SeatNode* newSeat;

            for(RowNode* curRow = right.head; curRow != NULL; curRow = curRow->next) {

                newSeat = new SeatNode;
                if(head == NULL) {
                    newRow = new RowNode;
                    newSeat->seat = curRow->rowHead->seat;
                    newRow->rowHead = newSeat;
                    head = newRow;
                }
                else {
                    newRow->next = new RowNode;
                    newSeat->seat = curRow->rowHead->seat;
                    newRow = newRow->next;
                    newRow->rowHead = newSeat;
                }
                newRow->rowHead = newSeat;
                for(SeatNode* cur = curRow->rowHead->next; cur != NULL; cur = cur->next) {
                    newSeat->next = new SeatNode;
                    newSeat = newSeat->next;
                    newSeat->seat = cur->seat;
                }
                newSeat->next = NULL;

            }
            newRow->next = NULL;

         }
            return *this; // to allow cascading
    }


    Movie::Movie(const long movieId, const int r) {
        const int rowNumber = ((MovieBookingSystem::numOfRow - 1) / (r + 1) + 1);
        const int columnNumber = ((MovieBookingSystem::numOfColumn - 1) / (r + 1) + 1);
        id = movieId;
        radius = r;
        seat = rowNumber * columnNumber;

        time_t t = id;
        time = ctime(&t);
        time = time.substr(0, time.size() - 1) + " "; //To delete the newline character

        head = NULL;
        RowNode* curRow = NULL;
         for (int j = 0; j < rowNumber; j++) {

            SeatNode* cur;
            SeatNode* newNode = new SeatNode;
            newNode->seat = 'o';

            RowNode* newRow = new RowNode;
            newRow->rowHead = newNode;
            if(head == NULL) {
                head = newRow;
                curRow = head;
            }
            else {
                curRow->next = newRow;
                curRow = curRow->next;
            }
            cur = newNode;
            for(int i = 0; i < columnNumber - 1; i++) {
                newNode = new SeatNode;
                newNode->seat = 'o';
                cur->next = newNode;
                cur = cur->next;
            }
            cur->next = NULL;
        }
        curRow->next = NULL;
    }

    long Movie:: getId() const {
        return id;
    }

    int Movie::getRadius() const {
        return radius;
    }


    int Movie::getSeat() const {
        return seat;
    }

    void Movie::setSeat(const int s) {
        seat = s;
    }

    void Movie::displayMovieInfo() {
        cout << "Movie at " << time;
    }

    void Movie::displayMovieSeats() {

        displayMovieInfo();
        cout << "has " << seat << " available seats" << endl;
        string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
         cout << "   ";
         for(int i = 0; i < 26; i = i + radius + 1) {
            cout << letters.at(i) << " ";
         }
         cout << endl;

         int j = 0;
         for(RowNode* curRow = head; curRow != NULL; curRow = curRow->next) {
            int rowNo = j * (radius + 1) + 1;
                if(rowNo < 10)
                    cout << " ";
                cout << rowNo << " ";
            for(SeatNode* cur = curRow->rowHead; cur != NULL; cur = cur->next)
                cout << cur->seat << " ";
            cout << endl;
            j++;
         }

    }

    bool Movie::occupySeat(const int row, const int col) {

        RowNode* curRow = head;
        SeatNode* cur;
        for(int i = 0; i < row; i++) {
            curRow = curRow->next;
        }
        cur = curRow->rowHead;
        for(int i = 0; i < col; i++) {
            cur = cur->next;
        }
        if(cur->seat == 'o') {
            cur->seat = 'x';
            seat--;
            return true;
        }

        return false;
    }

    void Movie::cancelSeat(const int row, const int col) {
        RowNode* curRow = head;
        SeatNode* cur;

        for(int i = 0; i < row; i++) {
            curRow = curRow->next;
        }
        cur = curRow->rowHead;
        for(int i = 0; i < col; i++) {
            cur = cur->next;
        }
        cur->seat = 'o';
        seat++;
    }



// ------------------ ReservationList class --------------------



    ReservationList::ReservationList() {
        head = NULL;
        size = 0;
    }

    ReservationList::ReservationList(const ReservationList& aList) {
        if(aList.isEmpty()) {
            head = NULL;
            size = 0;
        }
        else {
            size = aList.size;
            head = new ReservationNode();
            head->Code = aList.head->Code;
            ReservationNode* newNode = head;
            for(ReservationNode* cur = aList.head->next; cur != NULL; cur = cur->next) {
                newNode->next = new ReservationNode();
                newNode = newNode->next;
                newNode->Code = cur->Code;
            }
            newNode->next = NULL;

        }
    }

    ReservationList::~ReservationList() {
        while(!isEmpty()) {
            remove(0);
        }
    }

    bool ReservationList::isEmpty() const {
        return size == 0;
    }

    int ReservationList::getLength() const {
        return size;
    }

    bool ReservationList::retrieve (int index, int& resCode) const {
        if(index < 0 || index >= size) {
            return false;
        }

        ReservationNode* cur = find(index);
        resCode = cur->Code;
        return true;

    }

    bool ReservationList::retrieveMovie(int index, Movie*& m) const {
        if(index < 0 || index >= size) {
            return false;
        }

        ReservationNode* cur = find(index);
        m = cur->movie;
        return true;

    }

    bool ReservationList::retrieveSeat(int index, string& seat) const {
        if(index < 0 || index >= size) {
            return false;
        }

        ReservationNode* cur = find(index);
        seat = cur->seat;
        return true;

    }

    void ReservationList:: setReservInfo(int resCode, Movie* m, string seat) {
        for(ReservationNode* cur = head; cur != NULL; cur = cur->next) {
            if(cur->Code == resCode) {
                cur->movie = m;
                cur->seat = seat;
            }
        }
    }

    bool ReservationList::insert(int index, int resCode) {
        if(index < 0 || index > size) {
            return false;
        }

        size++;
        ReservationNode* newNode = new ReservationNode();
        newNode->Code = resCode;
        if(index == 0) {
            newNode->next = head;
            head = newNode;
        }
        else {
            ReservationNode* prev = find(index - 1);
            newNode->next = prev->next;
            prev->next = newNode;
        }
        return true;

    }

    bool ReservationList::remove(int index){
        ReservationNode* cur;
        if(index < 0 || index >= size) {
            return false;
        }
        else {
            --size;
            if(index == 0) {
                cur = head;
                head = head->next;
            }
            else {
                ReservationNode* prev = find(index - 1);
                cur = prev->next;
                prev->next = cur->next;

            }
            cur->next = NULL;
            delete cur;
            cur = NULL;
            return true;
        }
    }

    ReservationList::ReservationNode* ReservationList::find(int index) const {
        if(index < 0 || index >= size)
            return NULL;

        ReservationNode* cur = head;
        for(int i = 0; i < index; i++) {
            cur = cur->next;
        }
        return cur;
    }

    void ReservationList::linkedListInsert(ReservationNode *&headPtr,
                      int newItem){

   if ((headPtr == NULL) ||
       (newItem < headPtr->Code)){

      ReservationNode *newPtr = new ReservationNode;
      newPtr->Code = newItem;
      newPtr->next = headPtr;
      headPtr = newPtr;
   }
   else
      linkedListInsert(headPtr->next,newItem);
}

    // ------------------ MovieList class --------------------



    MovieList::MovieList() {
        head = NULL;
        size = 0;
    }

    MovieList::MovieList(const MovieList& aList) {

    }

    MovieList::~MovieList() {
        while(!isEmpty()) {
                remove(0);
        }
    }

    bool MovieList::isEmpty() const {
        return size == 0;
    }
    int MovieList::getLength() const {
        return size;
    }

    bool MovieList::retrieve(int index, Movie*& m) const {
        if(index < 0 || index >= size) {
            return false;
        }

        MovieNode* cur = find(index);
        m = &(cur->movie);
        return true;
    }

    void MovieList::insertTail (const Movie& m) {

        size++;
        MovieNode* newMovie = new MovieNode();
        newMovie->movie = m;
        if(head == NULL) {
            newMovie->next = head;
            head = newMovie;
            tail = head;
        }
        else {
            tail->next = newMovie;
            tail = newMovie;
        }
    }

    bool MovieList::remove(int index) {
        if(index < 0 || index > size) {
            return false;
        }

        MovieNode* cur = head;
        --size;
        if(index == 0) {
            head = head->next;
        }
        else {
            MovieNode* prev = find(index - 1);
            cur = prev->next;
            prev->next = cur->next;
        }
        cur->next = NULL;
        delete cur;
        cur = NULL;
        return true;
    }

    MovieList::MovieNode* MovieList::find(int index) const {
        if(index < 0 || index >= size) {
            return NULL;
        }

        MovieNode * cur = head;
        for(int i = 0; i < index; i++) {
            cur = cur->next;
        }
        return cur;
    }





