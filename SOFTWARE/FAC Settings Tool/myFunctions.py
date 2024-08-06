import serial
import serial.tools.list_ports

# comands
commands_list = {
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

# definitions
COMMAND_TYPE_SEPARATOR = ">"

# global variables
serial_connection = None
sendtryes = 0

def get_available_ports():
    ports = serial.tools.list_ports.comports()
    comList = []
    for p in ports:
        comList.append(p.device)
        print(p.device)
    print(len(ports), 'ports found')
    return comList

def connect_to_port(port):
    global serial_connection
    if port != 'No devices':
        try:
            serial_connection = serial.Serial(port, baudrate=115200, timeout=1)
            if serial_connection.is_open:
                print(f"Connected to {port}")
            return serial_connection
        except serial.SerialException as e:
            print(f"Errore di connessione: {e}")
            return None


def decode_settings(data):
    # divide the string
    key_value =  data.split('=')
    if len(key_value) == 2:
        key = key_value[0].strip()  # remove spaces
        value = key_value[1].strip()    # remove spaces and convert to integer
        try:
            value = int(value)
        except ValueError:
            print(f"Value not correct: {value}")
            return None, None
        return key, value
    else:
        print(f"Format not valid: {data}")
        return None, None

def get_settings():    # return the list of the settings read
    global serial_connection
    if serial_connection:
        serial_connection.write(b'READ>1')
        response = serial_connection.read(500)
        response = response.decode('utf-8')
        print(f"Current settings:\n {response}")
        commands = response.splitlines()
        for command in commands:
            key, val = decode_settings(command)
            if key and val is not None:
                if key in commands_list:
                    commands_list[key] = val
                if commands_list[key] == None:
                    return None
        for key, value in commands_list.items(): # check if there is some error in the settings recieved
            if value == None:
                return None
        print(commands_list)
        return commands_list
    else:
        return None
    


def any_port_is_connected():
    ports = get_available_ports()
    for port in ports: 
        try:
            with serial.Serial(port) as ser:
                return True
        except (serial.SerialException, OSError):
            return False
        
def get_serial_connected():
    ports = get_available_ports()
    for port in ports: 
        try:
            with serial.Serial(port) as ser:
                return ser
        except (serial.SerialException, OSError):
            return None

def save_settings():
    global serial_connection
    serial_connection.write(b'SAVE>1')

    response = serial_connection.read(500)
    response = response.decode('utf-8')
    response = response[:3]
    response = response.strip()
    print(response)
    if  response != "OK":
        print("resend the comand, FAC doest understand")
        save_settings()
    

def send_command(key, value):
    global serial_connection
    global sendtryes
    if sendtryes > 5:   # if the tries are more than 5 return
        return None
    
    keystr = f"{key}"
    mlstr = "ML"
    mrstr = "MR"
    wpstr = "WP"

    if keystr == "M1ASS" or keystr == "M2ASS" or keystr == "M3ASS":
        print("mx ass comand")
        if value == 0:
            valuestr = mlstr
        if value == 1:
            valuestr = mrstr
        if value == 2:
            valuestr = wpstr
    else:
        valuestr = f"{value}"

    comand_to_send = keystr+COMMAND_TYPE_SEPARATOR+valuestr
    comand_to_send_byte = comand_to_send.encode()
    print(comand_to_send_byte)

    serial_connection.write(comand_to_send_byte)
    response = serial_connection.read(200)
    response = response.decode('utf-8')
    response = response[:3]
    response = response.strip()
    print(response)
    if  response != "OK":
        print("resend the comand, FAC doest understand")
        sendtryes = sendtryes+1
        send_command(key, value)



# test funtions (the code below this line will be executed only if this file are executed directly)
if __name__ == "__main__":
    # ports = get_available_ports()
    # print("Avaliable ports:")
    # for port in ports:
    #     print(port)

    comando = decode_settings("TH2CH = 2")
    print(comando)