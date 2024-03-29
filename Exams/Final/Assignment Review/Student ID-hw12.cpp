#include <iostream>
#include <iomanip>
#include <fstream>
//#include <ctime>
using namespace std;

struct Date
{
    int year;
    int month;
    int day;
};

struct Reservation
{
    char flightNo[ 8 ]; // flight number
    char id[ 12 ];      // ID number
    char name[ 8 ];     // name
    char mobile[ 12 ];  // mobile phone number
    char date[ 12 ];    // departure date
    int tickets[ 8 ];
    // tickets[ 0 ]: not used
    // tickets[ 1 ]: number of full fare tickets
    // tickets[ 2 ]: number of child tickets
    // tickets[ 3 ]: number of infant tickets
    // tickets[ 4 ]: number of senior citizen tickets
    // tickets[ 5 ]: number of impaired tickets
    // tickets[ 6 ]: number of impaired companion tickets
    // tickets[ 7 ]: number of military tickets
};

struct Flight
{
    char flightNo[ 8 ];      // flight number
    char aircraft[ 8 ];      // aircraft model
    int departureAirport;    // departure airport code
    int arrivalAirport;      // arrival airport code
    char departureTime[ 8 ]; // departure time
    char arrivalTime[ 8 ];   // arrival time
    int dayWeek[ 7 ];        // the value of dayWeek[ i ] is 0 or 1
    // dayWeek[ 0 ] == 1 if and only if the flight flies on Sunday
    // dayWeek[ 1 ] == 1 if and only if the flight flies on Monday
    // dayWeek[ 2 ] == 1 if and only if the flight flies on Tuesday
    // dayWeek[ 3 ] == 1 if and only if the flight flies on Wednesday
    // dayWeek[ 4 ] == 1 if and only if the flight flies on Thursday
    // dayWeek[ 5 ] == 1 if and only if the flight flies on Friday
    // dayWeek[ 6 ] == 1 if and only if the flight flies on Saturday
};

struct FlightSeats
{
    char date[ 12 ];      // date
    char flightNo[ 8 ];   // flight number
    int departureAirport; // departure airport code
    int arrivalAirport;   // arrival airport code
    int availableSeats;   // the number of available seats
};

const char airportName[ 12 ][ 12 ] = { "", "Taipei",    "Taichung", "Chiayi",  "Tainan",
                                            "Kaohsiung", "Hualien", "Taitung",
                                            "Penghu",    "Kinmen",   "Beigan",  "Nangan" };

// row 0 and column 0 are not used,
// fullFare[ i ][ j ] is the full fare from airport i to airport j
const int fullFare[ 12 ][ 12 ] = {
    { 0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0 },
    { 0,    0,    0,    0,    0,    0, 1781, 2460, 2296, 2618, 2197, 2197 },
    { 0,    0,    0,    0,    0,    0,    0,    0, 1769, 2236,    0, 2686 },
    { 0,    0,    0,    0,    0,    0,    0,    0, 1721, 2217,    0,    0 },
    { 0,    0,    0,    0,    0,    0,    0,    0, 1697, 2231,    0,    0 },
    { 0,    0,    0,    0,    0,    0,    0,    0, 1876, 2393,    0,    0 },
    { 0, 1781,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0 },
    { 0, 2460,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0 },
    { 0, 2296, 1769, 1721, 1697, 1876,    0,    0,    0, 1768,    0,    0 },
    { 0, 2618, 2236, 2217, 2231, 2393,    0,    0, 1768,    0,    0,    0 },
    { 0, 2197,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0 },
    { 0, 2197, 2686,    0,    0,    0,    0,    0,    0,    0,    0,    0 } };

const int discount[ 8 ] = { 0, 100, 75, 0, 50, 50, 50, 95 };
// discount[ 0 ]: not used
// discount[ 1 ]: Full Fare Ticket (肂布)
// discount[ 2 ]: Child Ticket (ㄠ担布):                 25% off of the full fare
// discount[ 3 ]: Infant Ticket (捆ㄠ布):                Free of charge
// discount[ 4 ]: Senior Citizen Ticket (穛ρ布):        50% off of the full fare
// discount[ 5 ]: Impaired Ticket (稲み布):              50% off of the full fare
// discount[ 6 ]: Impaired Companion Ticket (稲み抄布): 50% off of the full fare
// discount[ 7 ]: Military Ticket (瓁布):               5% off of the full fare

