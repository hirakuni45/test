using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.IO.Ports;
using System.Diagnostics;
using Microsoft.VisualBasic.FileIO;

namespace checkercom
{
    public partial class main_Form : Form
    {
        internal CommandSet CommandEdit;            /**< エディット中コマンドオブジェクト       */
        internal CommandSet[, ,] CommandList;          /**< コマンドリスト中のコマンド用           */
        internal int CommandListIndex = 0;          /**< コマンドリスト中のアイテム数           */

        internal PinSet PinEdit;                    /**< エディット中ピンオブジェクト       */
        internal static PinSet[,] PinList;           /**< コマンドリスト中のピン用           */
        internal int PinListIndex = 0;              /**< ピンリスト中のアイテム数           */

        internal DeviceINF DevEdit;
        internal DeviceINF[] DevList;
        internal DeviceINF[] DevListOrg;

        internal int DevListIndex = 0;

        internal CommandINF ComEdit;
        internal CommandINF[,] ComList;
        internal CommandINF[,] ComListOrg;

        internal int ComListIndex = 0;

        internal ComboBox[] PinAssignList;

        private static string AppFileDir = Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().GetModules()[0].FullyQualifiedName);
        private OpenFileDialog csv;             // ファイルを開く
        private SaveFileDialog sfd;             // 名前を付けて保存
        private MySerialPort MySerialPort;

        public Common common;         // 共通クラス
        const string CRLF = "\r\n";
        private List<string[]> csvRecords = new List<string[]>();

        static Encoding sjisEnc = Encoding.GetEncoding("Shift_JIS");

        public main_Form()
        {
            int i;
            int j, k;

            // テンポラリコマンドオブジェクトインスタンス生成
            //            CommFE = new CommandSet();

            // コマンドセット(コマンドリスト表示用)のインスタンス生成
            CommandList = new CommandSet[(int)ConstGui.MAX_DEV_ITEMLIST, (int)ConstGui.MAX_COM_ITEMLIST, (int)ConstGui.MAX_NR_ITEM_IN_COMMANDLIST];

            for (i = 0; i < (int)ConstGui.MAX_DEV_ITEMLIST; i++)
            {
                for (j = 0; j < (int)ConstGui.MAX_COM_ITEMLIST; j++)
                {
                    for (k = 0; k < (int)ConstGui.MAX_NR_ITEM_IN_COMMANDLIST; k++)
                    {
                        CommandList[i, j, k] = new CommandSet();
                    }
                }
            }
            // ピンセット(ピンリスト表示用)のインスタンス生成
            PinList = new PinSet[(int)ConstGui.MAX_DEV_ITEMLIST, (int)ConstGui.MAX_PIN_ITEMLIST];
            for (i = 0; i < (int)ConstGui.MAX_DEV_ITEMLIST; i++)
            {
                for (j = 0; j < (int)ConstGui.MAX_PIN_ITEMLIST; j++)
                {
                    PinList[i, j] = new PinSet();
                }
            }


            // デバイス情報のインスタンス生成
            DevList = new DeviceINF[(int)ConstGui.MAX_DEV_ITEMLIST];
            for (i = 0; i < (int)ConstGui.MAX_DEV_ITEMLIST; i++)
            {
                DevList[i] = new DeviceINF();
            }
            DevListOrg = new DeviceINF[(int)ConstGui.MAX_DEV_ITEMLIST];

            for (i = 0; i < (int)ConstGui.MAX_DEV_ITEMLIST; i++)
            {
                DevListOrg[i] = new DeviceINF();
            }

            // コマンド情報のインスタンス生成
            ComList = new CommandINF[(int)ConstGui.MAX_DEV_ITEMLIST, (int)ConstGui.MAX_COMINF_ITEMLIST];
            for (i = 0; i < (int)ConstGui.MAX_DEV_ITEMLIST; i++)
            {
                for (j = 0; j < (int)ConstGui.MAX_COMINF_ITEMLIST; j++)
                {
                    ComList[i, j] = new CommandINF();
                }
            }

            ComListOrg = new CommandINF[(int)ConstGui.MAX_DEV_ITEMLIST, (int)ConstGui.MAX_COMINF_ITEMLIST];
            for (i = 0; i < (int)ConstGui.MAX_DEV_ITEMLIST; i++)
            {
                for (j = 0; j < (int)ConstGui.MAX_COMINF_ITEMLIST; j++)
                {
                    ComListOrg[i, j] = new CommandINF();
                }
            }

            // 編集用コマンドオブジェクトインスタンス生成
            CommandEdit = new CommandSet();
            PinEdit = new PinSet();
            DevEdit = new DeviceINF();
            ComEdit = new CommandINF();
            common = new Common();

            InitializeComponent();

            MySerialPort = new MySerialPort();
            MySerialPort.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.SerialPort_DataReceived);

            InitializeSerialPort();

            cb_PINNAME.SelectedIndex = 0;
            cb_CommandSel.SelectedIndex = 0;
            csv = new OpenFileDialog();
            sfd = new SaveFileDialog();
            InitializeCBox();

