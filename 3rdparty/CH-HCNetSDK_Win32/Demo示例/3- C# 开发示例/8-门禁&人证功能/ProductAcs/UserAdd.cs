using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AcsDemo.ProductAcs
{
    public partial class UserAdd : Form
    {
        public UserAdd()
        {
            InitializeComponent();
        }

        private void buttonCardInfoAdd_Click(object sender, EventArgs e)
        {
            CardAdd cardAdd = new CardAdd();
            cardAdd.ShowDialog();
        }
    }
}
