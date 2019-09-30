int reading [10];
int nextReadingIndex = 0;
int averageReading = 0;
int lastReading = 0;

int timeOfLastReading = 0;
int readingSeparation = 60000; // 1 minute delay between readings
int timeOfLastPublish = 0;
int publishSeparation = 600000; // 10 minute delay between publishing average reading


void setup() {

  pinMode(A0,INPUT_PULLUP);
  Particle.variable("lastReading", lastReading);
  Particle.variable("averageReading", averageReading);

  timeOfLastReading = -readingSeparation;
}


void loop() {

  if(millis() > timeOfLastReading + readingSeparation){
    addNewReading();
  }

  if(millis() > timeOfLastPublish + publishSeparation){
    publishAverageReading(averageReading);
  }

}

//--------------------------------------------------------------------------------------
void publishAverageReading(int newValue){
  char publishString[40];
  sprintf(publishString, "%d", newValue);
  Particle.publish("lightReading",publishString );
  timeOfLastPublish = millis();
}

//--------------------------------------------------------------------------------------
void addNewReading(){
  int newValue;
  
  newValue = analogRead(A0);

  reading[nextReadingIndex] = newValue;
  nextReadingIndex++;
  nextReadingIndex = nextReadingIndex % 10;

  lastReading = newValue;
  timeOfLastReading = millis();

  updateAverageReading();
}

//--------------------------------------------------------------------------------------
void updateAverageReading(){
  int sumOfReadings = 0;
  int countOfReadings = 0;

  for ( int readingIndex = 0; readingIndex < 10; readingIndex++ ) {
    if (reading[ readingIndex ] > 0){
      sumOfReadings += reading[ readingIndex ];
      countOfReadings++;
    }
  }
  averageReading = sumOfReadings / countOfReadings;
}