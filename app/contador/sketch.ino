const int ledVermelho = 3;
const int ledVerde = 2;

int qt_credito = 0;
bool conectado = false;
bool ja_carregou = false;

//int a = 8;  // preto
//int b = 9;  // marrom
//int c = 10; // vermelho
//int d = 11; // laranja
//int e = 12; // amarelo
//int f = 13; // verde
//int g = 7;  // azul
//int dig_1 = 6; // rosa (dezenas)
//int dig_2 = 5; // cinza (unidades)

const int l = 1, d = 0; // catodo comum

// segmentos a,b,c,d,e,f,g
const int segmentos[7] = {8,9,10,11,12,13,7}; 
const int digitos[2]   = {5,6}; // 0 = unidades, 1 = dezenas

int contador[2] = {7, 6}; // começa em 99

// tabela de números (a,b,c,d,e,f,g)
byte digito[10][7] = { 
  { l,l,l,l,l,l,d },  // 0
  { d,l,l,d,d,d,d },  // 1
  { l,l,d,l,l,d,l },  // 2
  { l,l,l,l,d,d,l },  // 3
  { d,l,l,d,d,l,l },  // 4
  { l,d,l,l,d,l,l },  // 5
  { l,d,l,l,l,l,l },  // 6
  { l,l,l,d,d,d,d },  // 7
  { l,l,l,l,l,l,l },  // 8
  { l,l,l,l,d,l,l }   // 9
};

void setup() {
  for(int i=0; i<7; i++) pinMode(segmentos[i], OUTPUT);
  for(int i=0; i<2; i++) pinMode(digitos[i], OUTPUT);
}

void loop() {
  // Pega os valores do array contador
  int dz_cont = contador[0]; // dz_cont será 1
  int un_cont = contador[1]; // un_cont será 2
  

  dezena(dz_cont);
  delay(5);
  
  unidade(un_cont);
  delay(5);
}

void unidade(int numero) {
  // habilita só o display de unidades
  digitalWrite(digitos[0], LOW);   // unidades ON
  digitalWrite(digitos[1], HIGH);  // dezenas OFF

  // acende os segmentos do número
  for(int i=0; i<7; i++) {
    digitalWrite(segmentos[i], digito[numero][i]);
  }
}

void dezena(int numero) {
  // habilita só o display de dezenas
  digitalWrite(digitos[0], HIGH);  // unidades OFF
  digitalWrite(digitos[1], LOW);   // dezenas ON

  // acende os segmentos do número
  for(int i=0; i<7; i++) {
    digitalWrite(segmentos[i], digito[numero][i]);
  }
}
