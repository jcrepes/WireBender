using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Windows.Forms;
using System.Diagnostics;

namespace WB3
{
    public partial class Form3 : Form
    {
        public Form3()
        {
            InitializeComponent();
            this.ResumeLayout(false);
            this.PerformLayout();
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form2_FormClosed);
        }
        private void Form2_FormClosed(object sender, FormClosedEventArgs e)
        {
            Application.Exit();
        }

        private void Form3_Load(object sender, EventArgs e)
        {

        }

        private void limitInputToDecimal(object sender, KeyPressEventArgs e) {
            if (!char.IsControl(e.KeyChar) && !char.IsDigit(e.KeyChar) && (e.KeyChar != '.'))
            {
                e.Handled = true;
            }

            // Allows decimals
            if ((e.KeyChar == '.') && ((sender as RichTextBox).Text.IndexOf('.') > -1))
            {
                e.Handled = true;
            }
            if (char.IsLetter(e.KeyChar))
            {
                MessageBox.Show("Please enter only numerical values. Decimals are allowed.");
            }
        }

        private void richTextBox1_KeyPress(object sender, KeyPressEventArgs e)
        {
            limitInputToDecimal(sender, e);
        }

        private void richTextBox2_KeyPress(object sender, KeyPressEventArgs e)
        {
            limitInputToDecimal(sender, e);
        }
        private void delete_Click(object sender, EventArgs e)
        {
            richBoxes0.RemoveAt(richBoxes0.Count - 1);
            richBoxes1.RemoveAt(richBoxes1.Count - 1);
            comboBoxes.RemoveAt(comboBoxes.Count - 1);
            this.Controls.RemoveAt(Controls.Count - 1);
            this.Controls.RemoveAt(Controls.Count - 1);
            this.Controls.RemoveAt(Controls.Count - 1);
            this.stepCount += -1;
            this.rtbCount += -2;
            this.richBox1p1 += -40;
            //deleteButton show/hide
            if (!comboBoxes.Any())
            {
                deleteButton.Visible = false;
            }
            button2.Location = new Point(241, richBox1p1 + 70);
            //button3.Location = new Point(241, richBox1p1 + 40);
            button4.Location = new Point(241, richBox1p1 + 100);
        }

        //ADD BUTTON
        private void button1_Click(object sender, EventArgs e)
        {
            richBoxes0.Add(new RichTextBox());
            //this.richBoxes1.Add(new RichTextBox());
            this.comboBoxes.Add(new ComboBox());
            rtbCount++;
            // 
            // richTextBox3
            // 
            richBoxes0[stepCount].Location = new Point(richBox1p0, (richBox1p1 + 40));
            richBoxes0[stepCount].Margin = new Padding(0);
            richBoxes0[stepCount].Multiline = false;
            richBoxes0[stepCount].Name = "richTextBox" + rtbCount.ToString();
            richBoxes0[stepCount].Size = new Size(rtbSize.Item1, rtbSize.Item2);
            richBoxes0[stepCount].TabIndex = 1;
            richBoxes0[stepCount].Text = "";
            richBoxes0[stepCount].KeyPress += new KeyPressEventHandler(this.richTextBox1_KeyPress);
            // 
            // richTextBox4
            // 
            richBoxes1.Add(new RichTextBox());
            rtbCount++;
            richBoxes1[stepCount].Location = new Point(richBox2p0, (richBox1p1 + 40));
            richBoxes1[stepCount].Multiline = false;
            richBoxes1[stepCount].Name = "richTextBox" + rtbCount.ToString();
            richBoxes1[stepCount].Size = new Size(73, rtbSize.Item2);
            richBoxes1[stepCount].TabIndex = 5;
            richBoxes1[stepCount].Text = "";
            richBoxes1[stepCount].KeyPress += new KeyPressEventHandler(this.richTextBox2_KeyPress);
            // 
            // comboBox
            // 
            comboBoxes[stepCount].FormattingEnabled = true;
            comboBoxes[stepCount].Items.AddRange(new object[] {
            "x",
            "y"});
            comboBoxes[stepCount].Location = new Point(comboPoint0, (richBox1p1 + 40));
            comboBoxes[stepCount].Name = "comboBox" + stepCount.ToString();
            comboBoxes[stepCount].Size = new Size(comboSize.Item1, comboSize.Item2);
            comboBoxes[stepCount].TabIndex = 6;
            comboBoxes[stepCount].Text = "Select...";
            //move position of back button
            button4.Location = new Point(241, richBox1p1 + 140);
            //move position of finish button
            button2.Location = new Point(241, richBox1p1 + 110);
            //Change button3 y location + 80 when preview button is showing
            //deleteButton show/hide
            if (comboBoxes.Any())
            {
                deleteButton.Visible = true;
            }

            //InitializeComponent();
            this.Controls.Add(richBoxes0[stepCount]);
            this.Controls.Add(richBoxes1[stepCount]);
            this.Controls.Add(comboBoxes[stepCount]);
            //this.ResumeLayout(false);
            //this.PerformLayout();

            //newest points set as new referrences
            richBox1p1 += 40;
            stepCount++;
        }

        public void commitToFile(string info)
        {
            using (StreamWriter file = File.AppendText(@"./WireBendOut.txt"))
            {
                file.WriteLine(info);
            }
        }

        private void emptyInput()
        {
            MessageBox.Show("Unable to finalize output until all input fields have been filled.");
        }

        //FINISH BUTTON
        private void button2_Click(object sender, EventArgs e)
        {
            if (richTextBox1.Text == "") {
                emptyInput();
                return;
            }
            if (richTextBox2.Text == "")
            {
                emptyInput();
                return;
            }
            if (comboBox1.Text == "Select...")
            {
                emptyInput();
                return;
            }
            //initial required single line of input
            float requiredLength = float.Parse(richTextBox1.Text);
            string roundLength = requiredLength.ToString("###########0.####");
            float requiredDegree = float.Parse(richTextBox2.Text);
            string roundDegree = requiredDegree.ToString("###########0.##");
            string commitString = (roundLength + " " + roundDegree + " " + comboBox1.Text+"\n");

            //added input
            for (int i = 0; i < richBoxes0.Count; i++) {

                if ((richBoxes0[i].Text == "") || (richBoxes1[i].Text == "") || (comboBoxes[i].Text == "Select..."))
                {
                    emptyInput();
                    return;
                }
                /*if ((richBoxes0[i].Text == null) || (richBoxes0[i].Text == null))
                {
                    emptyInput();
                    return;
                }*/
                float length = float.Parse(richBoxes0[i].Text);
                roundLength = length.ToString("###########0.####");
                float degree = float.Parse(richBoxes1[i].Text);
                roundDegree = degree.ToString("###########0.##");
                commitString += (roundLength + " " + roundDegree + " " + comboBoxes[i].Text + "\n");
            }
            commitToFile(commitString);
            //done writing
            MessageBox.Show("Congratulations! Your instructions have been saved.");
            Application.Exit();
        }

        //PREVIW BUTTON
        private void button3_Click(object sender, EventArgs e)
        {
            //Process.Start("directory file name and extension");
        }

        //BACK BUTTON
        private void button4_Click(object sender, EventArgs e)
        {
            File.Delete(@"./WireBendOut.txt");
            (new Form2()).Show();
            this.Hide();
        }
    }
}
