# Unstable Students
Gioco strano creato come progetto per Programmazione 1, che adesso mi sono messo in testa di voler portare su PS Vita. Più dettagli su come portare programmi in C su PS Vita [qui](https://gist.github.com/DionKill/2a802080d64b42beb867e351f9de8196).

## Rant
Questo gioco è stato, a dirla tutta, un parto assoluto. Iniziato il 27 dicembre 2025, l'ho finito e consegnato il 3 maggio alle 23:45! È stato il progetto più difficile che abbia fatto fin'ora, di gran lunga più lungo di quelli delle superiori, e il motivo è che la documentazione era assolutamente OSCENA. Mai visto un PDF con delle regole più fatte male di queste. Era davvero così brutto che, nel codice, ci sono riferimenti ai loro errori grammaticali (es. "Disessere Giocati"). Quindi se qualcuno li dovesse mai leggere, ecco il motivo..
Il PDF vorrei caricarlo qui perché fa pisciare, ma son sicuro che mi beccherei una denuncia per diffamazione. Quindi farò finta di niente.

Le commit poi sono una perla, più impegno messo in quelle che nel resto del progetto lmao

Ah e in più ho fatto un casino con le commit, perché ho cancellato il README originale che aveva il mio nome da tutte le commit, ma non di questo progetto, dal progetto che ho consegnato! Per fortuna il voto ormai è caricato ma spero non mi dicano mai nulla, è stato giusto uno sbaglio...

Ad ogni modo, qua sotto c'è la descrizione del progetto.
TLDR: è un clone del gioco di carte Unstable Unicorns, ma fatto col culo e qualche bug, da giocare interamente da riga di comando... Insomma una noia mortale.

---

## Descrizione del progetto originale

> ### **Table of Content**
>  1. [Spiegazione file sorgente](#spiegazione-file-sorgente)
>  1. [Descrizione e scopo strutture aggiuntive](#descrizione-e-scopo-strutture-aggiuntive)
>  1. [Descrizione flusso di gioco](#descrizione-flusso-di-gioco)
>  1. [Descrizione logica AI](#descrizione-logica-ai)
>      - [Scelta carte](#scelta-carte)
>      - [Strategia di gioco](#strategia-di-gioco)



```
REPOSITORY STRUCTURE
·
│
│ GAME SAVES
├── saves                       //cartella contenente i salvataggi
│   ├── savegame.sav            // Salvataggio
│   ├── savegame.sav.log        // Log del salvataggio
│   └── ···
│
│ SOURCE FILES                  //relazione e documentazione
├── main.c                      // Avvia il programma
├── strutture.c                 // Contiene tutte le strutture e la loro gestione
├── strutture.h
│── file.c                      // Gestione dei file di caricamento, salvataggio, e log
├── file.h
│── gioco.c                     // Gestione della logica del gioco, es. giocare una carta
├── gioco.h
│── grafica.c                   // Gestione grafica, printf colorate e ASCII art
├── grafica.h
│── misc.c                      // Miscellanee, es. cursore
├── misc.h
│
│ OTHER FILES
├── README.md                   //relazione e documentazione
└── .gitignore                  //file da ignorare
```




## Spiegazione file sorgente
> [!TIP]
> Per ogni file sorgente (.c/.h) presente nel progetto, spiegare brevemente il contenuto e lo scopo del file.
### Main.c
Il file main.c è il primo eseguibile del programma e l'unica cosa che fa è chiamare le funzioni necessarie ad avviare il gioco.
### Strutture.c/.h
Questi due file contengono tutte le definizioni e implementazioni delle funzioni, procedure, enum, struct e define che vengono usate nel progetto. Le funzioni e procedure contengono la creazione dei giocatori, la gestione delle carte (cioè shuffle, copia, sposta...) e altro che ha sempre a che vedere con la gestione delle strutture definite nell'header.
### File.c/.h
Questi file contengono tutta la gestione dei file, compresa la lettura delle carte dal file `mazzo.txt`, e del salvataggio e caricamente delle partite.
### Gioco.c/.h
Servono per la gestione del gioco appunto, quindi della gestione dei menù, del pescare le carte, vedere le carte, giocare le carte e la gestione dei turni.
### Grafica.c/.h e colori-ansi.h
Serve per la gestione grafica: qui ci sono solo ed esclusivamente procedure che stampano o modificano delle stringhe di testo per poter essere stampate dalle `printf()`. C'è un forte uso dei colori dall'header `colori-ansi.h`.
### Misc.c/.h
Questi due esistono solo ed esclusivamente per evitare di fare le cosiddette "inclusioni circolari", o perché contengono funzioni che non avevano un contesto preciso.


## Descrizione e scopo strutture aggiuntive
> [!TIP]
> Descrivere le strutture dati aggiuntive utilizzate nel progetto e perché vi sono tornate utili (se presenti).

Il progetto non include strutture aggiuntive.


## Descrizione flusso di gioco
> [!TIP]
> Descrivere ad alto livello come vengono gestite le varie fasi di gioco (es. il loop principale, la gestione degli eventi, ecc.)

### Prefazione
Questo gioco di carte ha regole molto complesse, ma in sostanza:
- Ogni carta ha effetti differenti che alterano ogni partita
	- Azioni: in sostanza, le mosse che la carta permette di fare
		- Le azioni hanno target e tipi
	- Target: i giocatori a cui verranno applicate le Azioni
	- Tipo: il tipo della carta, ad esempio MAGIA, BONUS, o LAUREANDO...
- Il giocatore ha più mazzi:
	- Il mazzo delle carte da giocare (dove vengono messe quelle pescate)
		- invisibile agli altri (a meno che... non ci sia una carta anche per questo...)
	- Il mazzo delle carte *Bonus/Malus*
		- Visibile agli altri
	- Il mazzo delle carte di tipo STUDENTE (*Matricole*, *Studenti Semplici* e *Laureandi*)
		- Visibile agli altri

Anche il banco ha i suoi mazzi:
- Mazzo Pesca
- Mazzo Scarti
- Mazzo Aula Studio (quello dove vanno le matricole quando vengono scartate)

Il gioco è difficile da maestrare... E onestamente anche da imparare. C'è bisogno di ricontrollare più volte le carte da giocare per essere sicuri di quel che si vuole fare.
### Avvio
Il gioco funziona interamente da terminale, quindi è anche possibile passargli dei parametri di avvio, come i salvataggi.
Vengono creati i puntatori a liste dei giocatori, e dei mazzi di ognuno, e i mazzi di **pesca**, **scarto**, e l'**Aula Studio** dove andranno le matricole.

### Gestione dei salvataggi
Vengono come prima cosa gestiti i salvataggi. Si prova prima con il caricamento, se non possibile, si esce o si crea un nuovo salvataggio:

#### Caricamento di una partita
Prima di caricare, vengono fatti diversi controlli:
- Se il salvataggio passato da terminale esiste, viene caricato
- Se il programma viene caricato senza parametri, si prova con il salvataggio normale denominato "savegame.sav" nella cartella "saves"
- Il file di salvataggio, contiene le grandezze di ogni lista, quella dei giocatori, come quella dei loro mazzi e di quelli del banco. Vengono caricati uno ad uno, e aggiunti alla lista allocandone lo spazio in memoria

#### Creazione di una partita
Se non esiste, stampa un messaggio, e si inizia la creazione di un nuovo file di salvataggio:
- In questo caso, al giocatore, viene chiesto quanti giocatori vuole, e il nome di ognuno di essi
- Dal file **mazzo.txt**, vengono lette e caricate tutte le carte del gioco, e vengono messe in una lista una dopo l'altra
	- Vengono poi divise in due mazzi, *Mazzo della Pesca* e *Mazzo delle Matricole*
	- Questi due mazzi vengono poi mischiati
		- Vengono poi date ai giocatori, una carta a un giocatore, poi la prossima all'altra, finché tutti non hanno 5 carte in mano e una matricola nella loro *Aula Studio*
		- Ogni giocatore può avere SOLO E SOLO UNA carta *Matricola*. Più dettagli dopo. 
- Inizia poi il gioco, dal primo giocatore inserito nella lista, con il turno 1

### Ciclo principale
Il ciclo principale del gioco incomincia da qui. Il gioco infatti fa un primo salvataggio, che verrà fatto poi all'inizio di ogni turno, dopo aver pescato la prima carta e dopo aver gestito le carte che hanno effetti all'inizio del turno (quando INIZIO).
Dopo, il giocatore avrà davanti il menù della fase azione.
Durante le scelte, il gioco ha una funzione che gestisce gli effetti della carta, e applica gli effetti secondo tutti i criteri, alcune volte però, se ad esempio il mazzo scelto è vuoto, non verrà chiesto al giocatore se vuole tornare indietro. In questo caso... Colpa del giocatore che non ha controllato bene le proprie carte e quelle visibili altrui!

### Fase azione
Viene visualizzato il conteggio dei turni e il nome del giocatore (ognuno ha il suo colore!).
Il gioco, dopo essere iniziato, parte dal primo giocatore in lista, che sia l'ultimo che ha giocato dal caricamento o il primo inserito dopo aver creato il salvataggio.
1. Viene pescata subito una carta
2. Se il giocatore ha carte che hanno effetti che si applicano all'inizio del turno, vengono gestiti

Viene quindi presentato il menù di scelta al giocatore:
1. Gioca una carta (se ne ha)
	1. Può annullare la decisione
2. Pesca una carta
3. Controlla le proprie carte
4. Controlla le carte degli altri
5. Esce (tasto 0)

#### Giocare una carta
Questa è la parte più importante di tutto il gioco, ovvero effettivamente farne qualcosa delle carte in mano.
Il giocatore può giocare una carta (se ne ha). Il programma gestisce autonomamente gli effetti delle carte, quello che deve fare il giocatore è scegliere la migliore strategia per vincere.
Si vince quando si hanno ben 6 carte **studente** nel proprio mazzo. Queste possono essere sia *Matricole* (ma ne può avere solo una, più dettagli dopo), che *Studenti Semplici*, che *Laureandi*.
- N.B.: c'è solo una carta *Matricola* per giocatore. Questo significa che se viene ELIMINATA dal mazzo, sarà persa per sempre, e non c'è modo di recuperarla. Se viene RUBATA, è possibile recuperarla, ma solo rubandola al giocatore che ha rubato per primo.

Le carte giocate vengono spostate in un mazzo specifico quando vengono giocate, ad esempio, le carte giocate di tipo MALUS vengono messe nel mazzo *Bonus/Malus* di un giocatore scelto (anche se stessi!), le carte BONUS vengono messe nel mazzo apposito, etc...

Se le carte non ci sono, si ritorna al menù precedente, e si è forzati a pescare una carta per continuare.

Il giocatore avrà la possibilità di giocare una carta con effetto BLOCCA se viene targettato da una carta, oppure potrebbe giocare una MALUS che impedisce di giocare carte di un certo tipo, come le carte BONUS ad esempio.
##### Gli effetti
Ci sono diversi effetti, forse anche troppi, e vengono combinati per creare carte che sono tutte diverse. Permettono di rubare, eliminare, o perfino bloccare la giocata o addirittura di vincere agli altri giocatori.

È possibile:
- GIOCARE un'altra carta
- PESCARE un'altra carta
- SCARTARE/ELIMINARE una carta da uno dei propri mazzi (o da qualcun'altro)
- RUBARE/PRENDERE una carta dal mazzo degli altri giocatori
- SCAMBIARE la propria mano con quella di un'altro giocatore
- BLOCCARE/IMPEDIRE di usare una carta o tipi di carte
- MOSTRARE il mazzo di un altro giocatore (finché l'effetto è attivo)
- INGEGNERIZZARE gli studenti di Informatica, così che non possano vincere

A tutto ciò, si aggiunge il fatto che alcune carte hanno queste sopracitate *Azioni* **solo in certi momenti**: possono essere all'inizio del turno, quando la carta esce dall'aula...
Si aggiunge anche che le carte hanno **target specifici**, ciò significa che alcune carte affliggono solo un giocatore, tutti o uno scelto dal giocante. Alcune carte possono anche chiedere conferma al giocatore per poter applicare i propri effetti.
#### Pescare una carta
Semplicissimo: il giocatore prende la carta in cima al mazzo da pesca, e la mette nel proprio mazzo delle carte giocabili.
#### Mostrare i propri mazzi e quelli degli altri
È ovviamente possibile controllare le proprie carte in mano, quelle sul tavolo, cioè ad esempio i Mazzi *Bonus/Malus* e *Aula Studio*. Questo sarà possibile solo se il gioco trova come carta MALUS una che ha l'effetto che può far mostrare il mazzo del giocatore (ovviamente è possibile vedere le proprie carte giocabili indipendentemente da questo tipo di carta)

### Fine del turno
La fine del turno avviene quando viene giocata o pescata una carta durante la *Fase Azione*.
Se il turno finisce, si fanno i controlli della vittoria: il giocatore vince quando ha 6 o più carte STUDENTE nel *Mazzo Aula Studio*, e non ha nessuna MALUS di tipologia INGEGNERE. Se questo fosse il caso, viene congratulato il giocatore per la vittoria e il programma terminato.


## Descrizione logica AI
> [!NOTE]
> Questa sezione va compilata solo se si sta svolgendo il progetto AVANZATO.

### Scelta carte...
...
### Strategia di gioco...
...
