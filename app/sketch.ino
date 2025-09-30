#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

// ------------------ LCD ------------------
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ------------------ PINOS ------------------
const int ledVermelho   = 4;
const int ledVerde      = 5;
const int botao_verde   = 1;
const int botao_vermelho= 3;
const int botao_amarelo = 0;

// ------------------ TECLADO ------------------
const byte rows = 4;
const byte cols = 4;
char keys[rows][cols] = {
  { '1','2','3','A'},
  { '4','5','6','B'},
  { '7','8','9','C'},
  { '*','0','#','D'}
};

byte rowPins[rows] = {13, 12, 11, 10};
byte colPins[cols] = {9, 8, 7, 6};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

// ------------------ VARIÁVEIS ------------------
String cpf = "";
String cpf_salvo = "";
String telefone = "";
String telefone_salvo = "";
String codigo_SMS = "12345";
int saldo = 0;
const int saldo_minimo = 20;

enum Estado { 
  DISPONIVEL, 
  DIGITANDO_CPF, 
  CONFIRMANDO_CPF,
  CPF_CADASTRADO,
  DIGITANDO_TELEFONE, 
  CONFIRMANDO_TELEFONE, 
  VALIDAR_SMS,
  ERRO_SMS,
  REENVIAR_SMS,
  CONSULTAR_SALDO,
  MOSTRAR_SALDO, 
  INSERIR_CREDITO, 
  FINALIZADO, 
  CARREGANDO 
};
Estado estadoAtual = DISPONIVEL;

char tecla; // tecla pressionada atual

// ------------------ SETUP ------------------
void setup() {
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(botao_verde, INPUT_PULLUP);
  pinMode(botao_vermelho, INPUT_PULLUP);
  pinMode(botao_amarelo, INPUT_PULLUP);
  
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.print("Iniciando...");
  delay(2000);
  lcd.clear();
  mostrarDisponivel();
}

// ------------------ LOOP ------------------
void loop() {
  tecla = keypad.getKey();

  switch (estadoAtual) {
    case DISPONIVEL:
      digitalWrite(ledVerde, HIGH);
      digitalWrite(ledVermelho, LOW);
      if (tecla == '#') {
        estadoAtual = DIGITANDO_CPF;
        cpf = "";
        lcd.backlight();
        lcd.clear();
      }
      break;

    case DIGITANDO_CPF:
      digitarCampo(cpf, "CPF");
      break;

    case CONFIRMANDO_CPF:
      confirmarCPF();
      break;

    case CPF_CADASTRADO:
      CPF_cadastrado();
      break;

    case DIGITANDO_TELEFONE:
      digitarCampo(telefone, "TELEFONE");
      break;

    case CONFIRMANDO_TELEFONE:
      confirmarTelefone();
      break;
    
    case VALIDAR_SMS:
      validar_sms();
      break;

    case ERRO_SMS:
      sms_invalido();
      break;
    
    case REENVIAR_SMS:
      reenviar_sms();
      break;

    case CONSULTAR_SALDO:
      consultarSaldo();
      break;

    case MOSTRAR_SALDO:
      mostrarSaldo();
      break;

    case INSERIR_CREDITO:
      inserirCredito();
      break;

    case CARREGANDO:
      piscandoLEDverde();
      break;

    case FINALIZADO:
      finalizado();
      break;
  }
}

// ------------------ FUNÇÕES ------------------

// Etapa 1 - DISPONIVEL
void mostrarDisponivel() {
  lcd.clear();
  digitalWrite(ledVermelho, LOW);
  digitalWrite(ledVerde, HIGH);
  lcd.print("Disponivel...");
  delay(750);
  lcd.setCursor(0, 1);
  lcd.print("Pressione #");
  delay(500);
  lcd.noBacklight();
}

// Etapa 2 - Digitar campos (DIGITANDO_CPF/DIGITANDO_TELEFONE)
void digitarCampo(String &campo, String nomeCampo) {
  bool botaoSim = digitalRead(botao_verde);
  bool botaoNao = digitalRead(botao_vermelho);
  bool botaoLimpar = digitalRead(botao_amarelo);

  lcd.setCursor(0,0);
  lcd.print("Digite " + nomeCampo + ":");
  lcd.setCursor(0,1);
  lcd.print(campo);

  if (tecla >= '0' && tecla <= '9') {
    int maxLen = (nomeCampo == "CPF") ? 11 : 15;
    int Maxlen = (nomeCampo == "TELEFONE") ? 11 : 15;
    if (campo.length() < maxLen && campo.length() < Maxlen) 
      campo += tecla;
  }
  else if (botaoNao==LOW) {
    cpf = "";
    if (nomeCampo == "CPF") {
      estadoAtual = DISPONIVEL;
      mostrarDisponivel();
    } else {
      estadoAtual = DIGITANDO_CPF;
    }
  } else if (botaoLimpar==LOW) {
    campo = "";
    mostrarDado(nomeCampo, campo);
  }
  else if (botaoSim==LOW) {
    if (nomeCampo == "CPF") {
      if (campo.length() != 11) {
        lcd.clear(); 
        lcd.print("CPF incompleto!"); 
        delay(1500);
        mostrarDado("CPF", campo);
      } else if (campo.length() == 11) {
          delay(750);
          lcd.clear();
          estadoAtual = CONFIRMANDO_CPF;
      }
    } else if (nomeCampo == "TELEFONE") {
      if (campo.length() != 11) {
        lcd.clear(); 
        lcd.print("Tel. incompleto!");
        lcd.setCursor(0,1);
        lcd.print("Digite com DDD");
        delay(1500);
        mostrarDado("TELEFONE", campo);
      } else if (campo.length() == 11) {
        delay(750);
        lcd.clear();
        estadoAtual = CONFIRMANDO_TELEFONE;
      }
    } 
  }
}

