namespace FAC_Settings_Tool {
    partial class FacSettingsTool {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FacSettingsTool));
            this.buttonConnect = new System.Windows.Forms.Button();
            this.comboBoxPorts = new System.Windows.Forms.ComboBox();
            this.gBoxMovements = new System.Windows.Forms.GroupBox();
            this.checkBoxTankOn = new System.Windows.Forms.CheckBox();
            this.comboBoxST = new System.Windows.Forms.ComboBox();
            this.comboBoxTH = new System.Windows.Forms.ComboBox();
            this.lableST = new System.Windows.Forms.Label();
            this.lableTH = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.checkBoxWPDD = new System.Windows.Forms.CheckBox();
            this.comboBoxWP = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.checkBoxArm = new System.Windows.Forms.CheckBox();
            this.comboBoxArm = new System.Windows.Forms.ComboBox();
            this.comboBoxS2 = new System.Windows.Forms.ComboBox();
            this.comboBoxS1 = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.checkBoxRevS2 = new System.Windows.Forms.CheckBox();
            this.checkBoxRevS1 = new System.Windows.Forms.CheckBox();
            this.checkBoxRevM3 = new System.Windows.Forms.CheckBox();
            this.checkBoxRevM2 = new System.Windows.Forms.CheckBox();
            this.checkBoxRevM1 = new System.Windows.Forms.CheckBox();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.comboBoxM3ASS = new System.Windows.Forms.ComboBox();
            this.comboBoxM2ASS = new System.Windows.Forms.ComboBox();
            this.comboBoxM1ASS = new System.Windows.Forms.ComboBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.label9 = new System.Windows.Forms.Label();
            this.numericUpDownCutOff = new System.Windows.Forms.NumericUpDown();
            this.label8 = new System.Windows.Forms.Label();
            this.numericUpDownVLim = new System.Windows.Forms.NumericUpDown();
            this.progressBarSave = new System.Windows.Forms.ProgressBar();
            this.buttonSave = new System.Windows.Forms.Button();
            this.buttonRead = new System.Windows.Forms.Button();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.buttonCheck = new System.Windows.Forms.Button();
            this.pictureBoxTick = new System.Windows.Forms.PictureBox();
            this.imagePanel = new System.Windows.Forms.Panel();
            this.linkLabel1 = new System.Windows.Forms.LinkLabel();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.labelM1 = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.label16 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.labelRX = new System.Windows.Forms.Label();
            this.label18 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.labelM2 = new System.Windows.Forms.Label();
            this.labelM3 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.buttonDisconnect = new System.Windows.Forms.Button();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.checkBoxRXPWM = new System.Windows.Forms.CheckBox();
            this.label12 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.fileToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.importToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.exportToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.buttonImport = new System.Windows.Forms.Button();
            this.buttonExport = new System.Windows.Forms.Button();
            this.gBoxMovements.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox5.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownCutOff)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownVLim)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxTick)).BeginInit();
            this.imagePanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.groupBox6.SuspendLayout();
            this.SuspendLayout();
            // 
            // buttonConnect
            // 
            this.buttonConnect.Enabled = false;
            this.buttonConnect.Location = new System.Drawing.Point(83, 7);
            this.buttonConnect.Name = "buttonConnect";
            this.buttonConnect.Size = new System.Drawing.Size(75, 23);
            this.buttonConnect.TabIndex = 0;
            this.buttonConnect.Text = "Connect";
            this.buttonConnect.UseVisualStyleBackColor = true;
            this.buttonConnect.Click += new System.EventHandler(this.buttonConnect_Click);
            // 
            // comboBoxPorts
            // 
            this.comboBoxPorts.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPorts.FormattingEnabled = true;
            this.comboBoxPorts.Location = new System.Drawing.Point(195, 9);
            this.comboBoxPorts.Name = "comboBoxPorts";
            this.comboBoxPorts.Size = new System.Drawing.Size(85, 21);
            this.comboBoxPorts.TabIndex = 4;
            this.comboBoxPorts.SelectedIndexChanged += new System.EventHandler(this.comboBoxPorts_SelectedIndexChanged);
            // 
            // gBoxMovements
            // 
            this.gBoxMovements.Controls.Add(this.checkBoxTankOn);
            this.gBoxMovements.Controls.Add(this.comboBoxST);
            this.gBoxMovements.Controls.Add(this.comboBoxTH);
            this.gBoxMovements.Controls.Add(this.lableST);
            this.gBoxMovements.Controls.Add(this.lableTH);
            this.gBoxMovements.Location = new System.Drawing.Point(7, 36);
            this.gBoxMovements.Name = "gBoxMovements";
            this.gBoxMovements.Size = new System.Drawing.Size(164, 94);
            this.gBoxMovements.TabIndex = 5;
            this.gBoxMovements.TabStop = false;
            this.gBoxMovements.Text = "Movements";
            // 
            // checkBoxTankOn
            // 
            this.checkBoxTankOn.AutoSize = true;
            this.checkBoxTankOn.Location = new System.Drawing.Point(6, 71);
            this.checkBoxTankOn.Name = "checkBoxTankOn";
            this.checkBoxTankOn.Size = new System.Drawing.Size(110, 17);
            this.checkBoxTankOn.TabIndex = 11;
            this.checkBoxTankOn.Text = "Tank mix enabled";
            this.checkBoxTankOn.UseVisualStyleBackColor = true;
            // 
            // comboBoxST
            // 
            this.comboBoxST.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.comboBoxST.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxST.FormattingEnabled = true;
            this.comboBoxST.Items.AddRange(new object[] {
            "CH.1",
            "CH.2",
            "CH.3",
            "CH.4",
            "CH.5",
            "CH.6",
            "CH.7",
            "CH.8"});
            this.comboBoxST.Location = new System.Drawing.Point(95, 44);
            this.comboBoxST.Name = "comboBoxST";
            this.comboBoxST.Size = new System.Drawing.Size(50, 21);
            this.comboBoxST.TabIndex = 3;
            // 
            // comboBoxTH
            // 
            this.comboBoxTH.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.comboBoxTH.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxTH.FormattingEnabled = true;
            this.comboBoxTH.Items.AddRange(new object[] {
            "CH.1",
            "CH.2",
            "CH.3",
            "CH.4",
            "CH.5",
            "CH.6",
            "CH.7",
            "CH.8"});
            this.comboBoxTH.Location = new System.Drawing.Point(95, 17);
            this.comboBoxTH.Name = "comboBoxTH";
            this.comboBoxTH.Size = new System.Drawing.Size(50, 21);
            this.comboBoxTH.TabIndex = 2;
            // 
            // lableST
            // 
            this.lableST.AutoSize = true;
            this.lableST.Location = new System.Drawing.Point(7, 47);
            this.lableST.Name = "lableST";
            this.lableST.Size = new System.Drawing.Size(49, 13);
            this.lableST.TabIndex = 1;
            this.lableST.Text = "Steering:";
            // 
            // lableTH
            // 
            this.lableTH.AutoSize = true;
            this.lableTH.Location = new System.Drawing.Point(7, 20);
            this.lableTH.Name = "lableTH";
            this.lableTH.Size = new System.Drawing.Size(46, 13);
            this.lableTH.TabIndex = 0;
            this.lableTH.Text = "Throttle:";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.checkBoxWPDD);
            this.groupBox1.Controls.Add(this.comboBoxWP);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Location = new System.Drawing.Point(7, 136);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(164, 68);
            this.groupBox1.TabIndex = 6;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Weapon";
            // 
            // checkBoxWPDD
            // 
            this.checkBoxWPDD.AutoSize = true;
            this.checkBoxWPDD.Location = new System.Drawing.Point(6, 44);
            this.checkBoxWPDD.Name = "checkBoxWPDD";
            this.checkBoxWPDD.Size = new System.Drawing.Size(145, 17);
            this.checkBoxWPDD.TabIndex = 10;
            this.checkBoxWPDD.Text = "Weapon double direction";
            this.checkBoxWPDD.UseVisualStyleBackColor = true;
            // 
            // comboBoxWP
            // 
            this.comboBoxWP.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.comboBoxWP.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxWP.FormattingEnabled = true;
            this.comboBoxWP.Items.AddRange(new object[] {
            "CH.1",
            "CH.2",
            "CH.3",
            "CH.4",
            "CH.5",
            "CH.6",
            "CH.7",
            "CH.8"});
            this.comboBoxWP.Location = new System.Drawing.Point(95, 17);
            this.comboBoxWP.Name = "comboBoxWP";
            this.comboBoxWP.Size = new System.Drawing.Size(50, 21);
            this.comboBoxWP.TabIndex = 4;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(7, 20);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(51, 13);
            this.label2.TabIndex = 0;
            this.label2.Text = "Weapon:";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.checkBoxArm);
            this.groupBox2.Controls.Add(this.comboBoxArm);
            this.groupBox2.Controls.Add(this.comboBoxS2);
            this.groupBox2.Controls.Add(this.comboBoxS1);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Location = new System.Drawing.Point(7, 210);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(164, 124);
            this.groupBox2.TabIndex = 6;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Servos and Arming";
            // 
            // checkBoxArm
            // 
            this.checkBoxArm.AutoSize = true;
            this.checkBoxArm.Location = new System.Drawing.Point(10, 98);
            this.checkBoxArm.Name = "checkBoxArm";
            this.checkBoxArm.Size = new System.Drawing.Size(132, 17);
            this.checkBoxArm.TabIndex = 9;
            this.checkBoxArm.Text = "Arming switch enabled";
            this.checkBoxArm.UseVisualStyleBackColor = true;
            // 
            // comboBoxArm
            // 
            this.comboBoxArm.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.comboBoxArm.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxArm.FormattingEnabled = true;
            this.comboBoxArm.Items.AddRange(new object[] {
            "CH.1",
            "CH.2",
            "CH.3",
            "CH.4",
            "CH.5",
            "CH.6",
            "CH.7",
            "CH.8"});
            this.comboBoxArm.Location = new System.Drawing.Point(95, 71);
            this.comboBoxArm.Name = "comboBoxArm";
            this.comboBoxArm.Size = new System.Drawing.Size(50, 21);
            this.comboBoxArm.TabIndex = 7;
            // 
            // comboBoxS2
            // 
            this.comboBoxS2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.comboBoxS2.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxS2.FormattingEnabled = true;
            this.comboBoxS2.Items.AddRange(new object[] {
            "CH.1",
            "CH.2",
            "CH.3",
            "CH.4",
            "CH.5",
            "CH.6",
            "CH.7",
            "CH.8"});
            this.comboBoxS2.Location = new System.Drawing.Point(95, 44);
            this.comboBoxS2.Name = "comboBoxS2";
            this.comboBoxS2.Size = new System.Drawing.Size(50, 21);
            this.comboBoxS2.TabIndex = 6;
            // 
            // comboBoxS1
            // 
            this.comboBoxS1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.comboBoxS1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxS1.FormattingEnabled = true;
            this.comboBoxS1.Items.AddRange(new object[] {
            "CH.1",
            "CH.2",
            "CH.3",
            "CH.4",
            "CH.5",
            "CH.6",
            "CH.7",
            "CH.8"});
            this.comboBoxS1.Location = new System.Drawing.Point(95, 17);
            this.comboBoxS1.Name = "comboBoxS1";
            this.comboBoxS1.Size = new System.Drawing.Size(50, 21);
            this.comboBoxS1.TabIndex = 5;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(7, 74);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(67, 13);
            this.label4.TabIndex = 4;
            this.label4.Text = "Arm (switch):";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(7, 47);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(41, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Servo2";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(7, 20);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(65, 13);
            this.label3.TabIndex = 0;
            this.label3.Text = "Servo1(HV):";
            // 
            // groupBox3
            // 
            this.groupBox3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox3.Controls.Add(this.checkBoxRevS2);
            this.groupBox3.Controls.Add(this.checkBoxRevS1);
            this.groupBox3.Controls.Add(this.checkBoxRevM3);
            this.groupBox3.Controls.Add(this.checkBoxRevM2);
            this.groupBox3.Controls.Add(this.checkBoxRevM1);
            this.groupBox3.Location = new System.Drawing.Point(813, 36);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(198, 133);
            this.groupBox3.TabIndex = 8;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Reverse Motors/Servos";
            // 
            // checkBoxRevS2
            // 
            this.checkBoxRevS2.AutoSize = true;
            this.checkBoxRevS2.Location = new System.Drawing.Point(6, 111);
            this.checkBoxRevS2.Name = "checkBoxRevS2";
            this.checkBoxRevS2.Size = new System.Drawing.Size(109, 17);
            this.checkBoxRevS2.TabIndex = 15;
            this.checkBoxRevS2.Text = "Servo2 Reversed";
            this.checkBoxRevS2.UseVisualStyleBackColor = true;
            // 
            // checkBoxRevS1
            // 
            this.checkBoxRevS1.AutoSize = true;
            this.checkBoxRevS1.Location = new System.Drawing.Point(6, 88);
            this.checkBoxRevS1.Name = "checkBoxRevS1";
            this.checkBoxRevS1.Size = new System.Drawing.Size(133, 17);
            this.checkBoxRevS1.TabIndex = 14;
            this.checkBoxRevS1.Text = "Servo1 (HV) Reversed";
            this.checkBoxRevS1.UseVisualStyleBackColor = true;
            // 
            // checkBoxRevM3
            // 
            this.checkBoxRevM3.AutoSize = true;
            this.checkBoxRevM3.Location = new System.Drawing.Point(6, 65);
            this.checkBoxRevM3.Name = "checkBoxRevM3";
            this.checkBoxRevM3.Size = new System.Drawing.Size(126, 17);
            this.checkBoxRevM3.TabIndex = 13;
            this.checkBoxRevM3.Text = "DC Motor3 Reversed";
            this.checkBoxRevM3.UseVisualStyleBackColor = true;
            // 
            // checkBoxRevM2
            // 
            this.checkBoxRevM2.AutoSize = true;
            this.checkBoxRevM2.Location = new System.Drawing.Point(6, 42);
            this.checkBoxRevM2.Name = "checkBoxRevM2";
            this.checkBoxRevM2.Size = new System.Drawing.Size(126, 17);
            this.checkBoxRevM2.TabIndex = 12;
            this.checkBoxRevM2.Text = "DC Motor2 Reversed";
            this.checkBoxRevM2.UseVisualStyleBackColor = true;
            // 
            // checkBoxRevM1
            // 
            this.checkBoxRevM1.AutoSize = true;
            this.checkBoxRevM1.Location = new System.Drawing.Point(6, 19);
            this.checkBoxRevM1.Name = "checkBoxRevM1";
            this.checkBoxRevM1.Size = new System.Drawing.Size(126, 17);
            this.checkBoxRevM1.TabIndex = 11;
            this.checkBoxRevM1.Text = "DC Motor1 Reversed";
            this.checkBoxRevM1.UseVisualStyleBackColor = true;
            // 
            // groupBox4
            // 
            this.groupBox4.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox4.Controls.Add(this.comboBoxM3ASS);
            this.groupBox4.Controls.Add(this.comboBoxM2ASS);
            this.groupBox4.Controls.Add(this.comboBoxM1ASS);
            this.groupBox4.Controls.Add(this.label7);
            this.groupBox4.Controls.Add(this.label6);
            this.groupBox4.Controls.Add(this.label5);
            this.groupBox4.Location = new System.Drawing.Point(813, 175);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(198, 103);
            this.groupBox4.TabIndex = 9;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "DC motor assignment";
            // 
            // comboBoxM3ASS
            // 
            this.comboBoxM3ASS.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.comboBoxM3ASS.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxM3ASS.FormattingEnabled = true;
            this.comboBoxM3ASS.Items.AddRange(new object[] {
            "M. Left",
            "M. Right",
            "M. Weapon"});
            this.comboBoxM3ASS.Location = new System.Drawing.Point(104, 73);
            this.comboBoxM3ASS.Name = "comboBoxM3ASS";
            this.comboBoxM3ASS.Size = new System.Drawing.Size(88, 21);
            this.comboBoxM3ASS.TabIndex = 15;
            this.comboBoxM3ASS.SelectedIndexChanged += new System.EventHandler(this.comboBoxM3ASS_SelectedIndexChanged);
            // 
            // comboBoxM2ASS
            // 
            this.comboBoxM2ASS.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.comboBoxM2ASS.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxM2ASS.FormattingEnabled = true;
            this.comboBoxM2ASS.Items.AddRange(new object[] {
            "M. Left",
            "M. Right",
            "M. Weapon"});
            this.comboBoxM2ASS.Location = new System.Drawing.Point(104, 46);
            this.comboBoxM2ASS.Name = "comboBoxM2ASS";
            this.comboBoxM2ASS.Size = new System.Drawing.Size(88, 21);
            this.comboBoxM2ASS.TabIndex = 14;
            this.comboBoxM2ASS.SelectedIndexChanged += new System.EventHandler(this.comboBoxM2ASS_SelectedIndexChanged);
            // 
            // comboBoxM1ASS
            // 
            this.comboBoxM1ASS.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.comboBoxM1ASS.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxM1ASS.FormattingEnabled = true;
            this.comboBoxM1ASS.Items.AddRange(new object[] {
            "M. Left",
            "M. Right",
            "M. Weapon"});
            this.comboBoxM1ASS.Location = new System.Drawing.Point(104, 19);
            this.comboBoxM1ASS.Name = "comboBoxM1ASS";
            this.comboBoxM1ASS.Size = new System.Drawing.Size(88, 21);
            this.comboBoxM1ASS.TabIndex = 13;
            this.comboBoxM1ASS.SelectedIndexChanged += new System.EventHandler(this.comboBoxM1ASS_SelectedIndexChanged);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(6, 22);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(61, 13);
            this.label7.TabIndex = 10;
            this.label7.Text = "DC Motor 1";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(6, 49);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(61, 13);
            this.label6.TabIndex = 11;
            this.label6.Text = "DC Motor 2";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(6, 76);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(61, 13);
            this.label5.TabIndex = 12;
            this.label5.Text = "DC Motor 3";
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.label9);
            this.groupBox5.Controls.Add(this.numericUpDownCutOff);
            this.groupBox5.Controls.Add(this.label8);
            this.groupBox5.Controls.Add(this.numericUpDownVLim);
            this.groupBox5.Location = new System.Drawing.Point(7, 394);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(164, 74);
            this.groupBox5.TabIndex = 13;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Battery Protections";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(7, 47);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(76, 13);
            this.label9.TabIndex = 12;
            this.label9.Text = "Cutoff voltage:";
            // 
            // numericUpDownCutOff
            // 
            this.numericUpDownCutOff.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.numericUpDownCutOff.DecimalPlaces = 1;
            this.numericUpDownCutOff.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.numericUpDownCutOff.Location = new System.Drawing.Point(95, 45);
            this.numericUpDownCutOff.Maximum = new decimal(new int[] {
            35,
            0,
            0,
            65536});
            this.numericUpDownCutOff.Minimum = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.numericUpDownCutOff.Name = "numericUpDownCutOff";
            this.numericUpDownCutOff.Size = new System.Drawing.Size(51, 20);
            this.numericUpDownCutOff.TabIndex = 11;
            this.numericUpDownCutOff.Value = new decimal(new int[] {
            2,
            0,
            0,
            0});
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(7, 21);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(69, 13);
            this.label8.TabIndex = 10;
            this.label8.Text = "Limit voltage:";
            // 
            // numericUpDownVLim
            // 
            this.numericUpDownVLim.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.numericUpDownVLim.DecimalPlaces = 1;
            this.numericUpDownVLim.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.numericUpDownVLim.Location = new System.Drawing.Point(95, 19);
            this.numericUpDownVLim.Maximum = new decimal(new int[] {
            4,
            0,
            0,
            0});
            this.numericUpDownVLim.Minimum = new decimal(new int[] {
            28,
            0,
            0,
            65536});
            this.numericUpDownVLim.Name = "numericUpDownVLim";
            this.numericUpDownVLim.Size = new System.Drawing.Size(51, 20);
            this.numericUpDownVLim.TabIndex = 0;
            this.numericUpDownVLim.Value = new decimal(new int[] {
            28,
            0,
            0,
            65536});
            // 
            // progressBarSave
            // 
            this.progressBarSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.progressBarSave.Location = new System.Drawing.Point(918, 484);
            this.progressBarSave.Name = "progressBarSave";
            this.progressBarSave.Size = new System.Drawing.Size(85, 23);
            this.progressBarSave.TabIndex = 14;
            this.progressBarSave.Visible = false;
            // 
            // buttonSave
            // 
            this.buttonSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonSave.Enabled = false;
            this.buttonSave.Location = new System.Drawing.Point(918, 513);
            this.buttonSave.Name = "buttonSave";
            this.buttonSave.Size = new System.Drawing.Size(85, 23);
            this.buttonSave.TabIndex = 15;
            this.buttonSave.Text = "Save on FAC";
            this.buttonSave.UseVisualStyleBackColor = true;
            this.buttonSave.Click += new System.EventHandler(this.buttonSave_Click);
            // 
            // buttonRead
            // 
            this.buttonRead.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonRead.Enabled = false;
            this.buttonRead.Location = new System.Drawing.Point(817, 513);
            this.buttonRead.Name = "buttonRead";
            this.buttonRead.Size = new System.Drawing.Size(95, 23);
            this.buttonRead.TabIndex = 16;
            this.buttonRead.Text = "Read from FAC";
            this.buttonRead.UseVisualStyleBackColor = true;
            this.buttonRead.Click += new System.EventHandler(this.buttonRead_Click);
            // 
            // buttonCheck
            // 
            this.buttonCheck.Location = new System.Drawing.Point(286, 7);
            this.buttonCheck.Name = "buttonCheck";
            this.buttonCheck.Size = new System.Drawing.Size(49, 23);
            this.buttonCheck.TabIndex = 17;
            this.buttonCheck.Text = "Check";
            this.buttonCheck.UseVisualStyleBackColor = true;
            this.buttonCheck.Click += new System.EventHandler(this.buttonCheck_Click);
            // 
            // pictureBoxTick
            // 
            this.pictureBoxTick.Image = ((System.Drawing.Image)(resources.GetObject("pictureBoxTick.Image")));
            this.pictureBoxTick.ImageLocation = "";
            this.pictureBoxTick.Location = new System.Drawing.Point(164, 7);
            this.pictureBoxTick.Name = "pictureBoxTick";
            this.pictureBoxTick.Size = new System.Drawing.Size(25, 25);
            this.pictureBoxTick.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pictureBoxTick.TabIndex = 0;
            this.pictureBoxTick.TabStop = false;
            this.pictureBoxTick.Visible = false;
            // 
            // imagePanel
            // 
            this.imagePanel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.imagePanel.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("imagePanel.BackgroundImage")));
            this.imagePanel.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.imagePanel.Controls.Add(this.linkLabel1);
            this.imagePanel.Controls.Add(this.pictureBox1);
            this.imagePanel.Controls.Add(this.labelM1);
            this.imagePanel.Controls.Add(this.label17);
            this.imagePanel.Controls.Add(this.label16);
            this.imagePanel.Controls.Add(this.label14);
            this.imagePanel.Controls.Add(this.labelRX);
            this.imagePanel.Controls.Add(this.label18);
            this.imagePanel.Controls.Add(this.label13);
            this.imagePanel.Controls.Add(this.labelM2);
            this.imagePanel.Controls.Add(this.labelM3);
            this.imagePanel.Controls.Add(this.label10);
            this.imagePanel.Location = new System.Drawing.Point(177, 36);
            this.imagePanel.Name = "imagePanel";
            this.imagePanel.Size = new System.Drawing.Size(630, 504);
            this.imagePanel.TabIndex = 11;
            // 
            // linkLabel1
            // 
            this.linkLabel1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.linkLabel1.AutoSize = true;
            this.linkLabel1.Location = new System.Drawing.Point(439, 3);
            this.linkLabel1.Name = "linkLabel1";
            this.linkLabel1.Size = new System.Drawing.Size(107, 13);
            this.linkLabel1.TabIndex = 22;
            this.linkLabel1.TabStop = true;
            this.linkLabel1.Text = "Floppy Lab Link Tree";
            this.linkLabel1.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.linkLabel1_LinkClicked);
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("pictureBox1.BackgroundImage")));
            this.pictureBox1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.pictureBox1.Location = new System.Drawing.Point(538, 3);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(89, 66);
            this.pictureBox1.TabIndex = 22;
            this.pictureBox1.TabStop = false;
            // 
            // labelM1
            // 
            this.labelM1.AutoSize = true;
            this.labelM1.Font = new System.Drawing.Font("Eras Bold ITC", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelM1.Location = new System.Drawing.Point(116, 362);
            this.labelM1.Name = "labelM1";
            this.labelM1.Size = new System.Drawing.Size(49, 16);
            this.labelM1.TabIndex = 26;
            this.labelM1.Text = "NONE";
            this.labelM1.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Font = new System.Drawing.Font("Eras Bold ITC", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label17.Location = new System.Drawing.Point(414, 278);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(141, 16);
            this.label17.TabIndex = 25;
            this.label17.Text = "AUX2 (ADC and I/O)";
            this.label17.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Font = new System.Drawing.Font("Eras Bold ITC", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label16.Location = new System.Drawing.Point(414, 331);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(141, 16);
            this.label16.TabIndex = 25;
            this.label16.Text = "AUX1 (ADC and I/O)";
            this.label16.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Font = new System.Drawing.Font("Eras Bold ITC", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label14.Location = new System.Drawing.Point(242, 93);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(56, 16);
            this.label14.TabIndex = 24;
            this.label14.Text = "2S LIPO";
            this.label14.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // labelRX
            // 
            this.labelRX.AutoSize = true;
            this.labelRX.Font = new System.Drawing.Font("Eras Bold ITC", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelRX.Location = new System.Drawing.Point(317, 93);
            this.labelRX.Name = "labelRX";
            this.labelRX.Size = new System.Drawing.Size(112, 16);
            this.labelRX.TabIndex = 23;
            this.labelRX.Text = "RECIEVER PWM";
            this.labelRX.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Font = new System.Drawing.Font("Eras Bold ITC", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label18.Location = new System.Drawing.Point(363, 393);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(61, 16);
            this.label18.TabIndex = 23;
            this.label18.Text = "SERVO2";
            this.label18.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Font = new System.Drawing.Font("Eras Bold ITC", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label13.Location = new System.Drawing.Point(265, 393);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(92, 16);
            this.label13.TabIndex = 23;
            this.label13.Text = "SERVO1(HV)";
            this.label13.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // labelM2
            // 
            this.labelM2.AutoSize = true;
            this.labelM2.Font = new System.Drawing.Font("Eras Bold ITC", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelM2.Location = new System.Drawing.Point(116, 308);
            this.labelM2.Name = "labelM2";
            this.labelM2.Size = new System.Drawing.Size(49, 16);
            this.labelM2.TabIndex = 22;
            this.labelM2.Text = "NONE";
            this.labelM2.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // labelM3
            // 
            this.labelM3.AutoSize = true;
            this.labelM3.Font = new System.Drawing.Font("Eras Bold ITC", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelM3.Location = new System.Drawing.Point(116, 255);
            this.labelM3.Name = "labelM3";
            this.labelM3.Size = new System.Drawing.Size(49, 16);
            this.labelM3.TabIndex = 21;
            this.labelM3.Text = "NONE";
            this.labelM3.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Font = new System.Drawing.Font("Eras Bold ITC", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label10.Location = new System.Drawing.Point(99, 197);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(114, 16);
            this.label10.TabIndex = 20;
            this.label10.Text = "POWER SWITCH";
            // 
            // buttonDisconnect
            // 
            this.buttonDisconnect.Enabled = false;
            this.buttonDisconnect.Location = new System.Drawing.Point(4, 7);
            this.buttonDisconnect.Name = "buttonDisconnect";
            this.buttonDisconnect.Size = new System.Drawing.Size(75, 23);
            this.buttonDisconnect.TabIndex = 18;
            this.buttonDisconnect.Text = "Disconnect";
            this.buttonDisconnect.UseVisualStyleBackColor = true;
            this.buttonDisconnect.Click += new System.EventHandler(this.buttonDisconnect_Click);
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.checkBoxRXPWM);
            this.groupBox6.Location = new System.Drawing.Point(7, 341);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(164, 47);
            this.groupBox6.TabIndex = 19;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "Other";
            // 
            // checkBoxRXPWM
            // 
            this.checkBoxRXPWM.AutoSize = true;
            this.checkBoxRXPWM.Location = new System.Drawing.Point(10, 19);
            this.checkBoxRXPWM.Name = "checkBoxRXPWM";
            this.checkBoxRXPWM.Size = new System.Drawing.Size(148, 17);
            this.checkBoxRXPWM.TabIndex = 10;
            this.checkBoxRXPWM.Text = "RX is PWM (Unchk PPM)";
            this.checkBoxRXPWM.UseVisualStyleBackColor = true;
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Font = new System.Drawing.Font("Eras Bold ITC", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label12.Location = new System.Drawing.Point(125, 307);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(94, 16);
            this.label12.TabIndex = 22;
            this.label12.Text = "MOTOR LEFT";
            this.label12.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Font = new System.Drawing.Font("Eras Bold ITC", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label15.Location = new System.Drawing.Point(317, 93);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(118, 16);
            this.label15.TabIndex = 23;
            this.label15.Text = "RECUEVER PWM";
            this.label15.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // fileToolStripMenuItem1
            // 
            this.fileToolStripMenuItem1.Name = "fileToolStripMenuItem1";
            this.fileToolStripMenuItem1.Size = new System.Drawing.Size(32, 19);
            // 
            // importToolStripMenuItem1
            // 
            this.importToolStripMenuItem1.Name = "importToolStripMenuItem1";
            this.importToolStripMenuItem1.Size = new System.Drawing.Size(32, 19);
            // 
            // exportToolStripMenuItem1
            // 
            this.exportToolStripMenuItem1.Name = "exportToolStripMenuItem1";
            this.exportToolStripMenuItem1.Size = new System.Drawing.Size(32, 19);
            // 
            // aboutToolStripMenuItem1
            // 
            this.aboutToolStripMenuItem1.Name = "aboutToolStripMenuItem1";
            this.aboutToolStripMenuItem1.Size = new System.Drawing.Size(32, 19);
            // 
            // buttonImport
            // 
            this.buttonImport.Location = new System.Drawing.Point(936, 7);
            this.buttonImport.Name = "buttonImport";
            this.buttonImport.Size = new System.Drawing.Size(75, 23);
            this.buttonImport.TabIndex = 20;
            this.buttonImport.Text = "Import";
            this.buttonImport.UseVisualStyleBackColor = true;
            this.buttonImport.Click += new System.EventHandler(this.buttonImport_Click);
            // 
            // buttonExport
            // 
            this.buttonExport.Location = new System.Drawing.Point(855, 7);
            this.buttonExport.Name = "buttonExport";
            this.buttonExport.Size = new System.Drawing.Size(75, 23);
            this.buttonExport.TabIndex = 21;
            this.buttonExport.Text = "Export";
            this.buttonExport.UseVisualStyleBackColor = true;
            this.buttonExport.Click += new System.EventHandler(this.buttonExport_Click);
            // 
            // FacSettingsTool
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1016, 548);
            this.Controls.Add(this.buttonExport);
            this.Controls.Add(this.buttonImport);
            this.Controls.Add(this.groupBox6);
            this.Controls.Add(this.buttonDisconnect);
            this.Controls.Add(this.pictureBoxTick);
            this.Controls.Add(this.buttonCheck);
            this.Controls.Add(this.buttonRead);
            this.Controls.Add(this.buttonSave);
            this.Controls.Add(this.progressBarSave);
            this.Controls.Add(this.groupBox5);
            this.Controls.Add(this.imagePanel);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.gBoxMovements);
            this.Controls.Add(this.comboBoxPorts);
            this.Controls.Add(this.buttonConnect);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "FacSettingsTool";
            this.Text = "FAC Settings Tool 2.0";
            this.gBoxMovements.ResumeLayout(false);
            this.gBoxMovements.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownCutOff)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownVLim)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxTick)).EndInit();
            this.imagePanel.ResumeLayout(false);
            this.imagePanel.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.groupBox6.ResumeLayout(false);
            this.groupBox6.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button buttonConnect;
        private System.Windows.Forms.ComboBox comboBoxPorts;
        private System.Windows.Forms.GroupBox gBoxMovements;
        private System.Windows.Forms.Label lableTH;
        private System.Windows.Forms.ComboBox comboBoxTH;
        private System.Windows.Forms.Label lableST;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox comboBoxST;
        private System.Windows.Forms.ComboBox comboBoxWP;
        private System.Windows.Forms.ComboBox comboBoxArm;
        private System.Windows.Forms.ComboBox comboBoxS2;
        private System.Windows.Forms.ComboBox comboBoxS1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.CheckBox checkBoxArm;
        private System.Windows.Forms.CheckBox checkBoxWPDD;
        private System.Windows.Forms.CheckBox checkBoxTankOn;
        private System.Windows.Forms.CheckBox checkBoxRevS2;
        private System.Windows.Forms.CheckBox checkBoxRevS1;
        private System.Windows.Forms.CheckBox checkBoxRevM3;
        private System.Windows.Forms.CheckBox checkBoxRevM2;
        private System.Windows.Forms.CheckBox checkBoxRevM1;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.ComboBox comboBoxM1ASS;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ComboBox comboBoxM3ASS;
        private System.Windows.Forms.ComboBox comboBoxM2ASS;
        private System.Windows.Forms.Panel imagePanel;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.NumericUpDown numericUpDownCutOff;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.NumericUpDown numericUpDownVLim;
        private System.Windows.Forms.ProgressBar progressBarSave;
        private System.Windows.Forms.Button buttonSave;
        private System.Windows.Forms.Button buttonRead;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.Button buttonCheck;
        private System.Windows.Forms.PictureBox pictureBoxTick;
        private System.Windows.Forms.Button buttonDisconnect;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.CheckBox checkBoxRXPWM;
        private System.Windows.Forms.Label labelM3;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label labelM2;
        private System.Windows.Forms.Label labelRX;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.Label labelM1;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem importToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem exportToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem1;
        private System.Windows.Forms.Button buttonImport;
        private System.Windows.Forms.Button buttonExport;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.LinkLabel linkLabel1;
    }
}

