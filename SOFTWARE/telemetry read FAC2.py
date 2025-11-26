import serial
import serial.tools.list_ports
import sys
import time
import threading

# --- Configurazione del baud rate ---
baud_rate = 9600
intervallo_automatico = 0.5  # Intervallo di tempo per la richiesta automatica

# --- Valore da inviare per la richiesta di telemetria ---
valore_richiesta = 4

# --- Codici per la decodifica del tipo di batteria ---
codici_batteria = {
    0: "USB",
    1: "1S",
    2: "2S",
    3: "3S",
    4: "4S",
    5: "UNKNOWN"
}

# --- Codici per la decodifica dello stato della scheda ---
codici_stato_scheda = {
    0: "DISARMED",
    1: "NORMAL",
    2: "CUTOFF"
}

# --- Variabili per il controllo del thread automatico ---
stop_flag = threading.Event()

# --- Funzione per decodificare i dati ---
def decodifica_dati_telemetria(dati_raw):
    """
    Decodifica i dati seriali in base allo schema fornito,
    incluso il Telemetry RES code, i canali, la Vbat, il tipo di batteria,
    lo stato della scheda e l'accelerazione sugli assi X, Y, Z.
    """
    # La stringa di dati ora è più lunga a causa dell'aggiunta dei dati di accelerazione
    if len(dati_raw) < 27:
        print("Errore: la stringa di dati ricevuta non ha la lunghezza corretta.")
        return None

    dati = list(dati_raw)
    
    # Decodifica dei dati esistenti
    telemetry_res_code = dati[0]
    ch1 = (dati[1] << 8) | dati[2]
    ch2 = (dati[3] << 8) | dati[4]
    ch3 = (dati[5] << 8) | dati[6]
    ch4 = (dati[7] << 8) | dati[8]
    ch5 = (dati[9] << 8) | dati[10]
    ch6 = (dati[11] << 8) | dati[12]
    ch7 = (dati[13] << 8) | dati[14]
    ch8 = (dati[15] << 8) | dati[16]
    vbat_mv = (dati[17] << 8) | dati[18]
    battery_type = dati[19]
    stato_scheda_raw = dati[20]
    
    # Decodifica dei nuovi dati di accelerazione (big-endian)
    # I valori a 16 bit devono essere interpretati come interi con segno (signed)
    # poiché l'accelerazione può essere negativa.
    accel_x = (dati[21] << 8) | dati[22]
    if accel_x & 0x8000:
        accel_x -= 0x10000
        
    accel_y = (dati[23] << 8) | dati[24]
    if accel_y & 0x8000:
        accel_y -= 0x10000
        
    accel_z = (dati[25] << 8) | dati[26]
    if accel_z & 0x8000:
        accel_z -= 0x10000

    # Decodifica il tipo di batteria e lo stato della scheda
    tipo_batteria = codici_batteria.get(battery_type, "Codice Sconosciuto")
    stato_scheda = codici_stato_scheda.get(stato_scheda_raw, "Codice Sconosciuto")

    return {
        "Telemetry RES code": telemetry_res_code,
        "ch1": ch1,
        "ch2": ch2,
        "ch3": ch3,
        "ch4": ch4,
        "ch5": ch5,
        "ch6": ch6,
        "ch7": ch7,
        "ch8": ch8,
        "Vbat [mV]": vbat_mv,
        "Tipo Batteria": tipo_batteria,
        "Stato Scheda": stato_scheda,
        "Accel X [mg]": accel_x,
        "Accel Y [mg]": accel_y,
        "Accel Z [mg]": accel_z
    }

