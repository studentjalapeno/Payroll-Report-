/*
   File: prog6_die9_dlm291.cpp

   Author: David Elias & Deven Meza
   C.S.1428.002
   Lab Section: L28 &L23
   Program: #6
   Due Date: 12/04/2017

   This program prints to an output file a title and column headers for a
   payroll report. It then reads an employees work record from an input file.
   The number of overtime hours worked, gross pay, state tax, federal tax,
   and net pay are calculated for each employee. The authors' personal
   identification information followed by the company payroll report is
   printed to the output file. Monetary values are displayed to two decimal
   places.

   An attempt to avoid repetitive code is made.

   An appropriate message is displayed to the console screen if either the
   input or the output file fails to open.

   An appropriate message is written to the console screen informing the
   user of the output file name to which the results have been written.

   The client file (main) calls the following void functions to process
   the data:

	  - printIdInfo prints the authors' personal information - name(s),
        class/section number, lab section number(s), due date - on the first,
        second, third and fourth lines. Two blank lines are left after the
        due date. The lecture section number is displayed in a three-digit
        field with leading zeros shown. The lab section number is displayed
        in a two-digit field with leading zeros shown. The calling routine
        determines the output stream to which the information is directed.
        (Refer to sample output below.)
	  - printReportHeadings prints to the output file the title & column
        headings for the payroll report. One blank line is left after the
        centered report title. Column headers are displayed on two lines.
        (Refer to sample output below.)
      - dataIn reads the employee ID#, hours worked and pay rate from
        an input file storing the values read into parallel arrays. The
        employee ID# is stored in a 1D array of integers. The hours worked
        and the pay rate are stored in the first and second columns of a
        2D array of reals.
	  - overTime calculates the number of overtime hours worked by the
        employee based on a 40 hour work week.
	  - grossPay calculates the employee's gross pay for the week. If the
        employee worked 40 hours or less, gross pay is the hourly pay rate
        multiplied by the number of hours worked; otherwise, the employee
        worked more than 40 hours, and they are paid the regular hourly
        rate for the first 40 hours plus time and a half for any hours
        over 40.
	  - stateTax calculates state taxes owed by the employee, which is
        calculated at a straight 6% of the employee's weekly gross pay.
        (6% is a sample tax rate. The tax rate will vary with each state.)
	  - federalTax calculates federal taxes owed by the employee. If
        weekly gross pay is $400.00 or less, the tax rate is 20%; otherwise,
        the employee's weekly gross pay is more than $400.00, and the tax
        rate is calculated at 20% for the first $400.00 and 31% for any
        amount over $400.00.
        (These rates will vary based on current federal tax regulations.)
	  - netPay calculates the employee's net pay for the week.
        (gross pay minus calculated taxes both state & federal).
	  - printReportData prints to the output file the information for each
        employee in tabular form. Monetary values are displayed to two
        digits of precision. (Refer to sample output below.)
	  - writeFileLocation prints an appropriate message to the console
        screen indicating to the user the name of the output file to which
        the results have been written. The output file name is provided by
        the calling routine.(Refer to sample output below.)


   The following named constants are declared globally:
      - the number of hours in a normal work week (40.00)
      - the state tax rate (0.06)
      - the federal tax rates (0.20; 0.31)
      - the cut off at which federal taxes increase (400.00)
      - parallel array dimensions
      - names used to reference individual columns in the payroll array

   Locally declared named constants include:
      - a string to hold the authors' names
      - a string to hold the authors' lab section number(s)
      - a string to hold the project's due date
      - an integer to hold the lecture section number
      - an integer representing the maximum string length allowed for
        input and output file names which are stored in character arrays
        of that length

==========================================================================

   Layout and content of the output are shown in the samples below.

   Input (file - prog6_00?inp.txt)
          one record per employee / each record containing three numbers

          ID#(integer)  hours worked (real)  pay rate (real)
          ...

   Constants: globally declared:
                 integer: ROWS
                          COLS
	                      (2D array column indices)
                              HRS_WRKD = 0,
                              PAYRATE = 1,
                              OVRTIME = 2,
                              GROSS = 3,
                              ST_TAX = 4,
                              FED_TAX = 5,
                              NETPAY = 6;

                 double: CUT_OFF (hours in a work week)
                         STATE_TX_RATE
                         TAX_CUT_OFF (division @ which net pay is taxed higher)
	  	                 LOW_TAX_RATE
                         HI_TAX_RATE

   Constants: locally declared:
                 string:  AUTHORS
                          LAB_SECTION
                          DUE_DATE

                 integer: LECTURE_SECTION
                          MAX_LENGTH_FN = ?  // filename's maximum length

   Output (console):
     Sample Console Output

     Authors' Names
     C.S.1428.?        // '?' represents author's 3-digit lecture section #
     Lab Section: L?   // '?' represents author's 2-digit lab section #
     --/--/--          // dashes represent due date, month/day/year
          <blank line>
          <blank line>
     Program results have been written to prog6_00?out.txt.

   Output (file: prog6_00?out.txt):
     Sample File Output

     Authors' Names
     C.S.1428.?        // '?' represents author's 3-digit lecture section #
     Lab Section: L?   // '?' represents author's 2-digit lab section #
     --/--/--          // dashes represent due date, month/day/year
          <blank line>
          <blank line>
                          Monthly Payroll Report
          <blank line>
      ID#     Hours    Hourly    Overtime   Gross    State   Federal    Net
             Worked     Rate       Hours     Pay      Tax      Tax      Pay
     1000     51.00      6.55      11.00   370.07    22.20    74.02   273.86
     ...
     1002     26.00     15.00       0.00   390.00    23.40    78.00   288.60
     ...

     =======================================================================

     <Output will vary based on actual input values.>
*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>     // 4 Code::Blocks

using namespace std;

const int ROWS = 10,    // number of employees
          COLS = 7;

          // 2D array (payroll) column indices
const int HRS_WRKD = 0,
          PAYRATE = 1,
          OVRTIME = 2,
          GROSS = 3,
          ST_TAX = 4,
          FED_TAX = 5,
          NETPAY = 6;

const double CUT_OFF = 40.00,      // work week
             STATE_TX_RATE = 0.06,
             TAX_CUT_OFF = 400.00, // earnings after which taxed at a higher rate
             LOW_TAX_RATE = 0.20,
             HI_TAX_RATE = 0.31;

void printIdInfo(ostream &out, const string AUTHORS,
                 const int LECTURE_SECTION, const string LAB_SECTION,
                 const string DUE_DATE );
void printReportHeadings ( ofstream &fout);
void dataIn ( ifstream &fin, int employee[], double payroll[][COLS]);
void overTime ( double payroll[][COLS]);
void grossPay ( double payroll[][COLS]);
void stateTax ( double payroll[][COLS] );
void federalTax ( double payroll[][COLS] );
void netPay (double payroll[][COLS]);
void writeFileLocation ( char output_filename[] );
void printReportData (ofstream &fout, int employee[], double payroll[][COLS]);

int main( )
{
   const string AUTHORS = "David Elias & Deven Meza ",
                LAB_SECTION = "L28 & 23",
                DUE_DATE = "12/04/2017";

   const int LECTURE_SECTION = 2,
             MAX_LENGTH_FN = 20;

   char input_filename[MAX_LENGTH_FN + 1] = "prog6_002inp.txt",
        output_filename[MAX_LENGTH_FN + 1] = "prog6_002out.txt";

   int employee[ROWS];          // employee ID#s

   double payroll[ROWS][COLS];  // payroll data

   ifstream fin;
   fin.open("prog6_002inp.txt");
   if ( !fin )
    {
        cout << endl << endl
             << " ***Program Terminated.*** " << endl << endl
             << "Input file failed to open." << endl;

        system ("PAUSE>NUL");

        return 1;
    }

   ofstream fout;
   fout.open("prog6_002out.txt");

   if ( !fout )
    {
        cout << endl << endl
             << " ***Program Terminated.*** " << endl << endl
             << "Output file failed to open." << endl;

        system ("PAUSE>NUL");

        return 1;
    }


   printIdInfo( fout, AUTHORS, LECTURE_SECTION, LAB_SECTION, DUE_DATE );
   printReportHeadings ( fout );
   dataIn ( fin, employee, payroll );
   overTime ( payroll );
   grossPay ( payroll );
   stateTax ( payroll );
   federalTax ( payroll );
   netPay ( payroll );
   printReportData ( fout, employee, payroll );
   printIdInfo( cout, AUTHORS, LECTURE_SECTION, LAB_SECTION, DUE_DATE );
   writeFileLocation ( output_filename );

   fin.close();
   fout.close();

   system("PAUSE>NUL");

   return 0;
}

/*
      Function: printIdInfo

      Author(s): David Elias & Deven Meza
      C.S.1428.002
      Lab Section: L28 & L23
      Program #6 Function 1
      11/30/2017

      The function, printIdInfo, prints the authors' name, class/section number,
      lab section number, and due date to the screen.

      Sample Output:
      Authors' names: David Elias & Deven Meza
      C.S.1428.002
      Lab Section: L28 & 23
      12/04/2017

      Receives: output file varrible, AUTHORS (string), LECTURE_SECTION (string),
      LAB_SECTION (string), DUE DATE (string)
      Constants: AUTHORS, LECTURE_SECTION, DUE_DATE
      Returns: Nothing; prints user's team identifying info to a output file
*/

