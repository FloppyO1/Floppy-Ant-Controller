using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Web;



namespace FAC_Settings_Tool {
    internal class MySerial {
        public SerialPort serialPort;
        public Dictionary<string, string> commandDictionary;

        public static class Commands {
            // Separator
            public const char COMMAND_TYPE_SEPARATOR = '>';
            // CHANNELs MAPPING
            public const string COMMAND_TH2CH = "TH2CH";   // throttel to channel..
            public const string COMMAND_ST2CH = "ST2CH";   // steering
            public const string COMMAND_WP2CH = "WP2CH";   // weapon
            public const string COMMAND_S12CH = "S12CH";   // servo 1 (HV)
            public const string COMMAND_S22CH = "S22CH";   // servo 2
            public const string COMMAND_ARM2CH = "ARM2CH";    // arm robot
            // DC MOTORs/ SERVOs REVERSE
            public const string COMMAND_M1REV = "M1REV";   // reverse the rotation of the motor
            public const string COMMAND_M2REV = "M2REV";
            public const string COMMAND_M3REV = "M3REV";
            public const string COMMAND_S1REV = "S1REV";   // reverse the position of the servo
            public const string COMMAND_S2REV = "S2REV";
            // Mx ASSIGNMENT
            public const string COMMAND_M1ASS = "M1ASS";   // assign the motor 1 to MOTOR LEFT, MOTOR RIGHT, WEAPPON MOTOR
            public const string COMMAND_M2ASS = "M2ASS";
            public const string COMMAND_M3ASS = "M3ASS";
            // VALUE
            public const string VALUE_ML = "ML";           // Motor Left
            public const string VALUE_MR = "MR";           // Motor Right
            public const string VALUE_WP = "WP";           // Weapon
            // WEAPPON DOUBLE/SINGLE DIRECTION
            public const string COMMAND_WPDD = "WPDD";     // 1 if double direction, 0 if single direction
            // TANK MODE
            public const string COMMAND_TANKON = "TNKON";  // 0 if disabled, 1 if enabled
            // OTHER
            public const string COMMAND_NODISARM = "NODISARM"; // VALUE = 0 if disabled, 1 if enabled
            public const string COMMAND_LIMITVOLTAGE = "VLIM";  // VALUE = voltage * 10 (3.3V -> 33)
            public const string COMMAND_CUTOFFVOLTAGE = "VCUTOFF"; // VALUE = voltage * 10 (3.3V -> 33)
            public const string COMMAND_RXPWMMODE = "RXPWM";  // 1 if the receiver outputs a PWM signal, else PPM
            // SAVE/READ SETTINGS
            public const string COMMAND_SAVE = "SAVE";     // Save settings
            public const string COMMAND_READ = "READ";     // Read settings
            public const string COMMAND_CONNECTED = "CON"; // Only to check if the device is a FAC or not
        }


        public MySerial() {
            serialPort = new SerialPort();
            commandDictionary = new Dictionary<string, string> {
                { Commands.COMMAND_TH2CH, "0" },
                { Commands.COMMAND_ST2CH, "0" },
                { Commands.COMMAND_WP2CH, "0" },
                { Commands.COMMAND_S12CH, "0" },
                { Commands.COMMAND_S22CH, "0" },
                { Commands.COMMAND_ARM2CH, "0" },
                { Commands.COMMAND_M1REV, "0" },
                { Commands.COMMAND_M2REV, "0" },
                { Commands.COMMAND_M3REV, "0" },
                { Commands.COMMAND_S1REV, "0" },
                { Commands.COMMAND_S2REV, "0" },
                { Commands.COMMAND_M1ASS, "0" },
                { Commands.COMMAND_M2ASS, "0" },
                { Commands.COMMAND_M3ASS, "0" },
                { Commands.COMMAND_WPDD, "0" },
                { Commands.COMMAND_TANKON, "0" },
                { Commands.COMMAND_NODISARM, "0" },
                { Commands.COMMAND_LIMITVOLTAGE, "0" },
                { Commands.COMMAND_CUTOFFVOLTAGE, "0" },
                { Commands.COMMAND_RXPWMMODE, "0" },
            };
        }

        public void saveOnFAC(ProgressBar progressBar) {
            foreach (var key in commandDictionary.Keys) {    // print the dictionary value
                Console.WriteLine(key + " > " + commandDictionary[key]);
            }
            progressBar.Visible = true;
            int progress = 0;
            foreach (var key in commandDictionary.Keys) {
                string encodedCommand = encodeCommand(key);
                Console.WriteLine("sending: " + encodedCommand);
                bool sendOk = sendData(encodedCommand);
                if (!sendOk) {
                    MessageBox.Show("error while saving, retry\ncommand: " + encodedCommand);
                    return;
                }
                progressBar.Value = map(progress, 0, commandDictionary.Count, 0, 100);
                progress++;
            }
            Console.WriteLine("saving");
            sendData("SAVE>1");
            progressBar.Visible = false;
        }

