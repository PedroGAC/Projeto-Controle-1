import pandas as pd

def substituir_media(arquivo):
    df = pd.read_csv(arquivo, sep=",", header=0, names=["Count", "PWM", "RPM"])
    df["RPM"] = pd.to_numeric(df["RPM"], errors="coerce")
    media_rpm = df.groupby("PWM")["RPM"].transform("mean")
    df["RPM"] = media_rpm
    return df

def dividir_arquivo(arquivo):
    df = pd.read_csv(arquivo, sep=",", header=None, names=["Count", "PWM", "RPM"])


    df_pwm = df[["Count", "PWM"]]
    df_pwm.to_csv('count_pwm_media.txt', index=False, sep=",")
    
    df_rpm = df[["Count", "RPM"]]
    df_rpm.to_csv('count_rpm_media.txt', index=False, sep=",")

arquivo = 'saida_nova.csv'
# df_substituido = substituir_media(arquivo)

# df_substituido.to_csv('pwm_com_media_rpm.txt', index=False, sep=",")

dividir_arquivo('dados_gerais_linear.txt')
print("Arquivo processado com sucesso!")
