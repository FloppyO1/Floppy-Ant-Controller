import tkinter as tk
from tkinter import scrolledtext, messagebox, ttk

# Le frequenze sono ora definite con lo stesso formato del tuo file notes.h
FREQUENCIES = {
    'NOTE_C3': 131, 'NOTE_CS3': 139, 'NOTE_D3': 147, 'NOTE_DS3': 156, 'NOTE_E3': 165, 'NOTE_F3': 175,
    'NOTE_FS3': 185, 'NOTE_G3': 196, 'NOTE_GS3': 208, 'NOTE_A3': 220, 'NOTE_AS3': 233, 'NOTE_B3': 247,
    'NOTE_C4': 262, 'NOTE_CS4': 277, 'NOTE_D4': 294, 'NOTE_DS4': 311, 'NOTE_E4': 330, 'NOTE_F4': 349,
    'NOTE_FS4': 370, 'NOTE_G4': 392, 'NOTE_GS4': 415, 'NOTE_A4': 440, 'NOTE_AS4': 466, 'NOTE_B4': 494,
    'NOTE_C5': 523, 'NOTE_CS5': 554, 'NOTE_D5': 587, 'NOTE_DS5': 622, 'NOTE_E5': 659, 'NOTE_F5': 698,
    'NOTE_FS5': 740, 'NOTE_G5': 784, 'NOTE_GS5': 831, 'NOTE_A5': 880, 'NOTE_AS5': 932, 'NOTE_B5': 988,
    'NOTE_C6': 1047, 'NOTE_CS6': 1109, 'NOTE_D6': 1175, 'NOTE_DS6': 1245, 'NOTE_E6': 1319, 'NOTE_F6': 1397,
    'NOTE_FS6': 1480, 'NOTE_G6': 1568, 'NOTE_GS6': 1661, 'NOTE_A6': 1760, 'NOTE_AS6': 1865, 'NOTE_B6': 1976
}

# Mappa per la notazione italiana
ITALIAN_NOTES = {
    'C': 'DO', 'CS': 'DOS', 'D': 'RE', 'DS': 'RES', 'E': 'MI', 'F': 'FA',
    'FS': 'FAS', 'G': 'SOL', 'GS': 'SOLS', 'A': 'LA', 'AS': 'LAS', 'B': 'SI'
}

class MusicComposerApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Python Music Composer")
        self.root.geometry("800x800")

        self.EIGHTH_NOTE_DURATION = 125
        self.QUARTER_NOTE_DURATION = 250
        self.HALF_NOTE_DURATION = 500
        self.WHOLE_NOTE_DURATION = 1000

        self.sequence = []
        self.current_octave = tk.StringVar(value='4')
        self.note_notation = tk.StringVar(value='english')
        self.note_duration = tk.StringVar(value=str(self.QUARTER_NOTE_DURATION))

        self.create_widgets()
        self.update_note_buttons()

    def create_widgets(self):
        main_frame = ttk.Frame(self.root, padding="10")
        main_frame.pack(fill="both", expand=True)

        control_frame = ttk.Frame(main_frame)
        control_frame.pack(fill='x', pady=5)

        ttk.Label(control_frame, text="Notazione:").pack(side=tk.LEFT, padx=5)
        notation_combo = ttk.Combobox(control_frame, textvariable=self.note_notation, values=['english', 'italian'], state="readonly")
        notation_combo.pack(side=tk.LEFT, padx=5)
        notation_combo.bind("<<ComboboxSelected>>", self.update_note_buttons)

        ttk.Label(control_frame, text="Ottava:").pack(side=tk.LEFT, padx=5)
        octave_combo = ttk.Combobox(control_frame, textvariable=self.current_octave, values=['3', '4', '5', '6'], state="readonly")
        octave_combo.pack(side=tk.LEFT, padx=5)
        octave_combo.bind("<<ComboboxSelected>>", self.update_note_buttons)
        
        action_frame = ttk.Frame(main_frame)
        action_frame.pack(fill='x', pady=5)
        
        ttk.Button(action_frame, text="Genera Codice", command=self.generate_code).pack(side=tk.LEFT, padx=5)
        ttk.Button(action_frame, text="Copia Codice", command=self.copy_code).pack(side=tk.LEFT, padx=5)
        ttk.Button(action_frame, text="Cancella Ultimo", command=self.delete_last).pack(side=tk.LEFT, padx=5)
        ttk.Button(action_frame, text="Cancella Tutto", command=self.clear_all).pack(side=tk.LEFT, padx=5)

        # Aggiungo il campo di testo per l'input della stringa
        string_input_frame = ttk.Frame(main_frame)
        string_input_frame.pack(fill='x', pady=5)
        ttk.Label(string_input_frame, text="Inserisci la stringa di note:").pack(side=tk.LEFT, padx=5)
        self.string_input = scrolledtext.ScrolledText(string_input_frame, wrap=tk.WORD, height=3)
        self.string_input.pack(fill="x", expand=True, padx=5, pady=5)
        
        ttk.Button(main_frame, text="Converti Stringa", command=self.convert_string).pack(pady=5)
        
        self.note_buttons_frame = ttk.Frame(main_frame)
        self.note_buttons_frame.pack(pady=10)
        
        duration_frame = ttk.Frame(self.note_buttons_frame)
        duration_frame.grid(row=0, column=0, columnspan=12, pady=5)
        ttk.Label(duration_frame, text="Durata Nota:").pack(side=tk.LEFT, padx=5)
        ttk.Radiobutton(duration_frame, text="Quarto (250ms)", variable=self.note_duration, value=str(self.QUARTER_NOTE_DURATION)).pack(side=tk.LEFT, padx=5)
        ttk.Radiobutton(duration_frame, text="Ottavo (125ms)", variable=self.note_duration, value=str(self.EIGHTH_NOTE_DURATION)).pack(side=tk.LEFT, padx=5)

        self.note_buttons = {}
        # Posizionamento dei tasti bianchi
        white_notes_map = {'C': 0, 'D': 1, 'E': 2, 'F': 3, 'G': 4, 'A': 5, 'B': 6}
        for note, pos in white_notes_map.items():
            btn = ttk.Button(self.note_buttons_frame, text=note, width=5, command=lambda n=note: self.add_note(n))
            btn.grid(row=2, column=pos*2, columnspan=2, padx=1, pady=5, sticky='ew')
            self.note_buttons[note] = btn
        
        # Posizionamento dei tasti neri
        black_notes_pos = {'CS': 1, 'DS': 3, 'FS': 7, 'GS': 9, 'AS': 11}
        for note, col in black_notes_pos.items():
            btn = ttk.Button(self.note_buttons_frame, text=note, width=3, command=lambda n=note: self.add_note(n))
            btn.grid(row=1, column=col, padx=0, pady=5, sticky='n')
            self.note_buttons[note] = btn

        pause_frame = ttk.Frame(main_frame)
        pause_frame.pack(pady=5)
        ttk.Label(pause_frame, text="Durata pausa:").pack(side=tk.LEFT, padx=5)
        ttk.Button(pause_frame, text="Quarto (250ms)", command=lambda: self.add_pause(self.QUARTER_NOTE_DURATION)).pack(side=tk.LEFT, padx=5)
        ttk.Button(pause_frame, text="Ottavo (125ms)", command=lambda: self.add_pause(self.EIGHTH_NOTE_DURATION)).pack(side=tk.LEFT, padx=5)
        
        ttk.Label(main_frame, text="Codice C generato:").pack(anchor='w', pady=(10, 0))
        self.code_box = scrolledtext.ScrolledText(main_frame, wrap=tk.WORD, height=15, width=80)
        self.code_box.pack(fill="both", expand=True, padx=5, pady=5)

    def update_note_buttons(self, event=None):
        notation = self.note_notation.get()
        for note, button in self.note_buttons.items():
            if notation == 'italian':
                display_text = ITALIAN_NOTES.get(note, note)
            else:
                display_text = note
            button.config(text=display_text)

    def add_note(self, note_name):
        full_note_name = f"NOTE_{note_name}{self.current_octave.get()}"
        if full_note_name in FREQUENCIES:
            duration = int(self.note_duration.get())
            self.sequence.append({'type': 'note', 'name': full_note_name, 'duration': duration})
            self.generate_code()
        else:
            messagebox.showwarning("Attenzione", f"La nota {full_note_name} non è disponibile nell'elenco delle frequenze.")

    def add_pause(self, duration):
        self.sequence.append({'type': 'pause', 'duration': duration})
        self.generate_code()
    
    def delete_last(self):
        if self.sequence:
            self.sequence.pop()
            self.generate_code()

    def clear_all(self):
        self.sequence.clear()
        self.generate_code()

    def convert_string(self):
        self.clear_all()
        text_input = self.string_input.get("1.0", tk.END).strip()
        
        duration_map = {
            '4': self.QUARTER_NOTE_DURATION,
            '8': self.EIGHTH_NOTE_DURATION
        }
        
        notes_string = text_input.replace(',', ' ').split()

        for item in notes_string:
            try:
                if '/' in item:
                    parts = item.split('/')
                    note = parts[0].strip()
                    duration_str = parts[1].strip()

                    if duration_str in duration_map:
                        duration = duration_map[duration_str]
                    else:
                        messagebox.showwarning("Errore di conversione", f"La durata '{duration_str}' non è supportata.")
                        self.clear_all()
                        return

                    if note == '-':
                        self.sequence.append({'type': 'pause', 'duration': duration})
                    else:
                        # Assicurati di aggiungere il prefisso NOTE_
                        full_note_name = f"NOTE_{note}"
                        if full_note_name in FREQUENCIES:
                            self.sequence.append({'type': 'note', 'name': full_note_name, 'duration': duration})
                        else:
                            messagebox.showwarning("Errore di conversione", f"La nota '{full_note_name}' non è valida.")
                            self.clear_all()
                            return
            except (IndexError, ValueError):
                messagebox.showwarning("Errore di formato", f"La stringa '{item}' non è nel formato corretto.")
                self.clear_all()
                return
        
        self.generate_code()

    def generate_code(self):
        code_lines = ["// Music Composer generator", ""]
        
        if not self.sequence:
            self.code_box.delete("1.0", tk.END)
            return

        for item in self.sequence:
            if item['type'] == 'note':
                code_lines.append(f"FAC_motor_make_noise({item['name']}, {item['duration']});")
            elif item['type'] == 'pause':
                code_lines.append(f"HAL_Delay({item['duration']});")
        
        self.code_box.delete("1.0", tk.END)
        self.code_box.insert("1.0", "\n".join(code_lines))

    def copy_code(self):
        code_to_copy = self.code_box.get("1.0", tk.END).strip()
        if code_to_copy:
            self.root.clipboard_clear()
            self.root.clipboard_append(code_to_copy)
            #messagebox.showinfo("Copia Codice", "Codice copiato negli appunti!")
        else:
            messagebox.showwarning("Attenzione", "Non c'è codice da copiare.")

if __name__ == "__main__":
    root = tk.Tk()
    app = MusicComposerApp(root)
    root.mainloop()