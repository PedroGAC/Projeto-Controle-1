#include <PID_v1.h>

// Pinos
const int pino_D0 = 2;
const int pino_pwm = 10;
const int pino_pot = A0;

// Variáveis do encoder
float rpm;
unsigned long timeold;
const unsigned int pulsos_por_volta = 9;
volatile byte pulsos;
int estadoAtual;
int estadoAnterior = LOW;

// Variáveis do PID
double Setpoint, Input, Output;
const double Kp = 0.025;
const double Ki = 0.01;
const double Kd = 0.015;

// Variáveis de controle do PWM
int lastPwm = 0;
double pwmBase = 0;  // PWM base dinâmico

// Instância do PID
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void setup() {
  Serial.begin(9600);
  
  pinMode(pino_D0, INPUT);
  pinMode(pino_pwm, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);
  
  pulsos = 0;
  rpm = 0;
  timeold = millis();
  
  // Configuração do PID
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-100, 100);  // Range maior para o PID
  
  Serial.println("Sistema iniciado");
}

void loop() {
  // Leitura do setpoint
  int potValue = analogRead(pino_pot);
  Setpoint = map(potValue, 0, 1023, 0, 3000);
  
  // Leitura do encoder
  estadoAtual = digitalRead(pino_D0);
  if (estadoAtual == HIGH && estadoAnterior == LOW) {
    pulsos++;
  }
  estadoAnterior = estadoAtual;
  
  // Cálculo do RPM e controle
  if (millis() - timeold >= 200) {
    unsigned long tempoDecorrido = millis() - timeold;
    rpm = (60.0 * 1000.0 * pulsos) / (pulsos_por_volta * tempoDecorrido);
    timeold = millis();
    pulsos = 0;
    
    // Ajuste do PWM base baseado no setpoint
    pwmBase = map(Setpoint, 0, 3000, 20, 180);
    
    // Atualização do PID
    Input = rpm;
    myPID.Compute();
    
    // Cálculo do novo PWM
    int targetPwm = pwmBase + Output;
    targetPwm = constrain(targetPwm, 0, 255);  // Expandido para usar todo o range do PWM
    
    // Suavização da mudança do PWM
    if (abs(targetPwm - lastPwm) > 5) {
      if (targetPwm > lastPwm) {
        lastPwm += 5;
      } else {
        lastPwm -= 5;
      }
    } else {
      lastPwm = targetPwm;
    }
    
    // Aplicação do PWM
    analogWrite(pino_pwm, lastPwm);
    
    // Debug
    Serial.print("Setpoint: ");
    Serial.print(Setpoint);
    Serial.print(" RPM: ");
    Serial.print(rpm, 2);
    Serial.print(" PWM: ");
    Serial.print(lastPwm);
    Serial.print(" PWM Base: ");
    Serial.print(pwmBase);
    Serial.print(" Output: ");
    Serial.println(Output);
  }
}