void printIdInfo(ostream &out, const string AUTHORS,
                 const int LECTURE_SECTION, const string LAB_SECTION,
                 const string DUE_DATE )
{
    out  << "Authors' names: " << AUTHORS << endl
         << "C.S.1428." << setw(3) << setfill('0') << LECTURE_SECTION << setw(0)
         << setfill(' ')<< endl
         << "Lab Section: " << LAB_SECTION << endl
         << DUE_DATE << endl << endl << endl;
}

/*
      Function: printReportHeadings

      Author(s): David Elias & Deven Meza
      C.S.1428.002
      Lab Section: L28 & L23
      Program #6 Function 2
      11/30/2017

      This function prints to the outputfile the title and column headings for
      the payroll report. Displays comlumn heads on two lines

      Sample Output:
                        Monthly Payroll Report

 ID#    Hours     Hourly    Overtime    Gross    State    Federal    Net
       Worked      Rate       Hours      Pay      Tax       Tax      Pay

      Receives: Output file variable
      Constants: None
      Returns: Nothing, prints employee's payroll report to an output file
*/

void printReportHeadings ( ofstream &fout)
{
    fout << "                     Monthly Payroll Report" << endl << endl
         << " ID#    Hours     Hourly    Overtime    Gross    State    Federal"
            "    Net  " << endl
         << "       Worked      Rate       Hours      Pay      Tax       Tax"
            "      Pay  " << endl;
}

