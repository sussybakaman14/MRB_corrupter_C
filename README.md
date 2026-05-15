# ⚠️ MBR Wiper & System Distorter Simulation

Un tool dimostrativo e di stampo educativo (Proof of Concept) scritto in C++ per l'analisi del comportamento dei malware di tipo **Wiper**. Il software simula la distruzione del Master Boot Record (MBR), la rimozione di file di configurazione di sistema e applica un payload grafico distorsivo a schermo intero.

## 🛑 DISCLAIMER IMPORTANTE
**QUESTO SOFTWARE È RILASCIATO ESCLUSIVAMENTE A SCOPO DIDATTICO E DI RICERCA.**  
L'esecuzione di questo programma su una macchina fisica reale **distruggerà l'avvio del sistema operativo** e causerà la perdita di dati stabili. 
* Non eseguire sul computer principale.
* Eseguire solo all'interno di ambienti di test dedicati (Sandbox) o Macchine Virtuali isolate.
* L'autore non si assume alcuna responsabilità per danni derivanti dall'uso improprio di questo codice.

---

## 🛠️ Caratteristiche del Payload

Il programma agisce attraverso diverse fasi sequenziali controllate dall'utente:

1. **Finestra di Consenso Privilegiata**: Un avviso interattivo (`MB_YESNO`) chiede conferma esplicita prima di procedere, impostando il comando di sicurezza `NO` come predefinito.
2. **MBR Overwrite**: Sovrascrive il settore 0 del disco primario (`\\\\.\\PhysicalDrive0`) con un buffer di 512 byte azzerati.
3. **Wiping dei File**: Tenta l'epurazione dei driver di terze parti in `System32\drivers` e dei font di Windows per causare il crash dei componenti grafici.
4. **Schermo Intero Persistente (`WS_EX_TOPMOST`)**: Crea un overlay grafico che blocca l'interfaccia utente standard.
5. **GDI Glitch Artifacts**: Sfrutta le API grafiche nativi di Windows (`BitBlt`, `PatBlt`) per generare un effetto visivo di inversione colore e "melting screen" (scioglimento dello schermo).
6. **Loop Audio**: Riproduzione asincrona continua di una risorsa sonora d'allarme (`PlaySoundA`).

---

## 💻 Requisiti di Compilazione

Per compilare il progetto è necessario un ambiente Windows con un compilatore C++ (C++11 o successivo):

* **IDE Consigliato**: Microsoft Visual Studio 2019 / 2022
* **SDK**: Windows Shortcuts & Win32 API
* **Librerie collegate**: `winmm.lib` (gestita automaticamente tramite `#pragma comment`)
* **Risorse**: Richiede un file `resource.h` e un file di risorse `.rc` contenente l'effetto audio associato all'ID `IDR_ALERT`.

### Configurazione dei Privilegi (UAC)
Per testare la reale efficacia della sovrascrittura del disco, l'eseguibile finale deve essere avviato come **Amministratore**. In Visual Studio è possibile forzare questa richiesta inserendo nel file manifesto del linker:
```text
Linker -> Manifest File -> UAC Execution Level -> requireAdministrator
```

---

## 🔍 Analisi di Sicurezza e Mitigazione Moderna

Nelle architetture hardware moderne (sistemi UEFI stabili con tabelle di partizione GPT e **Secure Boot** attivo), l'efficacia di questo specifico attacco MBR è mitigata:
* Il sistema non fa affidamento sul settore 0 per il boot manager.
* La protezione di integrità di Windows impedisce la modifica diretta del firmware di avvio senza bypassare i controlli del Kernel (ELAM / PatchGuard).
* La cancellazione dei file di sistema critici è parzialmente bloccata dalle restrizioni ACL legate all'utente speciale `TrustedInstaller`.

## 📄 Licenza

Questo progetto è rilasciato sotto Licenza MIT - Consulta il file [LICENSE](LICENSE) per ulteriori dettagli.
