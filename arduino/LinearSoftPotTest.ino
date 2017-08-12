/******************************************************************************
  (@SMR404) August 2017
  Edited version of SoftPot_Example.ino
  
  Originally an example sketch for SparkFun's soft membrane potentiometer
  (https://www.sparkfun.com/products/8680)
  Jim Lindblom @ SparkFun Electronics
  April 28, 2016

  - Connect the softpot's outside pins to Digital pin 11 and GND (the outer pin with an arrow
  indicator should be connected to GND).
  - Connect the middle pin to A0.


  Development environment specifics:
  Arduino 1.6.7
******************************************************************************/
const int SOFT_POT_PIN = A0; // Pin connected to softpot wiper
const int DRIVING_PIN = 11;   //drives the softpot
const int GRAPH_LENGTH = 40; // Length of line graph


//These are the average analogRead Values for each different capacity of the magazine. 31 array elements represents the possible number of rounds (0 through 30)
//Record the analog values printed to the monitor and change this array until you are happy with how it calculates the magazine capacity
const int capacityAnalogValues[31] = {25, 45, 71, 101, 121, 145, 165, 186, 212, 234,
                                      254, 277, 298, 324, 346, 372, 393, 420, 443, 471,
                                      496, 528, 554, 590, 621, 660 , 693, 737, 783, 825, 865
                                     };                                                                       //the final value of 865 is made up. The 30th round pushes the follower past the sensor and gives a reading of 0

void setup()
{
  Serial.begin(9600);
  pinMode(SOFT_POT_PIN, INPUT);
  pinMode(DRIVING_PIN, OUTPUT);
  digitalWrite(DRIVING_PIN, HIGH);
}

void loop()
{
  // Read in the soft pot's ADC value
  int calc;
  int softPotADC = analogRead(SOFT_POT_PIN);

  //if capacity is 30
  if (softPotADC == 0)      //value is zero when capacity is zero (the follower pushes past the softPot, may differ in your case)
  {
    Serial.println("The capacity is 30");
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
        Serial.print("The capacity is ");
        Serial.print((i - 1));
        break;
      }
      i++;
    }
  }
  Serial.println("\nANALOG READ VALUE: (" + String(softPotADC) + ")");
  delay(700);
}