/*
      Function: dataIn

      Author(s): David Elias & Deven Meza
      C.S.1428.002
      Lab Section: L28 & L23
      Program #6 Function 3
      11/30/2017

      This function reads the employees' Id number, hours worked and payrate
      from an input file, and then stores the values into parallel arrays. This
      stores the employee ID# into a 1D array of intergers. This stores hours
      worked and payrate from a 2D array.

      Receives: input file variable, employee array (double), payroll array
                (double), HRS_WRKD (int) (global), PAYRATE (int) (global)
      Constants: HRS_WRKD (int) (global), PAYRATE (int) (global)
      Returns: Nothing
*/

void dataIn ( ifstream &fin, int employee[], double payroll[][COLS])
{
    for ( int id = 0; id < ROWS; id ++)
        fin >> employee[id] >> payroll [id][HRS_WRKD] >> payroll[id][PAYRATE];
}

/*
      Function: overTime

      Author(s): David Elias & Deven Meza
      C.S.1428.002
      Lab Section: L28 & L23
      Program #6 Function 4
      11/30/2017

      This function determines if an employee qualifies for overtime
      hours and calculates the amount of overtime hours worked

      Receives: payroll array (double),HRS_WRKD (int) (global),
                CUT_OFF (double) (global), OVERTIME (int) (global)
      Constants: HRS_WRKD (int) (global), CUT_OFF (double) (global),
                 OVERTIME (int) (global)
      Returns: Nothing
*/

void overTime ( double payroll[][COLS])
{
    for ( int id = 0; id < ROWS; id++)
        if ( payroll [id][HRS_WRKD] > CUT_OFF)
        payroll[id][OVRTIME] = payroll[id][HRS_WRKD] - CUT_OFF;
        else
         payroll [id][OVRTIME] = 0;
}

/*
      Function: grossPay

      Author(s): David Elias & Deven Meza
      C.S.1428.002
      Lab Section: L28 & L23
      Program #6 Function 5
      11/30/2017

      This function calculates the gross pay of an employee for the week,
      including those with overtime hours.

      Receives: payroll array (double)
      Constants: OVRTIME (int) (global), GROSS (int) (global),
                 PAYRATE (int) (global), HR_WRKD (int) (global)
      Returns: nothing
*/

void grossPay ( double payroll[][COLS])
{
    for (int id = 0; id < ROWS; id++)
        if ( payroll[id][OVRTIME] > 0)
            payroll [id][GROSS] = payroll[id][PAYRATE] * payroll[id][HRS_WRKD]
            + ((payroll[id][OVRTIME] * payroll[id][PAYRATE]) / 2);
        else
            payroll[id][GROSS] = payroll[id][PAYRATE] * payroll[id][HRS_WRKD];
}