            // ピン・アサイン、コンボボックスにアイテム設定
            PinAssignList = new ComboBox[24];
            PinAssignList[0] = comboBox1;
            PinAssignList[1] = comboBox2;
            PinAssignList[2] = comboBox3;
            PinAssignList[3] = comboBox4;
            PinAssignList[4] = comboBox5;
            PinAssignList[5] = comboBox6;
            PinAssignList[6] = comboBox7;
            PinAssignList[7] = comboBox8;
            PinAssignList[8] = comboBox9;
            PinAssignList[9] = comboBox10;
            PinAssignList[10] = comboBox11;
            PinAssignList[11] = comboBox12;
            PinAssignList[12] = comboBox13;
            PinAssignList[13] = comboBox14;
            PinAssignList[14] = comboBox15;
            PinAssignList[15] = comboBox16;
            PinAssignList[16] = comboBox17;
            PinAssignList[17] = comboBox18;
            PinAssignList[18] = comboBox19;
            PinAssignList[19] = comboBox20;
            PinAssignList[20] = comboBox21;
            PinAssignList[21] = comboBox22;
            PinAssignList[22] = comboBox23;
            PinAssignList[23] = comboBox24;
        }


        private void InitializeSerialPort()
        {
            // 利用可能なシリアルポート名の配列を取得する.
            string[] ports = System.IO.Ports.SerialPort.GetPortNames();

            // 大文字小文字を区別しない序数比較で並び替える
            StringComparer ordICCmp = StringComparer.OrdinalIgnoreCase;
            Array.Sort(ports, ordICCmp);

            // シリアルポート名をコンボボックスにセットする.
            cbx_RS232Comport.Items.Clear();
            cbx_RS232Comport2.Items.Clear();

            foreach (string port in ports)
            {
                cbx_RS232Comport.Items.Add(port);
                cbx_RS232Comport2.Items.Add(port);
            }

            // 設定ファイルのポートが利用可能ならば設定する
            if ((Common.m_SerialPort.Length > 0) && (int.Parse(Common.m_SerialPort) < cbx_RS232Comport.Items.Count))
            {
                cbx_RS232Comport.SelectedIndex = int.Parse(Common.m_SerialPort);
                cbx_RS232Comport2.SelectedIndex = int.Parse(Common.m_SerialPort);
            }
            else
            {
            }
        }

        private void InitializeCBox()
        {
            cb_PINNAME.Text = "TWE";
            cb_PINNUM.Text = "1";
            cb_PININIT.Text = cb_PINC.Text = cb_PIND.Text = cb_PINE.Text = cb_PINF.Text = "0";

            cb_property1.Text = "C";
            cb_property2.Text = "C";
            cb_property3.Text = "C";
            cb_property4.Text = "C";
            cb_property5.Text = "C";
            cb_property6.Text = "C";
            cb_property7.Text = "C";

            cb_aux01.Text = cb_aux11.Text = cb_aux21.Text = cb_aux31.Text = cb_aux41.Text = cb_aux51.Text = cb_aux61.Text = cb_aux71.Text = "0";
            cb_aux02.Text = cb_aux12.Text = cb_aux22.Text = cb_aux32.Text = cb_aux42.Text = cb_aux52.Text = cb_aux62.Text = cb_aux72.Text = "0";
            cb_aux03.Text = cb_aux13.Text = cb_aux23.Text = cb_aux33.Text = cb_aux43.Text = cb_aux53.Text = cb_aux63.Text = cb_aux73.Text = "0";
            cb_aux04.Text = cb_aux14.Text = cb_aux24.Text = cb_aux34.Text = cb_aux44.Text = cb_aux54.Text = cb_aux64.Text = cb_aux74.Text = "0";
            cb_aux05.Text = cb_aux15.Text = cb_aux25.Text = cb_aux35.Text = cb_aux45.Text = cb_aux55.Text = cb_aux65.Text = cb_aux75.Text = "0";
            cb_aux06.Text = cb_aux16.Text = cb_aux26.Text = cb_aux36.Text = cb_aux46.Text = cb_aux56.Text = cb_aux66.Text = cb_aux76.Text = "0";
            cb_aux07.Text = cb_aux17.Text = cb_aux27.Text = cb_aux37.Text = cb_aux47.Text = cb_aux57.Text = cb_aux67.Text = cb_aux77.Text = "0";

            cb_aux0init.Text = cb_aux1init.Text = cb_aux2init.Text = cb_aux3init.Text = cb_aux4init.Text = cb_aux5init.Text = cb_aux6init.Text = cb_aux7init.Text = "0";

            cb_phase.Text = "φ1";
            cb_DEFphase.Text = "φ1";
            cb_base2.Text = cb_base3.Text = cb_base4.Text = cb_base5.Text = cb_base6.Text = cb_base7.Text = "DEC";
        }

        //////////////////////////////////////////////////////////////////////////////
        //                                                                          //
        // FORM CLOSE EVENT                                                         //
        //                                                                          //
        //////////////////////////////////////////////////////////////////////////////
        private void frm_Main_FormClosing(object sender, FormClosingEventArgs e)
        {
            try
            {
                if (MessageBox.Show("終了しますか?", "Information", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
                {
                }
                else
                {
                    e.Cancel = true;
                    return;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        //////////////////////////////////////////////////////////////////////////////
        //                                                                          //
        // シリアルポート接続                                                       //
        //                                                                          //
        //////////////////////////////////////////////////////////////////////////////
        private void btn_Port_Click(object sender, EventArgs e)
        {
            btn_ComPort.Enabled = false;
            btn_ComPort2.Enabled = false;
            // ボタンのラベル取得する
            if (btn_ComPort.Text == "開始")
            {
                // 「開始」ならポートオープン
                try
                {
                    if (MySerialPort.IsOpen)
                    {
                        // すでにポート開いてますよー
                        MessageBox.Show("すでに接続できています", "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                    else
                    {
                        MySerialPort.PortName = cbx_RS232Comport2.Text = cbx_RS232Comport.Text;
                        //                        SerialPort.BaudRate = int.Parse(cb_BaudRate.Text);
                        MySerialPort.Encoding = Encoding.ASCII;
                        MySerialPort.BaudRate = 115200;
                        MySerialPort.ReadTimeout = 10000;
                        MySerialPort.Open();
                        MySerialPort.DiscardInBuffer();
                        MySerialPort.DiscardOutBuffer();

                        // フォームの書き換えを保留
                        this.SuspendLayout();

                        btn_ComPort.Text = "停止";
                        btn_ComPort2.Text = "停止";


                        btn_ComPort.Enabled = true;
                        btn_ComPort2.Enabled = true;

                    }
                    // フォームの書き換えを再開
                    this.ResumeLayout();
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    btn_ComPort.Enabled = true;
                    btn_ComPort2.Enabled = true;
                }
            }
            else
            {
                // 「停止」ならポートクローズ
                try
                {
                    if (MySerialPort.IsOpen)
                    {
                        btn_ComPort.Enabled = true;
                        btn_ComPort2.Enabled = true;
                        MySerialPort.DiscardInBuffer();
                        MySerialPort.DiscardOutBuffer();
                        MySerialPort.Close();

                        // フォームの書き換えを保留
                        this.SuspendLayout();

                        btn_ComPort.Text = "開始";
                        btn_ComPort2.Text = "開始";

                        // フォームの書き換えを再開
                        this.ResumeLayout();
                    }
                    btn_ComPort.Enabled = true;
                    btn_ComPort2.Enabled = true;
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    btn_ComPort.Enabled = true;
                    btn_ComPort2.Enabled = true;
                }
                btn_ComPort.Text = "開始";
                btn_ComPort2.Text = "開始";

            }

        }


        //////////////////////////////////////////////////////////////////////////////
        //                                                                          //
        // シリアルポート接続  2                                                     //
        //                                                                          //
        //////////////////////////////////////////////////////////////////////////////
        private void btn_Port2_Click(object sender, EventArgs e)
        {
            btn_ComPort.Enabled = false;
            btn_ComPort2.Enabled = false;
            // ボタンのラベル取得する
            if (btn_ComPort2.Text == "開始")
            {
                // 「開始」ならポートオープン
                try
                {
                    if (MySerialPort.IsOpen)
                    {
                        // すでにポート開いてますよー
                        MessageBox.Show("すでに接続できています", "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                    else
                    {
                        MySerialPort.PortName = cbx_RS232Comport.Text = cbx_RS232Comport2.Text;
                        //                        SerialPort.BaudRate = int.Parse(cb_BaudRate.Text);
                        MySerialPort.Encoding = Encoding.ASCII;
                        MySerialPort.BaudRate = 115200;
                        MySerialPort.ReadTimeout = 10000;
                        MySerialPort.Open();
                        MySerialPort.DiscardInBuffer();
                        MySerialPort.DiscardOutBuffer();

                        // フォームの書き換えを保留
                        this.SuspendLayout();

                        btn_ComPort.Text = "停止";
                        btn_ComPort2.Text = "停止";


                        btn_ComPort.Enabled = true;
                        btn_ComPort2.Enabled = true;

                    }
                    // フォームの書き換えを再開
                    this.ResumeLayout();
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    btn_ComPort.Enabled = true;
                    btn_ComPort2.Enabled = true;
                }
            }
            else
            {
                // 「停止」ならポートクローズ
                try
                {
                    if (MySerialPort.IsOpen)
                    {
                        btn_ComPort.Enabled = true;
                        btn_ComPort2.Enabled = true;
                        MySerialPort.DiscardInBuffer();
                        MySerialPort.DiscardOutBuffer();
                        MySerialPort.Close();

                        // フォームの書き換えを保留
                        this.SuspendLayout();

                        btn_ComPort.Text = "開始";
                        btn_ComPort2.Text = "開始";

                        // フォームの書き換えを再開
                        this.ResumeLayout();
                    }
                    btn_ComPort.Enabled = true;
                    btn_ComPort2.Enabled = true;
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    btn_ComPort.Enabled = true;
                    btn_ComPort2.Enabled = true;
                }
                btn_ComPort.Text = "開始";
                btn_ComPort2.Text = "開始";

            }

        }

        //////////////////////////////////////////////////////////////////////////////
        //                                                                          //
        // シリアル受信 & ACK                  割り込み使用しない                   //
        //                                                                          //
        //////////////////////////////////////////////////////////////////////////////
        private void SerialPort_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
        }

        private void GetCommandParameterControl(ref CommandSet com)
        {
            // 初期化  ゴミを残さないため
            com.REPEAT_CNT = "1";
            com.PROPERTY = "C";
            com.AUX_DATA = "0";
            com.CONSTANT = "0";
            com.OUT_CNT = "1";
            com.ARG_BASE = "DEC";
            com.ARG_NUM = "1";
            com.ARG_DIV = "1";
            com.ARG_ADD = "0";
            com.ARG_UPPERBIT = "0";
            com.ARG_LOWERBIT = "1";
            com.ARG_HW = "0";
            com.ARG_DELAY = "0";
            com.COMMENT = "";


            // コマンド設定
            com.CommandID = (CommID)cb_CommandSel.SelectedIndex + 1;//初期値出力が0なので+1
            com.REPEAT_CNT = nu_repeat2.Value.ToString();

            switch (com.CommandID)
            {
                case CommID.定数出力:
                    com.CONSTANT = tb_const1.Text;
                    com.REPEAT_CNT = nu_repeat1.Value.ToString();
                    com.COMMENT = tb_COMMENT1.Text;
                    com.PROPERTY = cb_property1.Text;
                    com.AUX_DATA = Convert.ToString((
                                    (int.Parse(cb_aux71.Text)) * 0x80 +
                                    (int.Parse(cb_aux61.Text)) * 0x40 +
                                    (int.Parse(cb_aux51.Text)) * 0x20 +
                                    (int.Parse(cb_aux41.Text)) * 0x10 +
                                    (int.Parse(cb_aux31.Text)) * 0x08 +
                                    (int.Parse(cb_aux21.Text)) * 0x04 +
                                    (int.Parse(cb_aux11.Text)) * 0x02 +
                                    (int.Parse(cb_aux01.Text)) * 0x01
                                    ), 16);
                    break;
                case CommID.引数出力:
                    com.REPEAT_CNT = nu_repeat2.Value.ToString();
                    com.COMMENT = tb_COMMENT2.Text;
                    com.PROPERTY = cb_property2.Text;
                    com.AUX_DATA = Convert.ToString((
                                    (int.Parse(cb_aux72.Text)) * 0x80 +
                                    (int.Parse(cb_aux62.Text)) * 0x40 +
                                    (int.Parse(cb_aux52.Text)) * 0x20 +
                                    (int.Parse(cb_aux42.Text)) * 0x10 +
                                    (int.Parse(cb_aux32.Text)) * 0x08 +
                                    (int.Parse(cb_aux22.Text)) * 0x04 +
                                    (int.Parse(cb_aux12.Text)) * 0x02 +
                                    (int.Parse(cb_aux02.Text)) * 0x01
                                    ), 16);
                    com.OUT_CNT = nu_outcnt2.Value.ToString();
                    com.ARG_BASE = cb_base2.Text;
                    com.ARG_NUM = nu_argument2.Value.ToString();
                    break;
                case CommID.除算bit出力:
                    com.REPEAT_CNT = nu_repeat1.Value.ToString();
                    com.COMMENT = tb_COMMENT3.Text;
                    com.PROPERTY = cb_property3.Text;
                    com.AUX_DATA = Convert.ToString((
                                    (int.Parse(cb_aux73.Text)) * 0x80 +
                                    (int.Parse(cb_aux63.Text)) * 0x40 +
                                    (int.Parse(cb_aux53.Text)) * 0x20 +
                                    (int.Parse(cb_aux43.Text)) * 0x10 +
                                    (int.Parse(cb_aux33.Text)) * 0x08 +
                                    (int.Parse(cb_aux23.Text)) * 0x04 +
                                    (int.Parse(cb_aux13.Text)) * 0x02 +
                                    (int.Parse(cb_aux03.Text)) * 0x01
                                    ), 16);
                    com.OUT_CNT = nu_outcnt3.Value.ToString();
                    com.ARG_BASE = cb_base3.Text;
                    com.ARG_NUM = nu_argument3.Value.ToString();
                    com.ARG_DIV = nu_div3.Value.ToString();
                    break;
                case CommID.除加算出力:
                    com.REPEAT_CNT = nu_repeat4.Value.ToString();
                    com.COMMENT = tb_COMMENT4.Text;
                    com.PROPERTY = cb_property4.Text;
                    com.AUX_DATA = Convert.ToString((
                                    (int.Parse(cb_aux74.Text)) * 0x80 +
                                    (int.Parse(cb_aux64.Text)) * 0x40 +
                                    (int.Parse(cb_aux54.Text)) * 0x20 +
                                    (int.Parse(cb_aux44.Text)) * 0x10 +
                                    (int.Parse(cb_aux34.Text)) * 0x08 +
                                    (int.Parse(cb_aux24.Text)) * 0x04 +
                                    (int.Parse(cb_aux14.Text)) * 0x02 +
                                    (int.Parse(cb_aux04.Text)) * 0x01
                                    ), 16);
                    com.OUT_CNT = nu_outcnt4.Value.ToString();
                    com.ARG_BASE = cb_base4.Text;
                    com.ARG_NUM = nu_argument4.Value.ToString();
                    com.ARG_DIV = nu_div4.Value.ToString();
                    com.ARG_ADD = nu_add4.Value.ToString();
                    break;
                case CommID.TDS出力:
                    com.REPEAT_CNT = nu_repeat5.Value.ToString();
                    com.COMMENT = tb_COMMENT5.Text;
                    com.PROPERTY = cb_property5.Text;
                    com.AUX_DATA = Convert.ToString((
                                    (int.Parse(cb_aux75.Text)) * 0x80 +
                                    (int.Parse(cb_aux65.Text)) * 0x40 +
                                    (int.Parse(cb_aux55.Text)) * 0x20 +
                                    (int.Parse(cb_aux45.Text)) * 0x10 +
                                    (int.Parse(cb_aux35.Text)) * 0x08 +
                                    (int.Parse(cb_aux25.Text)) * 0x04 +
                                    (int.Parse(cb_aux15.Text)) * 0x02 +
                                    (int.Parse(cb_aux05.Text)) * 0x01
                                    ), 16);
                    com.OUT_CNT = nu_outcnt5.Value.ToString();
                    com.ARG_BASE = cb_base5.Text;
                    com.ARG_NUM = nu_argument5.Value.ToString();
                    com.ARG_DIV = nu_div5.Value.ToString();
                    break;
                case CommID.Chip出力:
                    com.REPEAT_CNT = nu_repeat6.Value.ToString();
                    com.COMMENT = tb_COMMENT6.Text;
                    com.PROPERTY = cb_property6.Text;
                    com.AUX_DATA = Convert.ToString((
                                    (int.Parse(cb_aux76.Text)) * 0x80 +
                                    (int.Parse(cb_aux66.Text)) * 0x40 +
                                    (int.Parse(cb_aux56.Text)) * 0x20 +
                                    (int.Parse(cb_aux46.Text)) * 0x10 +
                                    (int.Parse(cb_aux36.Text)) * 0x08 +
                                    (int.Parse(cb_aux26.Text)) * 0x04 +
                                    (int.Parse(cb_aux16.Text)) * 0x02 +
                                    (int.Parse(cb_aux06.Text)) * 0x01
                                    ), 16);
                    com.OUT_CNT = nu_outcnt6.Value.ToString();
                    com.ARG_BASE = cb_base6.Text;
                    com.ARG_NUM = nu_argument6.Value.ToString();
                    break;
                case CommID.指定bit出力:
                    com.REPEAT_CNT = nu_repeat7.Value.ToString();
                    com.COMMENT = tb_COMMENT7.Text;
                    com.PROPERTY = cb_property7.Text;
                    com.AUX_DATA = Convert.ToString((
                                    (int.Parse(cb_aux77.Text)) * 0x80 +
                                    (int.Parse(cb_aux67.Text)) * 0x40 +
                                    (int.Parse(cb_aux57.Text)) * 0x20 +
                                    (int.Parse(cb_aux47.Text)) * 0x10 +
                                    (int.Parse(cb_aux37.Text)) * 0x08 +
                                    (int.Parse(cb_aux27.Text)) * 0x04 +
                                    (int.Parse(cb_aux17.Text)) * 0x02 +
                                    (int.Parse(cb_aux07.Text)) * 0x01
                                    ), 16);
                    com.OUT_CNT = nu_outcnt7.Value.ToString();
                    com.ARG_BASE = cb_base7.Text;
                    com.ARG_NUM = nu_argument7.Value.ToString();
                    com.ARG_LOWERBIT = nu_lowerbit7.Value.ToString();
                    com.ARG_UPPERBIT = nu_upperbit7.Value.ToString();
                    break;
                case CommID.HW出力:
                    com.REPEAT_CNT = nu_repeat8.Value.ToString();
                    com.COMMENT = tb_COMMENT8.Text;
                    com.ARG_HW = tb_HW8.Text;
                    break;
                case CommID.Delay:
                    com.REPEAT_CNT = nu_repeat9.Value.ToString();
                    com.COMMENT = tb_COMMENT9.Text;
                    com.ARG_DELAY = nu_Delay9.Value.ToString();
                    break;

                default:
                    break;
            }
        }

        /** 追加ボタン押下げ時処理.
        * 　@param sender イベントの発生元オブジェクト
        * 　@param e      イベントの引数
        */
        private void buttonAppend_Click(object sender, EventArgs e)
        {
            // コマンドとパラメータをコントロールから取得
            GetCommandParameterControl(ref CommandEdit);

            if (CommandListIndex < (int)ConstGui.MAX_NR_ITEM_IN_COMMANDLIST)
            {
                CommandEdit.CopyTo(CommandList[Common.SelectDev, Common.SelectCom, CommandListIndex]);
                CommandListIndex++;
                UpdateCommandList(CommandListIndex - 1);
                ComList[Common.SelectDev, Common.SelectCom].COMMAND_COUNT = lv_CommandList.Items.Count;
                ComListOrg[Common.SelectDev, Common.SelectCom].COMMAND_COUNT = lv_CommandList.Items.Count;
            }
        }

        /** 削除ボタン押下げ時処理.
         * 　@param sender イベントの発生元オブジェクト
         * 　@param e      イベントの引数
         */
        private void buttonRemove_Click(object sender, EventArgs e)
        {
            int current_index;

            if (lv_CommandList.SelectedItems.Count > 0)
            {
                current_index = (int)lv_CommandList.SelectedItems[0].Index;

                for (int i = current_index; i < CommandListIndex - 1; i++)
                {
                    CommandList[Common.SelectDev, Common.SelectCom, i + 1].CopyTo(CommandList[Common.SelectDev, Common.SelectCom, i]);
                }

                CommandListIndex--;
                if (CommandListIndex == current_index && current_index != 0)
                    current_index--;

                UpdateCommandList(current_index);
                ComList[Common.SelectDev, Common.SelectCom].COMMAND_COUNT = lv_CommandList.Items.Count;
                ComListOrg[Common.SelectDev, Common.SelectCom].COMMAND_COUNT = lv_CommandList.Items.Count;
            }
        }

        /** 変更ボタン押下げ時処理.
         * 　@param sender イベントの発生元オブジェクト
         * 　@param e      イベントの引数
         */
        private void buttonModify_Click(object sender, EventArgs e)
        {
            int current_index;

            // コマンドとパラメータをコントロールから取得
            GetCommandParameterControl(ref CommandEdit);

            if (lv_CommandList.SelectedItems.Count > 0)
            {
                current_index = (int)lv_CommandList.SelectedItems[0].Index;
                CommandEdit.CopyTo(CommandList[Common.SelectDev, Common.SelectCom, current_index]);
                UpdateCommandList(current_index);
                ComList[Common.SelectDev, Common.SelectCom].COMMAND_COUNT = lv_CommandList.Items.Count;
                ComListOrg[Common.SelectDev, Common.SelectCom].COMMAND_COUNT = lv_CommandList.Items.Count;
            }
        }

        /** 挿入ボタン押下げ時処理.
         * 　@param sender イベントの発生元オブジェクト
         * 　@param e      イベントの引数
         */
        private void buttonInsert_Click(object sender, EventArgs e)
        {
            int current_index;

            // コマンドとパラメータをコントロールから取得
            GetCommandParameterControl(ref CommandEdit);

            if (lv_CommandList.SelectedItems.Count > 0 && CommandListIndex < (int)ConstGui.MAX_NR_ITEM_IN_COMMANDLIST)
            {
                current_index = (int)lv_CommandList.SelectedItems[0].Index;

                for (int i = CommandListIndex - 1; i >= current_index; i--)
                {
                    CommandList[Common.SelectDev, Common.SelectCom, i].CopyTo(CommandList[Common.SelectDev, Common.SelectCom, i + 1]);
                }

                CommandEdit.CopyTo(CommandList[Common.SelectDev, Common.SelectCom, current_index]);
                CommandListIndex++;

                UpdateCommandList(current_index);
                ComList[Common.SelectDev, Common.SelectCom].COMMAND_COUNT = lv_CommandList.Items.Count;
                ComListOrg[Common.SelectDev, Common.SelectCom].COMMAND_COUNT = lv_CommandList.Items.Count;
            }
        }

        /** ↑ボタン押下げ時処理.
         * 　@param sender イベントの発生元オブジェクト
         * 　@param e      イベントの引数
         */
        private void buttonUpArrow_Click(object sender, EventArgs e)
        {
            int current_index;
            CommandSet temp = new CommandSet();

            if (lv_CommandList.SelectedItems.Count > 0)
            {
                current_index = (int)lv_CommandList.SelectedItems[0].Index;

                if (current_index > 0)
                {
                    CommandList[Common.SelectDev, Common.SelectCom, current_index - 1].CopyTo(temp);
                    CommandList[Common.SelectDev, Common.SelectCom, current_index].CopyTo(CommandList[Common.SelectDev, Common.SelectCom, current_index - 1]);
                    temp.CopyTo(CommandList[Common.SelectDev, Common.SelectCom, current_index]);
                    UpdateCommandList(current_index - 1);
                }
                else
                    lv_CommandList.Focus();
            }
        }

        /** ↓ボタン押下げ時処理.
         * 　@param sender イベントの発生元オブジェクト
         * 　@param e      イベントの引数
         */
        private void buttonDownArrow_Click(object sender, EventArgs e)
        {
            int current_index;
            CommandSet temp = new CommandSet();

            if (lv_CommandList.SelectedItems.Count > 0)
            {
                current_index = (int)lv_CommandList.SelectedItems[0].Index;

                if (current_index < CommandListIndex - 1)
                {
                    CommandList[Common.SelectDev, Common.SelectCom, current_index + 1].CopyTo(temp);
                    CommandList[Common.SelectDev, Common.SelectCom, current_index].CopyTo(CommandList[Common.SelectDev, Common.SelectCom, current_index + 1]);
                    temp.CopyTo(CommandList[Common.SelectDev, Common.SelectCom, current_index]);
                    UpdateCommandList(current_index + 1);
                }
                else
                    lv_CommandList.Focus();
            }
        }

        /** コマンドリスト更新処理.
         * 　@param sel 選択するインデックス
         */
        private void UpdateCommandList(int sel)
        {
            string parameters = "";
            int i;

            // リストビュー更新開始
            lv_CommandList.BeginUpdate();

            // アイテムのクリア
            lv_CommandList.Items.Clear();

            if (CommandListIndex == 0)
                goto quit;

            // コマンドリスト表示
            for (i = 0; i < CommandListIndex; i++)
            {
                ListViewItem item;
                int index = i + 1;

                item = new ListViewItem(index.ToString("d"));

                // コンボボックスからコマンド文字列を取得して追加
                item.SubItems.Add(cb_CommandSel.Items[(int)CommandList[Common.SelectDev, Common.SelectCom, i].CommandID - 1].ToString());

                // コマンドパラメータを文字列化してアイテムに追加
                CommandList[Common.SelectDev, Common.SelectCom, i].BuildParameterText(ref parameters);
                item.SubItems.Add(parameters);    // パラメータ
                item.SubItems.Add(CommandList[Common.SelectDev, Common.SelectCom, i].REPEAT_CNT);
                item.SubItems.Add(CommandList[Common.SelectDev, Common.SelectCom, i].COMMENT);

                lv_CommandList.Items.Add(item);    // 項目挿入
            }

            // フォーカスがないと選択できないのでフォーカスした上で項目を選択する
            lv_CommandList.Focus();
            lv_CommandList.Items[sel].Selected = true; // 選択する
            lv_CommandList.Items[sel].Focused = true;  // カーソルを選択行に持っていく

            // 選択行が見えるようにする
            lv_CommandList.EnsureVisible(sel);

        quit:
            // リストビュー更新終了
            lv_CommandList.EndUpdate();
        }

        //配線定義
        private void GetPinParameterControl(ref PinSet pin)
        {
            // ピン設定
            pin.PinID = (PinID)cb_PINNAME.SelectedIndex;
            pin.PinName = cb_PINNAME.Text;
            pin.PinNum = cb_PINNUM.Text;
            pin.PinInit = cb_PININIT.Text;
            pin.PinC = cb_PINC.Text;
            pin.PinD = cb_PIND.Text;
            pin.PinE = cb_PINE.Text;
            pin.PinF = cb_PINF.Text;

            if ((cb_PINNAME.Text == "TWE") || (cb_PINNAME.Text.Substring(0, 3) == "DAT") || (cb_PINNAME.Text.Substring(0, 3) == "AUX"))
            {
                pin.PinInit = "";
                pin.PinC = "";
                pin.PinD = "";
                pin.PinE = "";
                pin.PinF = "";
            }


            switch (pin.PinID)
            {
                case PinID.TWE:
                    break;
                case PinID.CLE1:
                    break;
                case PinID.CLE2:
                    break;

                default:
                    break;
            }
        }

        /** 追加ボタン押下げ時処理.
        * 　@param sender イベントの発生元オブジェクト
        * 　@param e      イベントの引数
        */
        private void buttonAppend2_Click(object sender, EventArgs e)
        {
            // コマンドとパラメータをコントロールから取得
            GetPinParameterControl(ref PinEdit);

            if (PinListIndex < (int)ConstGui.MAX_PIN_ITEMLIST)
            {
                PinEdit.CopyTo(PinList[Common.SelectDev, PinListIndex]);
                PinListIndex++;
                UpdatePinList(PinListIndex - 1);
                DevList[Common.SelectDev].PIN_COUNT = lv_PinList.Items.Count;
                DevListOrg[Common.SelectDev].PIN_COUNT = lv_PinList.Items.Count;
            }
        }

        /** 削除ボタン押下げ時処理.
         * 　@param sender イベントの発生元オブジェクト
         * 　@param e      イベントの引数
         */
        private void buttonRemove2_Click(object sender, EventArgs e)
        {
            int current_index;

            if (lv_PinList.SelectedItems.Count > 0)
            {
                current_index = (int)lv_PinList.SelectedItems[0].Index;

                for (int i = current_index; i < PinListIndex - 1; i++)
                {
                    PinList[Common.SelectDev, i + 1].CopyTo(PinList[Common.SelectDev, i]);
                }

                PinListIndex--;
                if (PinListIndex == current_index && current_index != 0)
                    current_index--;

                UpdatePinList(current_index);
                DevList[Common.SelectDev].PIN_COUNT = lv_PinList.Items.Count;
                DevListOrg[Common.SelectDev].PIN_COUNT = lv_PinList.Items.Count;
            }
        }

        /** 変更ボタン押下げ時処理.
         * 　@param sender イベントの発生元オブジェクト
         * 　@param e      イベントの引数
         */
        private void buttonModify2_Click(object sender, EventArgs e)
        {
            int current_index;

            // コマンドとパラメータをコントロールから取得
            GetPinParameterControl(ref PinEdit);

            if (lv_PinList.SelectedItems.Count > 0)
            {
                current_index = (int)lv_PinList.SelectedItems[0].Index;
                PinEdit.CopyTo(PinList[Common.SelectDev, current_index]);
                UpdatePinList(current_index);
                DevList[Common.SelectDev].PIN_COUNT = lv_PinList.Items.Count;
                DevListOrg[Common.SelectDev].PIN_COUNT = lv_PinList.Items.Count;
            }
        }

        /** 挿入ボタン押下げ時処理.
         * 　@param sender イベントの発生元オブジェクト
         * 　@param e      イベントの引数
         */
        private void buttonInsert2_Click(object sender, EventArgs e)
        {
            int current_index;

            // コマンドとパラメータをコントロールから取得
            GetPinParameterControl(ref PinEdit);

            if (lv_PinList.SelectedItems.Count > 0 && PinListIndex < (int)ConstGui.MAX_PIN_ITEMLIST)
            {
                current_index = (int)lv_PinList.SelectedItems[0].Index;

                for (int i = PinListIndex - 1; i >= current_index; i--)
                {
                    PinList[Common.SelectDev, i].CopyTo(PinList[Common.SelectDev, i + 1]);
                }

                PinEdit.CopyTo(PinList[Common.SelectDev, current_index]);
                PinListIndex++;

                UpdatePinList(current_index);
                DevList[Common.SelectDev].PIN_COUNT = lv_PinList.Items.Count;
                DevListOrg[Common.SelectDev].PIN_COUNT = lv_PinList.Items.Count;
            }
        }

        /** ↑ボタン押下げ時処理.
         * 　@param sender イベントの発生元オブジェクト
         * 　@param e      イベントの引数
         */
        private void buttonUpArrow2_Click(object sender, EventArgs e)
        {
            int current_index;
            PinSet temp = new PinSet();

            if (lv_PinList.SelectedItems.Count > 0)
            {
                current_index = (int)lv_PinList.SelectedItems[0].Index;

                if (current_index > 0)
                {
                    PinList[Common.SelectDev, current_index - 1].CopyTo(temp);
                    PinList[Common.SelectDev, current_index].CopyTo(PinList[Common.SelectDev, current_index - 1]);
                    temp.CopyTo(PinList[Common.SelectDev, current_index]);
                    UpdatePinList(current_index - 1);
                }
                else
                    lv_PinList.Focus();
            }
        }

        /** ↓ボタン押下げ時処理.
         * 　@param sender イベントの発生元オブジェクト
         * 　@param e      イベントの引数
         */
        private void buttonDownArrow2_Click(object sender, EventArgs e)
        {
            int current_index;
            PinSet temp = new PinSet();

            if (lv_PinList.SelectedItems.Count > 0)
            {
                current_index = (int)lv_PinList.SelectedItems[0].Index;

                if (current_index < PinListIndex - 1)
                {
                    PinList[Common.SelectDev, current_index + 1].CopyTo(temp);
                    PinList[Common.SelectDev, current_index].CopyTo(PinList[Common.SelectDev, current_index + 1]);
                    temp.CopyTo(PinList[Common.SelectDev, current_index]);
                    UpdatePinList(current_index + 1);
                }
                else
                    lv_PinList.Focus();
            }
        }

        /** ピンリスト更新処理.
         * 　@param sel 選択するインデックス
         */
        private void UpdatePinList(int sel)
        {
            string parameters = "";
            int i;
            int data = 0;
            int aux = 0;

            // リストビュー更新開始
            lv_PinList.BeginUpdate();

            // アイテムのクリア
            lv_PinList.Items.Clear();

            if (PinListIndex == 0)
                goto quit;

            // コマンドリスト表示
            for (i = 0; i < PinListIndex; i++)
            {
                ListViewItem item;
                int index = i + 1;

                item = new ListViewItem(index.ToString("d"));

                // コンボボックスからコマンド文字列を取得して追加
                item.SubItems.Add(cb_PINNAME.Items[(int)PinList[Common.SelectDev, i].PinID].ToString());
                string tmp = cb_PINNAME.Items[(int)PinList[Common.SelectDev, i].PinID].ToString().Substring(0, 3);
                if (cb_PINNAME.Items[(int)PinList[Common.SelectDev, i].PinID].ToString().Substring(0, 3) == "AUX") aux++;
                if (cb_PINNAME.Items[(int)PinList[Common.SelectDev, i].PinID].ToString().Substring(0, 3) == "DAT") data++;

                // コマンドパラメータを文字列化してアイテムに追加
                PinList[Common.SelectDev, i].BuildParameterText(ref parameters);
                //                item.SubItems.Add(parameters);    // パラメータ
                item.SubItems.Add(PinList[Common.SelectDev, i].PinNum);
                item.SubItems.Add(PinList[Common.SelectDev, i].PinInit);
                item.SubItems.Add(PinList[Common.SelectDev, i].PinC);
                item.SubItems.Add(PinList[Common.SelectDev, i].PinD);
                item.SubItems.Add(PinList[Common.SelectDev, i].PinE);
                item.SubItems.Add(PinList[Common.SelectDev, i].PinF);

                lv_PinList.Items.Add(item);    // 項目挿入
            }

            // フォーカスがないと選択できないのでフォーカスした上で項目を選択する
            lv_PinList.Focus();
            lv_PinList.Items[sel].Selected = true; // 選択する
            lv_PinList.Items[sel].Focused = true;  // カーソルを選択行に持っていく

            // 選択行が見えるようにする
            lv_PinList.EnsureVisible(sel);

        quit:
            // リストビュー更新終了
            lv_PinList.EndUpdate();
            Common.DATANum = data.ToString();
            Common.AUXNum = aux.ToString();
        }
        /// FILE OPEN
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void bt_OPEN_Click(object sender, EventArgs e)
        {
            csv.FileName = "";
            csv.InitialDirectory = AppFileDir;

            csv.Filter = "CSVファイル(*.csv)|*.csv|すべてのファイル(*.*)|*.*";
            csv.FilterIndex = 1;

            csv.Title = "開くファイルを選択してください";

            //ダイアログボックスを閉じる前に現在のディレクトリを復元するようにする
            csv.RestoreDirectory = true;
            //存在しないファイルの名前が指定されたとき警告を表示する
            //デフォルトでTrueなので指定する必要はない
            csv.CheckFileExists = false;
            //存在しないパスが指定されたとき警告を表示する
            //デフォルトでTrueなので指定する必要はない
            csv.CheckPathExists = true;

            //ダイアログを表示する
            if (csv.ShowDialog() == DialogResult.OK)
            {
                //OKボタンがクリックされたとき
                //選択されたファイル名を表示する
                tb_FILE.Text = csv.FileName;
            }

        }

        private void cb_CommandSel_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (cb_CommandSel.Text)
            {
                case "定数出力":
                    pn_com1para.Visible = true;
                    pn_com2para.Visible = false;
                    pn_com3para.Visible = false;
                    pn_com4para.Visible = false;
                    pn_com5para.Visible = false;
                    pn_com6para.Visible = false;
                    pn_com7para.Visible = false;
                    pn_com8para.Visible = false;
                    pn_com9para.Visible = false;
                    break;
                case "引数出力":
                    pn_com1para.Visible = false;
                    pn_com2para.Visible = true;
                    pn_com3para.Visible = false;
                    pn_com4para.Visible = false;
                    pn_com5para.Visible = false;
                    pn_com6para.Visible = false;
                    pn_com7para.Visible = false;
                    pn_com8para.Visible = false;
                    pn_com9para.Visible = false;
                    break;
                case "除算bit出力":
                    pn_com1para.Visible = false;
                    pn_com2para.Visible = false;
                    pn_com3para.Visible = true;
                    pn_com4para.Visible = false;
                    pn_com5para.Visible = false;
                    pn_com6para.Visible = false;
                    pn_com7para.Visible = false;
                    pn_com8para.Visible = false;
                    pn_com9para.Visible = false;
                    break;
                case "除加算出力":
                    pn_com1para.Visible = false;
                    pn_com2para.Visible = false;
                    pn_com3para.Visible = false;
                    pn_com4para.Visible = true;
                    pn_com5para.Visible = false;
                    pn_com6para.Visible = false;
                    pn_com7para.Visible = false;
                    pn_com8para.Visible = false;
                    pn_com9para.Visible = false;
                    break;
                case "TDS出力":
                    pn_com1para.Visible = false;
                    pn_com2para.Visible = false;
                    pn_com3para.Visible = false;
                    pn_com4para.Visible = false;
                    pn_com5para.Visible = true;
                    pn_com6para.Visible = false;
                    pn_com7para.Visible = false;
                    pn_com8para.Visible = false;
                    pn_com9para.Visible = false;
                    break;
                case "Chip出力":
                    pn_com1para.Visible = false;
                    pn_com2para.Visible = false;
                    pn_com3para.Visible = false;
                    pn_com4para.Visible = false;
                    pn_com5para.Visible = false;
                    pn_com6para.Visible = true;
                    pn_com7para.Visible = false;
                    pn_com8para.Visible = false;
                    pn_com9para.Visible = false;
                    break;
                case "指定bit出力":
                    pn_com1para.Visible = false;
                    pn_com2para.Visible = false;
                    pn_com3para.Visible = false;
                    pn_com4para.Visible = false;
                    pn_com5para.Visible = false;
                    pn_com6para.Visible = false;
                    pn_com7para.Visible = true;
                    pn_com8para.Visible = false;
                    pn_com9para.Visible = false;
                    break;
                case "HW出力":
                    pn_com1para.Visible = false;
                    pn_com2para.Visible = false;
                    pn_com3para.Visible = false;
                    pn_com4para.Visible = false;
                    pn_com5para.Visible = false;
                    pn_com6para.Visible = false;
                    pn_com7para.Visible = false;
                    pn_com8para.Visible = true;
                    pn_com9para.Visible = false;
                    break;
                case "Delay":
                    pn_com1para.Visible = false;
                    pn_com2para.Visible = false;
                    pn_com3para.Visible = false;
                    pn_com4para.Visible = false;
                    pn_com5para.Visible = false;
                    pn_com6para.Visible = false;
                    pn_com7para.Visible = false;
                    pn_com8para.Visible = false;
                    pn_com9para.Visible = true;
                    break;
                default:
                    break;

            }
        }

        private void cb_PINNAME_SelectedIndexChanged(object sender, EventArgs e)
        {
            if ((cb_PINNAME.Text == "TWE") || (cb_PINNAME.Text.Substring(0, 3) == "DAT") || (cb_PINNAME.Text.Substring(0, 3) == "AUX"))
            {
                cb_PININIT.Visible = false;
                cb_PINC.Visible = false;
                cb_PIND.Visible = false;
                cb_PINE.Visible = false;
                cb_PINF.Visible = false;
                label11.Visible = false;
                label19.Visible = false;
                label39.Visible = false;
                label41.Visible = false;
                label40.Visible = false;
            }
            else
            {
                cb_PININIT.Visible = true;
                cb_PINC.Visible = true;
                cb_PIND.Visible = true;
                cb_PINE.Visible = true;
                cb_PINF.Visible = true;
                label11.Visible = true;
                label19.Visible = true;
                label39.Visible = true;
                label41.Visible = true;
                label40.Visible = true;

            }
        }

        ////////////////
        //デバイス品種
        private void GetDevParameterControl(ref DeviceINF dev)
        {
            // 
            dev.DEV_NAME = tb_DevName.Text;
            dev.DEV_NUM = nu_DevNum.Value.ToString();
            dev.AUX_INIT = Convert.ToString((
                            (int.Parse(cb_aux7init.Text)) * 0x80 +
                            (int.Parse(cb_aux6init.Text)) * 0x40 +
                            (int.Parse(cb_aux5init.Text)) * 0x20 +
                            (int.Parse(cb_aux4init.Text)) * 0x10 +
                            (int.Parse(cb_aux3init.Text)) * 0x08 +
                            (int.Parse(cb_aux2init.Text)) * 0x04 +
                            (int.Parse(cb_aux1init.Text)) * 0x02 +
                            (int.Parse(cb_aux0init.Text)) * 0x01
                            ), 16);
            dev.PERIOD = nu_DEFperiod.Value.ToString();
            dev.PHASE = cb_DEFphase.Text;
            dev.COMMENT = tb_DevCom.Text;
        }

        /** 追加ボタン押下げ時処理.
        * 　@param sender イベントの発生元オブジェクト
        * 　@param e      イベントの引数
        */
        private void buttonAppend0_Click(object sender, EventArgs e)
        {
            if (tb_DevName.Text == "")
            {
                MessageBox.Show("デバイス品種名が正しくありません", "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            int num = sjisEnc.GetByteCount(tb_DevName.Text);
            if (num != tb_DevName.Text.Length)
            {
                MessageBox.Show("全角が使用されています。", "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            // コマンドとパラメータをコントロールから取得
            GetDevParameterControl(ref DevEdit);

            if (DevListIndex < (int)ConstGui.MAX_DEV_ITEMLIST)
            {
                DevEdit.CopyTo(DevList[DevListIndex]);
                DevListIndex++;
                UpdateDevList(DevListIndex - 1);
            }
        }

        /** 削除ボタン押下げ時処理.
         * 　@param sender イベントの発生元オブジェクト
         * 　@param e      イベントの引数
         */
        private void buttonRemove0_Click(object sender, EventArgs e)
        {
            int current_index;

            if (lv_DevList.SelectedItems.Count > 0)
            {
                current_index = (int)lv_DevList.SelectedItems[0].Index;

                for (int i = current_index; i < DevListIndex - 1; i++)
                {
                    DevList[i + 1].CopyTo(DevList[i]);
                }

                DevListIndex--;
                if (DevListIndex == current_index && current_index != 0)
                    current_index--;

                UpdateDevList(current_index);
            }
        }

        /** 変更ボタン押下げ時処理.
         * 　@param sender イベントの発生元オブジェクト
         * 　@param e      イベントの引数
         */
        private void buttonModify0_Click(object sender, EventArgs e)
        {
            int current_index;
            if (tb_DevName.Text == "")
            {
                MessageBox.Show("デバイス品種名が正しくありません", "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            int num = sjisEnc.GetByteCount(tb_DevName.Text);
            if (num != tb_DevName.Text.Length)
            {
                MessageBox.Show("全角が使用されています。", "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // コマンドとパラメータをコントロールから取得
            GetDevParameterControl(ref DevEdit);

            if (lv_DevList.SelectedItems.Count > 0)
            {
                current_index = (int)lv_DevList.SelectedItems[0].Index;
                if (DevList[current_index].OLD_NAME == "")        //変更は救う
                    DevList[current_index].OLD_NAME = DevList[current_index].DEV_NAME;

                DevEdit.CopyTo(DevList[current_index]);
                UpdateDevList(current_index);
            }
        }


        /** デバイスリスト更新処理.
         * 　@param sel 選択するインデックス
         */
        private void UpdateDevList(int sel)
        {
            //            string parameters = "";
            int i;

            // リストビュー更新開始
            lv_DevList.BeginUpdate();

            // アイテムのクリア
            lv_DevList.Items.Clear();

            if (DevListIndex == 0)
                goto quit;

            // コマンドリスト表示
            for (i = 0; i < DevListIndex; i++)
            {
                ListViewItem item;
                int index = i + 1;

                item = new ListViewItem(index.ToString("d"));

                // コンボボックスからコマンド文字列を取得して追加
                item.SubItems.Add(DevList[i].DEV_NAME);

                // コマンドパラメータを文字列化してアイテムに追加
                //                DevList[i].BuildParameterText(ref parameters);
                //                item.SubItems.Add(parameters);    // パラメータ
                item.SubItems.Add(DevList[i].DEV_NUM);
                item.SubItems.Add("0x"+DevList[i].AUX_INIT);        //AUX関連は16進
                item.SubItems.Add(DevList[i].PERIOD);
                item.SubItems.Add(DevList[i].PHASE);
                item.SubItems.Add(DevList[i].COMMENT);

                lv_DevList.Items.Add(item);    // 項目挿入
            }

            // フォーカスがないと選択できないのでフォーカスした上で項目を選択する
            lv_DevList.Focus();
            lv_DevList.Items[sel].Selected = true; // 選択する
            lv_DevList.Items[sel].Focused = true;  // カーソルを選択行に持っていく

            // 選択行が見えるようにする
            lv_DevList.EnsureVisible(sel);

        quit:
            // リストビュー更新終了
            lv_DevList.EndUpdate();
        }

        private void bt_ID_Click(object sender, EventArgs e)
        {
            int num = sjisEnc.GetByteCount(tb_ID.Text);
            if (num != tb_ID.Text.Length)
            {
                MessageBox.Show("全角が使用されています。", "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            tb_InfoID.Text = tb_ID.Text;
        }

        private void bt_DevRegist_Click(object sender, EventArgs e)
        {
            Cursor.Current = Cursors.WaitCursor;
            //コンボボックス作成
            cb_SelDev.Items.Clear();

            //Device情報変更時 ツリーの再構築
            DevListREG();
            Common.DEVICE_COUNT = lv_DevList.Items.Count;
            if (Common.DEVICE_COUNT == 0)
            {
                DevListOrg[0].COM_COUNT = 0;
                DevListOrg[0].PIN_COUNT = 0;
                ComListOrg[0, 0].COMMAND_COUNT = 0;
                ComList[0, 0].COMMAND_COUNT = 0;
            }

            for (int i = 0; i < Common.DEVICE_COUNT; i++)
            {
                DevList[i].OLD_NAME = "";
                DevList[i].CopyTo(DevListOrg[i]);
                cb_SelDev.Items.Add(DevList[i].DEV_NAME);
            }
            if (Common.DEVICE_COUNT != 0) cb_SelDev.Text = DevList[0].DEV_NAME;
            else
            {
                cb_ComNameSel.Items.Clear();
                cb_SelDev.Text = "";
                tb_DevName.Text = "";
                nu_DevNum.Value = 0;
            }
            tb_InfoKind.Text = Common.DEVICE_COUNT.ToString();
            tb_InfoRest.Text = ((int)ConstGui.MAX_DEV_ITEMLIST - Common.DEVICE_COUNT).ToString();
            Cursor.Current = Cursors.Default;


            //全てのデータ作り直し
            //
        }

        ////////////////


        ////////////////
        //コマンド情報
        private void GetCominfParameterControl(ref CommandINF com)
        {
            // 
            com.COM_NAME = tb_CommandName.Text;
            com.COMMENT = tb_CNComment.Text;
        }

        /** 追加ボタン押下げ時処理.
        * 　@param sender イベントの発生元オブジェクト
        * 　@param e      イベントの引数
        */
        private void buttonAppendC_Click(object sender, EventArgs e)
        {
            if (tb_CommandName.Text == "")
            {
                MessageBox.Show("コマンド名が正しくありません", "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            int num = sjisEnc.GetByteCount(tb_CommandName.Text);
            if (num != tb_CommandName.Text.Length)
            {
                MessageBox.Show("全角が使用されています。", "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            // コマンドとパラメータをコントロールから取得
            GetCominfParameterControl(ref ComEdit);

            if (ComListIndex < (int)ConstGui.MAX_COM_ITEMLIST)
            {
                ComEdit.CopyTo(ComList[Common.SelectDev, ComListIndex]);
                ComListIndex++;
                UpdateComList(ComListIndex - 1);
                DevList[Common.SelectDev].COM_COUNT = lv_ComNameList.Items.Count;
                DevList[Common.SelectDev].COM_NUM = lv_ComNameList.Items.Count.ToString();
            }
        }

        /** 削除ボタン押下げ時処理.
         * 　@param sender イベントの発生元オブジェクト
         * 　@param e      イベントの引数
         */
        private void buttonRemoveC_Click(object sender, EventArgs e)
        {
            int current_index;

            if (lv_ComNameList.SelectedItems.Count > 0)
            {
                current_index = (int)lv_ComNameList.SelectedItems[0].Index;

                for (int i = current_index; i < ComListIndex - 1; i++)
                {
                    ComList[Common.SelectDev, i + 1].CopyTo(ComList[Common.SelectDev, i]);
                }

                ComListIndex--;
                if (ComListIndex == current_index && current_index != 0)
                    current_index--;

                UpdateComList(current_index);
                DevList[Common.SelectDev].COM_COUNT = lv_ComNameList.Items.Count;
                DevList[Common.SelectDev].COM_NUM = lv_ComNameList.Items.Count.ToString();
            }
        }

        /** 変更ボタン押下げ時処理.
         * 　@param sender イベントの発生元オブジェクト
         * 　@param e      イベントの引数
         */
        private void buttonModifyC_Click(object sender, EventArgs e)
        {
            int current_index;

            if (tb_CommandName.Text == "")
            {
                MessageBox.Show("コマンド名が正しくありません", "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            int num = sjisEnc.GetByteCount(tb_CommandName.Text);
            if (num != tb_CommandName.Text.Length)
            {
                MessageBox.Show("全角が使用されています。", "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            // コマンドとパラメータをコントロールから取得
            GetCominfParameterControl(ref ComEdit);

            if (lv_ComNameList.SelectedItems.Count > 0)
            {
                current_index = (int)lv_ComNameList.SelectedItems[0].Index;
                if (ComList[Common.SelectDev, current_index].OLD_NAME == "")      //変更は救ってあげる
                    ComList[Common.SelectDev, current_index].OLD_NAME = ComList[Common.SelectDev, current_index].COM_NAME;

                ComEdit.CopyTo(ComList[Common.SelectDev, current_index]);
                UpdateComList(current_index);
                DevList[Common.SelectDev].COM_COUNT = lv_ComNameList.Items.Count;
                DevList[Common.SelectDev].COM_NUM = lv_ComNameList.Items.Count.ToString();
            }
        }


        /** コマンド名リスト更新処理.
         * 　@param sel 選択するインデックス
         */
        private void UpdateComList(int sel)
        {
            //            string parameters = "";
            int i;

            // リストビュー更新開始
            lv_ComNameList.BeginUpdate();

            // アイテムのクリア
            lv_ComNameList.Items.Clear();

            if (ComListIndex == 0)
                goto quit;

            // コマンドリスト表示
            for (i = 0; i < ComListIndex; i++)
            {
                ListViewItem item;
                int index = i + 1;

                item = new ListViewItem(index.ToString("d"));

                // コンボボックスからコマンド文字列を取得して追加
                item.SubItems.Add(ComList[Common.SelectDev, i].COM_NAME);

                // コマンドパラメータを文字列化してアイテムに追加
                //                DevList[i].BuildParameterText(ref parameters);
                //                item.SubItems.Add(parameters);    // パラメータ
                item.SubItems.Add(ComList[Common.SelectDev, i].COMMENT);

                lv_ComNameList.Items.Add(item);    // 項目挿入
            }

            // フォーカスがないと選択できないのでフォーカスした上で項目を選択する
            lv_ComNameList.Focus();
            lv_ComNameList.Items[sel].Selected = true; // 選択する
            lv_ComNameList.Items[sel].Focused = true;  // カーソルを選択行に持っていく

            // 選択行が見えるようにする
            lv_ComNameList.EnsureVisible(sel);

        quit:
            // リストビュー更新終了
            lv_ComNameList.EndUpdate();
        }

        private void bt_ComRegist_Click(object sender, EventArgs e)
        {

            Cursor.Current = Cursors.WaitCursor;
            cb_ComNameSel.Items.Clear();

            //CommandListの再構築
            ComListREG();
            DevList[Common.SelectDev].COM_COUNT = lv_ComNameList.Items.Count;
            DevList[Common.SelectDev].COM_NUM = lv_ComNameList.Items.Count.ToString();
            DevListOrg[Common.SelectDev].COM_COUNT = lv_ComNameList.Items.Count;
            DevListOrg[Common.SelectDev].COM_NUM = lv_ComNameList.Items.Count.ToString();

            for (int i = 0; i < DevList[Common.SelectDev].COM_COUNT; i++)
            {
                ComList[Common.SelectDev, i].OLD_NAME = "";
                ComList[Common.SelectDev, i].CopyTo(ComListOrg[Common.SelectDev, i]);
                cb_ComNameSel.Items.Add(ComList[Common.SelectDev, i].COM_NAME);
            }
            if (lv_ComNameList.Items.Count != 0)
                cb_ComNameSel.Text = ComList[Common.SelectDev, 0].COM_NAME;
            else
            {
                cb_ComNameSel.Text = "";
                ComListOrg[Common.SelectDev, 0].COMMAND_COUNT = 0;
            }
            Cursor.Current = Cursors.Default;
        }

        //タブ選択時 特殊処理
        private void tabControl1_Selecting(object sender, TabControlCancelEventArgs e)
        {
            if (e.TabPageIndex == 1)    //デバイス品種
            {
                for (int i = 0; i < Common.DEVICE_COUNT; i++)
                {
                    DevListIndex = Common.DEVICE_COUNT;
                    DevListOrg[i].CopyTo(DevList[i]);
                }
                UpdateDevList(0);
            }
            if (e.TabPageIndex == 2)    //配線定義
            {
                PinListIndex = DevListOrg[Common.SelectDev].PIN_COUNT;
                UpdatePinList(0);
            }

            if (e.TabPageIndex == 3)    //コマンド名定義
            {
                //ORG → TEMP
                for (int i = 0; i < DevListOrg[Common.SelectDev].COM_COUNT; i++)
                {
                    ComListOrg[Common.SelectDev, i].CopyTo(ComList[Common.SelectDev, i]);
                }

                ComListIndex = DevListOrg[Common.SelectDev].COM_COUNT;
                UpdateComList(0);
            }
            if (e.TabPageIndex == 4)    //コマンド定義
            {
                tb_DATANum.Text = Common.DATANum;
                tb_AUXNum.Text = Common.AUXNum;
                CommandListIndex = ComListOrg[Common.SelectDev, Common.SelectCom].COMMAND_COUNT;
                UpdateCommandList(0);

            }
            if (e.TabPageIndex == 7)    //配線コピー定義
            {
                setupPinAssign_gui();
            }
        }

        private void lv_CommandList_SelectedIndexChanged(object sender, EventArgs e)
        {
            int aux, bit;

            if (lv_CommandList.SelectedItems.Count > 0)
            {
                int i = lv_CommandList.SelectedItems[0].Index;
                switch (CommandList[Common.SelectDev, Common.SelectCom, i].CommandID)
                {
                    case CommID.定数出力:
                        cb_CommandSel.Text = "定数出力";
                        aux = Convert.ToInt32(CommandList[Common.SelectDev, Common.SelectCom, i].AUX_DATA, 16);
                        bit = ((aux >> 7) & 1);
                        cb_aux71.Text = bit.ToString();
                        bit = ((aux >> 6) & 1);
                        cb_aux61.Text = bit.ToString();
                        bit = ((aux >> 5) & 1);
                        cb_aux51.Text = bit.ToString();
                        bit = ((aux >> 4) & 1);
                        cb_aux41.Text = bit.ToString();
                        bit = ((aux >> 3) & 1);
                        cb_aux31.Text = bit.ToString();
                        bit = ((aux >> 2) & 1);
                        cb_aux21.Text = bit.ToString();
                        bit = ((aux >> 1) & 1);
                        cb_aux11.Text = bit.ToString();
                        bit = ((aux >> 0) & 1);
                        cb_aux01.Text = bit.ToString();
                        cb_property1.Text = CommandList[Common.SelectDev, Common.SelectCom, i].PROPERTY;
                        nu_repeat1.Value = int.Parse(CommandList[Common.SelectDev, Common.SelectCom, i].REPEAT_CNT);
                        tb_COMMENT1.Text = CommandList[Common.SelectDev, Common.SelectCom, i].COMMENT;
                        tb_const1.Text = CommandList[Common.SelectDev, Common.SelectCom, i].CONSTANT;
                        break;
                    case CommID.引数出力:
                        cb_CommandSel.Text = "引数出力";
                        aux = Convert.ToInt32(CommandList[Common.SelectDev, Common.SelectCom, i].AUX_DATA, 16);
                        bit = ((aux >> 7) & 1);
                        cb_aux72.Text = bit.ToString();
                        bit = ((aux >> 6) & 1);
                        cb_aux62.Text = bit.ToString();
                        bit = ((aux >> 5) & 1);
                        cb_aux52.Text = bit.ToString();
                        bit = ((aux >> 4) & 1);
                        cb_aux42.Text = bit.ToString();
                        bit = ((aux >> 3) & 1);
                        cb_aux32.Text = bit.ToString();
                        bit = ((aux >> 2) & 1);
                        cb_aux22.Text = bit.ToString();
                        bit = ((aux >> 1) & 1);
                        cb_aux12.Text = bit.ToString();
                        bit = ((aux >> 0) & 1);
                        cb_aux02.Text = bit.ToString();
                        cb_property2.Text = CommandList[Common.SelectDev, Common.SelectCom, i].PROPERTY;
                        nu_repeat2.Value = int.Parse(CommandList[Common.SelectDev, Common.SelectCom, i].REPEAT_CNT);
                        tb_COMMENT2.Text = CommandList[Common.SelectDev, Common.SelectCom, i].COMMENT;
                        nu_outcnt2.Value = int.Parse(CommandList[Common.SelectDev, Common.SelectCom, i].OUT_CNT);
                        cb_base2.Text = CommandList[Common.SelectDev, Common.SelectCom, i].ARG_BASE;
                        nu_argument2.Value = int.Parse(CommandList[Common.SelectDev, Common.SelectCom, i].ARG_NUM);
                        break;
                    case CommID.除算bit出力:
                        cb_CommandSel.Text = "除算bit出力";
                        aux = Convert.ToInt32(CommandList[Common.SelectDev, Common.SelectCom, i].AUX_DATA, 16);
                        bit = ((aux >> 7) & 1);
                        cb_aux73.Text = bit.ToString();
                        bit = ((aux >> 6) & 1);
                        cb_aux63.Text = bit.ToString();
                        bit = ((aux >> 5) & 1);
                        cb_aux53.Text = bit.ToString();
                        bit = ((aux >> 4) & 1);
                        cb_aux43.Text = bit.ToString();
                        bit = ((aux >> 3) & 1);
                        cb_aux33.Text = bit.ToString();
                        bit = ((aux >> 2) & 1);
                        cb_aux23.Text = bit.ToString();
                        bit = ((aux >> 1) & 1);
                        cb_aux13.Text = bit.ToString();
                        bit = ((aux >> 0) & 1);
                        cb_aux03.Text = bit.ToString();
                        cb_property3.Text = CommandList[Common.SelectDev, Common.SelectCom, i].PROPERTY;
                        nu_repeat3.Value = int.Parse(CommandList[Common.SelectDev, Common.SelectCom, i].REPEAT_CNT);
                        tb_COMMENT3.Text = CommandList[Common.SelectDev, Common.SelectCom, i].COMMENT;
                        nu_outcnt3.Value = int.Parse(CommandList[Common.SelectDev, Common.SelectCom, i].OUT_CNT);
                        cb_base3.Text = CommandList[Common.SelectDev, Common.SelectCom, i].ARG_BASE;
                        nu_argument3.Value = int.Parse(CommandList[Common.SelectDev, Common.SelectCom, i].ARG_NUM);
                        nu_div3.Value = int.Parse(CommandList[Common.SelectDev, Common.SelectCom, i].ARG_DIV);
                        break;
                    case CommID.除加算出力:
                        cb_CommandSel.Text = "除加算出力";
                        aux = Convert.ToInt32(CommandList[Common.SelectDev, Common.SelectCom, i].AUX_DATA, 16);
                        bit = ((aux >> 7) & 1);
                        cb_aux74.Text = bit.ToString();
                        bit = ((aux >> 6) & 1);
                        cb_aux64.Text = bit.ToString();
                        bit = ((aux >> 5) & 1);
                        cb_aux54.Text = bit.ToString();
                        bit = ((aux >> 4) & 1);
                        cb_aux44.Text = bit.ToString();
                        bit = ((aux >> 3) & 1);
                        cb_aux34.Text = bit.ToString();
                        bit = ((aux >> 2) & 1);
                        cb_aux24.Text = bit.ToString();
                        bit = ((aux >> 1) & 1);
                        cb_aux14.Text = bit.ToString();
                        bit = ((aux >> 0) & 1);
                        cb_aux04.Text = bit.ToString();
                        cb_property4.Text = CommandList[Common.SelectDev, Common.SelectCom, i].PROPERTY;
                        nu_repeat4.Value = int.Parse(CommandList[Common.SelectDev, Common.SelectCom, i].REPEAT_CNT);
                        tb_COMMENT4.Text = CommandList[Common.SelectDev, Common.SelectCom, i].COMMENT;
                        nu_outcnt4.Value = int.Parse(CommandList[Common.SelectDev, Common.SelectCom, i].OUT_CNT);
                        cb_base4.Text = CommandList[Common.SelectDev, Common.SelectCom, i].ARG_BASE;
                        nu_argument4.Value = int.Parse(CommandList[Common.SelectDev, Common.SelectCom, i].ARG_NUM);
                        nu_div4.Value = int.Parse(CommandList[Common.SelectDev, Common.SelectCom, i].ARG_DIV);
                        nu_add4.Value = int.Parse(CommandList[Common.SelectDev, Common.SelectCom, i].ARG_ADD);
                        break;
                    case CommID.TDS出力:
                        cb_CommandSel.Text = "TDS出力";
                        aux = Convert.ToInt32(CommandList[Common.SelectDev, Common.SelectCom, i].AUX_DATA, 16);
                        bit = ((aux >> 7) & 1);
                        cb_aux75.Text = bit.ToString();
                        bit = ((aux >> 6) & 1);
                        cb_aux65.Text = bit.ToString();
                        bit = ((aux >> 5) & 1);
                        cb_aux55.Text = bit.ToString();
                        bit = ((aux >> 4) & 1);
                        cb_aux45.Text = bit.ToString();
                        bit = ((aux >> 3) & 1);
                        cb_aux35.Text = bit.ToString();
                        bit = ((aux >> 2) & 1);
                        cb_aux25.Text = bit.ToString();
                        bit = ((aux >> 1) & 1);
                        cb_aux15.Text = bit.ToString();
                        bit = ((aux >> 0) & 1);
                        cb_aux05.Text = bit.ToString();
                        cb_property5.Text = CommandList[Common.SelectDev, Common.SelectCom, i].PROPERTY;
                        nu_repeat5.Value = int.Parse(CommandList[Common.SelectDev, Common.SelectCom, i].REPEAT_CNT);
                        tb_COMMENT5.Text = CommandList[Common.SelectDev, Common.SelectCom, i].COMMENT;
                        nu_outcnt5.Value = int.Parse(CommandList[Common.SelectDev, Common.SelectCom, i].OUT_CNT);
                        cb_base5.Text = CommandList[Common.SelectDev, Common.SelectCom, i].ARG_BASE;
                        nu_argument5.Value = int.Parse(CommandList[Common.SelectDev, Common.SelectCom, i].ARG_NUM);
                        nu_div5.Value = int.Parse(CommandList[Common.SelectDev, Common.SelectCom, i].ARG_DIV);
                        break;
                    case CommID.Chip出力:
                        cb_CommandSel.Text = "Chip出力";
                        aux = Convert.ToInt32(CommandList[Common.SelectDev, Common.SelectCom, i].AUX_DATA, 16);
                        bit = ((aux >> 7) & 1);
                        cb_aux76.Text = bit.ToString();
                        bit = ((aux >> 6) & 1);
                        cb_aux66.Text = bit.ToString();
                        bit = ((aux >> 5) & 1);
                        cb_aux56.Text = bit.ToString();
                        bit = ((aux >> 4) & 1);
                        cb_aux46.Text = bit.ToString();
                        bit = ((aux >> 3) & 1);
                        cb_aux36.Text = bit.ToString();
                        bit = ((aux >> 2) & 1);
                        cb_aux26.Text = bit.ToString();
                        bit = ((aux >> 1) & 1);
                        cb_aux16.Text = bit.ToString();
                        bit = ((aux >> 0) & 1);
                        cb_aux06.Text = bit.ToString();
                        cb_property6.Text = CommandList[Common.SelectDev, Common.SelectCom, i].PROPERTY;
                        nu_repeat6.Value = int.Parse(CommandList[Common.SelectDev, Common.SelectCom, i].REPEAT_CNT);
                        tb_COMMENT6.Text = CommandList[Common.SelectDev, Common.SelectCom, i].COMMENT;
                        nu_outcnt6.Value = int.Parse(CommandList[Common.SelectDev, Common.SelectCom, i].OUT_CNT);
                        cb_base6.Text = CommandList[Common.SelectDev, Common.SelectCom, i].ARG_BASE;
                        nu_argument6.Value = int.Parse(CommandList[Common.SelectDev, Common.SelectCom, i].ARG_NUM);
                        break;
                    case CommID.指定bit出力:
                        cb_CommandSel.Text = "指定bit出力";
                        aux = Convert.ToInt32(CommandList[Common.SelectDev, Common.SelectCom, i].AUX_DATA, 16);
                        bit = ((aux >> 7) & 1);
                        cb_aux77.Text = bit.ToString();
                        bit = ((aux >> 6) & 1);
                        cb_aux67.Text = bit.ToString();
                        bit = ((aux >> 5) & 1);
                        cb_aux57.Text = bit.ToString();
                        bit = ((aux >> 4) & 1);
                        cb_aux47.Text = bit.ToString();
                        bit = ((aux >> 3) & 1);
                        cb_aux37.Text = bit.ToString();
                        bit = ((aux >> 2) & 1);
                        cb_aux27.Text = bit.ToString();
                        bit = ((aux >> 1) & 1);
                        cb_aux17.Text = bit.ToString();
                        bit = ((aux >> 0) & 1);
                        cb_aux07.Text = bit.ToString();
                        cb_property7.Text = CommandList[Common.SelectDev, Common.SelectCom, i].PROPERTY;
                        nu_repeat7.Value = int.Parse(CommandList[Common.SelectDev, Common.SelectCom, i].REPEAT_CNT);
                        tb_COMMENT7.Text = CommandList[Common.SelectDev, Common.SelectCom, i].COMMENT;
                        nu_outcnt7.Value = int.Parse(CommandList[Common.SelectDev, Common.SelectCom, i].OUT_CNT);
                        cb_base7.Text = CommandList[Common.SelectDev, Common.SelectCom, i].ARG_BASE;
                        nu_argument7.Value = int.Parse(CommandList[Common.SelectDev, Common.SelectCom, i].ARG_NUM);
                        nu_upperbit7.Value = int.Parse(CommandList[Common.SelectDev, Common.SelectCom, i].ARG_UPPERBIT);
                        nu_lowerbit7.Value = int.Parse(CommandList[Common.SelectDev, Common.SelectCom, i].ARG_LOWERBIT);
                        break;
                    case CommID.HW出力:
                        cb_CommandSel.Text = "HW出力";
                        nu_repeat8.Value = int.Parse(CommandList[Common.SelectDev, Common.SelectCom, i].REPEAT_CNT);
                        tb_HW8.Text = CommandList[Common.SelectDev, Common.SelectCom, i].ARG_HW;
                        break;
                    case CommID.Delay:
                        cb_CommandSel.Text = "Delay";
                        nu_repeat9.Value = int.Parse(CommandList[Common.SelectDev, Common.SelectCom, i].REPEAT_CNT);
                        nu_Delay9.Value = int.Parse(CommandList[Common.SelectDev, Common.SelectCom, i].ARG_DELAY);
                        break;
                    default:
                        break;
                }
            }
        }

        private void lv_DevList_SelectedIndexChanged(object sender, EventArgs e)
        {
            int aux, bit;

            if (lv_DevList.SelectedItems.Count > 0)
            {
                int i = lv_DevList.SelectedItems[0].Index;
                tb_DevName.Text = DevList[i].DEV_NAME;
                nu_DevNum.Value = int.Parse(DevList[i].DEV_NUM);
                nu_DEFperiod.Value = int.Parse(DevList[i].PERIOD);
                cb_DEFphase.Text = DevList[i].PHASE;
                tb_DevCom.Text = DevList[i].COMMENT;
                aux = Convert.ToInt32(DevList[i].AUX_INIT, 16);
                bit = ((aux >> 7) & 1);
                cb_aux7init.Text = bit.ToString();
                bit = ((aux >> 6) & 1);
                cb_aux6init.Text = bit.ToString();
                bit = ((aux >> 5) & 1);
                cb_aux5init.Text = bit.ToString();
                bit = ((aux >> 4) & 1);
                cb_aux4init.Text = bit.ToString();
                bit = ((aux >> 3) & 1);
                cb_aux3init.Text = bit.ToString();
                bit = ((aux >> 2) & 1);
                cb_aux2init.Text = bit.ToString();
                bit = ((aux >> 1) & 1);
                cb_aux1init.Text = bit.ToString();
                bit = ((aux >> 0) & 1);
                cb_aux0init.Text = bit.ToString();

            }

        }

        private void lv_PinList_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (lv_PinList.SelectedItems.Count > 0)
            {
                int i = lv_PinList.SelectedItems[0].Index;
                cb_PINNAME.Text = PinList[Common.SelectDev, i].PinName;
                cb_PINNUM.Text = PinList[Common.SelectDev, i].PinNum;
                cb_PININIT.Text = PinList[Common.SelectDev, i].PinInit;
                cb_PINC.Text = PinList[Common.SelectDev, i].PinC;
                cb_PIND.Text = PinList[Common.SelectDev, i].PinD;
                cb_PINE.Text = PinList[Common.SelectDev, i].PinE;
                cb_PINF.Text = PinList[Common.SelectDev, i].PinF;

            }

        }

        private void lv_ComNameList_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (lv_ComNameList.SelectedItems.Count > 0)
            {
                int i = lv_ComNameList.SelectedItems[0].Index;
                tb_CommandName.Text = ComList[Common.SelectDev, i].COM_NAME;
                tb_CNComment.Text = ComList[Common.SelectDev, i].COMMENT;

            }

        }

        private void cb_ComNameSel_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (cb_ComNameSel.Items.Count > 0)
            {
                Common.SelectCom = cb_ComNameSel.SelectedIndex;
                CommandListIndex = ComListOrg[Common.SelectDev, Common.SelectCom].COMMAND_COUNT;
                UpdateCommandList(0);
            }
        }

        private void cb_SelDev_SelectedIndexChanged(object sender, EventArgs e)
        {
            cb_ComNameSel.Items.Clear();
            if (cb_SelDev.Items.Count > 0)
            {
                Common.SelectDev = cb_SelDev.SelectedIndex;// Common.DEVICE_COUNT;
                Common.SelectCom = 0;
                DevListIndex = Common.DEVICE_COUNT;
                PinListIndex = DevListOrg[Common.SelectDev].PIN_COUNT;
                ComListIndex = DevListOrg[Common.SelectDev].COM_COUNT;
                CommandListIndex = ComListOrg[Common.SelectDev, Common.SelectCom].COMMAND_COUNT;
                for (int i = 0; i < DevListOrg[Common.SelectDev].COM_COUNT; i++)
                {
                    cb_ComNameSel.Items.Add(ComListOrg[Common.SelectDev, i].COM_NAME);
                }
                cb_ComNameSel.Text = ComListOrg[Common.SelectDev, 0].COM_NAME;

                for (int i = 0; i < DevListOrg[Common.SelectDev].COM_COUNT; i++)
                {
                    ComListOrg[Common.SelectDev, i].CopyTo(ComList[Common.SelectDev, i]);
                }

                UpdatePinList(0);
                UpdateComList(0);
                UpdateCommandList(0);
            }
            tb_DATANum.Text = Common.DATANum;
            tb_AUXNum.Text = Common.AUXNum;

        }
        //ファイルSAVE
        private void bt_filesave_Click(object sender, EventArgs e)
        {
            int i, j, k;
            string fileName = tb_FILE.Text;
            bool Initial_Flag = false;       // EEPROM保存ファイルの初期化フラグ

            sfd.FileName = fileName;
            sfd.InitialDirectory = AppFileDir;

            sfd.Filter = "設定アプリファイル(*.csv)|*.csv|一時ファイル(*.frm)|*.frm|すべてのファイル(*.*)|*.*";
            sfd.FilterIndex = 1;

            sfd.Title = "保存先のファイルを選択してください";

            //ダイアログボックスを閉じる前に現在のディレクトリを復元するようにする
            sfd.RestoreDirectory = true;
            //既に存在するファイル名を指定したとき警告する
            //デフォルトでTrueなので指定する必要はない
            sfd.OverwritePrompt = true;
            //存在しないパスが指定されたとき警告を表示する
            //デフォルトでTrueなので指定する必要はない
            sfd.CheckPathExists = true;

            //ダイアログを表示する
            if (sfd.ShowDialog() == DialogResult.OK)
            {

                //ファイルを開く
                System.IO.StreamWriter fs = new System.IO.StreamWriter(sfd.FileName,
                Initial_Flag,
                System.Text.Encoding.GetEncoding("shift_jis"));
                //保存する
                fs.Write("##START##" + CRLF);
                fs.Write("#HeaderInformation#" + CRLF);
                fs.Write(tb_InfoID.Text + "," + tb_InfoKind.Text + CRLF);
                fs.Write("#DeviceInformation#" + CRLF);
                //デバイス情報
                for (i = 0; i < Common.DEVICE_COUNT; i++)
                {
                    if (DevListOrg[i].DEV_NAME == "") break;
                    fs.Write(DevListOrg[i].DEV_NAME + ",");
                    fs.Write(DevListOrg[i].DEV_NUM + ",");
                    fs.Write(DevListOrg[i].COM_NUM + ",");
                    fs.Write(DevListOrg[i].AUX_INIT + ",");
                    fs.Write(DevListOrg[i].PERIOD + ",");
                    fs.Write(DevListOrg[i].PHASE + ",");
                    fs.Write(DevListOrg[i].COMMENT + CRLF);
                }
                //配線情報
                fs.Write("#PinInformation#" + CRLF);
                for (i = 0; i < Common.DEVICE_COUNT; i++)
                {
                    if (DevListOrg[i].DEV_NAME == "") break;
                    fs.Write(DevListOrg[i].PIN_COUNT + CRLF);
                    for (j = 0; j < DevListOrg[i].PIN_COUNT; j++)
                    {
                        fs.Write(PinList[i, j].PinName + ",");
                        fs.Write(PinList[i, j].PinNum + ",");
                        fs.Write(PinList[i, j].PinInit + ",");
                        fs.Write(PinList[i, j].PinC + ",");
                        fs.Write(PinList[i, j].PinD + ",");
                        fs.Write(PinList[i, j].PinE + ",");
                        fs.Write(PinList[i, j].PinF + CRLF);
                    }
                }
                //コマンド情報
                fs.Write("#CommandInformation#" + CRLF);
                for (i = 0; i < Common.DEVICE_COUNT; i++)
                {
                    if (DevListOrg[i].DEV_NAME == "") break;
                    fs.Write(DevListOrg[i].COM_COUNT + CRLF);
                    for (j = 0; j < DevListOrg[i].COM_COUNT; j++)
                    {
                        //                        fs.Write(i+","+j+",");
                        fs.Write(ComListOrg[i, j].COM_NAME + ",");
                        fs.Write(ComListOrg[i, j].COMMENT + CRLF);
                    }
                }
                //コマンド
                fs.Write("#CommandInstruction#" + CRLF);
                for (i = 0; i < Common.DEVICE_COUNT; i++)
                {
                    if (DevListOrg[i].DEV_NAME == "") break;
                    for (j = 0; j < DevListOrg[i].COM_COUNT; j++)
                    {
                        if (ComListOrg[i, j].COM_NAME == "") break;
                        fs.Write(ComListOrg[i, j].COMMAND_COUNT + CRLF);
                        for (k = 0; k < ComListOrg[i, j].COMMAND_COUNT; k++)
                        {
                            fs.Write(CommandList[i, j, k].CommandID + ",");
                            fs.Write(CommandList[i, j, k].REPEAT_CNT + ",");
                            fs.Write(CommandList[i, j, k].PROPERTY + ",");
                            fs.Write(CommandList[i, j, k].AUX_DATA + ",");
                            fs.Write(CommandList[i, j, k].CONSTANT + ",");
                            fs.Write(CommandList[i, j, k].OUT_CNT + ",");
                            fs.Write(CommandList[i, j, k].ARG_BASE + ",");
                            fs.Write(CommandList[i, j, k].ARG_NUM + ",");
                            fs.Write(CommandList[i, j, k].ARG_DIV + ",");
                            fs.Write(CommandList[i, j, k].ARG_ADD + ",");
                            fs.Write(CommandList[i, j, k].ARG_UPPERBIT + ",");
                            fs.Write(CommandList[i, j, k].ARG_LOWERBIT + ",");
                            fs.Write(CommandList[i, j, k].ARG_HW + ",");
                            fs.Write(CommandList[i, j, k].ARG_DELAY + ",");
                            fs.Write(CommandList[i, j, k].COMMENT + CRLF);
                        }

                    }
                }
                //閉じる
                fs.Close();
                MessageBox.Show("ファイルを保存しました。", "ファイル保存", MessageBoxButtons.OK, MessageBoxIcon.Information);

            }
        }

        //ファイル LOAD
        private void bt_fileread_Click(object sender, EventArgs e)
        {
            string fileName = tb_FILE.Text;
            string[] column = { "" };
            int devmax = 0;
            int devcnt = 0, pincnt = 0, commandcnt = 0;
            int errornum = 0;
            int[] comcnt = new int[64];
            int id = 0;

            if (MessageBox.Show("指定ファイルのコマンド定義を読み込みます。", "内部データの初期化",
                MessageBoxButtons.OKCancel, MessageBoxIcon.Warning) == DialogResult.OK)
            {
                if (System.IO.File.Exists(fileName))
                {
                    TextFieldParser parser = new TextFieldParser(fileName, Encoding.GetEncoding("shift_JIS"));
                    parser.TextFieldType = FieldType.Delimited;
                    parser.SetDelimiters(",");

                    Common.ReadProc = 0;

                    while (parser.EndOfData == false)
                    {
                        column = parser.ReadFields();

                        //                        for (int i = 0; i < column.Length; i++)
                        //                        {
                        if (Common.ReadProc == 0)
                        {
                            devcnt = 0;
                            if (column[0] == "#HeaderInformation#")
                            {
                                Common.ReadProc = 1;
                                //                                    break;
                            }
                            if (column[0] == "#DeviceInformation#")
                            {
                                Common.ReadProc = 2;
                                //                                    break;
                            }
                            if (column[0] == "#PinInformation#")
                            {
                                Common.ReadProc = 3;
                                //                                    break;
                            }
                            if (column[0] == "#CommandInformation#")
                            {
                                Common.ReadProc = 4;
                                //                                    break;
                            }
                            if (column[0] == "#CommandInstruction#")
                            {
                                Common.ReadProc = 5;
                                //                                    break;
                            }
                        }
                        else
                        {
                            switch (Common.ReadProc)
                            {
                                case 0:
                                    break;
                                case 1:     //ヘッダー
                                    tb_ID.Text = column[0];
                                    tb_InfoID.Text = column[0];
                                    tb_InfoKind.Text = column[1];
                                    tb_InfoRest.Text = ((int)ConstGui.MAX_DEV_ITEMLIST - int.Parse(column[1])).ToString();
                                    devmax = int.Parse(column[1]);
                                    Common.DEVICE_COUNT = devmax;
                                    if (devmax == 0) { errornum = 1; }
                                    Common.ReadProc = 0;
                                    break;
                                case 2:     //デバイス
                                    DevListOrg[devcnt].DEV_NAME = column[0];
                                    DevListOrg[devcnt].DEV_NUM = column[1];
                                    DevListOrg[devcnt].COM_NUM = column[2];
                                    DevListOrg[devcnt].AUX_INIT = column[3];
                                    DevListOrg[devcnt].PERIOD = column[4];
                                    DevListOrg[devcnt].PHASE = column[5];
                                    if (column.Length > 6)
                                        DevListOrg[devcnt].COMMENT = column[6];
                                    else DevListOrg[devcnt].COMMENT = null;
                                    devcnt++;
                                    if (devcnt == devmax)
                                    {
                                        Common.ReadProc = 0;
                                    }
                                    break;
                                case 3:     //ピン
                                    pincnt = int.Parse(column[0]);
                                    DevListOrg[devcnt].PIN_COUNT = pincnt;
                                    for (int i = 0; i < pincnt; i++)
                                    {
                                        column = parser.ReadFields();
                                        switch (column[0])
                                        {
                                            case "TWE": id = 0; break;
                                            case "ALE1": id = 1; break;
                                            case "ALE2": id = 2; break;
                                            case "ALE3": id = 3; break;
                                            case "CLE1": id = 4; break;
                                            case "CLE2": id = 5; break;
                                            case "FMS": id = 6; break;
                                            case "RFU": id = 7; break;
                                            case "DATA00": id = 8; break;
                                            case "DATA01": id = 9; break;
                                            case "DATA02": id = 10; break;
                                            case "DATA03": id = 11; break;
                                            case "DATA04": id = 12; break;
                                            case "DATA05": id = 13; break;
                                            case "DATA06": id = 14; break;
                                            case "DATA07": id = 15; break;
                                            case "DATA08": id = 16; break;
                                            case "DATA09": id = 17; break;
                                            case "DATA10": id = 18; break;
                                            case "DATA11": id = 19; break;
                                            case "DATA12": id = 20; break;
                                            case "DATA13": id = 21; break;
                                            case "DATA14": id = 22; break;
                                            case "DATA15": id = 23; break;
                                            case "DATA16": id = 24; break;
                                            case "DATA17": id = 25; break;
                                            case "DATA18": id = 26; break;
                                            case "DATA19": id = 27; break;
                                            case "DATA20": id = 28; break;
                                            case "DATA21": id = 29; break;
                                            case "DATA22": id = 30; break;
                                            case "DATA23": id = 31; break;
                                            case "AUX0": id = 32; break;
                                            case "AUX1": id = 33; break;
                                            case "AUX2": id = 34; break;
                                            case "AUX3": id = 35; break;
                                            case "AUX4": id = 36; break;
                                            case "AUX5": id = 37; break;
                                            case "AUX6": id = 38; break;
                                            case "AUX7": id = 39; break;
                                            default: break;
                                        }
                                        PinList[devcnt, i].PinID = (PinID)id;
                                        PinList[devcnt, i].PinName = column[0];
                                        PinList[devcnt, i].PinNum = column[1];
                                        PinList[devcnt, i].PinInit = column[2];
                                        PinList[devcnt, i].PinC = column[3];
                                        PinList[devcnt, i].PinD = column[4];
                                        PinList[devcnt, i].PinE = column[5];
                                        if (column.Length > 6)
                                            PinList[devcnt, i].PinF = column[6];
                                        else PinList[devcnt, i].PinF = null;
                                    }
                                    devcnt++;
                                    if (devcnt == devmax)
                                    {
                                        Common.ReadProc = 0;
                                    }
                                    break;
                                case 4:     //コマンド
                                    comcnt[devcnt] = int.Parse(column[0]);
                                    DevListOrg[devcnt].COM_COUNT = comcnt[devcnt];
                                    for (int i = 0; i < comcnt[devcnt]; i++)
                                    {
                                        column = parser.ReadFields();
                                        ComListOrg[devcnt, i].COM_NAME = column[0];
                                        if (column.Length > 1)
                                            ComListOrg[devcnt, i].COMMENT = column[1];
                                        else ComListOrg[devcnt, i].COMMENT = null;

                                    }
                                    devcnt++;
                                    if (devcnt == devmax)
                                    {
                                        Common.ReadProc = 0;
                                    }
                                    break;
                                case 5:     //命令
                                    for (int i = 0; i < comcnt[devcnt]; i++)
                                    {
                                        if (i != 0) column = parser.ReadFields();
                                        commandcnt = int.Parse(column[0]);
                                        ComListOrg[devcnt, i].COMMAND_COUNT = commandcnt;
                                        for (int j = 0; j < commandcnt; j++)
                                        {
                                            column = parser.ReadFields();
                                            switch (column[0])
                                            {
                                                case "定数出力": id = 1; break;
                                                case "引数出力": id = 2; break;
                                                case "除算bit出力": id = 3; break;
                                                case "除加算出力": id = 4; break;
                                                case "TDS出力": id = 5; break;
                                                case "Chip出力": id = 6; break;
                                                case "指定bit出力": id = 7; break;
                                                case "HW出力": id = 8; break;
                                                case "Delay": id = 9; break;
                                                case "繰り返し": id = 31; break;
                                                default: break;
                                            }
                                            CommandList[devcnt, i, j].CommandID = (CommID)id;
                                            CommandList[devcnt, i, j].REPEAT_CNT = column[1];
                                            CommandList[devcnt, i, j].PROPERTY = column[2];
                                            CommandList[devcnt, i, j].AUX_DATA = column[3];
                                            CommandList[devcnt, i, j].CONSTANT = column[4];
                                            CommandList[devcnt, i, j].OUT_CNT = column[5];
                                            CommandList[devcnt, i, j].ARG_BASE = column[6];
                                            CommandList[devcnt, i, j].ARG_NUM = column[7];
                                            CommandList[devcnt, i, j].ARG_DIV = column[8];
                                            CommandList[devcnt, i, j].ARG_ADD = column[9];
                                            CommandList[devcnt, i, j].ARG_UPPERBIT = column[10];
                                            CommandList[devcnt, i, j].ARG_LOWERBIT = column[11];
                                            CommandList[devcnt, i, j].ARG_HW = column[12];
                                            CommandList[devcnt, i, j].ARG_DELAY = column[13];
                                            if (column.Length > 14)
                                                CommandList[devcnt, i, j].COMMENT = column[14];
                                            else CommandList[devcnt, i, j].COMMENT = null;
                                        }
                                    }
                                    devcnt++;
                                    if (devcnt == devmax)
                                    {
                                        Common.ReadProc = 0;
                                    }
                                    break;
                                default:
                                    break;
                            }
                        }
                        if (errornum != 0) break;

                    }
                    //画面再構築
                    DevListIndex = Common.DEVICE_COUNT;    // cb_SelDev.SelectedIndex;// Common.DEVICE_COUNT;

                    cb_SelDev.Items.Clear();
                    for (int i = 0; i < Common.DEVICE_COUNT; i++)
                    {
                        DevListOrg[i].CopyTo(DevList[i]);
                        cb_SelDev.Items.Add(DevList[i].DEV_NAME);
                    }
                    cb_SelDev.Text = DevList[0].DEV_NAME;
                    tb_InfoKind.Text = Common.DEVICE_COUNT.ToString();
                    tb_InfoRest.Text = ((int)ConstGui.MAX_DEV_ITEMLIST - Common.DEVICE_COUNT).ToString();


                    UpdateDevList(0);

                    if (Common.DEVICE_COUNT != 0)
                    {
                        for (int i = 0; i < DevList[Common.DEVICE_COUNT - 1].COM_COUNT; i++)
                        {
                            ComListOrg[Common.DEVICE_COUNT - 1, i].CopyTo(ComList[Common.DEVICE_COUNT - 1, i]);
                        }
                    }
                    PinListIndex = DevListOrg[Common.SelectDev].PIN_COUNT;
                    ComListIndex = DevListOrg[Common.SelectDev].COM_COUNT;
                    CommandListIndex = ComListOrg[Common.SelectDev, Common.SelectCom].COMMAND_COUNT;
                    UpdatePinList(0);
                    UpdateComList(0);
                    UpdateCommandList(0);

                    MessageBox.Show("ファイルを読み込みました。", "読み込みの終了", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                {
                    MessageBox.Show("ファイルが存在しません。", "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }

            }

        }

        private void bt_compile_Click(object sender, EventArgs e)
        {
            tb_OverView.Clear();

            ErrorSyntaxCheck();
            bt_filesave.Enabled = true;
            bt_send.Enabled = true;
        }

        private void bt_DispResultCLR_Click(object sender, EventArgs e)
        {
            tb_DispResult.Clear();
        }

        private void bt_send_Click(object sender, EventArgs e)
        {
            string sendbuf = "";
            string buf = "";
            int n;
            string res = "";

            if (ErrorSyntaxCheck() != 0)
            {
                MessageBox.Show("   ---定義確認 失敗---" + CRLF + "定義を確認し、修正してください。", "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                Cursor.Current = Cursors.Default;
                return;
            }

            if (MessageBox.Show("   ---定義確認 正常終了---" + CRLF + "ファームウェアへコマンド定義を送信します。", "FIRM書き換え",
                MessageBoxButtons.OKCancel, MessageBoxIcon.Warning) == DialogResult.OK)
            {
                Cursor.Current = Cursors.WaitCursor;
                int flag = MakeSendImage();
                if (flag != 0)
                {
                    MessageBox.Show("定義がEEPROM容量を超えています。", "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    Cursor.Current = Cursors.Default;
                    return;
                }
                try
                {
                    MySerialPort.DiscardInBuffer();
                    string send_Data = "EEPWR" + CRLF;
                    MySerialPort.Write(send_Data);

                    res = MySerialPort.ReadLine();
                    //                res = "EEPWR,OK";//debug
                    if (res.Length > 4) res = res.Substring(0, 4);
                    if (res == "DONE")
                        tb_DataDisplay.AppendText("送信開始" + CRLF);
                    else
                    {
                        tb_DataDisplay.AppendText("送信失敗" + CRLF);
                        Cursor.Current = Cursors.Default;
                        return;
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    tb_DataDisplay.AppendText("送信失敗" + CRLF);
                    Cursor.Current = Cursors.Default;
                    return;
                }

                if (res == "DONE")
                {
                    if (Common.SendASCIIImage.Length == 0)
                    {
                        tb_DataDisplay.AppendText("送出データがありません" + CRLF);
                        goto sendend;
                    }
                    buf = Common.SendASCIIImage;
                    while (buf.Length != 0)
                    {
                        if (buf.Length > 32) n = 32;
                        else n = buf.Length;

                        sendbuf = buf.Substring(0, n);
                        buf = buf.Substring(n);
                        try
                        {
                            string send_Data = "EEPDT," + sendbuf + CRLF;
                            MySerialPort.Write(send_Data);

                            res = MySerialPort.ReadLine();
                            tb_DataDisplay.AppendText(".");
                        }
                        catch (Exception ex)
                        {
                            MessageBox.Show(ex.Message, "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            tb_DataDisplay.AppendText("送信失敗" + CRLF);
                            Cursor.Current = Cursors.Default;
                            return;
                        }
                    }
                }
                tb_DataDisplay.AppendText(CRLF);

                try
                {
                    string send_Data = "EEPED" + CRLF;
                    MySerialPort.Write(send_Data);

                    res = MySerialPort.ReadLine();
                    tb_DataDisplay.AppendText("送信終了" + CRLF);
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    tb_DataDisplay.AppendText("送信失敗" + CRLF);
                    Cursor.Current = Cursors.Default;
                    return;
                }

            sendend:
                Cursor.Current = Cursors.Default;
                MessageBox.Show("送信完了しました。", "送信の終了", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        private void bt_comread_Click(object sender, EventArgs e)
        {

            string res = "";

            if (MessageBox.Show("ファームウェアからコマンド定義を読み込みます。", "内部データの初期化",
                MessageBoxButtons.OKCancel, MessageBoxIcon.Warning) == DialogResult.OK)
            {
                Cursor.Current = Cursors.WaitCursor;
                Common.RecvASCIIImage = "";
                try
                {
                    MySerialPort.DiscardInBuffer();
                    string send_Data = "EEPRD" + CRLF;
                    MySerialPort.Write(send_Data);

                    res = MySerialPort.ReadLine();
                    tb_DataDisplay.AppendText("読み込み開始" + CRLF);
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    tb_DataDisplay.AppendText("読み込み失敗" + CRLF);
                    Cursor.Current = Cursors.Default;
                    return;
                }
                while (res != "DONE")
                {
                    try
                    {
                        string send_Data = "EEPGT" + CRLF;
                        MySerialPort.Write(send_Data);
                        res = MySerialPort.ReadLine();
                        if (res.Length < 5) res = MySerialPort.ReadLine();
                        res = res.Substring(6);
                        if (res.Substring(res.Length - 1, 1) == "\r") res = res.Substring(0, res.Length - 1);
                        tb_DataDisplay.AppendText(".");
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.Message, "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        tb_DataDisplay.AppendText("読み込み失敗" + CRLF);
                        Cursor.Current = Cursors.Default;
                        return;
                    }
                    Common.RecvASCIIImage += res;
                }
                tb_DataDisplay.AppendText(CRLF);
                tb_DataDisplay.AppendText("読み込み終了" + CRLF);
                ParseRecieveData();

                //            System.Threading.Thread.Sleep(5000);
                Cursor.Current = Cursors.Default;
                MessageBox.Show("コマンド定義を読み込みました。", "読み込みの終了", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        private void bt_send1_Click(object sender, EventArgs e)
        {
            Cursor.Current = Cursors.WaitCursor;

            try
            {
                MySerialPort.DiscardInBuffer();
                string send_Data = "DEVWR," + nu_DeviceNum.Value.ToString() + CRLF;
                MySerialPort.Write(send_Data);
                string res = MySerialPort.ReadLine();
                tb_DataDisplay2.AppendText(res + CRLF);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            Cursor.Current = Cursors.Default;
        }

        private void bt_send2_Click(object sender, EventArgs e)
        {
            Cursor.Current = Cursors.WaitCursor;
            try
            {
                string p = "";

                switch (cb_phase.Text)
                {
                    case "φ1":
                        p = "0";
                        break;
                    case "φ2":
                        p = "1";
                        break;
                    case "φ3":
                        p = "2";
                        break;
                    case "φ4":
                        p = "3";
                        break;
                    default:
                        break;
                }
                MySerialPort.DiscardInBuffer();
                string send_Data = "CLKWR," + nu_period.Value.ToString() + "," + p + CRLF;
                MySerialPort.Write(send_Data);
                string res = MySerialPort.ReadLine();
                tb_DataDisplay2.AppendText(res + CRLF);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            Cursor.Current = Cursors.Default;

        }

        private void bt_send3_Click(object sender, EventArgs e)
        {
            Cursor.Current = Cursors.WaitCursor;
            try
            {
                MySerialPort.DiscardInBuffer();
                string send_Data = tb_command.Text + "," + tb_ARG1.Text + "," + tb_ARG2.Text
                    + "," + tb_ARG3.Text + "," + tb_ARG4.Text
                    + "," + tb_ARG5.Text + "," + tb_ARG6.Text
                    + "," + tb_ARG7.Text + "," + tb_ARG8.Text
                    + "," + tb_ARG9.Text + "," + tb_ARG10.Text
                    + "," + tb_ARG11.Text + "," + tb_ARG12.Text
                    + "," + tb_ARG13.Text + "," + tb_ARG14.Text
                    + "," + tb_ARG15.Text + "," + tb_ARG16.Text
                    + CRLF;
                MySerialPort.Write(send_Data);
                string res = MySerialPort.ReadLine();
                tb_DataDisplay2.AppendText(res + CRLF);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            Cursor.Current = Cursors.Default;

        }

        private void bt_recvMSGCLR_Click(object sender, EventArgs e)
        {
            tb_DataDisplay.Clear();
        }

        private void bt_recvMSGCLR2_Click(object sender, EventArgs e)
        {
            tb_DataDisplay2.Clear();

        }

        private void bt_abort_Click(object sender, EventArgs e)
        {
            Cursor.Current = Cursors.WaitCursor;
            try
            {
                string send_Data = "FPABT" + CRLF;
                MySerialPort.Write(send_Data);
                System.Threading.Thread.Sleep(500);
                MySerialPort.DiscardInBuffer();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            Cursor.Current = Cursors.Default;
        }

        private void setupPinAssign_gui()
        {
            for (int i = 0; i < 24; ++i)
            {
                bool state = true;
                if (PinListIndex == 0)
                {
                    state = false;
                }
                else
                {
                    for (int j = 0; j < PinListIndex; ++j)
                    {
                        if (PinList[Common.SelectDev, j].PinNum == (i + 1).ToString())
                        {
                            state = false;
                            break;
                        }
                    }
                }
                if (checkBox1.CheckState != CheckState.Checked) state = false;
                if(state)
                {
                    PinAssignList[i].Items.Clear();
                    for (int j = 0; j < PinListIndex; ++j)
                    {
                        PinAssignList[i].Items.Add(PinList[Common.SelectDev, j].PinNum);
                    }
                }
                PinAssignList[i].Enabled = state;
            }
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            setupPinAssign_gui();
        }
        ////////////////

    }

    class MySerialPort : System.IO.Ports.SerialPort
    {
        private static Stream openedBaseStream;

        public MySerialPort()
            : base()
        {
            openedBaseStream = null;
        }

        public new void Open()
        {
            base.Open();
            openedBaseStream = this.BaseStream;
        }

        protected override void Dispose(bool disposing)
        {
            try
            {
                if (openedBaseStream != null)
                    openedBaseStream.Close();
                openedBaseStream = null;
            }
            catch (UnauthorizedAccessException)
            {
            }
            catch (Exception) { }

            base.Dispose(disposing);
        }
    }
}
