int reading [10];
int nextReadingIndex = 0;
int lastReading = 0;

int averageReading [10];
int lastAverageReadingIndex = 0;
int lastAverageReading = 0;

int timeOfLastReading = 0;
int timeOfLastPublish = 0;

//Constants
char *sourceCode = "https://github.com/Toby-Mills/WeatherStation";
int readingSeparation = 60000; // 1 minute delay between readings
int publishSeparation = 600000; // 10 minute delay between publishing average reading

void setup() {
  //pins
  pinMode(A0,INPUT_PULLUP);
  
  //Register variables and methods to allow control via Particle Cloud
  Particle.variable("sourceCode", sourceCode, STRING);
  Particle.variable("lastReading", lastReading);
  Particle.variable("averageReading", averageReading);


  timeOfLastReading = -readingSeparation;
}


void loop() {

  if(millis() > timeOfLastReading + readingSeparation){
    addNewReading();
  }

  if(millis() > timeOfLastPublish + publishSeparation){
    publishAverageReading();
  }

}

//--------------------------------------------------------------------------------------
void publishAverageReading(){
  int valueToPublish;
  char stringToPublish[40];

  valueToPublish = averageReading[nextAverageReadingIndex -1];
  sprintf(stringToPublish, "%d", valueToPublish);
  Particle.publish("lightReading",stringToPublish );

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
  int previousAverageReading = 0;
  int previousAverageReadingIndex = 0;

  int previousAverageReadingIndex = lastAverageReadingIndex;
  int previousAverageReading = averageReading[previousAverageReadingIndex];

  for ( int readingIndex = 0; readingIndex < 10; readingIndex++ ) {
    if (reading[ readingIndex ] > 0){
      sumOfReadings += reading[ readingIndex ];
      countOfReadings++;
    }
  }
  
  lastAverageReadingIndex++;
  lastAverageReadingIndex = lastAverageReadingIndex % 10;
  averageReading[lastAverageReadingIndex] = sumOfReadings / countOfReadings;

  
}