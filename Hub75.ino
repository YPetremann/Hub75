#include <RGBmatrixPanel.h>
#include <Adafruit_GFX.h>
#include <Fonts/Picopixel.h>
//#define CLK  8   // USE THIS ON ADAFRUIT METRO M0, etc.
//#define CLK A4 // USE THIS ON METRO M4 (not M0)
#define CLK 11 // USE THIS ON ARDUINO MEGA
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3
#define SCREEN_WIDTH  64
#define SCREEN_HEIGHT 32
// #define LOG
// Enable double buffering
RGBmatrixPanel *matrix = new RGBmatrixPanel(A, B, C, D, CLK, LAT, OE, true, SCREEN_WIDTH);

bool report=false;
void setup() {
  Serial.begin(57600); // Increased baud rate for faster serial
  matrix->begin();
  matrix->setFont(&Picopixel);
  color(0,0,0);
  cls();
  show();
  Serial.println("");
  Serial.println("SerialHub75 V1.0");
  Serial.println("================");
  Serial.println("Tappe ? pour afficher l'aide");

  color(0,0,0);cls();show();
  color(127,000,255); moveTo(1,17); lineHTo(25); lineTo(35,7);lineHTo(62);show();
  color(255,000,255); moveBy(-61,11); lineHBy(24); lineBy(10,-10); lineHBy(27);show();
  color(255,000,192); moveBy(-61,11); lineHBy(24); lineBy(10,-10); lineHBy(27);show();
  color(255,000,000); moveBy(-61,11); lineHBy(24); lineBy(10,-10); lineHBy(27);show();
  color(255,127,000); moveBy(-61,11); lineHBy(24); lineBy(10,-10); lineHBy(27);show();
  color(255,255,000); moveBy(-61,11); lineHBy(24); lineBy(10,-10); lineHBy(27);show();
  color(000,255,000); moveBy(-61,11); lineHBy(24); lineBy(10,-10); lineHBy(27);show();
  color(000,255,255); moveBy(-61,11); lineHBy(24); lineBy(10,-10); lineHBy(27);show();
  color(000,127,255); moveBy(-61,11); lineHBy(24); lineBy(10,-10); lineHBy(27);show();
  color(000,000,255); moveBy(-61,11); lineHBy(24); lineBy(10,-10); lineHBy(27);show();

  color(255,255,000);moveTo(1,1);print("A");show();
  color(127,255,000);print("V");show();
  color(000,255,000);print("I");show();
  color(000,255,127);print("L");show();
  color(000,255,255);print("A");show();
  color(000,127,255);print("B");show();
  color(127,127,127);print("  ANIMATION");

  color(000,000,127);moveTo(2,8); rectTo(7,7);
  color(127,000,000);moveBy(11,3);circle(3);
  color(000,127,000);moveBy(5,3); triangleBy(3,-6,6,0);;

  report=true;
  moveTo(35,20);color(255,255,255);print("LET'S GO");show();
}

String inputString = "";
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      doCommands(inputString);
      inputString="";
    }
    else inputString += inChar;
  }
}

bool autoDisplay=false;
unsigned long previousMillis = 0;
const unsigned long interval = 250; // 250 milliseconds
void loop() {
  /*
  if(autoDisplay){
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      removeCursor();
      show();
      displayCursor();
      show();
    }
  }
  */
}

bool success=true;
void doCommands(String input) {
  input=input+"\n";
  String cmdsBuf = "";
  bool inString = false;
  bool inComment = false;
  //if(autoDisplay) removeCursor();
  
  for (int i = 0; i < input.length(); i++) {
    char c = input.charAt(i);
    if(c == '\n') inComment = false;
    if(!inString && c == '#') inComment = true;
    if(inComment) continue;
    if(c == '\n' || (!inString && c == ';')) {
      cmdsBuf.trim();
      if(inString) {
        Serial.print("Invalid command: ");
        Serial.println(cmdsBuf);
      } else if(cmdsBuf.length() > 0) {
        doCommand(cmdsBuf);
      }
      inString = false;
      cmdsBuf = "";
    } else {
      if(c == '"') inString = !inString;
      cmdsBuf += c;
    }
  }
  if(autoDisplay) {
    show();
    //displayCursor();
    //show();
  }
  if(success && report) {
    Serial.println("[ OK ]");
  }
}