const int maxNumReservations = 1000;

int enterChoice();

// perform booking
void booking();

// choose arrival airport by input its code, and assign the code to arrivalAirport
void chooseArrivalAirport( int departureAirport, int &arrivalAirport );

// input the departure date
void inputDepartureDate( char date[], int &departureDayWeek );

// return the number of days between currentDate and departureDate
int difference( Date departureDate, Date currentDate );

bool leapYear( int year );

// load all flights from Flight Schedule.dat and put them into the array flights
void loadFlightSchedules( Flight flights[], int &numFlights );

// load all flights from departureAirport to arrivalAirport,
// and put them into the array flights
void loadFlightSchedules( Flight flights[], int &numFlights,
        int departureAirport, int arrivalAirport, int departureDayWeek );

// load all flightSeats from departureAirport to arrivalAirport on departureDate,
// and put them into the array flightSeats
void loadFlightSeats( FlightSeats flightSeats[], int &numFlightSeats, char departureDate[],
                        int departureAirport, int arrivalAirport );

// display all flights from departureAirport to arrivalAirport
void displayFlights( const Flight flights[], int numFlights,
                    FlightSeats flightSeats[], int &numFlightSeats,
                    int departureAirport, int arrivalAirport, char date[] );

// choose a flight by input its number,
// and assign the number to reservation.flightNo
void chooseFlight( Reservation &reservation, const Flight flights[], int numFlights );

// input the number of tickets for each ticket type,
// and assign them to reservation.tickets
void inputNumTickets( Reservation &reservation, FlightSeats flightSeats[], int &numFlightSeats );

// during booking and refund application,
// the corresponding availableSeats in Flight Seats.dat should be modified
void modifyFlightSeatsFile( Reservation &reservation, int totalNumTickets );

void displayReservation( const Reservation reservation,
                        const Flight flights[], int numFlights );

// append reservation to the end of Reservations.dat
void saveReservation( const Reservation reservation );

// perform booking inquiry
bool bookingInquiry( const char id[] );

// perform refund application
void refundApplication();

// load all reservations from Reservations.dat and put them to the array reservations
void loadReservations( Reservation reservations[], int &numReservations );

int main()
{
    cout << "Welcome to UNI Air Domestic Flights Booking System\n";
    char id[ 12 ];
    int choice;

    while( ( choice = enterChoice() ) != 4 )
    {
        switch( choice )
        {
        case 1:
            booking(); // perform booking
            break;
        case 2:
            cout << "\nEnter ID number: ";
            cin >> id;
            bookingInquiry( id ); // perform booking inquiry
            break;
        case 3:
            refundApplication(); // perform refund application
            break;

        default:
            cerr << "\nInput error!\n";
            break;
        }
    }

    cout << endl;
    system( "pause" );
}

int enterChoice()
{
    cout << "\nInput your choice:\n"
        << "1. Booking\n"
        << "2. Booking enquiry\n"
        << "3. Refund application\n"
        << "4. End\n? ";

    int menuChoice;
    cin >> menuChoice;
    return menuChoice;
}

