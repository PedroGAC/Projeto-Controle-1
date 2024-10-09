def carregar_dados(arquivo):
    pwm = []
    rpm = []
    with open(arquivo, 'r') as f:
        for linha in f:
            valor = linha.split()
            pwm.append(valor[0])
            rpm.append(valor[1])
    return pwm, rpm

def salvar_dados(dados, arquivo_saida, repeticoes=1):
    with open(arquivo_saida, 'w') as f:
        for i, valor in enumerate(dados):
            f.write(f"{i} {valor}\n")

pwm_novo, rpm_novo = carregar_dados('nova_medicao.txt')

repeticoes = 10

salvar_dados(pwm_novo, 'pwm_novo.txt', repeticoes)
salvar_dados(rpm_novo, 'rpm_novo.txt', repeticoes)
