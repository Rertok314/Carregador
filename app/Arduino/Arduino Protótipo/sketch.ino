#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);
const byte rows = 4;
const byte cols = 4;

char keys [rows][cols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowpin[rows] = { 13,12,11,10};
byte colpin[cols] = {9,8,7,6};

Keypad keypad = Keypad( makeKeymap(keys), rowpin, colpin, rows, cols);

String telefone = ""; 

void setup () {
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Digite telefone:");
  lcd.setCursor(0,1);
}

void loop() {
  char key = keypad.getKey();

  if (key != NO_KEY) {
    if (key >= '0' && key <= '9') {
      if (telefone.length() < 11) {
        telefone += key;
        lcd.print(key); 
      }
    }

    if (telefone.length() == 11) {
      delay(500); 
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Carregando...");
      delay(3000); 
      lcd.clear();

      telefone = "";
      lcd.setCursor(0,0);
      lcd.print("Digite telefone:");
      lcd.setCursor(0,1);
    }
  }
}