//Continuação da etapa 2
void mostrarDado(String nomeCampo, String valor) {
  lcd.clear();
  lcd.setCursor(0, 0);
  if (nomeCampo == "CPF") {
    lcd.print("Digite CPF:");
    } else {
      lcd.print("Digite telefone:");
    }
  lcd.setCursor(0, 1);
  lcd.print(valor);
}

// Etapa 3 - CONFIRMANDO_CPF
void confirmarCPF() {
  bool botaoSim = digitalRead(botao_verde);
  bool botaoNao = digitalRead(botao_vermelho);

  lcd.setCursor(0,0);
  lcd.print("Confirma CPF?");
  lcd.setCursor(0,1);
  lcd.print(cpf);

  if (botaoSim==LOW) {
    if (cpf==cpf_salvo){
      lcd.clear();
      estadoAtual = CPF_CADASTRADO;
      delay(300);
    } 
    else {
      cpf_salvo = cpf;
      estadoAtual = DIGITANDO_TELEFONE;
      lcd.clear();
      delay(300);
    }
  } else if (botaoNao == LOW) {
      cpf = "";                       
      estadoAtual = DIGITANDO_CPF;    
      lcd.clear();                     
      lcd.print("Digite CPF:");        
      lcd.setCursor(0,1);
      lcd.print(cpf);                  
      delay(300);                      
  }
}

//Continuação etapa 3 (CPF_CADASTRADO)
void CPF_cadastrado(){
  bool botaoSim = digitalRead(botao_verde);
  bool botaoNao = digitalRead(botao_vermelho);

  lcd.setCursor(0,0);
  lcd.print("Usar telefone?");
  lcd.setCursor(0,1);
  lcd.print(telefone_salvo);

  if (botaoSim==LOW){
    lcd.clear();
    estadoAtual = VALIDAR_SMS;
    delay(500);
  } else if(botaoNao==LOW){
    delay(1000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Novo telefone?");
    if(botaoSim==LOW){
      estadoAtual = DIGITANDO_TELEFONE;
      telefone = "";
      lcd.clear();
      delay(500);
    } else if(botaoNao==LOW){
      lcd.clear();
      estadoAtual=DIGITANDO_CPF;
      delay(500);
    }
  }
}

// Etapa 4 - Confirmando Telefone
void confirmarTelefone() {
  bool botaoSim = digitalRead(botao_verde);
  bool botaoNao = digitalRead(botao_vermelho);

  lcd.setCursor(0,0);
  lcd.print("Confirma Tel?");
  lcd.setCursor(0,1);
  lcd.print(telefone);

  if (botaoSim==LOW) {
    lcd.clear();
    telefone_salvo = telefone;
    estadoAtual = VALIDAR_SMS;
    delay(500);
  }

  if (botaoNao==LOW) {
    telefone = "";
    estadoAtual = DIGITANDO_TELEFONE;
    lcd.clear();
    lcd.print("Digite telefone:");
  }
}

// Etapa 5 - VALIDAR_SMS
void validar_sms(){
  static String codigo_digitado = "";
  bool botaoSim = digitalRead(botao_verde);
  bool botaoNao = digitalRead(botao_vermelho);  
  bool botaoLimpar = digitalRead(botao_amarelo);

  lcd.setCursor(0,0);
  lcd.print("Codigo SMS:");
  lcd.setCursor(0,1);
  lcd.print(codigo_digitado);

  if (tecla >= '0' && tecla <= '9') {
    int maxLen = (codigo_digitado) ? 5 : 15;
    if (codigo_digitado.length() < maxLen) 
      codigo_digitado += tecla;
    }
  if (botaoSim==LOW) {
      if (codigo_digitado == codigo_SMS) {
      codigo_digitado="";
      lcd.clear();
      lcd.print("Codigo OK");
      lcd.setCursor(0,1);
      lcd.print("Tel. confirmado");
      delay(1500);
      lcd.clear();
      estadoAtual = CONSULTAR_SALDO;
    }
    else {
      codigo_digitado="";
      lcd.clear();
      estadoAtual = ERRO_SMS;
      delay(500);
      }
    }
  if (botaoNao==LOW) {
    codigo_digitado = "";
    lcd.clear();
    delay(500);
    estadoAtual = DIGITANDO_TELEFONE;
  }
  if (botaoLimpar==LOW) {
    codigo_digitado = "";
    lcd.clear();
    lcd.print("Codigo SMS:");
    lcd.setCursor(0,1);
  }
}

// Continuação Etapa 5 - ERRO_SMS
void sms_invalido(){
  bool botaoSim = digitalRead(botao_verde);
  bool botaoNao = digitalRead(botao_vermelho);  

  lcd.print("Codigo invalido!");
    for(int i=0;i<3;i++){
    lcd.noBacklight();
    delay(500);
    lcd.backlight();
    delay(500);
  }
  delay(500);
  lcd.backlight();
  lcd.clear();
  estadoAtual = REENVIAR_SMS;
}

// Continuação Etapa 5 - REENVIAR_SMS
//precisa codar de acordo com a API
void reenviar_sms() {
  bool botaoSim = digitalRead(botao_verde);
  bool botaoNao = digitalRead(botao_vermelho);

  lcd.setCursor(0,0);
  lcd.print("Reenviar SMS?");

  //lcd.setCursor(0,1);
  //lcd.print("1. Repetir");

  if (botaoSim==LOW) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Reenviando SMS");
    lcd.setCursor(0,1);
    lcd.print("Aguarde...");
    delay(2000);
    lcd.clear();
    estadoAtual = VALIDAR_SMS;
    delay(300);
  } 
  else if (botaoNao==LOW) {
    lcd.clear();
    estadoAtual = DIGITANDO_CPF;
    delay(300);
  }
}

