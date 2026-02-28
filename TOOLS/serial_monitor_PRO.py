import tkinter as tk
from tkinter import ttk, scrolledtext
import serial
import serial.tools.list_ports
import threading
import time

class SerialMonitorApp:
    def __init__(self, root):
        self.root = root
        self.root.geometry("800x600")

        self.serial_port = None
        self.receiving_data = False

        self.setup_ui()
        self.update_ports()
        self.start_port_updater()

    def setup_ui(self):
        # Frame superiore per i controlli
        control_frame = ttk.Frame(self.root, padding="10")
        control_frame.pack(fill=tk.X)

        # Selezione porta COM
        ttk.Label(control_frame, text="Porta COM:").pack(side=tk.LEFT, padx=5)
        self.port_var = tk.StringVar()
        self.port_menu = ttk.Combobox(control_frame, textvariable=self.port_var, state="readonly")
        self.port_menu.pack(side=tk.LEFT, padx=5)

        # Pulsanti Connetti/Disconnetti
        self.connect_btn = ttk.Button(control_frame, text="Connetti", command=self.connect_serial)
        self.connect_btn.pack(side=tk.LEFT, padx=5)
        
        self.disconnect_btn = ttk.Button(control_frame, text="Disconnetti", command=self.disconnect_serial, state=tk.DISABLED)
        self.disconnect_btn.pack(side=tk.LEFT, padx=5)
        
        # Pulsante per aggiornare le porte
        self.refresh_btn = ttk.Button(control_frame, text="Aggiorna Porte", command=self.update_ports)
        self.refresh_btn.pack(side=tk.LEFT, padx=5)

        # Frame per i dati ricevuti e il formato di visualizzazione
        receive_frame = ttk.Frame(self.root, padding="10")
        receive_frame.pack(fill=tk.BOTH, expand=True)

        receive_control_frame = ttk.Frame(receive_frame)
        receive_control_frame.pack(fill=tk.X, pady=(0, 5))
        ttk.Label(receive_control_frame, text="Dati Ricevuti:").pack(side=tk.LEFT, anchor=tk.W)

        # Selettore formato di visualizzazione RX
        receive_format_frame = ttk.Frame(receive_control_frame)
        receive_format_frame.pack(side=tk.RIGHT)
        ttk.Label(receive_format_frame, text="Formato RX:").pack(side=tk.LEFT)
        self.receive_format_var = tk.StringVar(value="Tutti")
        ttk.Radiobutton(receive_format_frame, text="Tutti", variable=self.receive_format_var, value="Tutti").pack(side=tk.LEFT, padx=5)
        ttk.Radiobutton(receive_format_frame, text="Testo", variable=self.receive_format_var, value="Testo").pack(side=tk.LEFT, padx=5)
        ttk.Radiobutton(receive_format_frame, text="Hex", variable=self.receive_format_var, value="Hex").pack(side=tk.LEFT, padx=5)
        ttk.Radiobutton(receive_format_frame, text="Dec", variable=self.receive_format_var, value="Dec").pack(side=tk.LEFT, padx=5)

        self.received_text = scrolledtext.ScrolledText(receive_frame, wrap=tk.WORD, height=20, state="disabled")
        self.received_text.pack(fill=tk.BOTH, expand=True)

        # Frame inferiore per l'invio dei dati
        send_frame = ttk.Frame(self.root, padding="10")
        send_frame.pack(fill=tk.X)

        ttk.Label(send_frame, text="Dati da Inviare:").pack(anchor=tk.W)
        self.send_entry = ttk.Entry(send_frame)
        self.send_entry.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=(0, 5))

        # Pulsante per inviare i dati
        self.send_btn = ttk.Button(send_frame, text="Invia", command=self.send_data)
        self.send_btn.pack(side=tk.LEFT)

        # Opzioni di formato TX
        format_frame = ttk.Frame(send_frame)
        format_frame.pack(side=tk.LEFT, padx=(10, 0))

        self.format_var = tk.StringVar(value="Testo")
        ttk.Radiobutton(format_frame, text="Testo", variable=self.format_var, value="Testo").pack(anchor=tk.W)
        ttk.Radiobutton(format_frame, text="Hex", variable=self.format_var, value="Hex").pack(anchor=tk.W)
        ttk.Radiobutton(format_frame, text="Dec", variable=self.format_var, value="Dec").pack(anchor=tk.W)
        
        # Pulsante per cancellare l'output
        clear_btn = ttk.Button(self.root, text="Cancella Output", command=self.clear_output)
        clear_btn.pack(pady=5)
        
    def update_ports(self):
        ports = [port.device for port in serial.tools.list_ports.comports()]
        self.port_menu['values'] = ports
        if ports:
            self.port_var.set(ports[0])
        else:
            self.port_var.set("")

    def start_port_updater(self):
        def updater():
            while True:
                time.sleep(5)
                current_ports = [port.device for port in serial.tools.list_ports.comports()]
                if set(current_ports) != set(self.port_menu['values']):
                    self.root.after(0, self.update_ports)
        threading.Thread(target=updater, daemon=True).start()

    def connect_serial(self):
        port = self.port_var.get()
        if not port:
            self.append_to_log("[ERRORE] Seleziona una porta COM.", "red")
            return

        try:
            self.serial_port = serial.Serial(port, 9600, timeout=1)
            self.append_to_log(f"[INFO] Connesso alla porta {port}", "green")
            self.connect_btn['state'] = tk.DISABLED
            self.disconnect_btn['state'] = tk.NORMAL
            self.receiving_data = True
            self.receive_thread = threading.Thread(target=self.receive_data, daemon=True)
            self.receive_thread.start()
        except serial.SerialException as e:
            self.append_to_log(f"[ERRORE] Impossibile connettersi: {e}", "red")

    def disconnect_serial(self):
        if self.serial_port and self.serial_port.is_open:
            self.receiving_data = False
            self.serial_port.close()
            self.append_to_log("[INFO] Disconnesso dalla porta seriale.", "blue")
            self.connect_btn['state'] = tk.NORMAL
            self.disconnect_btn['state'] = tk.DISABLED

    def receive_data(self):
        while self.receiving_data:
            if self.serial_port and self.serial_port.in_waiting > 0:
                try:
                    data = self.serial_port.read_all()
                    self.root.after(0, self.display_data, data, "[RX]")
                except serial.SerialException as e:
                    self.root.after(0, self.append_to_log, f"[ERRORE] Errore di lettura: {e}", "red")
                    self.root.after(0, self.disconnect_serial)
                    break
            time.sleep(0.1)

    def send_data(self):
        if not self.serial_port or not self.serial_port.is_open:
            self.append_to_log("[ERRORE] Non sei connesso a una porta.", "red")
            return
            
        data_to_send = self.send_entry.get()
        if not data_to_send:
            return

        send_format = self.format_var.get()
        sent_bytes = b''
        try:
            if send_format == "Testo":
                sent_bytes = data_to_send.encode('utf-8')
            elif send_format == "Hex":
                hex_values = data_to_send.split()
                sent_bytes = bytes.fromhex("".join(hex_values))
            elif send_format == "Dec":
                dec_values = [int(val) for val in data_to_send.split()]
                sent_bytes = bytes(dec_values)
            
            self.serial_port.write(sent_bytes)
            self.append_to_log(f"[TX] Dati inviati", "blue")
            self.display_data(sent_bytes, "[TX]")
        except ValueError as e:
            self.append_to_log(f"[ERRORE] Formato dati non valido: {e}", "red")
        except serial.SerialException as e:
            self.append_to_log(f"[ERRORE] Errore di scrittura: {e}", "red")
            self.disconnect_serial()

    def display_data(self, data, direction):
        selected_format = self.receive_format_var.get() if direction == "[RX]" else self.format_var.get()
        log_message = ""
        
        # Formato Testo
        if selected_format in ["Tutti", "Testo"]:
            try:
                text_data = data.decode('utf-8', errors='replace')
                log_message += f"{direction} (Testo): {text_data}\n"
            except:
                log_message += f"{direction} (Testo): Dati non decodificabili\n"

        # Formato Esadecimale
        if selected_format in ["Tutti", "Hex"]:
            hex_data = ' '.join(f'{b:02X}' for b in data)
            log_message += f"{direction} (Hex): {hex_data}\n"

        # Formato Decimale
        if selected_format in ["Tutti", "Dec"]:
            dec_data = ' '.join(str(b) for b in data)
            log_message += f"{direction} (Dec): {dec_data}\n"

        if log_message:
            self.append_to_log(log_message.strip(), "black")

    def append_to_log(self, message, color="black"):
        self.received_text.config(state="normal")
        self.received_text.insert(tk.END, message + "\n", color)
        self.received_text.config(state="disabled")
        self.received_text.see(tk.END)
        self.received_text.tag_config("red", foreground="red")
        self.received_text.tag_config("green", foreground="green")
        self.received_text.tag_config("blue", foreground="blue")

    def clear_output(self):
        self.received_text.config(state="normal")
        self.received_text.delete(1.0, tk.END)
        self.received_text.config(state="disabled")

def main():
    root = tk.Tk()
    root.title("SerialMonitorPro")  # Nome finestra
    # root.iconphoto(True, tk.PhotoImage(file="/home/filippo-castellan/Documenti/icona.png"))
    app = SerialMonitorApp(root)
    root.mainloop()

if __name__ == "__main__":
    main()