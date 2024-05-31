#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h> // Sử dụng thư viện Servo cho ESP8266
const int trig1 = 5;     // chân trig của HC-SR04
const int echo1 = 4; 
const int trig2 = 0;     // chân trig của HC-SR04
const int echo2 = 12;
const int trig3 = 3;     // chân trig của HC-SR04
const int echo3 = 13;
// Thông tin mạng WiFi
const char* ssid = "OPPO A32";
const char* password = "12042002";

// Tạo một server trên cổng 80
ESP8266WebServer server(8080);

// Tạo một đối tượng servo
Servo myservo1;
Servo myservo2;


// Chân GPIO mà servo được kết nối
int servoPin1 = 2; // GPIO2
int servoPin2 = 14; // GPIO2
void setup() {

  Serial.begin(9600);
  pinMode(trig1,OUTPUT);   // chân trig sẽ phát tín hiệu
  pinMode(echo1,INPUT);
  pinMode(trig2,OUTPUT);   // chân trig sẽ phát tín hiệu
  pinMode(echo2,INPUT);
  pinMode(trig3,OUTPUT);   // chân trig sẽ phát tín hiệu
  pinMode(echo3,INPUT);
  // Kết nối tới mạng WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Gắn servo vào chân GPIO
  myservo1.attach(servoPin1);
      myservo1.write(0);

  myservo2.attach(servoPin2);
      myservo2.write(0);

  // Thiết lập hàm xử lý cho yêu cầu GET tại root "/"
  server.on("/setServo1", handleSetServo1);
  server.on("/setServo2", handleSetServo2);
  server.on("/sieuam1",thung);
  server.on("/sieuam2",khoangcach);
  

  // Bắt đầu server
  server.begin();
  Serial.println("HTTP server started");
}



// Hàm xử lý cho yêu cầu đặt góc servo
void handleSetServo1() {
  if (server.hasArg("angle")) {
    int angle = server.arg("angle").toInt();
    if (angle >= 0 && angle <= 180) {
      myservo1.write(angle);
      String message = "Servo angle set to ";
      message += angle;
      message += " degrees";
      server.send(200, "text/plain", message);
    } else {
      server.send(400, "text/plain", "Invalid angle. Please use a value between 0 and 180.");
    }
  } else {
    server.send(400, "text/plain", "No angle provided. Please use /setServo?angle=<0-180>.");
  }
}


// Hàm xử lý cho yêu cầu đặt góc servo
void handleSetServo2() {
  if (server.hasArg("angle")) {
    int angle = server.arg("angle").toInt();
    if (angle >= 0 && angle <= 180) {
      myservo2.write(angle);
      String message = "Servo angle set to ";
      message += angle;
      message += " degrees";
      server.send(200, "text/plain", message);
    } else {
      server.send(400, "text/plain", "Invalid angle. Please use a value between 0 and 180.");
    }
  } else {
    server.send(400, "text/plain", "No angle provided. Please use /setServo?angle=<0-180>.");
  }
}
void getdistance(int trig,int echo){
  unsigned long duration; // biến đo thời gian
    int distance;           // biến lưu khoảng cách
    
    /* Phát xung từ chân trig */
    digitalWrite(trig,0);   // tắt chân trig
    delayMicroseconds(2);
    digitalWrite(trig,1);   // phát xung từ chân trig
    delayMicroseconds(5);   // xung có độ dài 5 microSeconds
    digitalWrite(trig,0);   // tắt chân trig
    
    /* Tính toán thời gian */
    // Đo độ rộng xung HIGH ở chân echo. 
    duration = pulseIn(echo,HIGH);  
    // Tính khoảng cách đến vật.
    distance = int(duration/2/29.412);
    String dis = String(distance);
    server.send(200,"text/plain",dis);
    /* In kết quả ra Serial Monitor */
    Serial.print(distance);
    Serial.println("cm");
}
int getdistance1(int trig, int echo){
  unsigned long duration; 
  unsigned long duration1;// biến đo thời gian
    int distance;           // biến lưu khoảng cách
    int distance1;
    digitalWrite(trig,0);   // tắt chân trig
    delayMicroseconds(2);
    digitalWrite(trig,1);   // phát xung từ chân trig
    delayMicroseconds(5);   // xung có độ dài 5 microSeconds
    digitalWrite(trig,0); 
    /* Phát xung từ chân trig */
     
     // tắt chân trig
    
    /* Tính toán thời gian */
    // Đo độ rộng xung HIGH ở chân echo. 
    duration = pulseIn(echo,HIGH);  
    // Tính khoảng cách đến vật.
    distance = int(duration/2/29.412);
    
    /* In kết quả ra Serial Monitor */
    Serial.print(distance);
    Serial.println("cm");
    return distance;
}

void thung(){
  int d1 = getdistance1(trig1,echo1);
  int d2= getdistance1(trig3,echo3);
  String kc = String(d1)+"," +String(d2);
  server.send(200,"text/plain",kc);
}
void khoangcach(){
  getdistance(trig2,echo2);
}


void loop() {
  server.handleClient();
  
}