// Etapa 6 - CONSULTAR_SALDO
void consultarSaldo() {
  lcd.setCursor(0,0);
  lcd.print("Consultando seu");
  lcd.setCursor(0,1);
  lcd.print("saldo...");
  for(int i=0;i<5;i++){
    lcd.noBacklight();
    delay(500);
    lcd.backlight();
    delay(500);
  }
  lcd.clear();
  estadoAtual = MOSTRAR_SALDO;
}

// Continuação Etapa 6 - MOSTRAR_SALDO
void mostrarSaldo() {
  bool botaoSim = digitalRead(botao_verde);
  bool botaoNao = digitalRead(botao_vermelho);

  lcd.print("Saldo: R$ ");
  lcd.print(saldo);
  lcd.setCursor(0,1);
  lcd.print("Inserir credito?");

  if (botaoSim==LOW) {
    lcd.clear();
    estadoAtual = INSERIR_CREDITO;
  } else if (botaoNao==LOW) {
    estadoAtual = CARREGANDO;
  }
}

// Etapa 7 - INSERIR_CREDITO
void inserirCredito() {
  bool botaoSim = digitalRead(botao_verde);
  bool botaoNao = digitalRead(botao_vermelho);
  bool botaoLimpar = digitalRead(botao_amarelo);  

  static String valor = "";
  lcd.setCursor(0,0);
  lcd.print("Valor: ");
  lcd.print("R$ " + valor);  

  if (tecla >= '0' && tecla <= '9') {
    valor += tecla;
  } else if (botaoLimpar==LOW) {
    valor = "";
    lcd.clear();
    lcd.print("Valor: ");
  } else if (botaoNao==LOW) {
    estadoAtual = DIGITANDO_TELEFONE;   
  } else if (botaoSim==LOW && valor.length() > 0) {
    int v = valor.toInt();
    saldo += v;
    lcd.clear();
    delay(1000);
    lcd.setCursor(0,0);
    lcd.print("Adicionado");
    lcd.setCursor(0,1);
    lcd.print("R$ " + valor);
    delay(2000);
    valor = "";
    lcd.clear();
    estadoAtual = CARREGANDO;
  }
}

// Etapa 8 - CARREGANDO
void piscandoLEDverde() {
  lcd.clear();
  if(saldo < saldo_minimo){
    lcd.print(" Minimo R$ 20");
    lcd.setCursor(0,1);
    lcd.print(" para recarregar!");
    delay(2000);
    estadoAtual = MOSTRAR_SALDO;
    lcd.clear();
  } else {
    lcd.setCursor(0,0);
    lcd.print("Carregando...");

    int saldo_temp = saldo;
    for (int i=0; i < 8; i++) {
      saldo_temp -= 2;

      lcd.setCursor(0,1);
      lcd.print("                "); // limpa linha
      lcd.setCursor(0,1);
      lcd.print("Saldo: ");
      lcd.print(saldo_temp);

      digitalWrite(ledVerde, HIGH);
      delay(400);
      digitalWrite(ledVerde, LOW);
      delay(400);
    }
    saldo = saldo_temp;
    estadoAtual = FINALIZADO;
  }
}

void finalizado(){
  lcd.clear();
  lcd.backlight();
  lcd.print("Finalizado!");
  lcd.setCursor(0,1);
  lcd.print("Libere a vaga");
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledVermelho, HIGH);
  delay(5000);
  lcd.clear();
  estadoAtual = DISPONIVEL;
  mostrarDisponivel();
}