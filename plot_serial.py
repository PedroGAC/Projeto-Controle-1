import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time

# Configuração da porta serial
serial_port = '/dev/ttyUSB0'  # Substitua pelo nome da porta correta (ex.: 'COM3' no Windows)
baud_rate = 9600

# Inicializa a conexão serial
ser = serial.Serial(serial_port, baud_rate)

# Listas para armazenar os dados
time_data = []
setpoint_data = []
rpm_data = []
pwm_data = []
pwm_base_data = []
output_data = []
error_percent_data = []  # Lista para armazenar o erro percentual

# Configuração do gráfico
fig, (ax1, ax2, ax3) = plt.subplots(3, 1, figsize=(10, 15))
fig.suptitle("Controle de Velocidade do Motor")

# Gráfico superior: Setpoint e RPM
ax1.set_ylabel("RPM")
line_setpoint, = ax1.plot([], [], label="Setpoint", color="blue")
line_rpm, = ax1.plot([], [], label="RPM Medido", color="red")
ax1.legend()

# Gráfico do meio: PWM, PWM Base e Output PID
ax2.set_ylabel("Valores de Controle")
line_pwm, = ax2.plot([], [], label="PWM", color="green")
line_pwm_base, = ax2.plot([], [], label="PWM Base", color="orange")
line_output, = ax2.plot([], [], label="Output PID", color="purple")
ax2.legend()

# Gráfico inferior: Erro Percentual
ax3.set_xlabel("Tempo (s)")
ax3.set_ylabel("Erro (%)")
line_error, = ax3.plot([], [], label="Erro Percentual", color="brown")  # Linha para o erro percentual
ax3.legend()

# Tempo inicial
start_time = time.time()

def parse_data(line):
    parts = line.split()
    data = {}
    i = 0
    while i < len(parts):
        if parts[i].endswith(':'):
            key = parts[i].strip(':')
            if i + 1 < len(parts):
                try:
                    value = float(parts[i + 1])
                    # Ajusta a chave "Base" para "PWM Base"
                    if key == "Base":
                        key = "PWM Base"
                    data[key] = value
                    i += 2
                except ValueError:
                    i += 2
            else:
                i += 1
        else:
            i += 1
    return data

def update_graph(frame):
    if ser.in_waiting > 0:
        try:
            line = ser.readline().decode('utf-8').strip()
            
            # Ignora se a linha estiver vazia
            if not line:
                return
            
            data = parse_data(line)
            
            # Confirma se todos os dados necessários foram recebidos
            if all(key in data for key in ['Setpoint', 'RPM', 'PWM', 'PWM Base', 'Output']):
                # Atualiza o tempo com base no tempo real desde o início
                current_time = time.time() - start_time
                time_data.append(current_time)
                
                # Atualiza as listas de dados
                setpoint_data.append(data['Setpoint'])
                rpm_data.append(data['RPM'])
                pwm_data.append(data['PWM'])
                pwm_base_data.append(data['PWM Base'])
                output_data.append(data['Output'])
                
                # Calcula e armazena o erro percentual
                if data['Setpoint'] != 0:  # Evita divisão por zero
                    error_percent = ((data['Setpoint'] - data['RPM']) / data['Setpoint']) * 100
                else:
                    error_percent = 0
                error_percent_data.append(error_percent)

                # Limita o tamanho dos dados para manter o gráfico atualizado
                max_points = 100
                time_data[:] = time_data[-max_points:]
                setpoint_data[:] = setpoint_data[-max_points:]
                rpm_data[:] = rpm_data[-max_points:]
                pwm_data[:] = pwm_data[-max_points:]
                pwm_base_data[:] = pwm_base_data[-max_points:]
                output_data[:] = output_data[-max_points:]
                error_percent_data[:] = error_percent_data[-max_points:]

                # Atualiza os dados dos gráficos
                line_setpoint.set_data(time_data, setpoint_data)
                line_rpm.set_data(time_data, rpm_data)
                line_pwm.set_data(time_data, pwm_data)
                line_pwm_base.set_data(time_data, pwm_base_data)
                line_output.set_data(time_data, output_data)
                line_error.set_data(time_data, error_percent_data)

                # Ajusta os limites dos eixos
                ax1.relim()
                ax1.autoscale_view()
                ax2.relim()
                ax2.autoscale_view()
                ax3.relim()
                ax3.autoscale_view()
            else:
                # Ignora dados incompletos sem imprimir a mensagem
                pass

        except Exception as e:
            print(f"Erro ao processar dados: {e}")

# Animação para atualizar o gráfico em tempo real
ani = animation.FuncAnimation(fig, update_graph, interval=100, cache_frame_data=False)
plt.tight_layout()
plt.show()

# Fechar a porta serial ao finalizar o programa
ser.close()
