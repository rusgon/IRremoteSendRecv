/*
 * IRremote
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * For details, see http://arcfn.com/2009/08/multi-protocol-infrared-remote-library.htm http://arcfn.com
 *
 * Interrupt code based on NECIRrcv by Joe Knapp
 * http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1210243556
 * Also influenced by http://zovirl.com/2008/11/12/building-a-universal-remote-with-an-arduino/
   
 * Lines marked as GRR are created by Ruslan Goncharov rus.gon@gmail.com 02-oct-2012
 * In IRremoteSendRecv library the IRrecv and IRSender use different timers: 16 bit Timer1 for IRrecv and 8 bit Timer2 for IRSender.
 * Now the same device may send and receive data simultaneously.
 
 * Tested on Samsung devices. IRsend::sendSamsung method uses third parameter kShrink (between 0 and 1 [usually 0.8])
 * IRrecv::decodeSAMSUNG uses inner parameter kShrink = .9;
 */

#ifndef IRremoteSendRecv_h
#define IRremoteSendRecv_h

// The following are compile-time library options.
// If you change them, recompile the library.
// If DEBUG is defined, a lot of debugging output will be printed during decoding.
// TEST must be defined for the IRtest unittests to work.  It will make some
// methods virtual, which will be slightly slower, which is why it is optional.
// #define DEBUG
// #define TEST

//GRR for debugging
class decoded_value {
public:
  unsigned long value; // Decoded value
};

// Results returned from the decoder
class decode_results {
public:
  int decode_type; // NEC, SONY, RC5, UNKNOWN
  unsigned long value; // Decoded value
  int bits; // Number of bits in decoded value
  volatile unsigned int *rawbuf; // Raw intervals in .5 us ticks
  int rawlen; // Number of records in rawbuf.
  unsigned long address;
};

// Values for decode_type

#define UNKNOWN -1
#define NEC 1
#define SONY 2
#define RC5 3
#define RC6 4
#define SAMSUNG 5
#define JVC 6
#define PANASONIC 7


// Decoded value for NEC when a repeat code is received
#define REPEAT 0xffffffff

// main class for receiving IR
class IRrecv
{
public:
  IRrecv(int recvpin);
  void blink13(int blinkflag);
  int decode(decode_results *results);
  void enableIRIn();
  void resume();
  unsigned long getDecodedValue();//GRR for debugging
private:
  // These are called by decode
  int getRClevel(decode_results *results, int *offset, int *used, int t1);
  long decodeJVC(decode_results *results);
  long decodeSAMSUNG(decode_results *results);
  long decodePanasonic(decode_results *results);
  long decodeNEC(decode_results *results);
  long decodeSony(decode_results *results);
  long decodeRC5(decode_results *results);
  long decodeRC6(decode_results *results);
} 
;

// Only used for testing; can remove virtual for shorter code
#ifdef TEST
#define VIRTUAL virtual
#else
#define VIRTUAL
#endif

class IRsend
{
public:
  IRsend() {}
  void sendNEC(unsigned long data, int nbits);
  void sendJVC(unsigned long data, int nbits, int repeating );
  //void sendSamsung(unsigned long data, int nbits); // GRR
  void sendSamsung(unsigned long data, int nbits, float	kShrink); // GRR - kShrink "shrinks" time-length of transmitted logical "1" and "0"
  void sendPanasonic(unsigned long address, unsigned long data);
  void sendSony(unsigned long data, int nbits);
  void sendRaw(unsigned int buf[], int len, int hz);
  void sendRC5(unsigned long data, int nbits);
  void sendRC6(unsigned long data, int nbits);
  // private:
  void enableIROut(int khz);
  VIRTUAL void mark(int usec);
  VIRTUAL void space(int usec);
}
;

// Some useful constants

#define USECPERTICK 50  // microseconds per clock interrupt tick
#define RAWBUF 76 // Length of raw duration buffer

// Marks tend to be 100us too long, and spaces 100us too short
// when received due to sensor lag.
#define MARK_EXCESS 100

#endif