String cmdArgs="";
void doCommand(String cmdBuf){
  cmdBuf.trim();
  byte index=cmdBuf.indexOf(' ');
  if(index==-1) index=cmdBuf.length();
  String cmdName=cmdBuf.substring(0,index);
  cmdArgs=cmdBuf.substring(index);
  cmdArgs.trim();
  if(cmdName=="?") {
    Serial.println("Commands List:");
    Serial.println("C 0 0 255    # definie couleur a 0,0,255 (bleu)");
    Serial.println("M 5 10       # deplace position a 5,10");
    Serial.println("m 5 5        # deplace position de 5,10");
    Serial.println("c            # fait un cercle de rayon 5");
    Serial.println("T 5 5 10 10  # fait un triangle par 5,5 et 10,10");
    Serial.println("t 5 5 10 10  # fait un triangle de 5,5 et 10,10");
    Serial.println("L 5 5        # fait une line a 5,5");
    Serial.println("l 5 5        # fait une line de 5 5");
    Serial.println("H 5          # fait une ligne horizontale a 5");
    Serial.println("h 5          # fait une ligne horizontale de 5");
    Serial.println("V 5          # fait une ligne verticale a 5");
    Serial.println("v 5          # fait une ligne verticale de 5");
    Serial.println("P \"Texte\"    # ecrit Texte");
    Serial.println("R 10 5       # fait un rectangle a 10,5");
    Serial.println("r 10 5       # fait un rectangle de 10,5");
    Serial.println("F            # rempli l'ecran");
    Serial.println("Z            # affiche le dessin (affichage automatique)");
    Serial.println("z            # affiche le dessin (affichage manuel)");
  }
  else if(cmdName=="C") {
    int r = argInt(getArg(1,"R"),0);
    int v = argInt(getArg(2,"V"),0);
    int b = argInt(getArg(3,"B"),0);

    #ifdef LOG
      Serial.print("[ OK ] COLOR");
      Serial.print(" R "); Serial.print(r);
      Serial.print(" V "); Serial.print(v);
      Serial.print(" B "); Serial.print(b);
      Serial.println();
    #endif

    color(r,v,b);
  }
  else if(cmdName=="M"){
    int x = argInt(getArg(1,"X"),0);
    int y = argInt(getArg(2,"Y"),0);

    #ifdef LOG
      Serial.print("[ OK ] MOVE ABS");
      Serial.print(" X "); Serial.print(x);
      Serial.print(" Y "); Serial.print(y);
      Serial.println();
    #endif

    moveTo(x,y);
  }
  else if(cmdName=="m"){
    int x = argInt(getArg(1,"X"),0);
    int y = argInt(getArg(2,"Y"),0);

    #ifdef LOG
      Serial.print("[ OK ] MOVE REL");
      Serial.print(" X "); Serial.print(x);
      Serial.print(" Y "); Serial.print(y);
      Serial.println();
    #endif

    moveBy(x,y);
  }
  else if(cmdName=="c"){
    int r = argInt(getArg(1,"R"),0);

    #ifdef LOG
      Serial.print("[ OK ] CIRCLE");
      Serial.print(" R "); Serial.print(r);
      Serial.println();
    #endif

    circle(r);
  }
  else if(cmdName=="T"){
    int x1 = argInt(getArg(1,"U"),0);
    int y1 = argInt(getArg(2,"V"),0);
    int x2 = argInt(getArg(3,"X"),0);
    int y2 = argInt(getArg(4,"Y"),0);

    #ifdef LOG
      Serial.print("[ OK ] TRIANGLE ABS");
      Serial.print(" R "); Serial.print(r);
      Serial.println();
    #endif

    triangleBy(x1,y1,x2,y2);
  }
  else if(cmdName=="t"){
    int x1 = argInt(getArg(1,"U"),0);
    int y1 = argInt(getArg(2,"V"),0);
    int x2 = argInt(getArg(3,"X"),0);
    int y2 = argInt(getArg(4,"Y"),0);

    #ifdef LOG
      Serial.print("[ OK ] TRIANGLE REF");
      Serial.print(" R "); Serial.print(r);
      Serial.println();
    #endif

    triangleTo(x1,y1,x2,y2);
  }
  else if(cmdName=="L"){
    int x = argInt(getArg(1,"X"),0);
    int y = argInt(getArg(2,"Y"),0);

    #ifdef LOG
      Serial.print("[ OK ] LINE ABS");
      Serial.print(" X "); Serial.print(x);
      Serial.print(" Y "); Serial.print(y);
      Serial.println();
    #endif

    lineTo(x,y);
  }
  else if(cmdName=="l"){
    int x = argInt(getArg(1,"X"),0);
    int y = argInt(getArg(2,"Y"),0);

    #ifdef LOG
      Serial.print("[ OK ] LINE REL");
      Serial.print(" X "); Serial.print(x);
      Serial.print(" Y "); Serial.print(y);
      Serial.println();
    #endif

    lineBy(x,y);
  }
  else if(cmdName=="H"){
    int x = argInt(getArg(1,"X"),0);

    #ifdef LOG
      Serial.print("[ OK ] LINE H ABS");
      Serial.print(" X "); Serial.print(x);
      Serial.println();
    #endif

    lineVTo(x);
  }
  else if(cmdName=="h"){
    int x = argInt(getArg(1,"X"),0);

    #ifdef LOG
      Serial.print("[ OK ] LINE H REL");
      Serial.print(" X "); Serial.print(x);
      Serial.println();
    #endif

    lineHBy(x);
  }
  else if(cmdName=="V"){
    int y = argInt(getArg(1,"Y"),0);

    #ifdef LOG
      Serial.print("[ OK ] LINE V ABS");
      Serial.print(" Y "); Serial.print(y);
      Serial.println();
    #endif

    lineHTo(y);
  }
  else if(cmdName=="v"){
    int y = argInt(getArg(1,"Y"),0);

    #ifdef LOG
      Serial.print("[ OK ] LINE V REL");
      Serial.print(" Y "); Serial.print(y);
      Serial.println();
    #endif

    lineHBy(y);
  }
  else if(cmdName=="P"){
    String t = argString(getArg(1,"T"),"?");

    #ifndef LOG
      Serial.print("[ OK ] PRINT");
      Serial.print(" T "); Serial.print(t);
      Serial.println();
    #endif

    print(t);
  }
  else if(cmdName=="R"){
    int x = argInt(getArg(1,"X"),0);
    int y = argInt(getArg(2,"Y"),0);

    #ifdef LOG
      Serial.print("[ OK ] RECT ABS");
      Serial.print(" X "); Serial.print(x);
      Serial.print(" Y "); Serial.print(y);
      Serial.println();
    #endif

    rectTo(x,y);
  }
  else if(cmdName=="r"){
    int x = argInt(getArg(1,"X"),0);
    int y = argInt(getArg(2,"Y"),0);

    #ifdef LOG
      Serial.print("[ OK ] RECT REL");
      Serial.print(" X "); Serial.print(x);
      Serial.print(" Y "); Serial.print(y);
      Serial.println();
    #endif

    rectBy(x,y);
  }
  else if(cmdName=="F" || cmdName=="f" ){
    #ifdef LOG
      Serial.print("[ OK ] FILLSCREEN");
      Serial.println();
    #endif

    cls();
  }
  else if(cmdName=="z"){
    #ifdef LOG
      Serial.print("[ OK ] SHOW");
      Serial.println();
    #endif
    autoDisplay=false;
    show();
  }
  else if(cmdName=="Z"){
    #ifdef LOG
      Serial.print("[ OK ] SHOW");
      Serial.println();
    #endif
    autoDisplay=true;
    show();
  }
  else {
    success=false;
    Serial.print("[FAIL] invalid command : ");
    Serial.print(cmdName);
    Serial.print(" ");
    Serial.println(cmdArgs);
  }
}

