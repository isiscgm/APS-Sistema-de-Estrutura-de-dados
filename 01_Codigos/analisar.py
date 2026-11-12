"""Analisa medicoes reais do benchmark; requer pandas, numpy, matplotlib."""
import csv
from pathlib import Path
import numpy as np
import pandas as pd
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

BASE = Path(__file__).resolve().parent
raw = pd.read_csv(BASE/'medicoes_brutas.csv')
raw = raw.dropna(subset=['ciclos_estimados'])
if raw.duplicated(['estrutura','N','operacao','repeticao']).any():
    raise SystemExit('ERRO: repeticoes duplicadas no CSV')
ns = [1000,2500,5000,7500,10000,15000,20000,25000,30000,40000,50000,60000,70000,80000,100000]
estruturas = ['Lista', 'AVL', 'ListaDupla']
ops = ['Insercao', 'Remocao', 'Listagem']
# Nao apresentar resultados incompletos como se fossem experimentos concluidos.
expected = pd.MultiIndex.from_product([estruturas, ns, ops], names=['estrutura','N','operacao'])
counts = raw.groupby(['estrutura','N','operacao']).size().reindex(expected, fill_value=0)
if not (counts == 10).all():
    missing = counts[counts != 10]
    raise SystemExit('Benchmark incompleto: cada combinacao precisa de 10 medicoes. Pendencias:\n'+str(missing))
summary = raw.groupby(['estrutura','N','operacao'], as_index=False)['ciclos_estimados'].agg(mediana='median', minimo='min', maximo='max')
summary.to_csv(BASE/'resumo_estatistico.csv', index=False, float_format='%.6f')
wide = summary.pivot(index=['estrutura','N'],columns='operacao',values='mediana').reset_index()
wide['custo_ponderado'] = .70*wide['Insercao']+.15*wide['Remocao']+.15*wide['Listagem']
wide.to_csv(BASE/'custo_ponderado.csv', index=False,float_format='%.6f')

for op, titulo, nome in [
    ('Insercao','Inserção','grafico_1_insercao.png'),
    ('Remocao','Remoção','grafico_2_remocao.png'),
    ('Listagem','Listagem ordenada','grafico_3_listagem.png'),
    ('custo_ponderado','Custo ponderado total','grafico_4_custo_ponderado.png')]:
    fig, axes = plt.subplots(1,2,figsize=(15,5.5))
    for e in estruturas:
        d = (wide[wide.estrutura==e] if op=='custo_ponderado' else summary[(summary.estrutura==e)&(summary.operacao==op)]).sort_values('N')
        y = d['custo_ponderado'] if op=='custo_ponderado' else d['mediana']
        for ax in axes:
            ax.plot(d.N,y,marker='o',markersize=3,linewidth=1.6,label=e)
    axes[0].set_title('Escala linear — valores absolutos')
    axes[1].set_title('Escala logarítmica — destaca valores pequenos')
    axes[1].set_yscale('symlog',linthresh=1e5)
    for ax in axes:
        ax.set_xlabel('Quantidade de registros (N)')
        ax.set_ylabel('Ciclos estimados (mediana)' if op!='custo_ponderado' else 'Ciclos estimados (custo ponderado)')
        ax.grid(alpha=.25);ax.legend()
    fig.suptitle(titulo+' — comparação das estruturas (15 pontos)')
    fig.tight_layout()
    fig.savefig(BASE/nome,dpi=180);plt.close(fig)

fits=[]
for e in estruturas:
    for op in ops+['custo_ponderado']:
        d = (wide[wide.estrutura==e] if op=='custo_ponderado' else summary[(summary.estrutura==e)&(summary.operacao==op)]).sort_values('N')
        y = (d['custo_ponderado'] if op=='custo_ponderado' else d['mediana']).to_numpy(float)
        x = d.N.to_numpy(float)
        # Medicoes nulas nao admitem logaritmo; excluir apenas do ajuste,
        # sem substituir, inventar ou alterar os dados experimentais.
        valid = np.isfinite(y) & (y > 0)
        n_valid = int(valid.sum())
        if n_valid < 3:
            fits.append([e,op,np.nan,np.nan,np.nan,n_valid,'Pontos positivos insuficientes']); continue
        lx,ly=np.log(x[valid]),np.log(y[valid])
        b,log_a=np.polyfit(lx,ly,1)
        pred=log_a+b*lx
        denom=np.sum((ly-ly.mean())**2)
        r2=1-np.sum((ly-pred)**2)/denom if denom>0 else np.nan
        fits.append([e,op,float(np.exp(log_a)),float(b),float(r2),n_valid,'Ajuste log-log com exclusao explicita de valores zero'])
pd.DataFrame(fits,columns=['estrutura','operacao','coeficiente_a','expoente_b','R2_log','pontos_utilizados','metodo']).to_csv(BASE/'ajustes_potencia.csv',index=False,float_format='%.8f')
print('ATENCAO: ciclos zero por operacao:',raw.loc[raw.ciclos_estimados<=0].groupby(['estrutura','operacao']).size().to_dict())
print('Criados: resumo_estatistico.csv, custo_ponderado.csv, ajustes_potencia.csv e 4 graficos PNG')

# Diagnostico por combinacao, para documentar ruido e valores nulos.
diag=raw.groupby(['estrutura','N','operacao'],as_index=False).agg(
    repeticoes=('ciclos_estimados','size'),
    zeros=('ciclos_estimados',lambda v:int((v<=0).sum())),
    media=('ciclos_estimados','mean'),
    desvio_padrao=('ciclos_estimados','std'))
diag.to_csv(BASE/'diagnostico_medicoes.csv',index=False,float_format='%.6f')
print('Diagnostico criado: diagnostico_medicoes.csv; zeros nao foram substituidos.')
