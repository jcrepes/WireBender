using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WB3
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
        }
        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            Application.Exit();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            (new Form2()).Show();
            //Form2 f2 = new Form2();
            //f2.Show(); //opens form2
            //System.Diagnostics.Debug.WriteLine("f2 SHOW"); 
            this.Hide(); //closes form1
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }
    }
}