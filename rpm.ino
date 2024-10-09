int pino_D0 = 2;
int pino_pwm = 10;
float rpm;
unsigned long timeold;
unsigned int pulsos_por_volta = 9;  // Verifique se o número de pulsos por volta está correto
volatile byte pulsos;
int estadoAtual;
int estadoAnterior = LOW;
int pwmValue = 0;                // Valor inicial do PWM
unsigned long lastPwmChange = 0;  // Variável para controlar o tempo da última mudança do PWM

void setup() {
  Serial.begin(9600);
  pinMode(pino_D0, INPUT);
  pinMode(pino_pwm, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);
  pulsos = 0;
  rpm = 0;
  timeold = millis();               // Inicializa o tempo
  analogWrite(pino_pwm, pwmValue);  // Define o PWM inicial
}

void loop() {
  estadoAtual = digitalRead(pino_D0);
  // Detecta transição de LOW para HIGH (bordas de subida)
  if (estadoAtual == HIGH && estadoAnterior == LOW) {
    pulsos++;  // Incrementa a contagem de pulsos
  }
  estadoAnterior = estadoAtual;

  if (millis() - timeold >= 1000) {  // Calcular RPM a cada 1 segundo
    unsigned long tempoDecorrido = millis() - timeold;
    rpm = (60.0 * 1000.0 * pulsos) / (pulsos_por_volta * tempoDecorrido);  // Cálculo de RPM
    timeold = millis();                                                    // Atualiza o tempo
    pulsos = 0;                                                            // Reseta o contador de pulsos

    // Imprimir PWM e RPM no formato solicitado
    Serial.print(pwmValue);
    Serial.print(" ");
    Serial.println(rpm, 2);  // Exibir RPM com 2 casas decimais
  }
  if(pwmValue < 256){
      // Verifica se passaram 3 segundos desde a última mudança do PWM
    if (millis() - lastPwmChange >= 3000) {
      // Altera o valor do PWM
      // pwmValue = random(40, 256);  // Gera um valor aleatório entre 40 e 255
      pwmValue++;
      analogWrite(pino_pwm, pwmValue);
      lastPwmChange = millis();  // Atualiza o tempo da última mudança
    }
  }
}