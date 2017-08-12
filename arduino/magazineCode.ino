/*
Modified version of Samy Kamkar's (@samyk) magspoof code.
@miaoski converted the original magspoof code to work with arduino instead of Atiny85.
I (@SMR404) took miaoski's code and added the dynamic tracks and analog softpot reading.

If you use this code, you will need to calibrate your magazine using the softpot debug code LinearSoftPotTest.ino
Record the value of the analog reading at every capacity (with magazine inserted into complete rifle and alter the
array, capacityAnalogValues[], below.

*/


#include <stdlib.h>

void setPolarity(int polarity);
void playBit(int bit);


#include <time.h>       /* time */
#define PIN_A    3      // left pin, to L293D pin 2
#define PIN_B    4      // right pin, to L293D pin 7
#define ENABLE_PIN 13   //the onboard led

#define SOFT_POT_PIN A0  //analog pin 2, goes to the resistor or potentiometer
#define DRIVING_PIN  11//the pin that drives the potentiometer
#define CLOCK_US 400
#define BETWEEN_ZERO 53 // 53 zeros between track1 & 2


int polarity = 0;
char revTrack[80];

const int sublen[] = { 32, 48, 48 };
const int bitlen[] = {  7,  5,  5 };

//track value, refreshed twice a second
char* tracks[] = {"^01^?"}; // Track 1(which is all we need, track 2 is useless); %BPAN^NAME^YYMMSSSCVV? + LRC





//These are the average analogRead Values for each different capacity of the magazine. 31 array elements represents the possible number of rounds (0 through 30)
/*
This array was created by measuring analog values when the magazine was placed in the lower receiver
with no upper receiver attached, skewing the values (the bolt pushes down on the follower, changing the value)
It has no practical application besides debugging 
const int capacityAnalogValues[31] = {25, 45, 71, 101, 121, 145, 165, 186, 212, 234,
                                      254, 277, 298, 324, 346, 372, 393, 420, 443, 471,
                                      496, 528, 554, 590, 621, 660 , 693, 737, 783, 825, 865
                                     };   //last value is made up, extrapolated from the previous pattern
*/
//This is a corrected array recorded while the magazine was firmly placed in the gun with the upper receiver attached, resulting in useful values
const int capacityAnalogValues[31] = {38, 58, 76, 105, 128, 150, 172, 196, 218, 240, 259,
                                      284, 306, 330, 355, 383, 406, 430, 457, 485, 513, 542, 
                                      573, 603, 640, 677, 716, 751, 798, 844, 880
                                     }; //last value is made up, extrapolated from the previous pattern

int currentCapacity = 0;
int previousCapacity = 0;

