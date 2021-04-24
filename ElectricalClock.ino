/********************
  Yi-Tse Wu
  2021-04-24
********************/

// include libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Global variables
LiquidCrystal_I2C screen(0x27, 20, 3);
long int second;
int day, daySum, secondShow, minuteShow, hourShow, yearShow, monthShow, dayShow, weekendShow;
String weekend[7] = {"Monday", "Tuesday", "Wenseday", "Thursday", "Friday", "Saturday", "Sunday"};
int dayLimit[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// Function
void setupScreen(String s1, String s2);
void setupCharacter(String c, int loca1, int loca2);
void screenCorrectShow(int timeVariable, int location, int endLocation, int limit);

void setup()
{
  // Programmer sets second, day, and yearShow to regulate time
  second = 44300;
  day = 95;
  yearShow = 2021;
  weekendShow = 0;
  monthShow = 0;
  setupScreen("Today is ", "Current time is ");

  pinMode(A0, INPUT);
}

void loop()
{

  // Use second variable to get time data
  secondShow = second % 60;
  minuteShow = (second / 60) % 60;
  hourShow = (second / 60) / 60;

  // Use function to show current time on screen
  screenCorrectShow(hourShow, 0, 3, 24);
  screenCorrectShow(minuteShow, 5, 3, 60);
  screenCorrectShow(secondShow, 10, 3, 60);

  // Make second zero to avoid overflow / make weekend add 1
  if (second == 86400)
  {
    second = 0;
    day ++;
    weekendShow ++;
  }
  if (analogRead(A0) == 0)
  {
    day ++;
    weekendShow ++;
  }
  if (day > 365)
  {
    day = 0;
    yearShow += 1;
  }

  // Use for loop to get month
  if (second == 0 || monthShow == 0 || analogRead(A0) == 0)
  {
    daySum = 0;
    dayShow = day % 365;
    for (int i = 0 ; i < 12 ; i++)
    {
      if ( i == 11)
      {
        monthShow = 12;
        break;
      }
      if (day > daySum && day < (daySum + dayLimit[i]))
      {
        monthShow = i + 1;
        break;
      }
      daySum += dayLimit[i];
      dayShow -= dayLimit[i];
    }
    setupScreen("Today is ", "Current time is ");
  }

  // Show date on screen
  screen.setCursor(9, 0);
  screen.print(weekend[weekendShow % 7]);
  screenCorrectShow(yearShow, 0, 1, 10000);
  screenCorrectShow(monthShow, 7, 1, 12);
  screenCorrectShow(dayShow, 12, 1, 365);

  // Add second variable 1 / 1s
  delay(1000);
  second ++;
}

void setupScreen(String s1, String s2)
{
  screen.init();
  screen.backlight();
  screen.setCursor(0, 0);
  screen.print(s1);
  screen.setCursor(0, 2);
  screen.print(s2);
  setupCharacter(":", 3, 3);
  setupCharacter(":", 8, 3);
  setupCharacter("-", 5, 1);
  setupCharacter("-", 10, 1);
}

void setupCharacter(String c, int loca1, int loca2)
{
  screen.setCursor(loca1, loca2);
  screen.print(c);
}

void screenCorrectShow(int timeVariable, int location, int endLocation, int limit)
{
  // Make timeVariable zero
  if (timeVariable >= limit)
    timeVariable = 0;

  // When timeVariable < 10, lcd screen shows a error.
  // Need to add zero to correct it
  if (timeVariable < 10)
  {
    screen.setCursor(location, endLocation);
    screen.print("0");
    screen.setCursor(location + 1, endLocation);
    screen.print(timeVariable);
  }
  else
  {
    screen.setCursor(location, endLocation);
    screen.print(timeVariable);
  }

  // Avoid variable overflow
  if (timeVariable > 86400)
    timeVariable = 0;
}
