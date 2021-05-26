#include <Wire.h>
#include <VL53L0X.h>
#include <SoftwareSerial.h>
SoftwareSerial s(5, 6);
#define EN 8
#define DIR 7
#define STEP 4

int times = 5 ;//số lần lấy mẫu
float number_sample = 200.0;
float current_agle = 0 ;            //góc hiện tại
float a;
int b;
int i;
unsigned long time1 = 0;
////code Duy////
float x1 ; //giá trị đọc từ cảm biến lần 1
float x2 ; //giá trị đọc từ cảm biến lần 2
float x3 ; // giá trị đọc từ cảm biến lần 3
float x4 ; //giá trị đọc từ cảm biến lần 4
float x5 ; //giá trị đọc từ cảm biến lần 5
float x6 ; //giá trị đọc từ cảm biến lần 6
float xtb;// giá trị trung bình của 6 giá trị đo thu được
float xtb_new;// giá trị trung bình sau khi lọc
float xtb_newX;// trục X giá trị trung bình sau khi lọc
float xtb_newY;// trục Y giá trị trung bình sau khi lọc
float dlc;// độ lệch chuẩn
uint8_t n_sample = 0 ; // n_sample: số mẫu tin cậy sau khi lọc
float acpt = 3.0 ; // acpt: hệ số cho phép lệch chuẩn
String gocquay, khoangcach, data;
/////code Duy////
VL53L0X sensor;

void setup()
{
  pinMode(12, INPUT_PULLUP);
  pinMode(EN, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);
  digitalWrite(12, HIGH);
  digitalWrite(EN, LOW);
  digitalWrite(DIR, HIGH);
  Serial.begin(115200);
  s.begin(9600);
  Wire.begin();
  sensor.init();

  sensor.setTimeout(500);
  sensor.startContinuous();

  a = 360.0  / number_sample ;    //góc quay giữa các vị trí lấy mẫu
  b = int (a / 0.1125);           // số bước cần cấp vào để quay được a độ
  //Serial.println (b);
}

void loop()
{
  for ( current_agle ; current_agle <= 360.0; current_agle = current_agle + a)
  {

    for (  i = 0; i <= b; i++)
    {
      digitalWrite(STEP, HIGH);
      delayMicroseconds(500);
      digitalWrite(STEP, LOW);
      delayMicroseconds(500);
    }
    //Serial.println ( current_agle);
    for ( int n = 0; n <= times ; n ++)
    {

      //      int distance = sensor.readRangeContinuousMillimeters();
      //      Serial.print("Distance: ");
      //      Serial.print(distance);
      //      Serial.print("mm");
      //      if (sensor.timeoutOccurred())
      //      {
      //        Serial.print(" TIMEOUT");
      //      }
      //      Serial.println();
      //      delay(100);
      /////code Duy////
      if (n == 0)
      {

        x1 = sensor.readRangeContinuousMillimeters();
        //Serial.println(x1);
      }
      if (n == 1)
      {
        x2 = sensor.readRangeContinuousMillimeters();
        //Serial.println(x2);
      }
      if (n == 2)
      {
        x3 = sensor.readRangeContinuousMillimeters();
        //Serial.println(x3);
      }
      if (n == 3)
      {
        x4 = sensor.readRangeContinuousMillimeters();
        //Serial.println(x4);
      }
      if (n == 4)
      {
        x5 = sensor.readRangeContinuousMillimeters();
        //Serial.println(x5);
      }
      if (n == 5)
      {
        x6 = sensor.readRangeContinuousMillimeters();
        //Serial.println(x6);
      }
    }
    loc_ppchuan();
    //xtb_newX = xtb_new * cos(current_agle * 3.14 / 180.0);
    //s.write(xtb_newX);
    //Serial.print("Truc X cua khoang cach :");
    //Serial.println(xtb_newX);
    //xtb_newY = xtb_new * sin(current_agle * 3.14 / 180.0);
    //s.write(xtb_newY);
    //Serial.print("Truc Y cua khoang cach :");
    //Serial.println(xtb_newY);
    gocquay =  String(current_agle);
    khoangcach = String(xtb_new);
    data = gocquay + "," + khoangcach;
    if (xtb_new >= 30 && xtb_new <= 800)
    {
      Serial.println(data);
    }
    s.println(data);
    delay(100);
    /////code Duy////
  }
}
/////code Duy/////
void loc_ppchuan()
{
  xtb = (x1 + x2 + x3 + x4 + x5 + x6) * 1.0 / 6.0;
  //Serial.println(xtb);
  dlc = sqrt(sq((x1 - xtb) * 1.0 / 6.0)
             + sq((x2 - xtb) * 1.0 / 6.0)
             + sq((x3 - xtb) * 1.0 / 6.0)
             + sq((x4 - xtb) * 1.0 / 6.0)
             + sq((x5 - xtb) * 1.0 / 6.0)
             + sq((x6 - xtb) * 1.0 / 6.0));
  //Serial.println(dlc);
  xtb_new = 0;
  n_sample = 0;
  if (abs(x1 - xtb) < acpt * dlc)
  {
    xtb_new += x1;
    n_sample++;
  }
  if (abs(x2 - xtb) < acpt * dlc)
  {
    xtb_new += x2;
    n_sample++;
  }
  if (abs(x3 - xtb) < acpt * dlc)
  {
    xtb_new += x3;
    n_sample++;
  }
  if (abs(x4 - xtb) < acpt * dlc)
  {
    xtb_new += x4;
    n_sample++;
  }
  if (abs(x5 - xtb) < acpt * dlc)
  {
    xtb_new += x5;
    n_sample++;
  }
  if (abs(x6 - xtb) < acpt * dlc)
  {
    xtb_new += x6;
    n_sample ++;
  }
  xtb_new = xtb_new * 1.0 / n_sample * 1.0 + 10.0;
  //Serial.println(n_sample);
  //Serial.println("khoang cach trung binh sau khi loc:");
  //Serial.println(xtb_new);
}
/////code Duy////
