# Projeto de Controle de Velocidade do Motor CC

Este projeto implementa um sistema de controle para motor CC com Arduino, utilizando abordagens de modulação PWM para ajuste de velocidade e coleta de dados. A análise de dados é realizada em Python e MATLAB para identificação do sistema e obtenção da função de transferência.

## Estrutura do Projeto

1. **Aquisição de Dados com Arduino**  
   - O código `Arduino.ino` coleta dados de PWM e RPM em malha aberta com variação linear e aleatória do PWM.

2. **Tratamento e Análise dos Dados**  
   - Scripts em Python para combinar e processar dados, incluindo substituição pela média e divisão em arquivos (`carregar_dados`, `salvar_dados`, `substituir_media`, `dividir_arquivo`).
   - Arquivos de dados gerados: `pwm_com_media_rpm.txt`, `count_pwm_media.txt`, `count_rpm_media.txt`.

3. **Identificação de Sistemas em MATLAB**  
   - Código MATLAB para análise de identificação e modelagem do sistema usando o `iddata` e `ident`.

## Componentes do Sistema

- Motor CC
- Ponte H para controle de direção e intensidade da corrente
- Encoder para medição de rotação
- Arduino Uno para processamento dos dados e controle PWM

## Visualização e Análise de Dados

As abordagens de variação linear e aleatória de PWM são visualizadas com gráficos em Python, exibindo a resposta do motor para diferentes sinais de entrada.

## Requisitos

- **Python**: pandas, matplotlib
- **MATLAB**: para processamento de dados de identificação de sistemas
- **Arduino IDE**: para o código de controle em malha aberta do motor CC

## Referência

Baseado no relatório do projeto desenvolvido no curso de Controle I da Universidade Federal da Paraíba.
