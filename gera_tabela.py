import re
import csv

log_path = "tests/logs/test_log_bb.txt"
csv_output = "desempenho_bb.csv"

# Regex atualizada para capturar os novos campos
regex = (
    r"Rodando teste: (\S+)\n"
    r"Valor obtido: (\d+)\n"
    r"Valor esperado: (\d+)\n"
    r"Tempo: ([\d\.]+) ms\n"
    r"Memória atual: ([\d\.]+) KB\n"
    r"Pico de memória: ([\d\.]+) KB"
)

rows = []

with open(log_path, "r", encoding="utf-8") as file:
    content = file.read()
    matches = re.findall(regex, content)

    for match in matches:
        instancia, valor_obtido, valor_esperado, tempo_ms, mem_atual_kb, mem_pico_kb = match
        tempo_s = float(tempo_ms) / 1000
        qualidade = "✔️" if int(valor_obtido) == int(valor_esperado) else "❌"

        rows.append({
            "Instância": instancia,
            "Valor Obtido": int(valor_obtido),
            "Valor Esperado": int(valor_esperado),
            "Qualidade da Solução": qualidade,
            "Tempo (ms)": float(tempo_ms),
            "Tempo (s)": round(tempo_s, 3),
            "Memória Atual (KB)": float(mem_atual_kb),
            "Pico de Memória (KB)": float(mem_pico_kb)
        })

with open(csv_output, "w", newline="", encoding="utf-8") as csvfile:
    writer = csv.DictWriter(csvfile, fieldnames=rows[0].keys())
    writer.writeheader()
    writer.writerows(rows)

print(f"✅ CSV gerado com {len(rows)} instâncias em '{csv_output}'")

