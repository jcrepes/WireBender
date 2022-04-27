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

namespace WB3
{
    public partial class Form2 : Form
    {
        public Form2()
        {
            InitializeComponent();
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form2_FormClosed);
        }

        private void Form2_FormClosed(object sender, FormClosedEventArgs e)
        {
            Application.Exit();
        }

        private void Form2_Load(object sender, EventArgs e)
        {

        }
        public void commitToFile(string info)
        {
            using (StreamWriter file = File.AppendText(@"./WireBendOut.txt"))
            {
                file.WriteLine(info);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (comboBox1.Text == "Choose one...")
            {
                MessageBox.Show("Please select a wire material before proceeding.");
            }
            else if (comboBox2.Text == "Choose one...")
            {
                MessageBox.Show("Please select a unit before proceeding.");
            }
            else {
                string wireType = comboBox1.Text;
                File.Delete(@"./WireBendOut.txt");
                commitToFile(wireType);
                (new Form3()).Show();
                this.Hide();
            }
        }
    }
}
