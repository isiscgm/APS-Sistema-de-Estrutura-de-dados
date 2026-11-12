import random
from datetime import datetime, timedelta
from pathlib import Path

random.seed(42)
quantidades = [1000, 5000, 10000, 50000, 100000]
inicio = datetime(2020, 1, 1)

for n in quantidades:
    valores = [inicio + timedelta(seconds=i) for i in random.sample(range(n * 3), n)]
    random.shuffle(valores)
    nome = Path(f"timestamps_{n}.txt")
    nome.write_text("".join(v.strftime("%Y-%m-%d %H:%M:%S") + "\n" for v in valores), encoding="utf-8")
    print(f"Gerado: {nome} ({n} registros)")
