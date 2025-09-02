void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}

// simulacao carregando
void loop() {
  // put your main code here, to run repeatedly:
  carregador_1();
}

void carregador_1() {
    kw = 3;
    int qt_kw = medidor(kw);
    bool vaga = conector();
    if(qt_kw != 0 and vaga == true) {
      carregando();
  } else if (qt_kw == 0 and vaga == true) {
      finalizado();
  } else {
    disponivel();
  }
}

void medidor(kw) {
  kw--;
}

bool conector() {
  return true;
}

// verde
void carregando() {
  Serial.println("Carregando");
  digitalWrite(11, HIGH);
  delay(1000);
  digitalWrite(11, LOW);
  delay(1000);
}

// amarelo
void disponivel() {
  Serial.println("Disponível");
  digitalWrite(12, HIGH);
  delay(1000);
  digitalWrite(12, LOW);
  delay(1000);
}

// vermelho
void finalizado() {
  Serial.println("Carregado");
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(1000);
}