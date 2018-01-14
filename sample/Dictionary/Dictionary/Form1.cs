using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Globalization;


namespace Dictionary
{
    public partial class Form1 : Form
    {
        private Dictionary<string, List<string>> DicMng;
        private List<string> CsvList;

        public Form1()
        {
            InitializeComponent();
        }

        /// <summary>
        /// 読み込み処理
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Form1_Load(object sender, EventArgs e)
        {
            DicMng = new Dictionary<string, List<string>>();
            CsvList = new List<string>();

            // カレントディレクトリ取得
            string strCrtDir = Directory.GetCurrentDirectory();

            try
            {
                #region csvファイル検索
                foreach (string FileName in Directory.GetFiles(strCrtDir, "*.csv"))
                {
                    string strTemp = FileName;
                    CsvList.Add(strTemp);
                }
                #endregion

                #region csvファイル読み込み
                List<string> List = new List<string>();
                for (int i = 0; i < CsvList.Count; i++)
                {
                    // csvファイルを一行ずつ読み込む
                    StreamReader sr = new StreamReader(CsvList[i].ToString(), Encoding.GetEncoding(932));
                    while (sr.Peek() > -1)
                    {
                        // 一行ずつ格納
                        List.Add(sr.ReadLine());
                    }
                    sr.Close();
                }
                #endregion

                #region 文字列を分割
                foreach (String Line in List)
                {
                    string[] strSplit = Line.Split(',');
                    string Key = strSplit[0].ToString();
                    List<string> data = new List<string>();
                    for (int i = 1; i < strSplit.Length; i++)
                    {
                        if (strSplit[i] == null || strSplit[i] == "")
                        {
                            continue;
                        }

                        data.Add(strSplit[i].ToString());
                    }

                    DicMng.Add(Key, data);
                }
                #endregion
            }
            catch
            {
            }
        }

        /// <summary>
        /// 編集押下処理
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button1_Click(object sender, EventArgs e)
        {
            FormEdit Edit = new FormEdit(CsvList);
            Edit.ShowDialog();
            this.Form1_Load(sender,e);

        }

        private void DictionarySrh()
        {
            try
            {
                if (textBox1.Text == "" || textBox1.Text == null)
                {
                    return;
                }

                listBox1.Items.Clear();
                List<string> list = new List<string>();
                list = DicMng[textBox1.Text];
                foreach (string Data in list)
                {
                    listBox1.Items.Add(Data);
                }
            }
            catch
            {
            }
        }

        private void textBox1_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == (char)Keys.Enter)
            {
                DictionarySrh();
            }
        }
    }
}
