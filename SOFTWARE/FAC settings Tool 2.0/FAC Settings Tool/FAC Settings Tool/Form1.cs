using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Security.Policy;
using System.Web;
using System.Windows.Forms;
using static FAC_Settings_Tool.MySerial;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace FAC_Settings_Tool {
    public partial class FacSettingsTool : Form {
        MySerial mySerial;

        public FacSettingsTool() {
            InitializeComponent();
            // my code
            mySerial = new MySerial();
        }
        private void ApplyDarkTheme() { // to be made correctly
            this.BackColor = Color.FromArgb(30, 30, 30);
            this.ForeColor = Color.White;

            // Cambia i colori dei controlli
            foreach (Control control in this.Controls) {
                control.BackColor = Color.FromArgb(30, 30, 30);
                control.ForeColor = Color.White;
            }
        }

        private void buttonConnect_Click(object sender, EventArgs e) {
            string selectedPort = comboBoxPorts.SelectedItem.ToString();
            Console.WriteLine("opening serial");
            if (mySerial.openPort(selectedPort)) {
                if (mySerial.isAFAC()) {

                    mySerial.readAllComands();
                    updateGraphics();

                    pictureBoxTick.Visible = true;
                    comboBoxPorts.Enabled = false;
                    buttonCheck.Enabled = false;
                    buttonConnect.Enabled = false;
                    buttonRead.Enabled = true;
                    buttonSave.Enabled = true;
                    buttonDisconnect.Enabled = true;
                }
                else {
                    mySerial.serialPort.Close();
                    MessageBox.Show("Connect to the correct PORT");
                }
            }
        }

        private void buttonCheck_Click(object sender, EventArgs e) {
            comboBoxPorts.Items.Clear();
            comboBoxPorts.Items.AddRange(mySerial.getAvailablePorts());
            try {
                comboBoxPorts.SelectedIndex = 0;
                string selectedPort = comboBoxPorts.SelectedItem.ToString();
                Console.WriteLine(selectedPort + " selected");
                buttonConnect.Enabled = true;
            }
            catch { }
        }

        private void comboBoxPorts_SelectedIndexChanged(object sender, EventArgs e) {
            string selectedPort = comboBoxPorts.SelectedItem.ToString();
            buttonConnect.Enabled = true;
            Console.WriteLine(selectedPort + " selected");
        }

        private void buttonRead_Click(object sender, EventArgs e) {
            mySerial.readAllComands();
            updateGraphics();
        }

        private void buttonDisconnect_Click(object sender, EventArgs e) {
            mySerial.serialPort.Close();
            if (!mySerial.serialPort.IsOpen) {
                pictureBoxTick.Visible = false;
                comboBoxPorts.Enabled = true;
                buttonCheck.Enabled = true;
                buttonConnect.Enabled = true;
                buttonRead.Enabled = false;
                buttonSave.Enabled = false;
                buttonDisconnect.Enabled = false;
            }
        }
        private void comboBoxM1ASS_SelectedIndexChanged(object sender, EventArgs e) {
            UpdateComboBoxes(comboBoxM1ASS, comboBoxM2ASS, comboBoxM3ASS);
        }
        private void comboBoxM2ASS_SelectedIndexChanged(object sender, EventArgs e) {
            UpdateComboBoxes(comboBoxM2ASS, comboBoxM1ASS, comboBoxM3ASS);
        }
        private void comboBoxM3ASS_SelectedIndexChanged(object sender, EventArgs e) {
            UpdateComboBoxes(comboBoxM3ASS, comboBoxM1ASS, comboBoxM2ASS);
        }

        private void UpdateComboBoxes(System.Windows.Forms.ComboBox selectedComboBox, System.Windows.Forms.ComboBox comboBox1, System.Windows.Forms.ComboBox comboBox2) {
            List<int> indexes = new List<int> { 0, 1, 2 };
            indexes.Remove(selectedComboBox.SelectedIndex);
            indexes.Remove(comboBox1.SelectedIndex);
            indexes.Remove(comboBox2.SelectedIndex);
            if (selectedComboBox.SelectedIndex == comboBox1.SelectedIndex) {
                comboBox1.SelectedIndex = indexes[0];
            }
            if (selectedComboBox.SelectedIndex == comboBox2.SelectedIndex) {
                comboBox2.SelectedIndex = indexes[0];
            }
            updateLabels();
        }

        private void updateLabels() {
            //labelM1.Visible = true;
            //labelM2.Visible = true;
            //labelM3.Visible = true;
            // update image's lables
            switch (comboBoxM1ASS.SelectedIndex) {
                case 0:
                    labelM1.Text = "MOTOR LEFT";
                    break;
                case 1:
                    labelM1.Text = "MOTOR RIGHT";
                    break;
                case 2:
                    labelM1.Text = "MOTOR WEAP";
                    break;
                default:
                    labelM1.Text = "NONE";
                    break;
            }
            switch (comboBoxM2ASS.SelectedIndex) {
                case 0:
                    labelM2.Text = "MOTOR LEFT";
                    break;
                case 1:
                    labelM2.Text = "MOTOR RIGHT";
                    break;
                case 2:
                    labelM2.Text = "MOTOR WEAP";
                    break;
                default:
                    labelM2.Text = "NONE";
                    break;
            }
            switch (comboBoxM3ASS.SelectedIndex) {
                case 0:
                    labelM3.Text = "MOTOR LEFT";
                    break;
                case 1:
                    labelM3.Text = "MOTOR RIGHT";
                    break;
                case 2:
                    labelM3.Text = "MOTOR WEAP";
                    break;
                default:
                    labelM3.Text = "NONE";
                    break;
            }
        }

        private void updateGraphics() {
            int value;

            // channels
            value = int.Parse(mySerial.getCommandValue("TH2CH"));
            comboBoxTH.SelectedIndex = value - 1;

            value = int.Parse(mySerial.getCommandValue("ST2CH"));
            comboBoxST.SelectedIndex = value - 1;

            value = int.Parse(mySerial.getCommandValue("WP2CH"));
            comboBoxWP.SelectedIndex = value - 1;

            value = int.Parse(mySerial.getCommandValue("S12CH"));
            comboBoxS1.SelectedIndex = value - 1;

            value = int.Parse(mySerial.getCommandValue("S22CH"));
            comboBoxS2.SelectedIndex = value - 1;

            value = int.Parse(mySerial.getCommandValue("ARM2CH"));
            comboBoxArm.SelectedIndex = value - 1;

            value = int.Parse(mySerial.getCommandValue("M1REV"));
            if (value == 1) checkBoxRevM1.Checked = true;
            else checkBoxRevM1.Checked = false;

            // reverse
            value = int.Parse(mySerial.getCommandValue("M2REV"));
            if (value == 1) checkBoxRevM2.Checked = true;
            else checkBoxRevM2.Checked = false;

            value = int.Parse(mySerial.getCommandValue("M3REV"));
            if (value == 1) checkBoxRevM3.Checked = true;
            else checkBoxRevM3.Checked = false;

            value = int.Parse(mySerial.getCommandValue("S1REV"));
            if (value == 1) checkBoxRevS1.Checked = true;
            else checkBoxRevS1.Checked = false;

            value = int.Parse(mySerial.getCommandValue("S2REV"));
            if (value == 1) checkBoxRevS2.Checked = true;
            else checkBoxRevS2.Checked = false;

            // motor ass
            value = int.Parse(mySerial.getCommandValue("M1ASS"));
            comboBoxM1ASS.SelectedIndex = value;

            value = int.Parse(mySerial.getCommandValue("M2ASS"));
            comboBoxM2ASS.SelectedIndex = value;

            value = int.Parse(mySerial.getCommandValue("M3ASS"));
            comboBoxM3ASS.SelectedIndex = value;

            // other
            value = int.Parse(mySerial.getCommandValue("WPDD"));
            if (value == 1) checkBoxWPDD.Checked = true;
            else checkBoxWPDD.Checked = false;

            value = int.Parse(mySerial.getCommandValue("TNKON"));
            if (value == 1) checkBoxTankOn.Checked = true;
            else checkBoxTankOn.Checked = false;

            value = int.Parse(mySerial.getCommandValue("NODISARM"));
            if (value == 1) checkBoxArm.Checked = false;
            else checkBoxArm.Checked = true;

            value = int.Parse(mySerial.getCommandValue("RXPWM"));
            if (value == 1) checkBoxRXPWM.Checked = true;
            else checkBoxRXPWM.Checked = false;

            value = int.Parse(mySerial.getCommandValue("VLIM"));
            numericUpDownVLim.Value = value / 10m;

            value = int.Parse(mySerial.getCommandValue("VCUTOFF"));
            numericUpDownCutOff.Value = value / 10m;
        }

        private void buttonSave_Click(object sender, EventArgs e) {
            string value;
            int temp;
            // channels
            temp = comboBoxTH.SelectedIndex + 1;
            value = temp.ToString();
            mySerial.commandDictionary[Commands.COMMAND_TH2CH] = value;

            temp = comboBoxST.SelectedIndex + 1;
            value = temp.ToString();
            mySerial.commandDictionary[Commands.COMMAND_ST2CH] = value;

            temp = comboBoxWP.SelectedIndex + 1;
            value = temp.ToString();
            mySerial.commandDictionary[Commands.COMMAND_WP2CH] = value;

            temp = comboBoxS1.SelectedIndex + 1;
            value = temp.ToString();
            mySerial.commandDictionary[Commands.COMMAND_S12CH] = value;

            temp = comboBoxS2.SelectedIndex + 1;
            value = temp.ToString();
            mySerial.commandDictionary[Commands.COMMAND_S22CH] = value;

            temp = comboBoxArm.SelectedIndex + 1;
            value = temp.ToString();
            mySerial.commandDictionary[Commands.COMMAND_ARM2CH] = value;

            // reverse
            value = checkBoxRevM1.Checked ? "1" : "0";
            mySerial.commandDictionary[Commands.COMMAND_M1REV] = value;

            value = checkBoxRevM2.Checked ? "1" : "0";
            mySerial.commandDictionary[Commands.COMMAND_M2REV] = value;

            value = checkBoxRevM3.Checked ? "1" : "0";
            mySerial.commandDictionary[Commands.COMMAND_M3REV] = value;

            value = checkBoxRevS1.Checked ? "1" : "0";
            mySerial.commandDictionary[Commands.COMMAND_S1REV] = value;

            value = checkBoxRevS2.Checked ? "1" : "0";
            mySerial.commandDictionary[Commands.COMMAND_S2REV] = value;

            // motor ass
            value = comboBoxM1ASS.SelectedIndex.ToString();
            mySerial.commandDictionary[Commands.COMMAND_M1ASS] = value;

            value = comboBoxM2ASS.SelectedIndex.ToString();
            mySerial.commandDictionary[Commands.COMMAND_M2ASS] = value;

            value = comboBoxM3ASS.SelectedIndex.ToString();
            mySerial.commandDictionary[Commands.COMMAND_M3ASS] = value;

            // other
            value = checkBoxWPDD.Checked ? "1" : "0";
            mySerial.commandDictionary[Commands.COMMAND_WPDD] = value;

            value = checkBoxTankOn.Checked ? "1" : "0";
            mySerial.commandDictionary[Commands.COMMAND_TANKON] = value;

            value = checkBoxArm.Checked ? "0" : "1";
            mySerial.commandDictionary[Commands.COMMAND_NODISARM] = value;

            value = checkBoxRXPWM.Checked ? "1" : "0";
            mySerial.commandDictionary[Commands.COMMAND_RXPWMMODE] = value;

            decimal decValue = numericUpDownVLim.Value * 10;
            temp = (int)decValue;
            value = temp.ToString();
            mySerial.commandDictionary[Commands.COMMAND_LIMITVOLTAGE] = value;

            decValue = numericUpDownCutOff.Value * 10;
            temp = (int)decValue;
            value = temp.ToString();
            mySerial.commandDictionary[Commands.COMMAND_CUTOFFVOLTAGE] = value;

            mySerial.saveOnFAC(progressBarSave);
        }

        static void ExportCsv(Dictionary<string, string> dictionary) {
            using (SaveFileDialog saveFileDialog = new SaveFileDialog()) {
                saveFileDialog.Filter = "CSV files (*.csv)|*.csv|All files (*.*)|*.*";
                saveFileDialog.Title = "Salva il file CSV";

                if (saveFileDialog.ShowDialog() == DialogResult.OK) {
                    using (StreamWriter writer = new StreamWriter(saveFileDialog.FileName)) {
                        writer.WriteLine("Chiave,Valore");
                        foreach (var kvp in dictionary) {
                            writer.WriteLine($"{kvp.Key},{kvp.Value}");
                        }
                    }
                }
            }
        }
        static Dictionary<string, string> ImportCsv() {
            Dictionary<string, string> dictionary = new Dictionary<string, string>();

            using (OpenFileDialog openFileDialog = new OpenFileDialog()) {
                openFileDialog.Filter = "CSV files (*.csv)|*.csv|All files (*.*)|*.*";
                openFileDialog.Title = "Chose the .CSV file CSV";

                if (openFileDialog.ShowDialog() == DialogResult.OK) {
                    using (StreamReader reader = new StreamReader(openFileDialog.FileName)) {
                        reader.ReadLine();
                        while (!reader.EndOfStream) {
                            string line = reader.ReadLine();
                            string[] parts = line.Split(',');
                            if (parts.Length == 2) {
                                dictionary[parts[0]] = parts[1];
                            }
                        }
                    }
                }

                else {
                    return null;
                }
            }
            return dictionary;
        }

        private void buttonImport_Click(object sender, EventArgs e) {
            Dictionary<string, string> dictionary = ImportCsv();
            if (dictionary != null) {
                updateGraphics();
                updateLabels();
            }
        }

        private void buttonExport_Click(object sender, EventArgs e) {
            ExportCsv(mySerial.commandDictionary);
        }

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e) {
            Process.Start("https://linktr.ee/FloppyLab");
        }
    }
}