/*
      Function: stateTax

      Author(s): David Elias & Deven Meza
      C.S.1428.002
      Lab Section: L28 & L23
      Program #6 Function 6
      11/30/2017

      This function calculates the state tax from the employee's gross pay.
      This is calculated at 6% of the employees weekly gross pay

      Receives: payroll array (double), ST_TAX (int) (global) ,
                STATE_TX_RATE (double) (global)
      Constants: ST_TAX (int) (global) , STATE_TX_RATE (double) (global)
      Returns: Nothing
*/

void stateTax ( double payroll[][COLS] )
{
    for (int id = 0; id < ROWS; id++)
    payroll[id][ST_TAX] = (payroll[id][GROSS] * STATE_TX_RATE);
}

/*
      Function: federalTax

      Author(s): David Elias & Deven Meza
      C.S.1428.002
      Lab Section: L28 & L23
      Program #6 Function 7
      11/30/2017

      This function calculates the federal taxes the employee owes. If the
      employee's weekly gross pay is less than $400 the tax rate is calculated
      at 20%. Otherwise, if the employee weekly gross is more than $400, the
      tax is calculated at 20% for the first $400 and 31% for any amount over
      $400

      Receives: input file variable, employee array (int), payroll array (int),
                GROSS (int) (global) FED_TAX (int) (global)
                LOW_TAX_RATE (double) (global) HI_TAX_RATE (double) (global)
      Constants: GROSS (int) (global) FED_TAX (int) (global)
                 LOW_TAX_RATE (double) (global) HI_TAX_RATE (double) (global)
      Returns: Nothing
*/

void federalTax ( double payroll[][COLS] )
{

    for (int id = 0; id < ROWS; id++)
        if ( payroll[id][GROSS] < TAX_CUT_OFF)
            payroll[id][FED_TAX] = payroll[id][GROSS] * LOW_TAX_RATE;
        else
        {
            payroll[id][FED_TAX] = payroll[id][GROSS] - TAX_CUT_OFF;
            payroll[id][FED_TAX] = payroll[id][GROSS] * LOW_TAX_RATE +
            (payroll[id][FED_TAX] * HI_TAX_RATE);
        }
}

/*
      Function: netPay

      Author(s): David Elias & Deven Meza
      C.S.1428.002
      Lab Section: L28 & L23
      Program #6 Function 8
      11/30/2017

      This function calculates the employee's net pay for the week.

      Receives: input file varrible, employee array (int),
                payroll array (double), NETPAY, GROSS (int) (global),
                ST_TAX (int) (global),  FED_TAX (int) (global)
      Constants: NETPAY, GROSS (int) (global), ST_TAX (int) (global),
                 FED_TAX (int) (global)
      Returns: Nothing
*/

void netPay (double payroll[][COLS])
{
    for (int id = 0; id < ROWS; id++)
        payroll[id][NETPAY] = payroll[id][GROSS] - (payroll[id][ST_TAX] +
                                                    payroll[id][FED_TAX]);
}

/*
      Function: printReportData

      Author(s): David Elias & Deven Meza
      C.S.1428.002
      Lab Section: L28 & L23
      Program #6 Function 9
      11/30/2017

      This function prints the information for each employee to the output file
      in tabular form. Displays monetary values to two digits of precision

      Sample Output:
    1000     51.00      6.55     11.00    370.07     22.20     74.02    273.86
    1002     26.00     15.00      0.00    390.00     23.40     78.00    288.6

      Receives: input file varrible, employee array, payroll array
      Constants: None
      Returns: Nothing, writes results to a file
*/

void printReportData (ofstream &fout, int employee[], double payroll[][COLS])
{
    for ( int id = 0; id < ROWS; id++)
    {
        fout << employee[id] << "  ";

        for ( int col = 0; col < COLS; col++)
            fout << fixed << setprecision(2) << setw(8) << payroll[id][col]
                 << "  ";
            fout << endl;
    }
}

/*
      Function: writeFileLocation

      Author(s): David Elias & Deven Meza
      C.S.1428.002
      Lab Section: L28 & L23
      Program #6 Function 10
      11/30/2017

      This function rpints an a message to the console screen indicating to the
      user the name of the output file to which the results have been written

      Sample Display:
            Program results have been written to prog6_002out.txt
            (This actualy file name may differ)

      Receives: input file variable (char), employee array (double),
                payroll array (double)
      Constants: None
      Returns: Nothing, writes a message to the screen
*/
void writeFileLocation ( char output_filename[] )
{
    cout << "Program results have been written to " << output_filename;
}
