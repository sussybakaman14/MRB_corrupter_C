# Security Policy

## Supported Versions

Questo progetto è un Proof of Concept (PoC) statico a scopo educativo. Gli aggiornamenti di sicurezza vengono applicati esclusivamente all'ultima versione principale rilasciata per correggere comportamenti imprevisti o migliorare i meccanismi di contenimento.


| Version | Supported          |
| ------- | ------------------ |
| 1.0.x   | :white_check_mark: |
| < 1.0   | :x:                |

## Reporting a Vulnerability

### ⚠️ Nota Importante sul Software
Questo repository contiene codice strutturato intenzionalmente per essere distruttivo (Wiper simulation) all'interno di ambienti di test isolati. Di conseguenza:
* Il comportamento distruttivo del payload **non è considerato una vulnerabilità**, ma l'effetto previsto del codice.
* I tentativi di esecuzione al di fuori di una sandbox o su macchine reali sono a totale rischio dell'utente.

### Come segnalare una vulnerabilità reale
Se riscontri una vulnerabilità di sicurezza effettiva nel codice (ad esempio, un bug che permette l'esecuzione di codice remoto non intenzionale, un exploit nel parser delle risorse, o un bypass delle barriere di sicurezza preventive come la finestra di conferma), ti preghiamo di segnalarlo responsabilmente:

1. **Non aprire una Issue pubblica**: Per evitare l'esposizione di falle critiche, non pubblicare i dettagli del bug direttamente sul tracker del repository.
2. **Invia una segnalazione privata**: Utilizza la funzionalità **"Report a vulnerability"** direttamente nella scheda *Security* di questo repository GitHub, oppure contatta il maintainer all'indirizzo email specificato nel profilo.
3. **Cosa includere**: 
   * Una descrizione dettagliata del bug scoperto.
   * I passaggi precisi per riprodurre il problema (PoC).
   * L'impatto potenziale sull'ambiente di analisi (es. breakout dalla macchina virtuale).

### Tempi di risposta e processo
* **Triage**: Riceverai una conferma di ricezione entro **48 ore** dalla segnalazione.
* **Aggiornamenti**: Verrai informato sullo stato di avanzamento della correzione ogni **5 giorni lavorativi**.
* **Risoluzione**: Se la vulnerabilità verrà accettata, verrà sviluppata una patch e rilasciata una nuova sotto-versione. Il tuo contributo verrà pubblicamente accreditato nelle note di rilascio, salvo tua diversa richiesta.