String getArg(byte index, String key){
  String val = getArg(key);
  if(val.length()==0) val = getArg(index);
  return val;
}

String getArg(byte index) {
  byte currentIndex = 1; // args are 1-based
  bool inQuote = false;
  byte start = 0;
  byte length = 0;
  byte i = 0;
  while (i < cmdArgs.length()) {
    while (i < cmdArgs.length() && cmdArgs[i] == ' ') i++;
    if (i >= cmdArgs.length()) break;

    start = i;
    if (cmdArgs[i] == '"') {
      inQuote = true;
      i++; // skip opening quote
      while (i < cmdArgs.length() && cmdArgs[i] != '"') i++;
      i++; // include closing quote
    } else {
      while (i < cmdArgs.length() && cmdArgs[i] != ' ') i++;
    }
    if (currentIndex == index) {
      String ret=cmdArgs.substring(start, i);
      return ret;
    }
    currentIndex++;
  }
  return ""; // not found
}

String getArg(String key) {
  bool inQuote = false;
  byte i = 0;
  byte keyLen = key.length();
  while (i < cmdArgs.length()) {
    // skip leading spaces
    while (i < cmdArgs.length() && cmdArgs[i] == ' ') i++;

    if (i >= cmdArgs.length()) break;

    byte start = i;

    // Handle quoted arguments
    if (cmdArgs[i] == '"') {
      i++; // skip opening quote
      while (i < cmdArgs.length() && cmdArgs[i] != '"') i++;
      i++; // include closing quote
    } else {
      // Check if the argument starts with the key
      if (cmdArgs[i] == key[0]) {
        if (cmdArgs.substring(i, i + keyLen) == key) {
          byte valueStart = i + keyLen;
          byte valueEnd = valueStart;
          if (valueStart < cmdArgs.length() && cmdArgs[valueStart] == '"') {
            valueStart++;
            valueEnd = valueStart;
            while (valueEnd < cmdArgs.length() && cmdArgs[valueEnd] != '"') valueEnd++;
            return cmdArgs.substring(valueStart, valueEnd); 
          } else {
            while (valueEnd < cmdArgs.length() && cmdArgs[valueEnd] != ' ') valueEnd++;
            return cmdArgs.substring(valueStart, valueEnd);
          }
        }
      }
      while (i < cmdArgs.length() && cmdArgs[i] != ' ') i++;
    }
    i++;
  }
  return ""; // not found
}

