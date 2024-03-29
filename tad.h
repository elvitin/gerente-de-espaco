typedef struct arquivo
{
	char nome[11];
	int tamanho;
	int tamanhoFragmento;
	struct arquivo *prox;
} Arq;

typedef struct disco
{
	int celulaCapacidade;
	int espacoLivre;
	struct arquivo *arq;
	struct disco *prox;

} Celula;

Celula *criaSetor()
{
	Celula *novaCelula = (Celula *)malloc(sizeof(Celula));
	novaCelula->celulaCapacidade = novaCelula->espacoLivre = 0;
	novaCelula->arq = NULL;
	novaCelula->prox = NULL;
	return novaCelula;
}

void criaDisco(struct disco **cabeca)
{
	*cabeca = NULL;
	Celula *nova;
	int i;
	for (i = 0; i < 8; i++)
	{
		nova = criaSetor();
		nova->prox = *cabeca;
		*cabeca = nova;
	}
}

void converteKb(int *tam, char un)
{
	*tam *= un != 'K' ? 1024 * (un == 'G' ? 1024 : 1) : 1;

	//Outras conversoes
	/*
	 * tam = (un == 'M') ? (*tam) * 1024 : *tam;
	 * tam = (un == 'G') ? (*tam) * 1024 * 1024 : *tam;
	 * tam *= un == 'M' ? 1024 : (un == 'G' ? 1024 * 1024 : 1);
	 */
}

Arq *criaArq(const char *nome, int tam, int tamFrag, Arq *prox)
{
	Arq *novoArq = (Arq *)malloc(sizeof(Arq));
	strcpy(novoArq->nome, nome);
	novoArq->tamanho = tam;
	novoArq->tamanhoFragmento = tamFrag;
	novoArq->prox = prox;
	return novoArq;
}

void redefineCelulas(struct disco *hd, int tam, char un)
{
	converteKb(&tam, un);
	tam /= 8;

	while (hd != NULL)
	{
		hd->celulaCapacidade = hd->espacoLivre = tam;
		hd->arq = NULL;
		hd = hd->prox;
	}
}

bool estaLivre(struct disco *hd, int tam)
{
	int espLivre = 0;
	while (hd != NULL && espLivre < tam)
	{
		espLivre += hd->espacoLivre;
		hd = hd->prox;
	}
	return espLivre >= tam;
}
/*
bool temEspSeq(struct disco *hd, int tam, Celula **setor, Arq **arqCabeca)
{
	int espacoSeq = 0;

	*setor = hd;
	*arqCabeca = hd->arq;

	while (hd != NULL && espacoSeq < tam)
	{

		if (hd->arq == NULL)
			espacoSeq += hd->celulaCapacidade;
		else
		{
			espacoSeq += hd->espacoLivre;

			if (espacoSeq < tam)
			{
				espacoSeq = hd->espacoLivre;

				if (hd->espacoLivre == 0 && hd->prox != NULL)
				{
					*setor = hd->prox;
					*arqCabeca = hd->prox->arq;
				}
				else
				{
					*setor = hd;
					*arqCabeca = hd->arq;
					while ((*arqCabeca)->prox != NULL)
						*arqCabeca = (*arqCabeca)->prox;
				}
			}
		}


		hd = hd->prox;
	}
	return espacoSeq >= tam;
}
*/

bool temEspSeq(struct disco *hd, int tam, Celula **setor, Arq **arqCabeca)
{
	int espacoSeq = 0;

	*setor = hd;
	*arqCabeca = hd->arq;

	while (hd != NULL && espacoSeq < tam)
	{
		espacoSeq += hd->espacoLivre;

		if (hd->arq != NULL)
		{
			espacoSeq = hd->espacoLivre;

			if (espacoSeq < tam)
			{
				espacoSeq = hd->espacoLivre;

				if (hd->espacoLivre == 0 && hd->prox != NULL)
				{
					*setor = hd->prox;
					*arqCabeca = hd->prox->arq;
				}
				else
				{
					*setor = hd;
					*arqCabeca = hd->arq;

					while ((*arqCabeca)->prox != NULL)
						*arqCabeca = (*arqCabeca)->prox;
				}
			}
		}
		hd = hd->prox;
	}
	return espacoSeq >= tam;
}

