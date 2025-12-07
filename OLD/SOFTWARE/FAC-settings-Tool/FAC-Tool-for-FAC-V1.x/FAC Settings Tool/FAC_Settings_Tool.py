# to build use the following comand
# python -m PyInstaller FAC_Settings_Tool.py --onefile --icon=icon.ico -w  


import PySimpleGUI as sg
import myFunctions as mf
import time
import csv

# comands
commands_list = { # add the comand also to the myfunctions file
    'TH2CH': None,
    'SH2CH': None,
    'WP2CH': None,
    'S12CH': None,
    'S22CH': None,
    'ARM2CH': None,
    'M1REV': None,
    'M2REV': None,
    'M3REV': None,
    'S1REV': None,
    'S2REV': None,
    'M1ASS': None,
    'M2ASS': None,
    'M3ASS': None,
    'WPDD': None,
    'TNKON': None,
    'NODISARM': None,
    'VLIM': None,
    'VCUTOFF': None,
    'RXPWM': None
}

# variables
ports = mf.get_available_ports()
isConnected = False

# layout and menu
menu_def = [
    ['File', ['!Import', '!Export','---', 'Exit']],
    ['About', ['Info']]
]

channel_options = ['CH.1', 'CH.2', 'CH.3', 'CH.4', 'CH.5', 'CH.6', 'CH.7', 'CH.8']
motor_options = ['M.Left', 'M.Right', 'M.Weap.']

column1_layout = [
    [sg.Frame(title = "Movements Channels", size = (170, 80),layout=[
        [sg.Text("Throttle:", size=(10,1)), sg.Combo(values=channel_options, key = 'TH2CH', default_value='CH.1')],
        [sg.Text("Steering:", size=(10,1)), sg.Combo(values=channel_options, key = 'SH2CH', default_value='CH.1')],
    ])],
    [sg.Frame(title = "Weapon Channel", size = (170, 50),layout=[
        [sg.Text("Weapon:", size=(10,1)), sg.Combo(values=channel_options, key = 'WP2CH', default_value='CH.1')]
    ])],
    [sg.Frame(title = "Other Channels", size = (170, 104),layout=[
        [sg.Text("Servo1(HV):", size=(10,1)), sg.Combo(values=channel_options, key = 'S12CH', default_value='CH.1')],
        [sg.Text("Servo2:", size=(10,1)), sg.Combo(values=channel_options, key = 'S22CH', default_value='CH.1')],
        [sg.Text("Arm: (switch)", size=(10,1)), sg.Combo(values=channel_options, key = 'ARM2CH', default_value='CH.1')]
    ])],
    [sg.Frame(title = "Other", size = (170, 150),layout=[
        [sg.Text("Wp 2 dir: ", size=(10,1), tooltip="if checked the weapon can speen forward and backward"),sg.Checkbox('', key='WPDD')],
        [sg.Text("Tank Mix: ", size=(10,1), tooltip="if unchecked the THch->M1, STch->M2, WP->M3"),sg.Checkbox('', key='TNKON')],
        [sg.Text("No Disarm: ", size=(10,1), tooltip="if checked you can use the disarm channel"),sg.Checkbox('', key='NODISARM')],
        [sg.Text("RX is PWM: ", size=(10,1), tooltip="if checked means that your reciever is setted to outout a PWM signal, else PPM"),sg.Checkbox('', key='RXPWM')]
    ])]
]

column2_layout = [
    [sg.Frame(title = "Reverse Motors/Servos", size = (180, 180), layout=[
        [sg.Text("Reverse M1:", size=(15,1)),sg.Checkbox('', key='M1REV')],
        [sg.Text("Reverse M2:", size=(15,1)),sg.Checkbox('', key='M2REV')],
        [sg.Text("Reverse M3:", size=(15,1)),sg.Checkbox('', key='M3REV')],
        [sg.Text("Reverse S1:", size=(15,1)),sg.Checkbox('', key='S1REV')],
        [sg.Text("Reverse S2:", size=(15,1)),sg.Checkbox('', key='S2REV')],
    ])],
    [sg.Frame(title = "DC motors assignment", size = (180, 110), layout=[
        [sg.Text("Motor 1:", size=(10,1)), sg.Combo(values=motor_options, key = 'M1ASS', default_value='M.Left')],
        [sg.Text("Motor 2:", size=(10,1)), sg.Combo(values=motor_options, key = 'M2ASS', default_value='M.Left')],
        [sg.Text("Motor 3:", size=(10,1)), sg.Combo(values=motor_options, key = 'M3ASS', default_value='M.Left')]
    ])],
    [sg.Frame(title = "Battery protections", size = (180, 80), layout=[
        [sg.Text("Limit Voltage: ", size=(10,1), tooltip="voltage (per cell) at witch the weapon speed is limited"), sg.Spin([i/10 for i in range(28, 41)], initial_value=0.0, key='VLIM', size=(5, 1))],
        [sg.Text("Cutoff Voltage: ", size=(10,1), tooltip="voltage (per cell) at witch the robot disarm to not damage the battery"), sg.Spin([i/10 for i in range(25, 36)], initial_value=0.0, key='VCUTOFF', size=(5, 1))]
    ])],
    #[sg.Text('', size = (1,1))],
    [sg.Text('', size = (7,1)),sg.ProgressBar(max_value=100, orientation='h', size=(10, 20), key='-PROGRESSBAR-', visible=False)]
]

main_layout = [
    [sg.Menu(menu_def, key='MENU')],
    [sg.Button("Connect", disabled=True), sg.Combo([], size=(10,5), default_value='No devices', key='-PORT_SELECTED-')],
    [sg.HorizontalSeparator()],
    [sg.Column(column1_layout, vertical_alignment='top'), sg.Column(column2_layout, vertical_alignment='top')],
    [sg.Button("Read from FAC",key='-READFROMFAC-', disabled=True), sg.Text('', size = (21,1)), sg.Button("Save on FAC", key='-SAVEONFAC-', disabled=True)],
]

