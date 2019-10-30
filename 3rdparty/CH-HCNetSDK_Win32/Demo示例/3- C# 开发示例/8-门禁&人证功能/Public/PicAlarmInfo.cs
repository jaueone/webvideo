using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
        
namespace AcsDemo.Public
{
    public partial class PicAlarmInfo : Form
    {
        public string path = null;

        public PicAlarmInfo()
        {
            InitializeComponent();
        }
        public void showPic()
        {
            pictureBox.SizeMode = PictureBoxSizeMode.StretchImage;
            pictureBox.Image = Image.FromFile(path);//双引号里是图片的路径
            Show();
        }
    }
}