String argString(String val, String defaultVal) {
  if (val.length() == 0) return defaultVal;
  if (val.length() >= 2 && val[0] == '"' && val[val.length() - 1] == '"') {
    val = val.substring(1, val.length() - 1);
  }
  return val;
}

int argInt(String val, int defaultVal) {
  if (val.length() == 0) return defaultVal;
  for (int i = 0; i < val.length(); i++) {
    if (!isDigit(val[i]) && !(i == 0 && val[i] == '-')) return defaultVal;
  }
  return val.toInt();
}

uint16_t col=0;
bool bright = true;
// C
void color(byte r, byte g, byte b){
  col=matrix->Color888(r,g,b);
    // Calculate perceived brightness
  float luminance = 0.299 * r + 0.587 * g + 0.114 * b;
  
  // You can tweak this threshold
  bright = luminance > 127;
}

int cx=0;
int cy=0;
uint16_t ocol=0;
void removeCursor(){
  matrix->drawPixel(cx,cy,ocol);
}

void displayCursor(){
  //ocol = matrix->getPixel(cx,cy);
  uint16_t ncol = millis() % 500 > 250 ? col: bright ? matrix->Color888(0,0,0) : matrix->Color888(255,255,255);
  matrix->drawPixel(cx,cy, ncol);
}

// M
void moveTo(int ax,int ay){ cx=ax; cy=ay; }
// m
void moveBy(int rx,int ry){ cx+=rx; cy+=ry; }
// L
void lineTo(int ax,int ay){ 
  matrix->drawLine(cx,cy, ax,ay, col);
  moveTo(ax,ay);
}
// l
void lineBy(int rx, int ry){ lineTo(cx+rx,cy+ry); }
// H
void lineHTo(int ax){ lineTo(ax,cy); }
// V
void lineVTo(int ay){ lineTo(cx,ay); }
// h
void lineHBy(int rx){ lineBy(rx,0); }
// v
void lineVBy(int ry){ lineBy(0,ry); }
// P
void print(String text){
  Serial.print('"');
  Serial.print(text);
  Serial.println('"');

  matrix->setCursor(cx, cy+4);
  matrix->setTextColor(col);
  matrix->print(text);
  int x1;
  int y1;
  int tw;
  int th;
  matrix->getTextBounds(text, cx, cy+4, &x1, &y1, &tw, &th);
  cx+=tw+1;
}
// R
void rectTo(int ax, int ay){ matrix->fillRect(cx,cy, ax,ay, col); }
// r
void rectBy(int rx, int ry){ matrix->fillRect(cx,cy, cx+rx,cy+ry, col); }
// c
void circle(int r){ matrix->fillCircle(cx,cy, r, col); }
void triangleBy(int x1, int y1, int x2, int y2) {
  matrix->fillTriangle(cx, cy, cx + x1, cy + y1, cx + x2, cy + y2, col);
  moveTo(cx + x2, cy + y2);
}
void triangleTo(int x1, int y1, int x2, int y2) {
  matrix->fillTriangle(cx, cy, x1, y1, x2, y2, col);
  moveTo(x2, y2);
}
// F
void cls(){ matrix->fillScreen(col);}

// Z or z
void show(){ matrix->swapBuffers(true); }