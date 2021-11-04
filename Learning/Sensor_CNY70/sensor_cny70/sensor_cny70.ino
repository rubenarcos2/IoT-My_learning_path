volatile byte REV;       //  VOLATILE DATA TYPE TO STORE REVOLUTIONS
unsigned long int rpm, maxRPM;  //  DEFINE RPM AND MAXIMUM RPM
unsigned long time;         //  DEFINE TIME TAKEN TO COVER ONE REVOLUTION
int RPMlen , prevRPM;  //  INTEGERS TO STORE LED VALUE AND CURRENT RPM AND PREVIOUS RPM
long prevtime = 0;       //  STORE IDLE TIME TO TOGGLE MENU

void setup()
{
  Serial.begin(9600);   // GET VALUES USING SERIAL MONITOR
  attachInterrupt(0, RPMCount, RISING);     //  ADD A HIGH PRIORITY ACTION ( AN INTERRUPT)  WHEN THE SENSOR GOES FROM LOW TO HIGH

  REV = 0;      //  START ALL THE VARIABLES FROM 0
  rpm = 0;
  time = 0;

  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(3, HIGH);             //  VCC PIN FOR SENSOR
  digitalWrite(4, LOW);              // GND PIN FOR SENSOR

  delay(2000);
}

void loop()
{
  long currtime = millis();                 // GET CURRENT TIME
  long idletime = currtime - prevtime;        //  CALCULATE IDLE TIME

  if (REV >= 5 )                 //  IT WILL UPDATE AFETR EVERY 5 READINGS
  {
    rpm = 30 * 1000 / (millis() - time) * REV; //  CALCULATE  RPM USING REVOLUTIONS AND ELAPSED TIME

    if (rpm > maxRPM)
      maxRPM = rpm;                             //  GET THE MAX RPM THROUGHOUT THE RUN

    time = millis();
    REV = 0;

    int x = rpm;                               //  CALCULATE NUMBER OF DIGITS IN RPM
    while (x != 0)
    {
      x = x / 10;
      RPMlen++;
    }

    if (RPMlen != prevRPM)                          // IF THE RPM FALLS TO A LOWER NUMBER WITH LESS DIGITS , THE LCD WILL GET CLEARED
    {
      prevRPM = RPMlen;
    }
    
    prevtime = currtime;                        // RESET IDLETIME
  }

  if (idletime > 5000 )                     //  IF THERE ARE NO READING FOR 5 SEC , THE SCREEN WILL SHOW MAX RPM
  {
    prevtime = currtime;
  }
}

void RPMCount()                                // EVERYTIME WHEN THE SENSOR GOES FROM LOW TO HIGH , THIS FUNCTION WILL BE INVOKED
{
  REV++;                                         // INCREASE REVOLUTIONS
}
//////////////////////////////////////////////////////////////  END OF THE PROGRAM  ///////////////////////////////////////////////////////////////////////