        public static int map(int value, int fromLow, int fromHigh, int toLow, int toHigh) {
            return (int)((value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow);
        }

        public string[] getAvailablePorts() {
            string[] ports = SerialPort.GetPortNames();
            return ports;
        }

        public bool readAllComands() {
            if (serialPort.IsOpen) {
                serialPort.WriteLine("READ>1");

                List<string> lines = new List<string>();
                while (true) {
                    try {
                        string line = serialPort.ReadLine();
                        lines.Add(line);
                    }
                    catch (TimeoutException) {
                        Console.WriteLine("data ended");
                        break;
                    }
                }

                foreach (string line in lines) {
                    if (line.StartsWith("SETTINGS:") || line.StartsWith("write") || line.StartsWith("than") || string.IsNullOrWhiteSpace(line)) continue;   // if the line stats with a non comand skip
                    string nomalizedLine = line.Replace(" ", "");
                    Console.WriteLine(nomalizedLine);
                    if (!decodeCommand(nomalizedLine)) { // error in loading settings
                        MessageBox.Show("Error in loading settings, command not found in dictionary");
                        return false;
                    }
                }

                foreach (var key in commandDictionary.Keys) {    // print the dictionary value
                    Console.WriteLine(key + " > " + commandDictionary[key]);
                }
            }

            return false;
        }

        public bool isAFAC() {
            if (serialPort.IsOpen) {
                if (sendData(Commands.COMMAND_CONNECTED)) {
                    return true;
                }
            }
            return false;
        }

        private bool decodeCommand(string s) {  // return true if s is in the dictionary
            string[] commandDecoded = s.Split(Commands.COMMAND_TYPE_SEPARATOR);  // comandDecoded[0] is comand, [1] is value
            if (commandDictionary.ContainsKey(commandDecoded[0])) {
                commandDictionary[commandDecoded[0]] = commandDecoded[1]; // if the comand is present update it in the dictionary
                return true;
            }
            return false;
        }

        private string encodeCommand(string key) {
            string command;
            string value = commandDictionary[key];
            if (key == Commands.COMMAND_M1ASS || key == Commands.COMMAND_M2ASS || key == Commands.COMMAND_M3ASS) {
                Console.WriteLine("assignment command detected");
                switch (commandDictionary[key]) {
                    case "0":
                        value = Commands.VALUE_ML;
                        break;
                    case "1":
                        value = Commands.VALUE_MR;
                        break;
                    case "2":
                        value = Commands.VALUE_WP;
                        break;
                }
            }
            command = key + Commands.COMMAND_TYPE_SEPARATOR + value;
            return command;
        }

        public bool openPort(string port) {
            serialPort.PortName = port;
            serialPort.BaudRate = 9600;
            serialPort.ReadTimeout = 1000;
            serialPort.WriteTimeout = 1000;

            try {
                serialPort.Open();
                Console.WriteLine("serial Port opened correctly");
                return true;
            }
            catch (UnauthorizedAccessException) {
                MessageBox.Show("Acess to " + port + " denied.");
            }
            catch (IOException) {
                MessageBox.Show("I/O error at the port " + port + ".");
            }
            catch (ArgumentException) {
                MessageBox.Show("Port name not valid");
            }
            return false;
        }

        public string getCommandValue(string command) {
            if (commandDictionary.ContainsKey(command)) {
                return commandDictionary[command];
            }
            return null;
        }

        public string receiveAllData() {
            try {
                if (serialPort.IsOpen) {
                    return serialPort.ReadExisting();   // read everything
                }
                else {
                    MessageBox.Show("Port not open");
                }
            }
            catch (TimeoutException) {
            }
            return string.Empty;
        }
        public string receiveLineData() {
            try {
                if (serialPort.IsOpen) {
                    string line = serialPort.ReadLine();    // read until \n
                    do {
                        line = serialPort.ReadLine();
                    } while (string.IsNullOrWhiteSpace(line));
                    return line;
                }
                else {
                    MessageBox.Show("Port not open");
                }
            }
            catch (TimeoutException) {
            }
            return string.Empty;
        }
        public bool sendData(string data) {
            try {
                if (serialPort.IsOpen) {
                    serialPort.WriteLine(data);
                    string ack = receiveLineData();
                    Console.WriteLine($"{ack}");
                    if (ack == "OK") return true;
                    else return false;
                }
                else {
                    MessageBox.Show("Port not open");
                    return false;
                }
            }
            catch (TimeoutException) {
                return false;
            }
        }
    }
}