// perform booking
void booking()
{
    // choose departure airport by input its code,
    // and assign the code to departureAirport
    int departureAirport;
    do {
        cout << "\nDeparture airport:\n";
        for (int i = 1; i <= 11; i++)
            cout << setw(2) << i << ". " << airportName[i] << endl;
        cout << "? ";

        cin >> departureAirport;
    } while (departureAirport < 1 || departureAirport > 11);

    int arrivalAirport;
    // choose arrival airport by input its code, and assign the code to arrivalAirport
    chooseArrivalAirport(departureAirport, arrivalAirport);

    Reservation reservation = { "", "", "", "", "", 0, 0, 0, 0, 0, 0, 0, 0 };

    int departureDayWeek; // departureDayWeek == 0 if departure date is Sunday
    // departureDayWeek == 1 if departure date is Monday
    // departureDayWeek == 2 if departure date is Tuesday
    // departureDayWeek == 3 if departure date is Wednesday
    // departureDayWeek == 4 if departure date is Thursday
    // departureDayWeek == 5 if departure date is Friday
    // departureDayWeek == 6 if departure date is Saturday

    inputDepartureDate(reservation.date, departureDayWeek);

    Flight flights[20];
    int numFlights = 0;

    // load all flights from departureAirport to arrivalAirport,
    // and put them into the array flights
    loadFlightSchedules(flights, numFlights, departureAirport, arrivalAirport,
        departureDayWeek);

    if (numFlights == 0) {
        cout << "No flights from " << airportName[departureAirport] <<
            " to " << airportName[arrivalAirport] << " on " << reservation.date << "! \n";
        return;
    }

    FlightSeats flightSeats[20];
    int numFlightSeats = 0;

    loadFlightSeats(flightSeats, numFlightSeats, reservation.date,
        departureAirport, arrivalAirport);

    int maxNumSeats = 0;
    for (int i = 1; i <= numFlights; i++)
        if (maxNumSeats < flightSeats[i].availableSeats)
            maxNumSeats = flightSeats[i].availableSeats;

    if (maxNumSeats == 0)
    {
        cout << "No available seats from " << airportName[departureAirport] <<
            " to " << airportName[arrivalAirport] << " on " << reservation.date << "!\n";
        return;
    }

    // display all flights from departureAirport to arrivalAirport
    displayFlights(flights, numFlights, flightSeats, numFlightSeats,
        departureAirport, arrivalAirport, reservation.date);

    // choose a flight by input its number, and assign the number to reservation.flightNo
    chooseFlight(reservation, flights, numFlights);

    // input the number of tickets for each ticket type,
    // and assign them to reservation.tickets
    inputNumTickets(reservation, flightSeats, numFlightSeats);

    int totalNumTickets = -reservation.tickets[3];
    for (int i = 1; i < 8; i++)
        totalNumTickets += reservation.tickets[i];

    if (totalNumTickets > 0)
        modifyFlightSeatsFile(reservation, totalNumTickets);
    else
    {
        cout << "Your booking could not be completed!\n";
        return;
    }

    displayReservation(reservation, flights, numFlights);

    cout << "\nID number: ";
    cin >> reservation.id;
    cout << "\nName: ";
    cin >> reservation.name;
    cout << "\nMobile: ";
    cin >> reservation.mobile;

    // append reservation to the end of Reservations.dat
    saveReservation(reservation);

    cout << "\nBooking completed!\n";
}

// choose arrival airport by input its code, and assign the code to arrivalAirport
void chooseArrivalAirport( int departureAirport, int &arrivalAirport )
{
    int can[100]{};
    do {
        cout << "\nArrival Airport:\n";
        for (int i = 1; i <= 11; i++) {
            if (fullFare[departureAirport][i] != 0) {
                cout << right << setw(2) << i << ". " << airportName[i] << endl;
                can[i] = 1;
            }
        }
        cout << "? ";
        cin >> arrivalAirport;
    } while (can[arrivalAirport] == 0); // 狦ノfullFare[departureAirport][arrivalAirport] == 0 穦Τ块禬筁11猵ㄆ
}