//refatorar
bool insere(struct disco *hd, char *nomeArq, int tam, char un)
{
	converteKb(&tam, un);

	if (estaLivre(hd, tam))
	{
		Arq *arqInsercao = NULL;
		Celula *celuInsercao = NULL;
		//											101							202
		if (temEspSeq(hd, tam, &celuInsercao, &arqInsercao))
		{
			int tempFrag;
			int auxTamFrag = tam;

			while (auxTamFrag > 0)
			{
				tempFrag = auxTamFrag;
				auxTamFrag -= celuInsercao->espacoLivre;

				if (auxTamFrag <= 0)
				{
					//Arq *criaArq(const char *nome, int tam, int tamFrag, Arq *prox)
					if (celuInsercao->arq == NULL)
						celuInsercao->arq = criaArq(nomeArq, tam, tempFrag, arqInsercao);
					else
						arqInsercao->prox = criaArq(nomeArq, tam, tempFrag, NULL);

					celuInsercao->espacoLivre -= tempFrag;
				}
				else
				{
					if (celuInsercao->arq == NULL)
						celuInsercao->arq = criaArq(nomeArq, tam, celuInsercao->espacoLivre, arqInsercao);
					else
						arqInsercao->prox = criaArq(nomeArq, tam, celuInsercao->espacoLivre, NULL);

					celuInsercao->espacoLivre = 0;
					celuInsercao = celuInsercao->prox;
					arqInsercao = celuInsercao->arq;
				}
			}
			return true;
		}
	}
	return false;
}

void propriedades(struct disco *hd, char *buffstring)
{
	*buffstring = '\0';
	double result;

	while (hd != NULL)
	{
		result = (double)hd->espacoLivre / hd->celulaCapacidade;

		if (result <= 0.25)
			strcat(buffstring, "[#]");
		else if (result <= 0.75)
			strcat(buffstring, "[-]");
		else
			strcat(buffstring, "[ ]");

		hd = hd->prox;
	}
}

void deleta(struct disco *hd, char *nomeArq)
{
	Arq *auxArq = NULL;
	Arq *auxArqAnt = NULL;

	while (hd != NULL)
	{
		auxArq = hd->arq;
		while (auxArq != NULL)
		{
			if (!strcmp(auxArq->nome, nomeArq))
			{
				hd->espacoLivre = hd->espacoLivre + auxArq->tamanhoFragmento;

				if (auxArq == hd->arq)
				{
					hd->arq = hd->arq->prox;
					free(auxArq);
					auxArq = hd->arq;
				}
				else
				{
					auxArqAnt->prox = auxArqAnt->prox->prox;
					free(auxArq);
					auxArq = auxArqAnt->prox;
				}
			}
			else
			{
				auxArqAnt = auxArq;
				auxArq = auxArq->prox;
			}
		}
		hd = hd->prox;
	}
}

void formataCelula(Arq **arqui)
{
	if (*arqui != NULL)
	{
		formataCelula(&(*arqui)->prox);
		free(*arqui);
		*arqui = NULL;
	}
}

void formataDisco(struct disco *hd)
{
	while (hd != NULL)
	{
		hd->celulaCapacidade = hd->espacoLivre = 0;
		formataCelula(&(hd)->arq);
		hd = hd->prox;
	}
}

bool estaFragmentado(struct disco *hd)
{
	while (false /* condition */)
	{
		/* code */
		puts("pass");
	}
	return true;
}

void otimiza(struct disco *hd)
{
	if (estaFragmentado(hd))
	{
		Arq *tempArq;
		Celula *tempCell = hd;
		int tamCelula = hd->celulaCapacidade;

		descritor lista;
		init(&lista);

		while (tempCell != NULL)
		{
			tempArq = tempCell->arq;
			while (tempArq != NULL)
			{
				if (!buscaInfo(lista, tempArq->nome))
					insereApos(&lista, tempArq->nome, tempArq->tamanho);
				tempArq = tempArq->prox;
			}
			tempCell = tempCell->prox;
		}

		formataDisco(hd);
		redefineCelulas(hd, tamCelula * 8, 'K');

		while (!estaVazio(lista))
		{
			insere(hd, lista.primeiroNodo->nome, lista.primeiroNodo->tamanho, 'K');
			removeNodo(&lista, lista.primeiroNodo->nome);
		}
	}
}

void destroiDisco(Celula **hd)
{
	if (*hd != NULL)
	{
		destroiDisco(&(*hd)->prox);
		free(*hd);
		*hd = NULL;
	}
}

void bonus(struct disco *hd)
{
	Arq *aux;
	Celula *hdAux = hd;
	int i = 1;
	int j;
	while (hdAux != NULL)
	{
		printf("\nCelula[%d] Capacidade: %dKB\n", i++, hdAux->celulaCapacidade);
		printf("espacoLivre: %dKB\n", hdAux->espacoLivre);

		aux = hdAux->arq;
		j = 1;
		while (aux != NULL)
		{
			printf("		[%d] nome: %s\n", j++, aux->nome);
			printf("		tam: %dKB\n", aux->tamanho);
			printf("		tamFrag: %dKB\n", aux->tamanhoFragmento);
			aux = aux->prox;
		}
		hdAux = hdAux->prox;
	}
	getc(stdin);
}