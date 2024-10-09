#Aqui estou combinando de forma "Aleatória" os dados lineares e aleatórios.

import random

def carregar_dados(arquivo):
    dados = []
    with open(arquivo, 'r') as f:
        for linha in f:
            _, valor = linha.strip().split(',')
            dados.append(valor)
    return dados

def salvar_dados(dados, arquivo_saida):
    with open(arquivo_saida, 'w') as f:
        for i, valor in enumerate(dados):
            f.write(f"{i}, {valor}\n")

pwm_linear = carregar_dados('pwm_linear.txt')
rpm_linear = carregar_dados('rpm_linear.txt')

pwm_aleatorio = carregar_dados('pwm_aleatorio.txt')
rpm_aleatorio = carregar_dados('rpm_aleatorio.txt')

dados_combinados_pwm = pwm_linear + pwm_aleatorio
dados_combinados_rpm = rpm_linear + rpm_aleatorio

random.shuffle(dados_combinados_pwm)
random.shuffle(dados_combinados_rpm)

salvar_dados(dados_combinados_pwm, 'pwm_combinado.txt')
salvar_dados(dados_combinados_rpm, 'rpm_combinado.txt')