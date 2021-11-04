#include <Wire.h>
#include <RtcDS3231.h>

RtcDS3231<TwoWire> rtcClock(Wire);

bool Century = false;
bool h12;
bool PM;
byte ADay, AHour, AMinute, ASecond, ABits;
bool ADy, A12h, Apm;
char t2[32];

String getRTCTime() {
  RtcDateTime timestamp = rtcClock.GetDateTime();

  sprintf(t2, "%02d/%02d/%02d %02d:%02d:%02d" , timestamp.Day(), timestamp.Month(), timestamp.Year(),
          timestamp.Hour(), timestamp.Minute(), timestamp.Second());
  return t2;
}

void setRTCTimeFromNTP() {
  String fechaHoraNTP = getNTP_Local_Time();
  uint8_t dia = fechaHoraNTP.substring(0, 2).toInt();
  uint8_t mes = fechaHoraNTP.substring(4, 5).toInt();
  uint16_t anio = fechaHoraNTP.substring(7, 10).toInt();
  uint8_t hora = fechaHoraNTP.substring(11, 13).toInt();
  uint8_t minutos = fechaHoraNTP.substring(14, 16).toInt();
  uint8_t seg = fechaHoraNTP.substring(17, 19).toInt();
  //RtcDateTime(uint16_t year, uint8_t month, uint8_t dayOfMonth, uint8_t hour, uint8_t minute, uint8_t second)
  RtcDateTime timestamp = RtcDateTime(anio, mes, dia, hora, minutos, seg);
  rtcClock.SetDateTime(timestamp);
}

String getRTCAlarm1() {
  DS3231AlarmOne alarm1 = rtcClock.GetAlarmOne();
  sprintf(t2, "%02d:%02d:%02d" , alarm1.Hour(), alarm1.Minute(), alarm1.Second());
  return t2;
}

String getRTCAlarm2() {
  DS3231AlarmTwo alarm2 = rtcClock.GetAlarmTwo();
  sprintf(t2, "%02d:%02d:%02d" , alarm2.Hour(), alarm2.Minute());
  return t2;
}

void setRTCAlarm1(String alarma) {
  uint8_t hora = alarma.substring(0, 2).toInt();
  uint8_t minutos = alarma.substring(3, 5).toInt();
  /*DS3231AlarmOne( uint8_t dayOf, uint8_t hour, uint8_t minute, uint8_t second, DS3231AlarmOneControl controlFlags)
    Construct an alarm one settings object.
    dayOf - (0-6) (1-31) day of the week or the day of the month , see flags below
    hour - (0-23) the hour of the day
    minute - (0-59) the minute of the hour
    second - (0-59) the second of the minute
    controlFlags - one of the following to control the alarm operation:
    DS3231AlarmOneControl_HoursMinutesSecondsDayOfMonthMatch - a normal monthly calendar alarm
    DS3231AlarmOneControl_OncePerSecond - triggers every second
    DS3231AlarmOneControl_SecondsMatch - triggers once a minute when the seconds match
    DS3231AlarmOneControl_MinutesSecondsMatch - triggers once an hour when the minute and seconds match
    DS3231AlarmOneControl_HoursMinutesSecondsMatch - triggers once a day when hours, minutes, and seconds match
    DS3231AlarmOneControl_HoursMinutesSecondsDayOfWeekMatch - triggers once a week on the given day of theweek, hour, minute, and second
  */
  DS3231AlarmOne alarm1(0, hora, minutos, 0, DS3231AlarmOneControl_HoursMinutesSecondsMatch);
  rtcClock.SetAlarmOne(alarm1);
  rtcClock.LatchAlarmsTriggeredFlags();
  Serial.println("[INFO] RTC (DS3231): Alarma 1 activada. Hora: " + getRTCAlarm1());
}

