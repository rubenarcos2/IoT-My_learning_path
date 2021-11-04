// https://openweathermap.org/weather-conditions#Weather-Condition-Codes-2

#define TIME_OFFSET 1UL * 3600UL // UTC + 0 hour

// OpenWeather API Details, replace x's with your API key
String api_key = ""; // Obtain this from your OpenWeather account

// Set both your longitude and latitude to at least 4 decimal places
String latitude =  "36.7191"; // 90.0000 to -90.0000 negative for Southern hemisphere
String longitude = "-4.4522"; // 180.000 to -180.000 negative for West

String units = "metric";  // or "imperial"
String language = "es";   // See notes tab

OW_Weather ow; // Weather forecast library instance

float weather_1h_Temp = 0;
float weather_1h_Hum = 0;
String weather_1h_Desc = "";
int lluvia_1h = 0;
float weather_2h_Temp = 0;
float weather_2h_Hum = 0;
String weather_2h_Desc = "";
int lluvia_2h = 0;
float weather_1TempMin = 0;
float weather_1TempMax = 0;
float weather_1Hum = 0;
String weather_1Desc = "";
int lluvia_1d = 0;
float weather_2TempMin = 0;
float weather_2TempMax = 0;
float weather_2Hum = 0;
String weather_2Desc = "";

/***************************************************************************************
**                          Convert unix time to a time string
***************************************************************************************/
String strTime(time_t unixTime)
{
  unixTime += TIME_OFFSET;
  return ctime(&unixTime);
}

void getWeather() {

  // Create the structures that hold the retrieved weather
  OW_current *current = new OW_current;
  OW_hourly *hourly = new OW_hourly;
  OW_daily  *daily = new OW_daily;
  
  Serial.print("[INFO] Requesting weather information from OpenWeather... ");

  ow.getForecast(current, hourly, daily, api_key, latitude, longitude, units, language);

  delay(100);

  if (hourly)
  {
    weather_1h_Temp = hourly->temp[1];
    //Serial.println(hourly->temp[1]);
    weather_1h_Hum = hourly->humidity[1];
    //Serial.println(hourly->humidity[1]);
    weather_1h_Desc = hourly->description[1];
    //Serial.println(hourly->description[1]);
    lluvia_1h = hourly->id[1];
    weather_2h_Temp = hourly->temp[2];
    //Serial.println(hourly->temp[2]);
    weather_2h_Hum = hourly->humidity[2];
    //Serial.println(hourly->humidity[2]);
    weather_2h_Desc = hourly->description[2];
    //Serial.println(hourly->description[2]);
    lluvia_2h = hourly->id[2];
  }
  
  if (daily)
  {
    weather_1TempMin = daily->temp_min[1];
    //Serial.println(daily->temp_min[1]);
    weather_1TempMax = daily->temp_max[1];
    //Serial.println(daily->temp_max[1]);
    weather_1Hum = daily->humidity[1];
    //Serial.println(daily->humidity[1]);
    weather_1Desc = daily->description[1];
    //Serial.println(daily->description[1]);
    lluvia_1d = daily->id[1];
    weather_2TempMin = daily->temp_min[2];
    //Serial.println(daily->temp_min[2]);
    weather_2TempMax = daily->temp_max[2];
    //Serial.println(daily->temp_max[2]);
    weather_2Hum = daily->humidity[2];
    //Serial.println(daily->humidity[2]);
    weather_2Desc = daily->description[2];
    //Serial.println(daily->description[2]);
  }

  // Delete to free up space and prevent fragmentation as strings change in length
  delete current;
  delete hourly;
  delete daily;
}
