#ifndef __CitizenPrinter_h
#define __CitizenPrinter_h

#include <SoftwareSerial.h>
#include <Arduino.h>

/**
 * A simple software library for encapsulating the serial communication with the
 * Citizen iDP Dot Matrix Printers.
 * 
 * This has been tested to work on:
 * - Citizen iDP-562
 * 
 */
class CitizenPrinter {
public:

  /**
   * Initialize a new printer class, providing pin connection information.
   */
  CitizenPrinter(int rx_pin, int tx_pin, int dtr_pin, int fault_pin);

  /**
   * Open up the serial connection to your printer.
   */
  void begin();

  /**
   * Print a single character or control code.
   */
  void print(const char chr);

  /**
   * Print a string of characters.
   */
  void print(const char *str);

  /**
   * Print a string of characters and start anew line.
   */
  void println(const char *str = nullptr);

  /**
   * Print n spaces, or other characters, in a row.
   */
  void indent(int spaces, const char c = ' ');

  /**
   * Print a full row of dashes.
   */
  void hr();

  /**
   * Center the string on a line.
   */
  void printCenter(const char *str);

  /**
   * Right-align this string on a line.
   */
  void printRight(const char *str);

  /**
   * Prints the first string left-aligned, and the second right-aligned.
   */
  void printTable(const char *strL, const char *strR);

  /**
   * Send a Form Feed command to the printer.
   */
  void formFeed();

  /**
   * Bring the printer online.
   */
  void online();

  /**
   * Bring the printer offline.
   */
  void offline();

  /**
   * Set red text for the next line.
   */
  void red();

  /**
   * Reverse print (black on white) for the next line or until called again, whichever
   * happens first.
   */
  void reverse();

  /**
   * Clear the printer buffer.
   */
  void flush();

  /**
   * Start graphic mode for the current line. Graphics helpers pending.
   */
  void graphicStart();

  /**
   * Enable 2.4mm line height for Graphics mode.
   */
  void shortLines();

  /**
   * Enable 5mm line height for normal printing.
   */
  void normalLines();
  
private:

  int dtr_pin = 0;
  int fault_pin = 0;
  SoftwareSerial *port = nullptr;
};

#endif