namespace Calculadora
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            resultBox = new TextBox();
            button1 = new Button();
            button2 = new Button();
            button3 = new Button();
            button4 = new Button();
            button5 = new Button();
            button6 = new Button();
            button7 = new Button();
            button8 = new Button();
            button9 = new Button();
            button10 = new Button();
            button11 = new Button();
            button12 = new Button();
            button13 = new Button();
            button14 = new Button();
            button15 = new Button();
            button16 = new Button();
            button17 = new Button();
            button18 = new Button();
            button19 = new Button();
            button20 = new Button();
            button21 = new Button();
            button22 = new Button();
            button23 = new Button();
            button24 = new Button();
            SuspendLayout();
            // 
            // resultBox
            // 
            resultBox.BackColor = SystemColors.Control;
            resultBox.BorderStyle = BorderStyle.None;
            resultBox.Font = new Font("Segoe UI", 21.75F, FontStyle.Bold, GraphicsUnit.Point, 0);
            resultBox.Location = new Point(12, 40);
            resultBox.MaxLength = 17;
            resultBox.Multiline = true;
            resultBox.Name = "resultBox";
            resultBox.ReadOnly = true;
            resultBox.Size = new Size(318, 45);
            resultBox.TabIndex = 0;
            resultBox.Text = "0";
            resultBox.TextAlign = HorizontalAlignment.Right;
            // 
            // button1
            // 
            button1.FlatStyle = FlatStyle.System;
            button1.Font = new Font("Segoe UI", 21.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            button1.Location = new Point(12, 355);
            button1.Name = "button1";
            button1.Size = new Size(75, 60);
            button1.TabIndex = 1;
            button1.Tag = "1";
            button1.Text = "1";
            button1.UseVisualStyleBackColor = true;
            button1.Click += addNumber;
            // 
            // button2
            // 
            button2.FlatStyle = FlatStyle.System;
            button2.Font = new Font("Segoe UI", 21.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            button2.Location = new Point(93, 355);
            button2.Name = "button2";
            button2.Size = new Size(75, 60);
            button2.TabIndex = 2;
            button2.Tag = "2";
            button2.Text = "2";
            button2.UseVisualStyleBackColor = true;
            button2.Click += addNumber;
            // 
            // button3
            // 
            button3.FlatStyle = FlatStyle.System;
            button3.Font = new Font("Segoe UI", 21.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            button3.Location = new Point(174, 355);
            button3.Name = "button3";
            button3.Size = new Size(75, 60);
            button3.TabIndex = 3;
            button3.Tag = "3";
            button3.Text = "3";
            button3.UseVisualStyleBackColor = true;
            button3.Click += addNumber;
            // 
            // button4
            // 
            button4.FlatStyle = FlatStyle.System;
            button4.Font = new Font("Segoe UI", 21.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            button4.Location = new Point(174, 289);
            button4.Name = "button4";
            button4.Size = new Size(75, 60);
            button4.TabIndex = 6;
            button4.Tag = "6";
            button4.Text = "6";
            button4.UseVisualStyleBackColor = true;
            button4.Click += addNumber;
            // 
            // button5
            // 
            button5.FlatStyle = FlatStyle.System;
            button5.Font = new Font("Segoe UI", 21.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            button5.Location = new Point(93, 289);
            button5.Name = "button5";
            button5.Size = new Size(75, 60);
            button5.TabIndex = 5;
            button5.Tag = "5";
            button5.Text = "5";
            button5.UseVisualStyleBackColor = true;
            button5.Click += addNumber;
            // 
            // button6
            // 
            button6.FlatStyle = FlatStyle.System;
            button6.Font = new Font("Segoe UI", 21.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            button6.Location = new Point(12, 289);
            button6.Name = "button6";
            button6.Size = new Size(75, 60);
            button6.TabIndex = 4;
            button6.Tag = "4";
            button6.Text = "4";
            button6.UseVisualStyleBackColor = true;
            button6.Click += addNumber;
            // 
            // button7
            // 
            button7.FlatStyle = FlatStyle.System;
            button7.Font = new Font("Segoe UI", 21.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            button7.Location = new Point(174, 223);
            button7.Name = "button7";
            button7.Size = new Size(75, 60);
            button7.TabIndex = 9;
            button7.Tag = "9";
            button7.Text = "9";
            button7.UseVisualStyleBackColor = true;
            button7.Click += addNumber;
            // 
            // button8
            // 
            button8.FlatStyle = FlatStyle.System;
            button8.Font = new Font("Segoe UI", 21.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            button8.Location = new Point(93, 223);
            button8.Name = "button8";
            button8.Size = new Size(75, 60);
            button8.TabIndex = 8;
            button8.Tag = "8";
            button8.Text = "8";
            button8.UseVisualStyleBackColor = true;
            button8.Click += addNumber;
            // 
            // button9
            // 
            button9.FlatStyle = FlatStyle.System;
            button9.Font = new Font("Segoe UI", 21.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            button9.Location = new Point(12, 223);
            button9.Name = "button9";
            button9.Size = new Size(75, 60);
            button9.TabIndex = 7;
            button9.Tag = "7";
            button9.Text = "7";
            button9.UseVisualStyleBackColor = true;
            button9.Click += addNumber;
            // 
            // button10
            // 
            button10.FlatStyle = FlatStyle.System;
            button10.Font = new Font("Segoe UI", 21.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            button10.Location = new Point(174, 421);
            button10.Name = "button10";
            button10.Size = new Size(75, 60);
            button10.TabIndex = 12;
            button10.Tag = ".";
            button10.Text = ".";
            button10.UseVisualStyleBackColor = true;
            button10.Click += dotButton;
            // 
            // button11
            // 
            button11.FlatStyle = FlatStyle.System;
            button11.Font = new Font("Segoe UI", 21.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            button11.Location = new Point(93, 421);
            button11.Name = "button11";
            button11.Size = new Size(75, 60);
            button11.TabIndex = 11;
            button11.Tag = "0";
            button11.Text = "0";
            button11.UseVisualStyleBackColor = true;
            button11.Click += addNumber;
            // 
            // button12
            // 
            button12.FlatStyle = FlatStyle.System;
            button12.Font = new Font("Segoe UI", 21.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            button12.Location = new Point(12, 421);
            button12.Name = "button12";
            button12.Size = new Size(75, 60);
            button12.TabIndex = 10;
            button12.Tag = "±";
            button12.Text = "±";
            button12.UseVisualStyleBackColor = true;
            button12.Click += plusMinus;
            // 
            // button13
            // 
            button13.FlatStyle = FlatStyle.System;
            button13.Font = new Font("Segoe UI", 21.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            button13.Location = new Point(255, 421);
            button13.Name = "button13";
            button13.Size = new Size(75, 60);
            button13.TabIndex = 16;
            button13.Tag = "=";
            button13.Text = "=";
            button13.UseVisualStyleBackColor = true;
            button13.Click += resultButton;
            // 
            // button14
            // 
            button14.FlatStyle = FlatStyle.System;
            button14.Font = new Font("Segoe UI", 21.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            button14.Location = new Point(255, 223);
            button14.Name = "button14";
            button14.Size = new Size(75, 60);
            button14.TabIndex = 15;
            button14.Tag = "×";
            button14.Text = "×";
            button14.UseVisualStyleBackColor = true;
            button14.Click += operatorButton;
            // 
            // button15
            // 
            button15.FlatStyle = FlatStyle.System;
            button15.Font = new Font("Segoe UI", 21.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            button15.Location = new Point(255, 289);
            button15.Name = "button15";
            button15.Size = new Size(75, 60);
            button15.TabIndex = 14;
            button15.Tag = "-";
            button15.Text = "-";
            button15.UseVisualStyleBackColor = true;
            button15.Click += operatorButton;
            // 
            // button16
            // 
            button16.FlatStyle = FlatStyle.System;
            button16.Font = new Font("Segoe UI", 21.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            button16.Location = new Point(255, 355);
            button16.Name = "button16";
            button16.Size = new Size(75, 60);
            button16.TabIndex = 13;
            button16.Tag = "+";
            button16.Text = "+";
            button16.UseVisualStyleBackColor = true;
            button16.Click += operatorButton;
            // 
            // button17
            // 
            button17.FlatStyle = FlatStyle.System;
            button17.Font = new Font("Segoe UI", 21.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            button17.Location = new Point(255, 157);
            button17.Name = "button17";
            button17.Size = new Size(75, 60);
            button17.TabIndex = 20;
            button17.Tag = "÷";
            button17.Text = "÷";
            button17.UseVisualStyleBackColor = true;
            button17.Click += operatorButton;
            // 
            // button18
            // 
            button18.FlatStyle = FlatStyle.System;
            button18.Font = new Font("Segoe UI", 21.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            button18.Location = new Point(174, 157);
            button18.Name = "button18";
            button18.Size = new Size(75, 60);
            button18.TabIndex = 19;
            button18.Tag = "√";
            button18.Text = "√";
            button18.UseVisualStyleBackColor = true;
            button18.Click += operatorButton;
            // 
            // button19
            // 
            button19.FlatStyle = FlatStyle.System;
            button19.Font = new Font("Segoe UI", 21.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            button19.Location = new Point(93, 157);
            button19.Name = "button19";
            button19.Size = new Size(75, 60);
            button19.TabIndex = 18;
            button19.Tag = "²";
            button19.Text = "x²";
            button19.UseVisualStyleBackColor = true;
            button19.Click += operatorButton;
            // 
            // button20
            // 
            button20.FlatStyle = FlatStyle.System;
            button20.Font = new Font("Segoe UI", 21.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            button20.Location = new Point(12, 157);
            button20.Name = "button20";
            button20.Size = new Size(75, 60);
            button20.TabIndex = 17;
            button20.Tag = "i";
            button20.Text = "1/x";
            button20.UseVisualStyleBackColor = true;
            button20.Click += operatorButton;
            // 
            // button21
            // 
            button21.FlatStyle = FlatStyle.System;
            button21.Font = new Font("Segoe UI", 21.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            button21.Location = new Point(255, 91);
            button21.Name = "button21";
            button21.Size = new Size(75, 60);
            button21.TabIndex = 24;
            button21.Tag = "⌫";
            button21.Text = "⌫";
            button21.UseVisualStyleBackColor = true;
            button21.Click += backspaceButton;
            // 
            // button22
            // 
            button22.FlatStyle = FlatStyle.System;
            button22.Font = new Font("Segoe UI", 21.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            button22.Location = new Point(174, 91);
            button22.Name = "button22";
            button22.Size = new Size(75, 60);
            button22.TabIndex = 23;
            button22.Tag = "C";
            button22.Text = "C";
            button22.UseVisualStyleBackColor = true;
            button22.Click += Cbutton;
            // 
            // button23
            // 
            button23.FlatStyle = FlatStyle.System;
            button23.Font = new Font("Segoe UI", 21.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            button23.Location = new Point(93, 91);
            button23.Name = "button23";
            button23.Size = new Size(75, 60);
            button23.TabIndex = 22;
            button23.Tag = "E";
            button23.Text = "CE";
            button23.UseVisualStyleBackColor = true;
            button23.Click += CEbutton;
            // 
            // button24
            // 
            button24.FlatStyle = FlatStyle.System;
            button24.Font = new Font("Segoe UI", 21.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            button24.Location = new Point(12, 91);
            button24.Name = "button24";
            button24.Size = new Size(75, 60);
            button24.TabIndex = 21;
            button24.Tag = "%";
            button24.Text = "%";
            button24.UseVisualStyleBackColor = true;
            button24.Click += operatorButton;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(346, 503);
            Controls.Add(button21);
            Controls.Add(button22);
            Controls.Add(button23);
            Controls.Add(button24);
            Controls.Add(button17);
            Controls.Add(button18);
            Controls.Add(button19);
            Controls.Add(button20);
            Controls.Add(button13);
            Controls.Add(button14);
            Controls.Add(button15);
            Controls.Add(button16);
            Controls.Add(button10);
            Controls.Add(button11);
            Controls.Add(button12);
            Controls.Add(button7);
            Controls.Add(button8);
            Controls.Add(button9);
            Controls.Add(button4);
            Controls.Add(button5);
            Controls.Add(button6);
            Controls.Add(button3);
            Controls.Add(button2);
            Controls.Add(button1);
            Controls.Add(resultBox);
            FormBorderStyle = FormBorderStyle.FixedDialog;
            Name = "Form1";
            Text = "Calculadora";
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private TextBox resultBox;
        private Button button1;
        private Button button2;
        private Button button3;
        private Button button4;
        private Button button5;
        private Button button6;
        private Button button7;
        private Button button8;
        private Button button9;
        private Button button10;
        private Button button11;
        private Button button12;
        private Button button13;
        private Button button14;
        private Button button15;
        private Button button16;
        private Button button17;
        private Button button18;
        private Button button19;
        private Button button20;
        private Button button21;
        private Button button22;
        private Button button23;
        private Button button24;
    }
}
