def processar_arquivos (input_file, geral_output_file):
    with open(input_file, 'r') as infile, open(geral_output_file, 'w') as geral_output:
        count = 0
        geral_output.write("Count, PWM, RPM\n")
        for line in infile:
            parts = line.split("->")
            if len(parts) == 2:
                _, values = parts[0].strip(), parts[1].strip()
                pwm, rpm = values.split()

                # pwm_outfile.write(f"{count}, {pwm}\n")
                # rpm_outfile.write(f"{count}, {rpm}\n")
                geral_output.write(f"{count}, {pwm}, {rpm}\n")
                count += 1
input_linear = 'linear.txt'
input_aleatorio = 'aleatorio.txt'

# processar_arquivos(input_linear, 'pwm_linear.txt', 'rpm_linear.txt')
# processar_arquivos(input_aleatorio, 'pwm_aleatorio.txt', 'rpm_aleatorio.txt')
processar_arquivos(input_linear, 'dados_gerais_linear')

print("Arquivos processados com sucesso!")
