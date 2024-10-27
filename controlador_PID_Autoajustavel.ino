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
double Kp = 0.1, Ki = 0.0, Kd = 0.0;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

// Variáveis para auto-tuning
bool tuning = true;
unsigned long lastTime = 0;
const int PWM_BASE = 100;     // PWM base durante o auto-tuning
const int PWM_STEP = 50;      // Amplitude do relay
double rpmMax = 0;            // RPM máximo durante oscilação
double rpmMin = 9999;         // RPM mínimo durante oscilação
unsigned long periodStart = 0; // Tempo de início do período
int crossings = 0;            // Contagem de cruzamentos
double avgRPM = 0;            // RPM médio
bool relayHigh = true;        // Estado do relay
double Tu = 0;                // Período de oscilação
double Ku = 0;                // Ganho crítico

void setup() {
  Serial.begin(9600);
  
  pinMode(pino_D0, INPUT);
  pinMode(pino_pwm, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);
  
  pulsos = 0;
  rpm = 0;
  timeold = millis();
  
  // Configuração inicial do PID
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(0, 200);
  
  Serial.println("Sistema iniciado - Modo Auto-tuning");
}

void loop() {
  // Leitura do setpoint via potenciômetro
  int potValue = analogRead(pino_pot);
  Setpoint = map(potValue, 0, 1023, 0, 3000);
  
  // Leitura do encoder
  estadoAtual = digitalRead(pino_D0);
  if (estadoAtual == HIGH && estadoAnterior == LOW) {
    pulsos++;
  }
  estadoAnterior = estadoAtual;
  
  // Cálculo do RPM e controle
  if (millis() - timeold >= 200) {  // Atualiza a cada 200ms
    unsigned long tempoDecorrido = millis() - timeold;
    rpm = (60.0 * 1000.0 * pulsos) / (pulsos_por_volta * tempoDecorrido);
    timeold = millis();
    pulsos = 0;
    Input = rpm;
    
    if (tuning) {
      autotunePID();
    } else {
      myPID.Compute();
      analogWrite(pino_pwm, Output);
    }
    
    // Debug
    Serial.print("Setpoint: ");
    Serial.print(Setpoint);
    Serial.print(" RPM: ");
    Serial.print(rpm, 2);
    Serial.print(" PWM: ");
    Serial.print(Output);
    if (!tuning) {
      Serial.print(" Kp: ");
      Serial.print(Kp);
      Serial.print(" Ki: ");
      Serial.print(Ki);
      Serial.print(" Kd: ");
      Serial.print(Kd);
    }
    Serial.println();
  }
}

void autotunePID() {
  // Atualiza média móvel do RPM
  avgRPM = 0.7 * avgRPM + 0.3 * rpm;
  
  // Implementa relay feedback
  if (rpm > avgRPM && !relayHigh) {
    relayHigh = true;
    handleCrossing();
  } else if (rpm < avgRPM && relayHigh) {
    relayHigh = false;
    handleCrossing();
  }
  
  // Aplica PWM baseado no estado do relay
  int pwmValue = PWM_BASE + (relayHigh ? PWM_STEP : -PWM_STEP);
  pwmValue = constrain(pwmValue, 0, 200);
  analogWrite(pino_pwm, pwmValue);
  
  // Atualiza RPM máximo e mínimo
  rpmMax = max(rpmMax, rpm);
  rpmMin = min(rpmMin, rpm);
}

void handleCrossing() {
  crossings++;
  
  if (crossings == 1) {
    // Inicia medição do período
    periodStart = millis();
  } 
  else if (crossings >= 6) {  // 3 ciclos completos
    // Calcula período e amplitude
    Tu = (millis() - periodStart) / 3000.0;  // Converte para segundos
    double amplitude = (rpmMax - rpmMin) / 2;
    Ku = (4 * PWM_STEP) / (PI * amplitude);
    
    // Aplica regras de Ziegler-Nichols
    Kp = 0.6 * Ku;
    Ki = 1.2 * Ku / Tu;
    Kd = 0.075 * Ku * Tu;
    
    // Configura PID com novos valores
    myPID.SetTunings(Kp, Ki, Kd);
    tuning = false;
    
    Serial.println("\nAuto-tuning concluído!");
    Serial.print("Tu: "); Serial.print(Tu);
    Serial.print(" Ku: "); Serial.print(Ku);
    Serial.println("\nNovos parâmetros PID:");
    Serial.print("Kp: "); Serial.print(Kp);
    Serial.print(" Ki: "); Serial.print(Ki);
    Serial.print(" Kd: "); Serial.println(Kd);
  }
}