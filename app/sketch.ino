const int ledVermelho = 3;
const int ledVerde = 2;

int qt_credito = 0;
bool conectado = false;
bool ja_carregou = false;

void setup() {
  Serial.begin(115200);
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledVerde, OUTPUT);
}

void loop() {
  carregador_1();
}

void carregador_1() {
  conectado = conector_1();

  if (conectado) {
    if (qt_credito == 0 && ja_carregou == false) {
      qt_credito = credito_1();
      ja_carregou = true;
    }

    if (qt_credito > 0) {
      carregando();
      Serial.print("Crédito restante: ");
      Serial.println(qt_credito);
      qt_credito--;
      qt_credito--;

    } else {
      finalizado();
      conectado = false;
    }
  } else {
    disponivel();
  }
}

int credito_1() {
  return 16; // valor inicial de crédito
}

bool conector_1() {
  return true; // simulação: sempre conectado
}

void carregando() {
  Serial.println("Carregando...");
  digitalWrite(ledVermelho, LOW);
  digitalWrite(ledVerde, HIGH);
  delay(500);
  digitalWrite(ledVerde, LOW);
  delay(500);
}

void disponivel() {
  Serial.println("Disponível");
  digitalWrite(ledVermelho, LOW);  
  digitalWrite(ledVerde, HIGH);
  delay(1000);
}

void finalizado() {
  Serial.println("Carregado!");
  digitalWrite(ledVermelho, HIGH);
  digitalWrite(ledVerde, LOW);
  delay(2000);
}