void inputDepartureDate( char date[], int &departureDayWeek )
{
    tm structuredTime;
    time_t rawTime = time( nullptr );
    localtime_s( &structuredTime, &rawTime );

    int year = structuredTime.tm_year + 1900;
    int month = structuredTime.tm_mon + 1;
    int day = structuredTime.tm_mday;
    
    Date now{ year, month, day };
    
    int choice;
    int mdLeap[13]{ 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int mdNormal[13]{ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int bufM = month, bufY = year;
    do {
        cout << "\nDeparture Date (Month):\n";
        for (int i = 1; i <= 6; i++, bufM++) {
            if (bufM > 12) {
                bufM -= 12;
                bufY++;
            }
            cout << i << ". " << bufY << "-" << bufM << endl;
        }
        cout << "? ";
        cin >> choice;
    } while (choice < 1 || choice > 6);

    int deMonth = month + choice - 1; /*匡兜眖1秨﹍ 块セる琌month + 1 - 1*/
    int deYear = year;
    int deDay;
    int firstDay = 1, lastDay;
    
    if (deMonth > 12) {
        deMonth -= 12;
        deYear++;
    }

    do {
        if (leapYear(deYear)) {
            if (deMonth == month) {
                cout << "\nDay (" << day << " ~ " << mdLeap[deMonth] << "): ";
                firstDay = day;
            }
            else {
                cout << "\nDay (1 ~ " << mdLeap[deMonth] << "): ";
            }
            lastDay = mdLeap[deMonth];
        }
        else {
            if (deMonth == month) {
                cout << "\nDay (" << day << " ~ " << mdNormal[deMonth] << "): ";
                firstDay = day;
            }
            else {
                cout << "\nDay (1 ~ " << mdNormal[deMonth] << "): ";
            }
            lastDay = mdNormal[deMonth];
        }

        cin >> deDay;
    } while (deDay < firstDay || deDay > lastDay);

    Date bye{ deYear, deMonth, deDay };

    char deY[5], deM[3], deD[3];
    char bufDate[12] = "";

    _itoa_s(deYear, deY, 10); _itoa_s(deMonth, deM, 10); _itoa_s(deDay, deD, 10);

    strcat_s(bufDate, 12, deY); strcat_s(bufDate, 12, "/"); strcat_s(bufDate, 12, deM); 
    strcat_s(bufDate, 12, "/"); strcat_s(bufDate, 12, deD);
    strcpy_s(date, 12, bufDate);

    int diffDay = difference(bye, now);
    cout << diffDay << endl;
    diffDay %= 7;
    departureDayWeek = structuredTime.tm_wday + diffDay;

    if (departureDayWeek > 6) departureDayWeek -= 7;
    cout << departureDayWeek << endl;
}

int difference( Date departureDate, Date currentDate )
{
    int mdLeap[13]{ 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int mdNormal[13]{ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int diffDays{}, 
        diffMon = departureDate.month > currentDate.month ? departureDate.month - currentDate.month : 12 - currentDate.month + departureDate.month;


    if (leapYear(departureDate.year) || leapYear(currentDate.year)) {
        diffDays += mdLeap[currentDate.month] - currentDate.day + departureDate.day;
        if (diffMon > 1 || (diffMon == 1 && departureDate.year != currentDate.year)) {
            for (int i = currentDate.month + 1, times = 1; times < diffMon; i++, times++) {
                if (i > 12) {
                    i -= 12;
                }
                diffDays += mdLeap[i];
            }
        }
        else if (diffMon == 1) {
            diffDays += mdLeap[currentDate.month] - currentDate.day;
            diffDays += departureDate.day;
        }
        else {
            diffDays = departureDate.day - currentDate.day;
        }
    }
    else {
        cout << ": " << diffMon << endl;
        if (diffMon > 1 || (diffMon == 1 && departureDate.year != currentDate.year)) {
            diffDays += mdLeap[currentDate.month] - currentDate.day + departureDate.day;
            for (int i = currentDate.month + 1, times = 1; times < diffMon; i++, times++) {
                if (i > 12) {
                    i -= 12;
                }
                diffDays += mdNormal[i];
            }
        }
        else if (diffMon == 1) {
            diffDays += mdNormal[currentDate.month] - currentDate.day;
            diffDays += departureDate.day;
        }
        else {
            diffDays = departureDate.day - currentDate.day;
        }
    }

    return diffDays;
}

bool leapYear( int year )
{
    return ( year % 400 == 0 || ( year % 4 == 0 && year % 100 != 0 ) );
}

// load all flights from Flight Schedule.dat and put them into the array flights
void loadFlightSchedules( Flight flights[], int &numFlights )
{
    ifstream flight("Flight Schedule.dat", ios::in| ios::binary);

    if (!flight) {
        cerr << "Flight Schedule in f(loadFlightSchedules) cannot be opened!\n";
        exit(1);
    }
    flight.seekg(0, ios::end);
    numFlights = flight.tellg() / sizeof(Flight);
    flight.seekg(0, ios::beg);

    for (int i = 1; i <= numFlights; i++) {
        flight.read(reinterpret_cast<char*>(&flights[i]), sizeof(Flight));
    }

    flight.clear(); flight.close();
}

// load all flights from departureAirport to arrivalAirport,
// and put them into the array flights
void loadFlightSchedules( Flight flights[], int &numFlights,
        int departureAirport, int arrivalAirport, int departureDayWeek )
{
    ifstream flight("Flight Schedule.dat", ios::in | ios::binary);
    if (!flight) {
        cerr << "Flight Schedule in f(loadFlightSchedules) cannot be opened!\n";
        exit(1);
    }
    Flight buf;
    flight.seekg(0, ios::end);
    int numAll = flight.tellg() / sizeof(Flight);
    flight.seekg(0, ios::beg);
    for (int i = 1; i <= numAll; i++) {
        flight.read(reinterpret_cast<char*>(&buf), sizeof(Flight));
        if (buf.departureAirport == departureAirport && buf.arrivalAirport == arrivalAirport
            && buf.dayWeek[departureDayWeek] == 1) {
            flights[++numFlights] = buf;
        }
    }

    flight.clear(); flight.close();
}

void loadFlightSeats( FlightSeats flightSeats[], int &numFlightSeats, char departureDate[],
        int departureAirport, int arrivalAirport )
{
    ifstream flightSeat("Flight Seats.dat", ios::in | ios::binary);

    if (!flightSeat) {
        cerr << "Flight Seats in f(loadFlightSeats) cannot be opened!\n";
        exit(1);
    }

    FlightSeats buf;
    flightSeat.seekg(0, ios::end);
    int bufNum = flightSeat.tellg() / sizeof(FlightSeats);
    flightSeat.seekg(0, ios::beg);

    for (int i = 1; i < bufNum; i++) {
        flightSeat.read(reinterpret_cast<char*>(&buf), sizeof(FlightSeats));
        if (buf.departureAirport == departureAirport && buf.arrivalAirport == arrivalAirport
            && strcmp(buf.date, departureDate) == 0) {
            flightSeats[++numFlightSeats] = buf;
        }
    }

    flightSeat.clear(); flightSeat.close();
}

// display all flights from departureAirport to arrivalAirport
void displayFlights( const Flight flights[], int numFlights,
                    FlightSeats flightSeats[], int &numFlightSeats,
                    int departureAirport, int arrivalAirport, char date[] )
{
    cout << airportName[ departureAirport ] << " to " << airportName[ arrivalAirport ]
        << " / Depart date: " << date << ".\n\n";
    cout << "Flight   Departure   Arrival   Fare   No. Available Seats\n";

    for( int i = 1; i <= numFlights; i++ )
        if( flightSeats[ i ].availableSeats  > 0 )
            cout << setw( 6 ) << flights[ i ].flightNo
                << setw( 12 ) << flights[ i ].departureTime
                << setw( 10 ) << flights[ i ].arrivalTime
                << setw( 7 ) << fullFare[ departureAirport ][ arrivalAirport ]
                << setw( 22 ) << flightSeats[ i ].availableSeats << endl;
}

// choose a flight by input its number, and assign the number to reservation.flightNo
void chooseFlight( Reservation &reservation, const Flight flights[], int numFlights )
{
    bool have = false;
    do {
        cout << "\nFlight No.: ";
        cin >> reservation.flightNo;
        for (int i = 1; i <= numFlights; i++) {
            if (strcmp(flights[i].flightNo, reservation.flightNo) == 0) {
                have = true; break;
            }
        }
    } while (!have);
}

// input the number of tickets for each ticket type, and assign them to reservation.tickets
void inputNumTickets( Reservation &reservation, FlightSeats flightSeats[], int &numFlightSeats )
{
    int i;
    for( i = 1; i <= numFlightSeats; i++ )
        if( strcmp( reservation.flightNo, flightSeats[ i ].flightNo ) == 0 )
            break;

    int numAvailableSeats = flightSeats[ i ].availableSeats;
    int maxNumTickets = ( numAvailableSeats < 4 ) ? numAvailableSeats : 4;

    do {
        cout << "\nNumber of infant tickets (0 ~ " << maxNumTickets << "): ";
        cin >> reservation.tickets[ 3 ];
    } while( reservation.tickets[ 3 ] < 0 || reservation.tickets[ 3 ] > maxNumTickets );

    int numTickets = 0;
    int numAdults = 0;
    do
    {
        numTickets = 0;

        cout << "\nNumber of full fare tickets (0 ~ " << maxNumTickets << "): ";
        cin >> reservation.tickets[ 1 ];
        numTickets += reservation.tickets[ 1 ];

        cout << "\nNumber of child tickets (0 ~ " << maxNumTickets << "): ";
        cin >> reservation.tickets[ 2 ];
        numTickets += reservation.tickets[ 2 ];

        cout << "\nNumber of senior citizen tickets (0 ~ " << maxNumTickets << "): ";
        cin >> reservation.tickets[ 4 ];
        numTickets += reservation.tickets[ 4 ];

        cout << "\nNumber of impaired tickets (0 ~ " << maxNumTickets << "): ";
        cin >> reservation.tickets[ 5 ];
        numTickets += reservation.tickets[ 5 ];

        cout << "\nNumber of impaired companion tickets (0 ~ " << maxNumTickets << "): ";
        cin >> reservation.tickets[ 6 ];
        numTickets += reservation.tickets[ 6 ];

        cout << "\nNumber of military tickets (0 ~ " << maxNumTickets << "): ";
        cin >> reservation.tickets[ 7 ];
        numTickets += reservation.tickets[ 7 ];

        numAdults = reservation.tickets[ 1 ] + reservation.tickets[ 4 ] +
                    reservation.tickets[ 5 ] + reservation.tickets[ 6 ] +
                    reservation.tickets[ 7 ];

        if( numTickets > maxNumTickets )
            cout << "\nYou can books a maximum of " << maxNumTickets
                << " passengers ( including children ).\n";
        else if( numAdults < reservation.tickets[ 3 ] )
            cout << "\nEach infant must always be accompanied by at least one adult.\n";
    } while( numTickets > maxNumTickets || numAdults < reservation.tickets[ 3 ] );

    cout << endl;
}

void modifyFlightSeatsFile( Reservation &reservation, int totalNumTickets )
{
    fstream edit("Flight Seats.dat", ios::in | ios::out | ios::binary);

    if (!edit) {
        cerr << "Flight Seats in f(loadFlightSeats) cannot be opened!\n";
        exit(1);
    }

    FlightSeats buf;
    edit.seekg(0, ios::end);
    int all = edit.tellg() / sizeof(FlightSeats);
    edit.seekg(0, ios::beg);

    for (int i = 1; i <= all; i++) {
        edit.read(reinterpret_cast<char*>(&buf), sizeof(FlightSeats));
        if (strcmp(buf.date, reservation.date) == 0 && strcmp(buf.flightNo, reservation.flightNo) == 0) {
            buf.availableSeats -= totalNumTickets;
            edit.seekg(-(int)sizeof(FlightSeats), ios::cur);
            edit.write(reinterpret_cast<const char*>(&buf), sizeof(FlightSeats));
            break;
        }
    }

    edit.clear(); edit.close();
}

void displayReservation( const Reservation reservation,
                            const Flight flights[], int numFlights )
{
    int k;
    for( k = 1; k <= numFlights; k++ )
        if( strcmp( reservation.flightNo, flights[ k ].flightNo ) == 0 )
            break;

    cout << "Ticket information:\n\n";
    cout << "Date: " << reservation.date << endl;
    cout << "Flight: B7-" << reservation.flightNo << endl << endl;

    int departureAirport = flights[ k ].departureAirport;
    int arrivalAirport = flights[ k ].arrivalAirport;

    cout << setw( 9 ) << right << airportName[ departureAirport ] << " -> "
        << setw( 9 ) << left << airportName[ arrivalAirport ] << endl;

    cout << setw( 9 ) << right << flights[ k ].departureTime << "    ";
    cout << setw( 9 ) << left << flights[ k ].arrivalTime << endl << endl;

    char ticketType[ 8 ][ 24 ] = { "", "Full Fare", "Child Fare", "Infant Fare",
                                        "Senior Citizen Fare",     "Impaired Fare",
                                        "Impaired Companion Fare", "Military Fare" };

    int total = 0;
    int fare;
    for( int i = 1; i <= 7; i++ )
        if( reservation.tickets[ i ] > 0 )
        {
            fare = fullFare[ departureAirport ][ arrivalAirport ] * discount[ i ] / 100;
            total += ( fare * reservation.tickets[ i ] );
            cout << right << setw( 23 ) << ticketType[ i ] << "  TWD "
                << setw( 4 ) << fare << " x " << reservation.tickets[ i ] << endl;
        }

    cout << "\nTotal: " << total << endl;
}

// append reservation to the end of Reservations.dat
void saveReservation( const Reservation reservation )
{
    ofstream save("Reservations.dat", ios::app | ios::binary);

    if (!save) {
        cerr << "Reservations cannot be opened!\n";
        exit(1);
    }
    save.write(reinterpret_cast<const char*>(&reservation), sizeof(Reservation));

    save.clear(); save.close();
}

// perform booking inquiry
bool bookingInquiry( const char id[] )
{
    Flight fli[200];
    int numFli;
    loadFlightSchedules(fli, numFli);

    Reservation res[maxNumReservations];
    int numRes;
    loadReservations(res, numRes);

    bool have = false;
    int item = 1;

    for (int i = 1; i <= numRes; i++) {
        if (strcmp(id, res[i].id) == 0) {
            have = true;
            cout << endl << item++ << ". ";
            displayReservation(res[i], fli, numFli);
            cout << endl;
        }
    }

    if (!have) cout << "\nNo reservations!\n";
    return have;
}

// perform refund application
void refundApplication()
{
    char id[ 12 ];

    cout << "\nEnter ID number: ";
    cin >> id;

    if( !bookingInquiry( id ) )
        return;

    int choice;
    cout << "\nEnter your choice: ";
    cin >> choice;
    Reservation reservations[ maxNumReservations ];
    int numReservations;
    loadReservations( reservations, numReservations );

    ofstream edit("Reservations.dat", ios::out | ios::binary);
    edit.seekp(0, ios::beg);
    int del{}, totalTickets{};
        
    for (int i = 1; i <= numReservations; i++) {
        bool canWrite = true;
        if (strcmp(reservations[i].id, id) == 0) {
            del++;
            if (del == choice) {
                canWrite = false;
                for (int j = 1; j < 8; j++) {
                    if (j == 3) continue;
                    totalTickets += reservations[i].tickets[j];
                }
                modifyFlightSeatsFile(reservations[i], -totalTickets);
            }
        }
        if (canWrite) {
            edit.write(reinterpret_cast<const char*>(&reservations[i]), sizeof(Reservation));
        }
    }

    cout << "\nThe seleted booking has been deleted.\n";
}

// load all reservations from Reservations.dat and put them to the array reservations
void loadReservations( Reservation reservations[], int &numReservations )
{
    fstream res("Reservations.dat", ios::in | ios::out | ios::binary);
    
    if (!res) {
        cerr << "Reservations cannot be open!\n";
        exit(1);
    }

    res.seekg(0, ios::end);
    numReservations = res.tellg() / sizeof(Reservation);
    res.seekg(0, ios::beg);

    for (int i = 1; i <= numReservations; i++) {
        res.read(reinterpret_cast<char*>(&reservations[i]), sizeof(Reservation));
    }

    res.clear(); res.close();
}