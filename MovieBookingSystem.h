/**
 * @author Furkan Güzelant ID: 21901515
 * @version 03.05.2021

*/



#ifndef __MOVIEBOOKINGSYSTEM_H
#define __MOVIEBOOKINGSYSTEM_H
#include <string>
using namespace std;


class Movie{
public:

    Movie();
    ~Movie();
    Movie(const long movieId, const int r);
    Movie& operator=( const Movie& right );
    long getId() const;
    int getRadius() const;
    int getSeat() const;
    void setSeat(const int s);
    bool occupySeat(const int row, const int col);
    void cancelSeat(const int row, const int col);
    void displayMovieInfo();
    void displayMovieSeats();

private:
    struct SeatNode {
        char seat;
        SeatNode* next;
    };
    struct RowNode {
        SeatNode* rowHead;
        RowNode* next;
    };
    Movie::SeatNode *find(int index) const;
    RowNode* head;

    int radius;
    long id;
    int seat;
    string time;
};


class ReservationList{
public:
    ReservationList();
    ReservationList( const ReservationList& aList );
    ~ReservationList();
    bool isEmpty() const;
    int getLength() const ;
    bool retrieve(int index, int& resCode) const;
    bool retrieveMovie(int index, Movie*& m) const;
    bool retrieveSeat(int index, string& s) const;
    bool insert(int index, int resCode);
    void setReservInfo(int resCode, Movie* m, string seat);
    bool remove(int index);

private:
    struct ReservationNode{
        int Code;
        Movie* movie;
        string seat;
        ReservationNode* next;
    };
    int size;

    ReservationNode *find(int index) const;

public:
    ReservationNode *head;
    void linkedListInsert(ReservationNode *&headPtr,
                      int newItem);
// ...
//you may define additional member functions and data members, if
//necessary
};


class MovieList {
public:
    MovieList();
    MovieList(const MovieList& aList);
    ~MovieList();
    bool isEmpty() const;
    int getLength() const ;
    bool retrieve(int index, Movie*& m) const;
    void insertTail(const Movie& m);
    bool remove(int index);
private:
    struct MovieNode{
        Movie movie;
        MovieNode* next;
    };
    int size;
    MovieNode *head;
    MovieNode *tail;
    MovieNode *find(int index) const;
};

class MovieBookingSystem{
public:
const static int numOfRow = 30;
const static int numOfColumn = 26;
MovieBookingSystem();
~MovieBookingSystem();
void addMovie( const long movieID, const int audienceRadius );
void cancelMovie( const long movieID);
void showAllMovies() const;
void showMovie( const long movieID) const;
int makeReservation( const long movieID, const int row, const char
col);
void cancelReservations( ReservationList resCode);
void showReservation( const int resCode) const;
private:
    ReservationList reservationCodes;
    MovieList movies;
    int reservCode;
// ...
//you may define additional member functions and data members, if
//necessary.
};


#endif
