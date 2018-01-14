using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Globalization;
using System.IO;

namespace Dictionary
{
    public partial class FormEdit : Form
    {
        private Dictionary<string, List<string>> m_Dic;
        private List<string> m_CrtList;
        private List<string> m_CsvList;

        #region コンストラクタ
        /// <summary>
        /// コンストラクタ
        /// </summary>
        public FormEdit()
        {
            InitializeComponent();
        }
        public FormEdit( List<string> FileList)
        {
            m_Dic = new Dictionary<string,List<string>>();
            m_CrtList = new List<string>();
            m_CsvList = new List<string>();
            m_CsvList = FileList;

            InitializeComponent();

            // CSVリスト初期化
            InitListBox();
        }
        #endregion

        #region CSVリスト初期化
        /// <summary>
        /// CSVリスト初期化
        /// </summary>
        private void InitListBox()
        {
            listBox1.Items.Clear();
            foreach (string File in m_CsvList)
            {
                string Name = Path.GetFileName(File);
                listBox1.Items.Add(Name);
            }
            listBox1.SelectedIndex = 0;
        }
        #endregion

        #region コンボボックス初期化
        /// <summary>
        /// コンボボックス初期化
        /// </summary>
        private void InitKeyCombo()
        {
            comboBox1.Items.Clear();
            foreach( string Key in m_Dic.Keys )
            {
                comboBox1.Items.Add(Key);
            }

            comboBox1.SelectedIndex = 0;
        }
        #endregion

        #region csvファイル読み込み
        /// <summary>
        /// CSVファイル読み込み
        /// </summary>
        private void LoadCsvFile()
        {
            m_CrtList.Clear();

            // csvファイルを一行ずつ読み込む
            StreamReader sr = new StreamReader(m_CsvList[listBox1.SelectedIndex], Encoding.GetEncoding(932));
            while (sr.Peek() > -1)
            {
                // 一行ずつ格納
                m_CrtList.Add(sr.ReadLine());
            }
            sr.Close();
        }
        #endregion

        #region 保存押下処理
        /// <summary>
        /// 保存押下処理
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void 保存ToolStripMenuItem_Click(object sender, EventArgs e)
        {

            try
            {
                // カレントディレクトリ取得
                string CrtDir = Directory.GetCurrentDirectory();
                StreamWriter sw = new StreamWriter(m_CsvList[listBox1.SelectedIndex],false,Encoding.GetEncoding(932));

                // ファイル出力
                foreach( string Key in m_Dic.Keys )
                {
                    List<string> data = new List<string>();
                    data = m_Dic[Key];

                    string strKey = Key;
                    string strWrite = "";
                    foreach( string item in data )
                    {
                        strWrite += "," + item;
                    }

                    string WriteData = strKey + strWrite + "\n";

                    sw.Write(WriteData);
                }
                
                // 閉じる
                sw.Close();

                this.Close();
            }
            catch
            {
            }
            finally
            {
            }
        }
        #endregion

        #region 終了押下処理
        /// <summary>
        /// 終了押下処理
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void 終了ToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            this.Close();
        }
        #endregion

        #region CSVリスト選択処理
        /// <summary>
        /// CSVリスト選択処理
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            // 選択したCSVファイルを読み込む
            LoadCsvFile();
            InitDic();

            // コンボボックスを初期化する。
            InitKeyCombo();
        }
        #endregion

        #region 文字列を分割
        private void InitDic()
        {
            m_Dic.Clear();
            foreach (String Line in m_CrtList)
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

                m_Dic.Add(Key, data);
            }
        }
        #endregion

        private void InitItemList()
        {
            string SelKey = comboBox1.Items[comboBox1.SelectedIndex].ToString();
            List<string> ItemList = m_Dic[SelKey];

            listBox2.Items.Clear();
            foreach (string item in ItemList)
            {
                listBox2.Items.Add(item);
            }
            if (listBox2.Items.Count > 0)
            {
                listBox2.SelectedIndex = 0;
            }
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            textBox1.Text = "";
            InitItemList();
        }

        private void comboBox1_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == (char)Keys.Enter)
            {
                // キー追加
                List<string> list = new List<string>();
                m_Dic.Add(comboBox1.Text, list );
                InitKeyCombo();
            }
        }

        private void textBox1_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == (char)Keys.Enter)
            {
                // アイテム追加
                List<string> list = m_Dic[comboBox1.Text];
                list.Add(textBox1.Text);
                m_Dic[comboBox1.Text] = list;
                InitItemList();
            }
        }

        private void listBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            textBox1.Text = listBox2.Items[listBox2.SelectedIndex].ToString();
        }

        /// <summary>
        /// 削除ボタン押下処理
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button1_Click(object sender, EventArgs e)
        {
            if (listBox2.SelectedIndex < 0)
            {
                return;
            }

            List<string> list = m_Dic[comboBox1.Text];
            list.RemoveAt(listBox2.SelectedIndex);
            m_Dic[comboBox1.Text] = list;
            InitItemList();
        }
    }
}