# window
window = sg.Window("FAC (Floppy Ant Controller) Settings Tool", main_layout, icon='icon.ico')

# functions
def save_commands_to_csv(commands):
    filename = sg.popup_get_file('Chose file location', save_as=True, file_types=(("CSV Files", "*.csv"),))
    if filename:
        with open(filename, 'w', newline='') as file:
            writer = csv.writer(file)
            for key, value in commands.items():
                writer.writerow([key, value])
        sg.popup('File saved!', f'Settings exported in {filename}')

def load_commands_from_csv():
    filename = sg.popup_get_file('Chose a CSV settings file', file_types=(("CSV Files", "*.csv"),))
    if filename:
        commands = {}
        with open(filename, 'r') as file:
            reader = csv.reader(file)
            for row in reader:
                if len(row) == 2:
                    key, value = row
                    try:
                        commands[key] = int(value)
                    except ValueError:
                        commands[key] = value
        return commands
    return None
# def update_ui_form_command_list(commands):
#     for key, value in commands_list.items():
#             if key in window.key_dict:
#                 if window[key].Type == sg.ELEM_TYPE_INPUT_CHECKBOX:
#                     tick = value
#                     window[key].update(value = tick)
#                 else:
#                     if key == 'M1ASS' or key == 'M2ASS' or key == 'M3ASS':
#                         option = motor_options[value % len(motor_options)]
#                         window[key].update(option)
#                     else:
#                         value = value-1
#                         option = channel_options[value % len(channel_options)]
#                         window[key].update(option)       

def update_ui_form_command_list(commands):
    for key, value in commands.items():
        if key in window.AllKeysDict:
            if window[key].Type == sg.ELEM_TYPE_INPUT_CHECKBOX:
                tick = bool(value)
                window[key].update(value=tick)
            elif window[key].Type == sg.ELEM_TYPE_INPUT_SPIN:
                if key in ['VLIM', 'VCUTOFF']:
                    window[key].update(value=value / 10.0)
                else:
                    window[key].update(value=float(value))
            elif window[key].Type == sg.ELEM_TYPE_INPUT_COMBO:
                if key in ['M1ASS', 'M2ASS', 'M3ASS']:
                    option = motor_options[value]
                else:
                    option = channel_options[value - 1]
                window[key].update(value=option)


# event loop
while True:
    event, values = window.read(timeout=500)
    if event == "Exit" or event == sg.WIN_CLOSED:
        break
    if event != '__TIMEOUT__':
        print(f'Event: {event}')

    # application code
    # buttons events
    if event == "Connect":
        port_selected = values['-PORT_SELECTED-']
        mf.connect_to_port(port_selected)
        commands_list = mf.get_settings()
        if commands_list != None:
            isConnected = True
            window['-SAVEONFAC-'].update(disabled = False)
            window['-READFROMFAC-'].update(disabled = False)
            window.write_event_value('-READFROMFAC-', "Custom event")

            new_menu_def = [
                ['File', ['Import', 'Export','---', 'Exit']],
                ['About', ['Info']]
            ]

            window['MENU'].update(new_menu_def)
            window["Connect"].update(disabled = True)
        else:
            sg.Popup('Connection error. Select the correct port and retry')
            isConnected = False

    if event == '-SAVEONFAC-':
        window['-PROGRESSBAR-'].update(0, visible=True)
        total_steps = len(commands_list)
        step = 0

        for key in commands_list.keys():
            if key in values:
                if isinstance(values[key], str): # if it is a combo
                    if key == 'M1ASS' or key == 'M2ASS' or key == 'M3ASS':
                        commands_list[key] = motor_options.index(values[key])
                    else:
                        commands_list[key] = channel_options.index(values[key])+1
                elif isinstance(values[key], bool):  # if it is a checkbox
                    commands_list[key] = int(values[key])
                elif isinstance(values[key], (int, float)):  # if it is a (Spin)
                    if key in ['VLIM', 'VCUTOFF']:
                        commands_list[key] = int(values[key] * 10)  # convert to the correct number
                    
        for key, value in commands_list.items():
            mf.send_command(key, value)
            step += 1
            progress = int((step / total_steps) * 100)
            window['-PROGRESSBAR-'].update(progress)
            

        mf.save_settings()
        window['-PROGRESSBAR-'].update(0, visible=False)
        

    if event == '-READFROMFAC-':
        port_selected = values['-PORT_SELECTED-']
        commands_list = mf.get_settings()
        update_ui_form_command_list(commands_list)

        
    if event == "Export":
        save_commands_to_csv(commands_list)

    if event == "Import":
        commands_list = load_commands_from_csv()
        update_ui_form_command_list(commands_list)

    # other events
    if event == "Info":
        sg.popup('This software is working in progress, if you find something to change or you want to improve it, sand an email to thefloppylab@gmail.com. Thank you and good fight XD!',title="Info about \"FAC Settings Tool\"" )


    #if not isConnected:
    if window.TKroot.winfo_exists() and int(time.time())% 1 == 0:
        ports = mf.get_available_ports()
        if ports:
            window['-PORT_SELECTED-'].update(values=ports, value = values['-PORT_SELECTED-'], size = (10,5))
        else:
            window['-PORT_SELECTED-'].update(values=ports, value='No devices', size = (10,5))
        if values['-PORT_SELECTED-'] != 'No devices':
            window["Connect"].update(disabled = False)
        else:
            window["Connect"].update(disabled = True)
window.close()
