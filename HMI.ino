#define TEMPER A0
#define PH A1
#define ECHO1 5
#define TRIG1 4
#define ECHO2 3
#define TRIG2 2
#define v1 13
#define v2 12
#define v3 11
#define ev 10
#define mt1 9
#define mt2 8
#define pump 7
#define reset 6
int i = 0;
String dfd = "";
union {
  char charByte[4];
  long valLong;
} value;
void setup() {
  Serial.begin(57600);
  pinMode(TEMPER, INPUT);
  pinMode(PH, INPUT);
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);
  pinMode(v1, OUTPUT);
  pinMode(v2, OUTPUT);
  pinMode(v3, OUTPUT);
  pinMode(ev, OUTPUT);
  pinMode(mt1, OUTPUT);
  pinMode(mt2, OUTPUT);
  pinMode(pump, OUTPUT);
  pinMode(reset, OUTPUT);
}
void loop() {
  if (Serial.available()){
    dfd += char(Serial.read());
  }
  value.charByte[0]= char(dfd[0]);
  value.charByte[1]= char(dfd[1]);
  value.charByte[2]= char(dfd[2]);
  value.charByte[3]= char(dfd[3]);
while(i<value.valLong){
  float x=analogRead(TEMPER);                
  int temp=((x*400)/523.776)+21;    
  Serial.print("page0.n0.val=");
  Serial.print(temp);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  if (temp >=75){
  int tval = map(temp,66.1,375,0,229);
  Serial.print("page0.z0.val=");
  Serial.print(tval);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  }  
  else if (temp<66.1) {
  int tval = map(temp,0,75,311,360);
  Serial.print("page0.z0.val=");
  Serial.print(tval);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  }
   //acidity reading
  float anatension=analogRead(PH);    
  float acid=((anatension*14)/1023)*100;     //value is multiplied by 100 btw, so 14ph is 1400
  int intacid=acid;
  Serial.print("page0.x0.val=");
  Serial.print(intacid);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  if (acid >=246.8){
  int aval = map(acid,246.8,1400,0,229);
  Serial.print("page0.z1.val=");
  Serial.print(aval);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  }  
  else if (acid<246.8) {
  float aval = map(acid,0,246.8,311,360);
  Serial.print("page0.z1.val=");
  Serial.print(aval);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  }
  //distance1 reading  
  digitalWrite(TRIG1, LOW);             //distance reading
  delayMicroseconds(2); 
  digitalWrite(TRIG1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG1, LOW);
  float dist1 = pulseIn(ECHO1, HIGH) / 58.2;
  //distance2 to volume reading
  digitalWrite(TRIG2, LOW);             //distance reading
  delayMicroseconds(2); 
  digitalWrite(TRIG2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG2, LOW);
  float dist2 = pulseIn(ECHO2, HIGH) / 58.2;
  float wl=(200-dist2)*0.01;
  int vol=wl*4*2*1000; //16000 liters capable
  int Avol=map(vol,0,16000,0,100);
  Serial.print("page0.j0.val=");
  Serial.print(Avol);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.print("page0.n1.val=");
  Serial.print(vol);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  if(dist1<=20){
  Serial.print("page0.t0.txt=");
  Serial.print("\"");
  Serial.print("ON");
  Serial.print("\"");
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  }

  else {
  Serial.print("page0.t0.txt=");
  Serial.print("\"");
  Serial.print("OFF");
  Serial.print("\"");
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  }
  if (temp>160 && acid>4.4 && dist1<=20 && dist2>5){
    Serial.print("page0.p0.pic=");
    Serial.print(11);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.print("page2.p1.pic=");
    Serial.print(9);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    digitalWrite(mt1, HIGH);
    digitalWrite(mt2, HIGH);
    digitalWrite(pump, LOW);
    digitalWrite(ev, LOW);
    digitalWrite(v1, LOW);
    digitalWrite(v2, LOW);
    digitalWrite(v3, LOW);
    delay(1000);
    Serial.print("page2.p1.pic=");
    Serial.print(10);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    digitalWrite(mt1, LOW);
    digitalWrite(mt2, LOW);
    digitalWrite(pump, HIGH);
    digitalWrite(ev, HIGH);
    digitalWrite(v1, HIGH);
    digitalWrite(v2, HIGH);
    digitalWrite(v3, HIGH);
    delay(1000);
    Serial.print("page2.p1.pic=");
    Serial.print(8);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    digitalWrite(mt1, LOW);
    digitalWrite(mt2, LOW);
    digitalWrite(pump, LOW);
    digitalWrite(ev, LOW);
    digitalWrite(v1, LOW);
    digitalWrite(v2, LOW);
    digitalWrite(v3, LOW);
      i++;
  }
  else {
    Serial.print("page2.p1.pic=");
    Serial.print(8);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    digitalWrite(mt1, LOW);
    digitalWrite(mt2, LOW);
    digitalWrite(pump, LOW);
    digitalWrite(ev, LOW);
    digitalWrite(v1, LOW);
    digitalWrite(v2, LOW);
    digitalWrite(v3, LOW);
}
}
dfd="";
}