# --- Funzione per il thread di telemetria automatica ---
def telemetria_automatica(ser, intervallo):
    while not stop_flag.is_set():
        try:
            ser.write(bytes([valore_richiesta]))
            # La lunghezza dei dati da leggere è ora 27
            dati_ricevuti = ser.read(27)
            
            if dati_ricevuti:
                print("\n--- Dati Ricevuti (Automatico) ---")
                hex_string = ' '.join([f'{b:02X}' for b in dati_ricevuti])
                print(f"Raw (HEX): {hex_string}")
                
                dati_decodificati = decodifica_dati_telemetria(dati_ricevuti)
                if dati_decodificati:
                    print("\n--- Dati Decodificati (Automatico) ---")
                    for chiave, valore in dati_decodificati.items():
                        print(f"{chiave}: {valore}")
            else:
                print("Nessun dato ricevuto in risposta.")
            
            time.sleep(intervallo)
        except serial.SerialException as e:
            print(f"Errore di comunicazione seriale nel thread: {e}")
            break
        except Exception as e:
            print(f"Errore nel thread di telemetria: {e}")
            break

# --- Ciclo principale dello script ---
try:
    # Trova le porte seriali disponibili
    porte_disponibili = serial.tools.list_ports.comports()
    
    if not porte_disponibili:
        print("Nessuna porta seriale trovata. Assicurati che il tuo dispositivo sia collegato.")
        sys.exit()

    print("Porte seriali disponibili:")
    for i, p in enumerate(porte_disponibili):
        print(f"[{i+1}] {p.device} ({p.description})")

    scelta_str = input("Inserisci il numero della porta seriale desiderata (o 'q' per uscire): ")
    if scelta_str.lower() == 'q':
        sys.exit()
    
    try:
        scelta = int(scelta_str) - 1
    except ValueError:
        print("Input non valido. Inserisci un numero.")
        sys.exit()
    
    if 0 <= scelta < len(porte_disponibili):
        porta_selezionata = porte_disponibili[scelta].device
        
        ser = serial.Serial(porta_selezionata, baud_rate, timeout=1)
        print(f"\nConnesso a {ser.name}")
        print("\nComandi:")
        print("  - INVIO: richiedi la telemetria una volta")
        print("  - 'start': avvia la richiesta automatica ogni 0.5s")
        print("  - 'stop': ferma la richiesta automatica")
        print("  - 'q': esci dal programma")

        thread_telemetria = None
        
        while True:
            comando = input("> ").lower()
            
            if comando == 'q':
                print("\nUscita dal programma.")
                break
            
            elif comando == 'start':
                if thread_telemetria is None or not thread_telemetria.is_alive():
                    print("\nAvvio della richiesta automatica...")
                    stop_flag.clear()
                    thread_telemetria = threading.Thread(target=telemetria_automatica, args=(ser, intervallo_automatico))
                    thread_telemetria.daemon = True
                    thread_telemetria.start()
                else:
                    print("La richiesta automatica è già in esecuzione.")
            
            elif comando == 'stop':
                if thread_telemetria and thread_telemetria.is_alive():
                    print("\nArresto della richiesta automatica...")
                    stop_flag.set()
                    thread_telemetria.join()
                else:
                    print("La richiesta automatica non è in esecuzione.")

            else:
                print("\nInvio richiesta di telemetria...")
                ser.write(bytes([valore_richiesta]))
                time.sleep(0.1)
                
                dati_ricevuti = ser.read(27)
                
                if dati_ricevuti:
                    print("--- Dati Ricevuti ---")
                    hex_string = ' '.join([f'{b:02X}' for b in dati_ricevuti])
                    print(f"Raw (HEX): {hex_string}")
                    
                    dati_decodificati = decodifica_dati_telemetria(dati_ricevuti)
                    if dati_decodificati:
                        print("\n--- Dati Decodificati ---")
                        for chiave, valore in dati_decodificati.items():
                            print(f"{chiave}: {valore}")
                else:
                    print("Nessun dato ricevuto in risposta.")
    else:
        print("Scelta non valida. Uscita.")

except serial.SerialException as e:
    print(f"Errore di connessione seriale: {e}")
except Exception as e:
    print(f"Si è verificato un errore: {e}")
finally:
    if 'ser' in locals() and ser.is_open:
        ser.close()
        print("Connessione seriale chiusa.")