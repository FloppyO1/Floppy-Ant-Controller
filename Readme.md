# Floppy Ant Controller V2 (F.A.C. or FAC)

## Introduzione

La FAC è una scheda di controllo per robot da combattimento, pensata come alternativa più avanzata della Malenki nano. 

La FAC permette di avere un grado di libertà e personalizzazione molto avanzato. Questo permette di essere impiegata come scheda di controllo per una vastissima quantità di robot da combattimento.

Con l'avanzamento delle versioni del firmware verranno aggiunte nuove funzionalità che integrano anche l'utilizzo del accelerometro/giroscopio in tegrato. Essendo un progetto opensource ogniuno può modificarlo e produrlo in autonomia. 

> Per la pubblicazione di contenuti ricavati da questo progetto guardare le condizioni dettate dalla LICENCE.

## Specifiche

* 3 uscite per motori DC:
  
  * `1.8A max`
  
  * `1000 step per direzione`
  
  * `frequenza modificabile (100->10'000 Hz)`

* 2 uscite servo: 
  
  * `HV (alimentazione dalla batteria)`
  
  * `1000 step`
  
  * `larghezza dell'impulso modificabile`
  
  * `frequenza modificabile`

* 2 ingressi digitali:
  
  * `3.3V max`

* 1 ingresso analogico:
  
  * `3.3V max`
  
  * `connessioni facili per potenziometri`

* Versione base supporto batterie 1-2S lipo (limitata dai driver DC):
  
  * `soglia CUT-OFF impostabile`
  
  * `tempo di CUT-OFF impostabile`

* LED di stato ad alta luminosità

* Compatibile con diversi ricevitori:
  
  * `PPM fino ad 8 canali`
  
  * `PWM fino a 4 canali`
  
  * `E-LRS (non ancora implementato, ma predisposto)`

* Impostazioni modificabili facilmente tramite tool dedicato https://factool.floppylab.it/ con cavo USB-C
  
  * Tool utilizzabile anche offline, salvandolo come applicazione
  
  * Nessuna installazione necessaria

* Dimensioni e peso contenuti:
  
  * `21mm * 31mm`
  
  * `2.4g without receiver`, `3.6g with FS2A`

## Nuove funzionalità

Con questa nuova versione della Floppy Ant Controller sono stati revisitati sia l'hardware che il firmware completamente. Questo ha permesso di inserire nuove funzionalità molto interessanti, la più significativa è la presenza di una IMU (Inertial Mesurement Unit), ovvero un chip con all'interno giroscopio/accelerometro. Questo da la possibilità di implementare algoritmi personalizzati che rendono più avanzato il controllo del vostro robot. Per esempio è già in sviluppo un mix che inverte in automatico il comando dello sterzo per quei robot che una volta sotto sopra possono comunque muoversi.

## Impostazioni BASE e AVANZATE

La FAC ha implementate delle impostazioni base che si trovano comunemente in un po' tutti i controller, ma ha anche altre funzionalità molto più avanzate per poter rendere più fine l'impostazione del proprio controller in base ai componenti utilizzati.

Le impostazioni sono state suddivise in quattro categorie, presenti anche nel FAC Tool:

1. Generali (General)

2. Mix e Funzioni speciali (Mix & SP. Functions)

3. Servo e Motori (Servos & Motors)

Qui sotto vengono spiegate più nel dettaglio. Nel FAC Tool in fianco ad ogni impostazione si trova un simbolo 🛈 dove si trova una descrizone della singola impostazione.

### Generali

Le impostazioni generali sono quelle che permettono di far funzionare correttamente la FAC, quindi quelle che con una nuova configurazione vanno sistemate per prime.

> Battery
> 
> Low battery voltage: è la tensione della batteria alla quale viene considerata scarica

## MIX e FUNZIONI SPECIALI

Oltre a tutte le impostazioni base e avanzate della FAC tra le quali:

* `gestione delle soglie della batteria`

* `scelta del canale dell'arming o disattivazione di esso`

* `scelta della percentuale di deadzone degli stick del radiocomando`

* `scelta del tipo di ricevitore utilizzato`

* `inversione, cambio frequenza e freno dei motori DC`

* `inversione, cambio frequenza e larghezza dell'impulso delle uscite servo`

utili per poter impostare correttamente il comportamento della FAC in base alla propria configurazione.
