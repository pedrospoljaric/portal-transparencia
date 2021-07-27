import numpy as np
import matplotlib.pyplot as plt

import sys
lsArgs = sys.argv

#..::Transferencias::..#############################################################################################################
for i in range(1, 4):
	
	data_file = open(lsArgs[i], 'r')
	data_lines = data_file.readlines()
	data_file.close()

	date = lsArgs[i][0:6]

	lsUfsNorte = ['AM', 'RR', 'AP', 'PA', 'TO', 'RO', 'AC']
	lsUfsNordeste = ['MA', 'PI', 'CE', 'RN', 'PE', 'PB', 'SE', 'AL', 'BA']
	lsUfsCentroOeste = ['MT', 'MS', 'GO']
	lsUfsSudeste = ['SP', 'RJ', 'ES', 'MG']
	lsUfsSul = ['PR', 'RS', 'SC']
	dicRegioes = {'Norte':0, 'Nordeste':0, 'Centro-Oeste':0, 'Sudeste':0, 'Sul':0, 'Outros':0} #Valor total por regiao

	lsUfs = []
	lsValores = []
	for line in data_lines: #Somatiza os valores por regiao e separa as informacoes do arquivo em 2 listas

		siglaUf = line.split(' ')[0]
		valorUf = line.split(' ')[1]
		lsUfs.append(siglaUf)
		lsValores.append(float(valorUf)/1000000)
		
		if (siglaUf in lsUfsNorte): dicRegioes['Norte'] += float(valorUf)/1000000
		elif (siglaUf in lsUfsNordeste): dicRegioes['Nordeste'] += float(valorUf)/1000000
		elif (siglaUf in lsUfsCentroOeste): dicRegioes['Centro-Oeste'] += float(valorUf)/1000000
		elif (siglaUf in lsUfsSudeste): dicRegioes['Sudeste'] += float(valorUf)/1000000
		elif (siglaUf in lsUfsSul): dicRegioes['Sul'] += float(valorUf)/1000000
		else: dicRegioes['Outros'] += float(valorUf)/1000000

	total = dicRegioes['Norte'] + dicRegioes['Nordeste'] + dicRegioes['Centro-Oeste'] + dicRegioes['Sudeste'] + dicRegioes['Sul'] + dicRegioes['Outros']

	#.....Barplot.....#

	fig = plt.figure(figsize=(10,6)) #Figura
	barra = fig.add_subplot(111) #Grafico

	ind = np.arange(len(lsUfs)) #Espacamento entre estados
	width = 0.4 #Largura da barra
	
	t = int(lsValores[-1])
	t = int(t/1000)
	t = t + 1
	t = t * 1000 #Limite superior do valor (teto em escala de milhares)

	barra.set_xlim(-2*width,len(ind)) #limites laterais
	barra.set_ylim(0, t) #limites inferior e superior
	barra.set_ylabel('Valor em milhoes de reais') #texto lateral
	plt.title('Transferencias por estado (' + date[4:6] + '/' + date[0:4] + ')', y=1.075, x=0.815, bbox={'facecolor':'0.8', 'pad':5}) #texto superior
	barra.set_xticks(ind) #espacamento entre barras
	xtickNames = barra.set_xticklabels(lsUfs) #unidades inferiores (estados)
	plt.setp(xtickNames, rotation=90, fontsize=10) #insercao dos textos inferiores em vertical
	rects1 = barra.bar(ind, lsValores, width, color="navy") #insercao das barras
	barra.set_position([0.1, 0.1, 0.85, 0.75]) #[left, bottom, width, height] tamanho e posicao do grafico
	plt.savefig(('Bar-' + lsArgs[i]).replace('txt', 'png'), dpi=300) #salva grafico em imagem png

	#.....Pieplot.....#

	srtDic = dicRegioes.items()
	srtDic = sorted(srtDic, key=lambda x: x[1], reverse = True) #tuples ordenados por valor
	
	lbls = srtDic[0][0], srtDic[1][0], srtDic[2][0], srtDic[3][0], srtDic[4][0], srtDic[5][0] #label = primeira informacao do tuple
	sizes = [srtDic[0][1]/total, srtDic[1][1]/total, srtDic[2][1]/total, srtDic[3][1]/total, srtDic[4][1]/total, srtDic[5][1]/total] #tamanhos dos arcos = porcentagem dos valores por regiao
	expl = (0.1, 0, 0, 0, 0, 0) #Parte com maior valor destacada

	fig1, pizza = plt.subplots()
	pizza.pie(sizes, explode=expl, labels=lbls, autopct='%1.1f%%', pctdistance=0.8) #autopct = exibir porcentagens, pctdistance = distancia das porcentagens em relacao ao centro
	pizza.axis('equal') #Grafico centralizado

	plt.title('Transferencias por regiao (' + date[4:6] + '/' + date[0:4] + ')', y=1.075, x=0.808, bbox={'facecolor':'0.8', 'pad':5}) #texto superior
	plt.savefig(('Pie-' + lsArgs[i]).replace('txt', 'png'), dpi=300) #salva grafico em imagem png

#..::Diarias::..#############################################################################################################

for i in range(4, 7):

	data_file = open(lsArgs[i], 'r')
	data_lines = data_file.readlines()
	data_file.close()

	date = lsArgs[i][0:6]

	lsMinisterios = []
	lsVal = []
	for line in data_lines: #Separa as informacoes do arquivo em 2 listas
	
		minis = ' '.join(line.split(' ')[2:])
		minis = ' '.join(minis.split(' ')[:-1])
		minis = ''.join([j if ord(j) < 128 else 'A' for j in minis]) #substitui caracter especial por A
		lsMinisterios.append(minis) #Ministerio sem as 2 primeiras palavras (Ex.: ministerio da saude -> saude)
		lsVal.append(float(line.split(' ')[-1])/1000)

	#.....Barplot.....#
	
	fig = plt.figure(figsize=(10,6)) #Figura
	barra2 = fig.add_subplot(111) #Grafico

	ind = np.arange(len(lsMinisterios))
	width = 0.4
	
	t = int(lsVal[-1])
	t = int(t/1000)
	t = t + 1
	t = t * 1000 #Limite da direita do valor (teto em escala de milhares)
	
	barra2.set_xlim(0, t) #limites laterais
	barra2.set_ylim(-2*width,len(ind)) #limites superior e inferior
	barra2.set_xlabel('Valor em milhares de reais') #texto inferior
	barra2.set_ylabel('Ministerio') #texto lateral
	plt.title('Diarias pagas por ministerio (' + date[4:6] + '/' + date[0:4] + ')', y=1.075, x=0.75, bbox={'facecolor':'0.8', 'pad':5}) #titulo
	barra2.set_yticks(ind) #espacamento entre unidades laterais
	ytickNames = barra2.set_yticklabels(lsMinisterios) #unidades laterais
	plt.setp(ytickNames, rotation=0, fontsize=8) #insercao das unidades laterais
	rects1 = barra2.barh(ind, lsVal, width, color="navy") #insercao das barras
	barra2.invert_yaxis() #valores passam a crescer de cima para baixo
	barra2.set_position([0.28, 0.1, 0.67, 0.75]) # [left, bottom, width, height] tamanho e posicao do grafico
	plt.savefig(('Bar-' + lsArgs[i]).replace('txt', 'png'), dpi=300) #salva grafico em imagem png