void setup() {


  //srand (time(NULL));         //debug
  pinMode(PIN_A, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  Serial.begin(9600);
  Serial.print("Start");
  storeRevTrack(2);

  pinMode(SOFT_POT_PIN, INPUT);    
  pinMode(DRIVING_PIN, OUTPUT);   //drives the soft pot
  digitalWrite(DRIVING_PIN, HIGH);
}

// Set the polarity of the electromagnet.
void setPolarity(int polarity) {
  if (polarity) {
    digitalWrite(PIN_B, LOW);
    digitalWrite(PIN_A, HIGH);
  } else {
    digitalWrite(PIN_A, LOW);
    digitalWrite(PIN_B, HIGH);
  }
}

void playBit(int bit) {
  polarity ^= 1;
  setPolarity(polarity);
  delayMicroseconds(CLOCK_US);

  if (bit == 1) {
    polarity ^= 1;
    setPolarity(polarity);
  }

  delayMicroseconds(CLOCK_US);
}

// when reversing
void reverseTrack(int track) {
  int i = 0;
  track--; // index 0
  polarity = 0;

  while(revTrack[i++] != '\0');
  i--;
  while(i--)
    for (int j = bitlen[track]-1; j >= 0; j--)
      playBit((revTrack[i] >> j) & 1);
}

// plays out a full track, calculating CRCs and LRC
void playTrack(int track)
{
  int tmp, crc, lrc = 0;
  track--; // index 0
  polarity = 0;

  // enable H-bridge and LED
  digitalWrite(ENABLE_PIN, HIGH);

  // First put out a bunch of leading zeros.
  for(int i = 0; i < 25; i++)
    playBit(0);

  Serial.print("Sending: ");
  for(int i = 0; tracks[track][i] != '\0'; i++) {
    Serial.print(tracks[track][i]);     // The delay that Track 1 needs.
    crc = 1;
    tmp = tracks[track][i] - sublen[track];

    for(int j = 0; j < bitlen[track]-1; j++) {
      crc ^= (tmp & 1);
      lrc ^= (tmp & 1) << j;
      playBit(tmp & 1);
      tmp >>= 1;
    }
    playBit(crc);
  }
  Serial.println();

  // finish calculating and send last "byte" (LRC)
  tmp = lrc;
  crc = 1;
  for(int j = 0; j < bitlen[track]-1; j++) {
    crc ^= tmp & 1;
    playBit(tmp & 1);
    tmp >>= 1;
  }
  playBit(crc);

  // if track 1, play 2nd track in reverse (like swiping back?)
/*
  if(track == 0) {
    for(int i = 0; i < BETWEEN_ZERO; i++)
      playBit(0);
    reverseTrack(2);
  }
  */
  // finish with 0's
  for (int i = 0; i < 25; i++)
    playBit(0);

  digitalWrite(PIN_A, LOW);
  digitalWrite(PIN_B, LOW);
  digitalWrite(ENABLE_PIN, LOW);
}



// stores track for reverse usage later
void storeRevTrack(int track) {
  int i, tmp, crc, lrc = 0;
  track--; // index 0
  polarity = 0;

  for (i = 0; tracks[track][i] != '\0'; i++) {
    crc = 1;
    tmp = tracks[track][i] - sublen[track];

    for (int j = 0; j < bitlen[track]-1; j++) {
      crc ^= tmp & 1;
      lrc ^= (tmp & 1) << j;
      tmp & 1 ?
        (revTrack[i] |= 1 << j) :
        (revTrack[i] &= ~(1 << j));
      tmp >>= 1;
    }
    crc ?
      (revTrack[i] |= 1 << 4) :
      (revTrack[i] &= ~(1 << 4));
  } 

  // finish calculating and send last "byte" (LRC)
  tmp = lrc;
  crc = 1;
  for (int j = 0; j < bitlen[track]-1; j++) {
    crc ^= tmp & 1;
    tmp & 1 ?
      (revTrack[i] |= 1 << j) :
      (revTrack[i] &= ~(1 << j));
    tmp >>= 1;
  }
  crc ?
    (revTrack[i] |= 1 << 4) :
    (revTrack[i] &= ~(1 << 4));

  i++;
  revTrack[i] = '\0';
}


//read voltage or resistance using analog input and use it to calculate a capacity
int getCapacity()
{ 

  
  ////////////    debugging to make sure magspoof is working, even if you don't have a softpot connected
//  int number;
//  number = rand() % 10 + 1;
//  return number;
  //////////////////
  
  int currentCapacity;
  int softPotADC;              //analog reading
  int calc;                   //placeholder variable used in calculating capacity
  
  softPotADC = analogRead(SOFT_POT_PIN);    //get reading from analog pin


  ////////////////////////////////////////////////////////
  //if capacity is 30
  //For the demonstated magazine, the follower passed the softpot at 30 rounds, resulting in a value of zero 
  if (softPotADC == 0)
  {
    Serial.println("The capacity is 30");
    currentCapacity = (int) 30;
  }
  else
  {
    int i = 1;
    //Covers capacities 1 - 29
    while ((i <= 30))
    {
      calc = (((capacityAnalogValues[i] - capacityAnalogValues[i - 1]) / 2) + capacityAnalogValues[i - 1]);
      if (calc >= softPotADC)            //if the analog value is above the lower threshold of the average analog capacity value
      {
        currentCapacity = (int)(i - 1);


        //these two lines are essential to the program and I don't really understand why. 
        //Try removing them and you'll see that this code doesn't function as intended (tracks[] has rubbish characters appended to it for some reason). 
        //Could be a timing issue? Anyway, solid programming standards /s
        Serial.println("The capacity is ");
        Serial.print(currentCapacity);

        
        break;
      }
      i++;
    }
  }
///////////////////////////////////////////////////////////////////////

  return currentCapacity;
}


//boolean indicator of whether the capacity value has changed
bool capacityChanged(int currentCapacity, int previousCapacity)
{
  if(!(currentCapacity == previousCapacity))
  {
    return 1;
  }
  else
  {
    return 0;
  }
}


void updateCapacityChar(int currentCapacity, char* tracks[])    //update the tracks to hold capacity value
{
      tracks[0][0] = '%';   //prefix
      tracks[0][1] = '^';
      tracks[0][2] = '0' + (currentCapacity/10);    //tens place
      tracks[0][3] = '0' + (currentCapacity%10);    //ones place
      tracks[0][4] = '^';
      tracks[0][5] = '?';   //suffix
}

void loop() {
//  memset(revTrack, 1, sizeof(revTrack));
    currentCapacity = getCapacity();      //read voltage or resistance and get capacity
//  if(capacityChanged(currentCapacity, previousCapacity))    //if the value has changed, send the new capacity value
//  {
    updateCapacityChar(currentCapacity, tracks);
    Serial.println(currentCapacity);
    Serial.println(tracks[0]); 
    playTrack(1);
    previousCapacity = currentCapacity;
    delay(500);
//  }
//  else
//  {
//    delay(40);
//  }
}