void setRTCAlarm2(String alarma) {
  uint8_t hora = alarma.substring(0, 2).toInt();
  uint8_t minutos = alarma.substring(3, 5).toInt();
  /*DS3231AlarmTwo( uint8_t dayOf, uint8_t hour, uint8_t minute, DS3231AlarmTwoControl controlFlags)
    Construct an alarm two settings object.
    dayOf - (0-6) (1-31) day of the week or the day of the month , see flags below
    hour - (0-23) the hour of the day
    minute - (0-59) the minute of the hour
    controlFlags - one of the following to control the alarm operation:
    DS3231AlarmTwoControl_HoursMinutesDayOfMonthMatch - a normal monthly calendar alarm
    DS3231AlarmTwoControl_OncePerMinute - triggers every minute
    DS3231AlarmTwoControl_MinutesMatch - triggers once an hour when the minute matched
    DS3231AlarmTwoControl_HoursMinutesMatch - triggers once a day when hours and minutes match
    DS3231AlarmTwoControl_HoursMinutesDayOfWeekMatch - triggers once a week on the given day of the week, hour, and minute
  */
  DS3231AlarmTwo alarm2(0, hora, minutos, DS3231AlarmTwoControl_HoursMinutesMatch);
  rtcClock.SetAlarmTwo(alarm2);
  rtcClock.LatchAlarmsTriggeredFlags();
  Serial.println("[INFO] RTC (DS3231): Alarma 2 activada. Hora: " + getRTCAlarm2());
}

bool isEnableRTCAlarm1() {
  switch (rtcClock.LatchAlarmsTriggeredFlags()) {
    case 0:
      return true;
      break;
    case 1:
      return false;
      break;
    case 2:
      return true;
      break;
  }
}

bool isEnableRTCAlarm2() {
  switch (rtcClock.LatchAlarmsTriggeredFlags()) {
    case 0:
      return false;
      break;
    case 1:
      return true;
      break;
    case 2:
      return true;
      break;
  }
}

String getRTCTemp() {
  RtcTemperature temp = rtcClock.GetTemperature();
  return String(temp.AsFloatDegC(), 2) + " C";
}

void initRTC() {
  // Start the I2C interface
  rtcClock.Begin();

  if (!rtcClock.IsDateTimeValid() && rtcClock.GetIsRunning()) {
    Serial.println("[INFO] RTC (DS3231) ha estado apagado. Requiere sincronizar hora.");
    Serial.println("[OK]   Actualizando hora NTP en el RTC (DS3231)");
    if (WiFi.status() == WL_CONNECTED) {
      setRTCTimeFromNTP();
    }
  } else if (!rtcClock.GetIsRunning()) {
    Serial.println("[ERROR] RTC (DS3231) no está conectado.");
  } else {
    rtcClock.Enable32kHzPin(false);
    /*
      DS3231SquareWavePin_ModeNone - disable the pin
      DS3231SquareWavePin_ModeBatteryBackup - the pin will trigger when the external power is lower than the battery power
      DS3231SquareWavePin_ModeClock - the pin will trigger at the frequency defined by SetSquareWavePinClockFrequency()
      DS3231SquareWavePin_ModeAlarmOne - the pin will trigger with alarm one
      DS3231SquareWavePin_ModeAlarmTwo - the pin will trigger with alarm two
      DS3231SquareWavePin_ModeAlarmBoth - the pin will trigger with either of the alarms
    */
    rtcClock.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmOne);

    Serial.println("[INFO] RTC (DS3231): Activado.    Hora actual: " + getRTCTime());
    switch (rtcClock.LatchAlarmsTriggeredFlags()) {
      case 0:
        Serial.println("[INFO] RTC (DS3231): Alarma 1 activada. Hora: " + getRTCAlarm1());
        break;
      case 1:
        Serial.println("[INFO] RTC (DS3231): Alarma 2 activada. Hora: " + getRTCAlarm2());
        break;
      case 2:
        Serial.println("[INFO] RTC (DS3231): Alarma 1 activada. Hora: " + getRTCAlarm1());
        Serial.println("[INFO] RTC (DS3231): Alarma 2 activada. Hora: " + getRTCAlarm2());
        break;
      default:
        Serial.println("[INFO] RTC (DS3231): Alarma 1 y 2 desactivadas");
        break;
    }
  }
}
