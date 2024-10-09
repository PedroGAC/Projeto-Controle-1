% % Carregar os dados de PWM (Entrada) e RPM (Saída)
% pwm_data = load('count_pwm.txt');  % Substitua com o caminho correto para o arquivo
% rpm_data = load('count_rpm.txt');  % Substitua com o caminho correto para o arquivo

% % Separar os dados em tempo e valores
% tempo_pwm = pwm_data(:, 1);  % A primeira coluna é o tempo
% valores_pwm = pwm_data(:, 2);  % A segunda coluna é o valor de PWM

% tempo_rpm = rpm_data(:, 1);  % A primeira coluna é o tempo
% valores_rpm = rpm_data(:, 2);  % A segunda coluna é o valor de RPM

% % Criar um objeto de dados de identificação de sistemas (iddata)
% % O tempo de amostragem é inferido a partir dos dados, mas você pode definir explicitamente
% data = iddata(valores_rpm, valores_pwm, 1);

% % Abrir o aplicativo 'ident' para trabalhar com a identificação de sistemas
% ident

% Carregar os dados de PWM (Entrada) e RPM (Saída)
pwm_data = load('pwm_novo.txt');  % Substitua com o caminho correto para o arquivo
rpm_data = load('rpm_novo.txt');  % Substitua com o caminho correto para o arquivo

% Separar os dados em tempo e valores
tempo_pwm = pwm_data(:, 1);  % A primeira coluna é o tempo
valores_pwm = pwm_data(:, 2);  % A segunda coluna é o valor de PWM

tempo_rpm = rpm_data(:, 1);  % A primeira coluna é o tempo
valores_rpm = rpm_data(:, 2);  % A segunda coluna é o valor de RPM

% Criar um objeto de dados de identificação de sistemas (iddata)
% O tempo de amostragem é inferido a partir dos dados, mas você pode definir explicitamente
data = iddata(valores_rpm, valores_pwm, 1);

% Abrir o aplicativo 'ident' para trabalhar com a identificação de sistemas
ident
