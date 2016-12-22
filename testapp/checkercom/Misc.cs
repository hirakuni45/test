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
        private int ErrorSyntaxCheck()
        {
            int i, j, k;
            int r1 = 0, r2 = 0;
            string fn, fp;
            int result = 0;
            int auxnum = 0;
            int auxmax = 0;
            int datanum = 0;
            int datamax = 0;

            //デバイス情報

            if (Common.DEVICE_COUNT == 0)
            {
                tb_DispResult.AppendText("定義がありません  " + CRLF);
                result = 1;
                goto chkend;
            }

            if (Common.DEVICE_COUNT - 1 > 0)
            {
                for (i = 0; i < Common.DEVICE_COUNT - 1; i++)
                {
                    fn = DevListOrg[i].DEV_NAME;
                    for (j = i + 1; j < Common.DEVICE_COUNT; j++)
                    {
                        if (fn == DevListOrg[j].DEV_NAME)
                        {
                            tb_DispResult.AppendText("デバイスの名前が重複しています  " + fn + CRLF);
                            result = 1;
                        }
                        if (DevListOrg[i].DEV_NUM == DevListOrg[j].DEV_NUM)
                        {
                            tb_DispResult.AppendText("品種番号が重複しています  " + fn + CRLF);
                            result = 1;
                        }
                    }
                }
            }

            //ピン情報
            ///ピン名,ピン番号重複
            if (Common.DEVICE_COUNT > 0)
            {
                for (i = 0; i < Common.DEVICE_COUNT; i++)
                {
                    if (DevListOrg[i].PIN_COUNT == 0)
                    {
                        tb_DispResult.AppendText("配線定義がありません  " + DevListOrg[i].DEV_NAME + CRLF);
                        result = 1;
                    }
                    if (DevListOrg[i].PIN_COUNT - 1 > 0)
                    {
                        for (j = 0; j < DevListOrg[i].PIN_COUNT; j++)
                        {
                            fn = PinList[i, j].PinName;
                            fp = PinList[i, j].PinNum;
                            for (k = j + 1; k < DevListOrg[i].PIN_COUNT; k++)
                            {
                                if (fn == PinList[i, k].PinName)
                                {
                                    tb_DispResult.AppendText("使用ピンの名前が重複しています  " + DevListOrg[i].DEV_NAME + CRLF);
                                    result = 1;
                                }
                                if (fp == PinList[i, k].PinNum)
                                {
                                    tb_DispResult.AppendText("使用ピンの番号が重複しています  " + DevListOrg[i].DEV_NAME + CRLF);
                                    result = 1;
                                }
                            }
                        }
                    }
                }
            }
            ///ピン名不連続
            if (Common.DEVICE_COUNT > 0)
            {
                for (i = 0; i < Common.DEVICE_COUNT; i++)
                {
                    auxnum = auxmax = datanum = datamax = 0;
                    for (j = 0; j < DevListOrg[i].PIN_COUNT; j++)
                    {
                        if (PinList[i, j].PinName.Substring(0, 3) == "AUX")
                        {
                            auxnum++;
                            if (int.Parse(PinList[i, j].PinName.Substring(3, 1)) >= auxmax) auxmax = int.Parse(PinList[i, j].PinName.Substring(3, 1)) + 1;
                        }
                        if (PinList[i, j].PinName.Substring(0, 3) == "DAT")
                        {
                            datanum++;
                            if (int.Parse(PinList[i, j].PinName.Substring(4, 2)) >= datamax) datamax = int.Parse(PinList[i, j].PinName.Substring(4, 2)) + 1;
                        }

                    }
                    if (auxnum != auxmax)
                    {
                        tb_DispResult.AppendText("AUXピンが不連続です  " + DevListOrg[i].DEV_NAME + CRLF);
                        result = 1;
                    }
                    if (datanum != datamax)
                    {
                        tb_DispResult.AppendText("DATAピンが不連続です  " + DevListOrg[i].DEV_NAME + CRLF);
                        result = 1;
                    }
                }
            }
            ////AUX
            ////DATA

            //コマンド情報
            if (Common.DEVICE_COUNT > 0)
            {
                for (i = 0; i < Common.DEVICE_COUNT; i++)
                {
                    if (DevListOrg[i].COM_COUNT == 0)
                    {
                        tb_DispResult.AppendText("コマンド名定義がありません  " + DevListOrg[i].DEV_NAME + CRLF);
                        result = 1;
                    }
                    if (DevListOrg[i].COM_COUNT - 1 > 0)
                    {
                        for (j = 0; j < DevListOrg[i].COM_COUNT; j++)
                        {
                            fn = ComListOrg[i, j].COM_NAME;
                            for (k = j + 1; k < DevListOrg[i].COM_COUNT; k++)
                            {
                                if (fn == ComListOrg[i, k].COM_NAME)
                                {
                                    tb_DispResult.AppendText("コマンドの名前が重複しています  " + DevListOrg[i].DEV_NAME + CRLF);
                                    result = 1;
                                }
                            }
                        }
                    }
                }
            }
            //基本命令  命令別
            if (Common.DEVICE_COUNT > 0)
            {
                for (i = 0; i < Common.DEVICE_COUNT; i++)
                {
                    int Command_size = 0;           //コマンドサイズ計算 ファーム側のバッファー用
                    for (j = 0; j < DevListOrg[i].COM_COUNT; j++)
                    {
                        if (ComListOrg[i, j].COMMAND_COUNT==0)
                        {
                            tb_DispResult.AppendText("コマンドがありません  " + DevListOrg[i].DEV_NAME + "(" + ComListOrg[i, j].COM_NAME + ")" +CRLF);
                            result = 1;
                        }
                        Command_size += 4;      //初期値と終端
                        for (k = 0; k < ComListOrg[i, j].COMMAND_COUNT; k++)
                        {
                            switch (CommandList[i, j, k].CommandID)
                            {
                                case CommID.定数出力:
                                    r1 = r2 = 0;
                                    try
                                    {
                                        int n = int.Parse(CommandList[i, j, k].CONSTANT);
                                    }
                                    catch
                                    {
                                        r1 = 1;
                                    }
                                    try
                                    {
                                        int n = Convert.ToInt32(CommandList[i, j, k].CONSTANT, 16);
                                    }
                                    catch
                                    {
                                        r2 = 1;
                                    }
                                    if ((r1 & r2) != 0)
                                    {
                                        tb_DispResult.AppendText("定数命令違反  " + DevListOrg[i].DEV_NAME + "(" + ComListOrg[i, j].COM_NAME + ")" + CRLF);
                                        result = 1;
                                    }
                                    if (CommandList[i, j, k].REPEAT_CNT != "1") Command_size += 4;
                                    Command_size += 5;
                                    break;
                                case CommID.引数出力:
                                    if (CommandList[i, j, k].REPEAT_CNT != "1") Command_size += 4;
                                    Command_size += 3;
                                    break;
                                case CommID.除算bit出力:
                                    if (CommandList[i, j, k].REPEAT_CNT != "1") Command_size += 4;
                                    Command_size += 5;
                                    break;
                                case CommID.除加算出力:
                                    if (CommandList[i, j, k].REPEAT_CNT != "1") Command_size += 4;
                                    Command_size += 7;
                                    break;
                                case CommID.TDS出力:
                                    if (int.Parse(CommandList[i, j, k].ARG_NUM) > 2000)
                                    {
                                        tb_DispResult.AppendText("TDS命令違反  " + DevListOrg[i].DEV_NAME + "(" + ComListOrg[i, j].COM_NAME + ")" + CRLF);
                                        result = 1;
                                    }
                                    if (CommandList[i, j, k].REPEAT_CNT != "1") Command_size += 4;
                                    Command_size += 7;
                                    break;
                                case CommID.Chip出力:
                                    if (int.Parse(CommandList[i, j, k].ARG_NUM) > 2000)
                                    {
                                        tb_DispResult.AppendText("Chip命令違反  " + DevListOrg[i].DEV_NAME + "(" + ComListOrg[i, j].COM_NAME + ")" + CRLF);
                                        result = 1;
                                    }
                                    if (CommandList[i, j, k].REPEAT_CNT != "1") Command_size += 4;
                                    Command_size += 3;
                                    break;
                                case CommID.指定bit出力:
                                    if (int.Parse(CommandList[i, j, k].ARG_UPPERBIT) < int.Parse(CommandList[i, j, k].ARG_LOWERBIT))
                                    {
                                        tb_DispResult.AppendText("指定bit命令違反  " + DevListOrg[i].DEV_NAME + "(" + ComListOrg[i, j].COM_NAME + ")" + CRLF);
                                        result = 1;
                                    }
                                    if (CommandList[i, j, k].REPEAT_CNT != "1") Command_size += 4;
                                    Command_size += 7;
                                    break;
                                case CommID.HW出力:
                                    r1 = r2 = 0;
                                    try
                                    {
                                        int n = int.Parse(CommandList[i, j, k].ARG_HW);
                                    }
                                    catch
                                    {
                                        r1 = 1;
                                    }
                                    try
                                    {
                                        int n = Convert.ToInt32(CommandList[i, j, k].ARG_HW, 16);
                                    }
                                    catch
                                    {
                                        r2 = 1;
                                    }
                                    if ((r1 & r2) != 0)
                                    {
                                        tb_DispResult.AppendText("HW命令違反  " + DevListOrg[i].DEV_NAME + "(" + ComListOrg[i, j].COM_NAME + ")" + CRLF);
                                        result = 1;
                                    }
                                    if (CommandList[i, j, k].REPEAT_CNT != "1") Command_size += 4;
                                    Command_size += 5;
                                    break;
                                case CommID.Delay:
                                    if (CommandList[i, j, k].REPEAT_CNT != "1") Command_size += 4;
                                    Command_size += 5;
                                    break;
                                default: break;
                            }

                        }

                    }
                    if (Command_size > (int)ConstGui.MAX_FIRM_MEM)
                    {
                        tb_DispResult.AppendText("コマンド定義オーバー(別デバイスで分離定義してください)  " + DevListOrg[i].DEV_NAME + CRLF);
                        result = 1;
                    }
                }
            }

            //etc
        chkend:
            if (result == 0)
            {
                tb_DispResult.AppendText("正常終了" + CRLF);
                MakeOverView();
            }
            tb_DispResult.AppendText("-------------" + CRLF);
            return result;
        }
        private int DevNameCheck(string dn)
        {
            int i;
            int result = 0;

            //デバイスNAME
            if (dn == "") result = 1;
            else
            {
                if (Common.DEVICE_COUNT > 0)
                {
                    for (i = 0; i < Common.DEVICE_COUNT; i++)
                    {
                        if (DevList[i].DEV_NAME == dn)
                        {
                            result = 1;
                        }
                    }
                }
            }
            return result;
        }
        private int ComNameCheck(string cn)
        {
            int i;
            int result = 0;

            //コマンドNAME
            if (cn == "") result = 1;
            else
            {
                if (ComList[Common.SelectDev, Common.SelectCom].COMMAND_COUNT > 0)
                {
                    for (i = 0; i < ComList[Common.SelectDev, Common.SelectCom].COMMAND_COUNT; i++)
                    {
                        if (ComList[Common.SelectDev, i].COM_NAME == cn)
                        {
                            result = 1;
                        }
                    }
                }
            }
            return result;
        }

        //ファームへの転送イメージ作成
        private int MakeSendImage()
        {
            //
            string asc = "";

            //全体ヘッダー
            Common.SendASCIIImage = "";
            STRtoHEX(tb_InfoID.Text, 16, ref asc);
            Common.SendASCIIImage += asc;
            asc = "";
            BYTESTRtoHEX(tb_InfoKind.Text, ref asc);
            Common.SendASCIIImage += asc;

            //デバイス品種情報
            for (int i = 0; i < Common.DEVICE_COUNT; i++)
            {
                asc = "";
                STRtoHEX(DevListOrg[i].DEV_NAME, 16, ref asc);//品種名
                Common.SendASCIIImage += asc;
                asc = "";
                BYTESTRtoHEX(DevListOrg[i].DEV_NUM, ref asc);//品種番号
                Common.SendASCIIImage += asc;
                asc = "";
                BYTESTRtoHEX(DevListOrg[i].COM_NUM, ref asc);//コマンド数
                Common.SendASCIIImage += asc;
                //配線情報
                asc = "";
                MakePinInfo(i, DevListOrg[i].PIN_COUNT, ref asc);
                Common.SendASCIIImage += asc;

                //AUX,Clock,Phase
                asc = "";
                byte bt = (byte)Convert.ToInt32(DevListOrg[i].AUX_INIT, 16);
                BYTEtoHEX(bt, ref asc);
                Common.SendASCIIImage += asc;
                asc = "";
                int ck = int.Parse(DevListOrg[i].PERIOD);
                bt = (byte)(ck & 0xff);
                BYTEtoHEX(bt, ref asc);
                Common.SendASCIIImage += asc;
                asc = "";
                bt = (byte)((ck >> 8) & 0xff);
                BYTEtoHEX(bt, ref asc);
                Common.SendASCIIImage += asc;
                asc = "";
                bt = (byte)((ck >> 16) & 0xff);
                BYTEtoHEX(bt, ref asc);
                Common.SendASCIIImage += asc;

                asc = "";
                if (DevListOrg[i].PHASE == "φ1") bt = 0;
                if (DevListOrg[i].PHASE == "φ2") bt = 1;
                if (DevListOrg[i].PHASE == "φ3") bt = 2;
                if (DevListOrg[i].PHASE == "φ4") bt = 3;
                BYTEtoHEX(bt, ref asc);
                Common.SendASCIIImage += asc;

                //コマンド情報
                asc = "";
                MakeComInfo(i, DevList[i].COM_COUNT, ref asc);
                Common.SendASCIIImage += asc;

            }

            if(Common.SendASCIIImage.Length>0x1fff0*2) {
                return 1;
            }
            return 0;
        }

        private void MakePinInfo(int devnum, int pinnum, ref string res)
        {
            string temp = "";
            byte tb = 0, tb2 = 0;
            int i, j, p = 0;

            for (i = 0; i < 24; i++)
            {
                for (j = 0, p = 0; j < pinnum; j++)
                {
                    if (i == int.Parse(PinList[devnum, j].PinNum)-1)    //PinNumは1 orijin
                    {
                        switch (PinList[devnum, j].PinID)
                        {
                            case PinID.TWE:
                                tb = 0xc0;
                                tb2 = 0;
                                break;
                            case PinID.ALE1:
                                tb = (byte)(int.Parse(PinList[devnum, j].PinInit) * 16 + int.Parse(PinList[devnum, j].PinC) * 8 + int.Parse(PinList[devnum, j].PinD) * 4 +
                                    int.Parse(PinList[devnum, j].PinE) * 2 + int.Parse(PinList[devnum, j].PinF) + 0x80);
                                tb2 = 0;
                                break;
                            case PinID.ALE2:
                                tb = (byte)(int.Parse(PinList[devnum, j].PinInit) * 16 + int.Parse(PinList[devnum, j].PinC) * 8 + int.Parse(PinList[devnum, j].PinD) * 4 +
                                    int.Parse(PinList[devnum, j].PinE) * 2 + int.Parse(PinList[devnum, j].PinF) + 0x80);
                                tb2 = 1;
                                break;
                            case PinID.ALE3:
                                tb = (byte)(int.Parse(PinList[devnum, j].PinInit) * 16 + int.Parse(PinList[devnum, j].PinC) * 8 + int.Parse(PinList[devnum, j].PinD) * 4 +
                                    int.Parse(PinList[devnum, j].PinE) * 2 + int.Parse(PinList[devnum, j].PinF) + 0x80);
                                tb2 = 2;
                                break;
                            case PinID.CLE1:
                                tb = (byte)(int.Parse(PinList[devnum, j].PinInit) * 16 + int.Parse(PinList[devnum, j].PinC) * 8 + int.Parse(PinList[devnum, j].PinD) * 4 +
                                    int.Parse(PinList[devnum, j].PinE) * 2 + int.Parse(PinList[devnum, j].PinF) + 0x80);
                                tb2 = 3;
                                break;
                            case PinID.CLE2:
                                tb = (byte)(int.Parse(PinList[devnum, j].PinInit) * 16 + int.Parse(PinList[devnum, j].PinC) * 8 + int.Parse(PinList[devnum, j].PinD) * 4 +
                                    int.Parse(PinList[devnum, j].PinE) * 2 + int.Parse(PinList[devnum, j].PinF) + 0x80);
                                tb2 = 4;
                                break;
                            case PinID.FMS:
                                tb = (byte)(int.Parse(PinList[devnum, j].PinInit) * 16 + int.Parse(PinList[devnum, j].PinC) * 8 + int.Parse(PinList[devnum, j].PinD) * 4 +
                                    int.Parse(PinList[devnum, j].PinE) * 2 + int.Parse(PinList[devnum, j].PinF) + 0x80);
                                tb2 = 5;
                                break;
                            case PinID.RFU:
                                tb = (byte)(int.Parse(PinList[devnum, j].PinInit) * 16 + int.Parse(PinList[devnum, j].PinC) * 8 + int.Parse(PinList[devnum, j].PinD) * 4 +
                                    int.Parse(PinList[devnum, j].PinE) * 2 + int.Parse(PinList[devnum, j].PinF) + 0x80);
                                tb2 = 6;
                                break;
                            case PinID.DATA00:
                            case PinID.DATA01:
                            case PinID.DATA02:
                            case PinID.DATA03:
                            case PinID.DATA04:
                            case PinID.DATA05:
                            case PinID.DATA06:
                            case PinID.DATA07:
                            case PinID.DATA08:
                            case PinID.DATA09:
                            case PinID.DATA10:
                            case PinID.DATA11:
                            case PinID.DATA12:
                            case PinID.DATA13:
                            case PinID.DATA14:
                            case PinID.DATA15:
                            case PinID.DATA16:
                            case PinID.DATA17:
                            case PinID.DATA18:
                            case PinID.DATA19:
                            case PinID.DATA20:
                            case PinID.DATA21:
                            case PinID.DATA22:
                            case PinID.DATA23:
                                tb = (byte)((int)PinList[devnum, j].PinID - (int)PinID.DATA00 + 0xa0);
                                tb2 = 0;
                                break;
                            case PinID.AUX0:
                            case PinID.AUX1:
                            case PinID.AUX2:
                            case PinID.AUX3:
                            case PinID.AUX4:
                            case PinID.AUX5:
                            case PinID.AUX6:
                            case PinID.AUX7:
                                tb = (byte)((int)PinList[devnum, j].PinID - (int)PinID.AUX0 + 24 + 0xa0);
                                tb2 = 0;
                                break;
                            default:
                                tb = 0;
                                tb2 = 0;
                                break;
                        }
                        p = 1;
                    }
                }
                if (p == 0)
                {
                    tb = 0;
                    tb2 = 0;
                }
                temp = "";
                BYTEtoHEX(tb, ref temp);
                res += temp;
                temp = "";
                BYTEtoHEX(tb2, ref temp);
                res += temp;
            }

        }

        private void MakeComInfo(int devnum, int comnum, ref string res)
        {
            string temp;
            byte[] tb = new byte[7];
            int cnst;

            for (int i = 0; i < comnum; i++)
            {
                //コマンド名
                temp = "";
                STRtoHEX(ComListOrg[devnum, i].COM_NAME, 4, ref temp);
                res += temp;

                //引数MAX計算


                res += "0000";                  //初期値
                for (int j = 0; j < ComListOrg[devnum, i].COMMAND_COUNT; j++)
                {
                    if (CommandList[devnum, i, j].PROPERTY == "C") tb[0] = 0;
                    if (CommandList[devnum, i, j].PROPERTY == "D") tb[0] = 0x40;
                    if (CommandList[devnum, i, j].PROPERTY == "E") tb[0] = 0x80;
                    if (CommandList[devnum, i, j].PROPERTY == "F") tb[0] = 0xc0;
                    tb[0] |= (byte)(CommandList[devnum, i, j].CommandID);
                    tb[1] = (byte)Convert.ToInt32(CommandList[devnum, i, j].AUX_DATA,16);
                    switch (CommandList[devnum, i, j].CommandID)
                    {
                        case CommID.定数出力:           //HW 出力に変換必要 → とりあえずしない
                            try
                            {
                                cnst = int.Parse(CommandList[devnum, i, j].CONSTANT);
                            }
                            catch
                            {
                                cnst = Convert.ToInt32(CommandList[devnum, i, j].CONSTANT, 16);
                            }
                            tb[2] = (byte)(cnst & 0xff);
                            tb[3] = (byte)((cnst >> 8) & 0xff);
                            tb[4] = (byte)((cnst >> 16) & 0xff);
                            for (int k = 0; k < 5; k++)
                            {
                                temp = "";
                                BYTEtoHEX(tb[k], ref temp);
                                res += temp;
                            }
                            break;
                        case CommID.引数出力:
                            tb[2] = (byte)((int.Parse(CommandList[devnum, i, j].OUT_CNT) - 1) << 5);  //OUT_CNT は1 orijin
                            if (CommandList[devnum, i, j].ARG_BASE == "HEX") tb[2] += 0x10;
                            tb[2] += (byte)(int.Parse(CommandList[devnum, i, j].ARG_NUM)-1);  //ARG_NUM は1 orijin
                            for (int k = 0; k < 3; k++)
                            {
                                temp = "";
                                BYTEtoHEX(tb[k], ref temp);
                                res += temp;
                            }
                            break;
                        case CommID.除算bit出力:
                            tb[2] = (byte)((int.Parse(CommandList[devnum, i, j].OUT_CNT)-1) << 5);
                            if (CommandList[devnum, i, j].ARG_BASE == "HEX") tb[2] += 0x10;
                            tb[2] += (byte)(int.Parse(CommandList[devnum, i, j].ARG_NUM)-1);
                            cnst = int.Parse(CommandList[devnum, i, j].ARG_DIV) - 1;  //ARG_DIV は1 orijin
                            tb[3] = (byte)(cnst & 0xff);
                            tb[4] = (byte)((cnst >> 8) & 0xff);
                            for (int k = 0; k < 5; k++)
                            {
                                temp = "";
                                BYTEtoHEX(tb[k], ref temp);
                                res += temp;
                            }
                            break;
                        case CommID.除加算出力:
                            tb[2] = (byte)((int.Parse(CommandList[devnum, i, j].OUT_CNT)-1) << 5);
                            if (CommandList[devnum, i, j].ARG_BASE == "HEX") tb[2] += 0x10;
                            tb[2] += (byte)(int.Parse(CommandList[devnum, i, j].ARG_NUM)-1);
                            cnst = int.Parse(CommandList[devnum, i, j].ARG_DIV)-1;
                            tb[3] = (byte)(cnst & 0xff);
                            tb[4] = (byte)((cnst >> 8) & 0xff);
                            cnst = int.Parse(CommandList[devnum, i, j].ARG_ADD);
                            tb[5] = (byte)(cnst & 0xff);
                            tb[6] = (byte)((cnst >> 8) & 0xff);
                            for (int k = 0; k < 7; k++)
                            {
                                temp = "";
                                BYTEtoHEX(tb[k], ref temp);
                                res += temp;
                            }
                            break;
                        case CommID.TDS出力:
                            tb[2] = 0;
                            if (CommandList[devnum, i, j].ARG_BASE == "HEX") tb[2] += 0x10;
                            tb[2] += (byte)(int.Parse(CommandList[devnum, i, j].ARG_NUM)-1);
                            cnst = int.Parse(CommandList[devnum, i, j].ARG_DIV)-1;
                            tb[3] = (byte)(cnst & 0xff);
                            tb[4] = (byte)((cnst >> 8) & 0xff);
                            cnst = int.Parse(CommandList[devnum, i, j].OUT_CNT)-1;
                            tb[5] = (byte)(cnst & 0xff);
                            tb[6] = (byte)((cnst >> 8) & 0xff);
                            for (int k = 0; k < 7; k++)
                            {
                                temp = "";
                                BYTEtoHEX(tb[k], ref temp);
                                res += temp;
                            }
                            break;
                        case CommID.Chip出力:
                            tb[2] = (byte)((int.Parse(CommandList[devnum, i, j].OUT_CNT)-1) << 5);
                            if (CommandList[devnum, i, j].ARG_BASE == "HEX") tb[2] += 0x10;
                            tb[2] += (byte)(int.Parse(CommandList[devnum, i, j].ARG_NUM)-1);
                            for (int k = 0; k < 3; k++)
                            {
                                temp = "";
                                BYTEtoHEX(tb[k], ref temp);
                                res += temp;
                            }
                            break;
                        case CommID.指定bit出力:
                            tb[2] = (byte)((int.Parse(CommandList[devnum, i, j].OUT_CNT)-1) << 5);
                            if (CommandList[devnum, i, j].ARG_BASE == "HEX") tb[2] += 0x10;
                            tb[2] += (byte)(int.Parse(CommandList[devnum, i, j].ARG_NUM)-1);
                            tb[3] = (byte)(int.Parse(CommandList[devnum, i, j].ARG_LOWERBIT));
                            tb[4] = (byte)(int.Parse(CommandList[devnum, i, j].ARG_UPPERBIT));
                            for (int k = 0; k < 5; k++)
                            {
                                temp = "";
                                BYTEtoHEX(tb[k], ref temp);
                                res += temp;
                            }
                            break;
                        case CommID.HW出力:
                            tb[0] = 0x08;
                            tb[1] = 0;
                            try
                            {
                                cnst = int.Parse(CommandList[devnum, i, j].ARG_HW);
                            }
                            catch
                            {
                                cnst = Convert.ToInt32(CommandList[devnum, i, j].ARG_HW, 16);
                            }
                            tb[2] = (byte)(cnst & 0xff);
                            tb[3] = (byte)((cnst >> 8) & 0xff);
                            tb[4] = (byte)((cnst >> 16) & 0xff);
                            for (int k = 0; k < 5; k++)
                            {
                                temp = "";
                                BYTEtoHEX(tb[k], ref temp);
                                res += temp;
                            }
                            break;
                        case CommID.Delay:
                            tb[0] = 0x09;
                            tb[1] = 0;
                            cnst = int.Parse(CommandList[devnum, i, j].ARG_DELAY);
                            tb[2] = (byte)(cnst & 0xff);
                            tb[3] = (byte)((cnst >> 8) & 0xff);
                            tb[4] = (byte)((cnst >> 16) & 0xff);
                            for (int k = 0; k < 5; k++)
                            {
                                temp = "";
                                BYTEtoHEX(tb[k], ref temp);
                                res += temp;
                            }
                            break;
                        case CommID.繰り返し:
                            break;
                        default:
                            break;
                    }
                    if (int.Parse(CommandList[devnum, i, j].REPEAT_CNT) > 1)
                    {
                        tb[0] = 0x1f;
                        tb[1] = 0;
                        cnst = int.Parse(CommandList[devnum, i, j].REPEAT_CNT) - 1;  //REPEAT は1 orijin
                        tb[2] = (byte)(cnst & 0xff);
                        tb[3] = (byte)((cnst >> 8) & 0xff);
                        //                        tb[4] = (byte)((cnst >> 8) & 0xff);
                        for (int k = 0; k < 4; k++)
                        {
                            temp = "";
                            BYTEtoHEX(tb[k], ref temp);
                            res += temp;
                        }
                    }
                }
                res += "FF00";              //終端
            }
        }

        private void STRtoHEX(string dt, int cnt, ref string res)
        {
            int high4, low4;
            string[] ASCII = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F" };

            byte[] data = System.Text.Encoding.ASCII.GetBytes(dt);

            for (int i = 0; i < cnt; i++)
            {
                if (i >= data.Length)
                {
                    res += ASCII[2] + ASCII[0];
                }
                else
                {
                    low4 = data[i] & 0xf;
                    high4 = (data[i] & 0xf0) >> 4;
                    res += ASCII[high4] + ASCII[low4];
                }
            }
        }

        private void HEXtoSTR(string dt, int cnt, ref string res)
        {
            int d;
            byte[] data = System.Text.Encoding.ASCII.GetBytes(dt);

            for (int i = 0; i < data.Length; i++)
            {
                if (data[i] >= (byte)'A') data[i] -= 55;
                else data[i] -= 48;
            }

            for (int i = 0; i < cnt / 2; i++)
            {
                d = data[i * 2] * 16 + data[i * 2 + 1];
                res += ((char)d).ToString();
            }

        }
        //2byte ASCII → byte
        private void HEXtoBYTE(string dt, ref byte res)
        {
            byte[] data = System.Text.Encoding.ASCII.GetBytes(dt);

            byte t1 = 0;
            byte t2 = 0;

            if (data[0] >= 'A') t1 = (byte)(data[0] - 'A' + 10);
            else t1 = (byte)(data[0] - '0');
            if (data[1] >= 'A') t2 = (byte)(data[1] - 'A' + 10);
            else t2 = (byte)(data[1] - '0');

            res = (byte)((t1 << 4) + t2);

        }

        private void BYTESTRtoHEX(string dt, ref string res)
        {
            int high4, low4;
            string[] ASCII = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F" };
            int a = int.Parse(dt);
            high4 = a >> 4;
            low4 = a & 0xf;
            res += ASCII[high4] + ASCII[low4];
        }
        private void BYTEtoHEX(byte dt, ref string res)
        {
            int high4, low4;
            string[] ASCII = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F" };
            high4 = dt >> 4;
            low4 = dt & 0xf;
            res += ASCII[high4] + ASCII[low4];
        }

        //ファームからの読み込み
        private void ParseRecieveData()
        {
            string asc = "";
            byte bb = 0;
            byte b1 = 0, b2 = 0, b3 = 0, b0 = 0;
            int index = 0;
            //debug

            //全体ヘッダー
            HEXtoSTR(Common.RecvASCIIImage.Substring(0, 32), 32, ref asc);
            asc = asc.TrimEnd();
            tb_InfoID.Text = asc;
            tb_ID.Text = asc;
            HEXtoBYTE(Common.RecvASCIIImage.Substring(32, 2), ref bb);
            tb_InfoKind.Text = bb.ToString();
            Common.DEVICE_COUNT = bb;

            //デバイス品種情報
            index = 34;
            for (int i = 0; i < bb; i++)
            {
                asc = "";
                HEXtoSTR(Common.RecvASCIIImage.Substring(index, 32), 32, ref asc);
                asc = asc.TrimEnd(); index += 32;
                DevListOrg[i].DEV_NAME = asc;
                HEXtoBYTE(Common.RecvASCIIImage.Substring(index, 2), ref b0);   //品種番号
                DevListOrg[i].DEV_NUM = b0.ToString(); index += 2;
                HEXtoBYTE(Common.RecvASCIIImage.Substring(index, 2), ref b0);   //コマンド数
                DevListOrg[i].COM_COUNT = b0;
                DevListOrg[i].COM_NUM = b0.ToString();                          //2013 8/20
                index += 2;
                //配線情報
                ReadPinInfo(i, ref index);

                HEXtoBYTE(Common.RecvASCIIImage.Substring(index, 2), ref b0);   //AUX初期値
                DevListOrg[i].AUX_INIT = Convert.ToString(b0, 16); index += 2;

                HEXtoBYTE(Common.RecvASCIIImage.Substring(index, 2), ref b1);   //
                index += 2;
                HEXtoBYTE(Common.RecvASCIIImage.Substring(index, 2), ref b2);   //
                index += 2;
                HEXtoBYTE(Common.RecvASCIIImage.Substring(index, 2), ref b3);   //
                index += 2;
                DevListOrg[i].PERIOD = (b3 * 0x100 + b2 * 0x10 + b1).ToString();

                HEXtoBYTE(Common.RecvASCIIImage.Substring(index, 2), ref b0);   //
                DevListOrg[i].PHASE = "φ" + (b0+1).ToString(); index += 2;

                DevListOrg[i].COMMENT = "";                 //COMMENT 

                //コマンド情報
                ReadComInfo(i, ref index);

            }
            //画面再構築
            DevListIndex = Common.DEVICE_COUNT;

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
                for (int i = 0; i < DevList[Common.DEVICE_COUNT-1].COM_COUNT; i++)
                {
                    ComListOrg[Common.DEVICE_COUNT-1, i].CopyTo(ComList[Common.DEVICE_COUNT-1, i]);
                }
            }
            PinListIndex = DevListOrg[Common.SelectDev].PIN_COUNT;
            ComListIndex = DevListOrg[Common.SelectDev].COM_COUNT;
            CommandListIndex = ComListOrg[Common.SelectDev, Common.SelectCom].COMMAND_COUNT;
            UpdatePinList(0);
            UpdateComList(0);
            UpdateCommandList(0);


        }
        private void ReadPinInfo(int devnum, ref int idx)
        {
            byte b1 = 0, b2 = 0;
            int pincnt = 0;
            int index = idx;

            for (int i = 0; i < 24; i++)
            {
                HEXtoBYTE(Common.RecvASCIIImage.Substring(index, 2), ref b1);
                HEXtoBYTE(Common.RecvASCIIImage.Substring(index + 2, 2), ref b2);
                index += 4;
                if ((b1 & 0x80) != 0)
                {
                    PinList[devnum, pincnt].PinNum = (i + 1).ToString();  //PinNumは1 orijin
                    if ((b1 & 0x40) != 0)//TWE
                    {
                        PinList[devnum, pincnt].PinID = PinID.TWE;
                        PinList[devnum, pincnt].PinName = "TWE";
                        PinList[devnum, pincnt].PinInit = "";
                        PinList[devnum, pincnt].PinC = "";
                        PinList[devnum, pincnt].PinD = "";
                        PinList[devnum, pincnt].PinE = "";
                        PinList[devnum, pincnt].PinF = "";
                    }
                    else if ((b1 & 0x20) != 0)//DATA AUX線
                    {
                        int num = b1 & 0x1f;
                        if (num < 24)//DATA
                        {
                            PinList[devnum, pincnt].PinName = "DATA" + num.ToString("00");
                        }
                        else         //AUX
                        {
                            PinList[devnum, pincnt].PinName = "AUX" + (num - 24).ToString("0");
                        }
                        PinList[devnum, pincnt].PinID = (PinID)(num + (int)PinID.DATA00);
                        PinList[devnum, pincnt].PinInit = "";
                        PinList[devnum, pincnt].PinC = "";
                        PinList[devnum, pincnt].PinD = "";
                        PinList[devnum, pincnt].PinE = "";
                        PinList[devnum, pincnt].PinF = "";
                    }
                    else                      //制御線
                    {
                        switch (b2)
                        {
                            case 0:
                                PinList[devnum, pincnt].PinID = PinID.ALE1;
                                PinList[devnum, pincnt].PinName = "ALE1";
                                break;
                            case 1:
                                PinList[devnum, pincnt].PinID = PinID.ALE2;
                                PinList[devnum, pincnt].PinName = "ALE2";
                                break;
                            case 2:
                                PinList[devnum, pincnt].PinID = PinID.ALE3;
                                PinList[devnum, pincnt].PinName = "ALE3";
                                break;
                            case 3:
                                PinList[devnum, pincnt].PinID = PinID.CLE1;
                                PinList[devnum, pincnt].PinName = "CLE1";
                                break;
                            case 4:
                                PinList[devnum, pincnt].PinID = PinID.CLE2;
                                PinList[devnum, pincnt].PinName = "CLE2";
                                break;
                            case 5:
                                PinList[devnum, pincnt].PinID = PinID.FMS;
                                PinList[devnum, pincnt].PinName = "FMS";
                                break;
                            case 6:
                                PinList[devnum, pincnt].PinID = PinID.RFU;
                                PinList[devnum, pincnt].PinName = "RFU";
                                break;
                            default:
                                break;
                        }
                        PinList[devnum, pincnt].PinInit = ((b1 >> 4) & 1).ToString();
                        PinList[devnum, pincnt].PinC = ((b1 >> 3) & 1).ToString();
                        PinList[devnum, pincnt].PinD = ((b1 >> 2) & 1).ToString();
                        PinList[devnum, pincnt].PinE = ((b1 >> 1) & 1).ToString();
                        PinList[devnum, pincnt].PinF = (b1 & 1).ToString();
                    }
                    pincnt++;
                }
            }
            DevListOrg[devnum].PIN_COUNT = pincnt;
            idx += 96;
        }

        private void ReadComInfo(int devnum, ref int idx)
        {
            string asc = "";
            int index = idx;
            byte b1 = 0, b2 = 0;
            int comcnt = 0;
            byte[] bt = new byte[10];
            string st = "";

            string dbg = "";

            for (int i = 0; i < DevListOrg[devnum].COM_COUNT; i++)
            {
                comcnt = 0;
                asc = "";
                HEXtoSTR(Common.RecvASCIIImage.Substring(index, 8), 8, ref asc);
                asc = asc.TrimEnd(); index += 8;
                ComListOrg[devnum, i].COM_NAME = asc.TrimEnd();

                ComListOrg[devnum, i].COMMENT = "";             //COMMENT INIT


                for (int j = 0; j < (int)ConstGui.MAX_NR_ITEM_IN_COMMANDLIST; j++)
                {
                    if (Common.RecvASCIIImage.Length > index + 14)          //デバッグ用変数
                        dbg = Common.RecvASCIIImage.Substring(index, 14);

                    asc = "";
                    HEXtoBYTE(Common.RecvASCIIImage.Substring(index, 2), ref b1);
                    index += 2;
                    HEXtoBYTE(Common.RecvASCIIImage.Substring(index, 2), ref b2);
                    index += 2;

                    if (b1 == 0) continue;
                    if (b1 == 0xff) break;
                    for (int k = 0; k < 5; k++)
                    {
                        bt[k] = 0;
                        if (Common.RecvASCIIImage.Length > index + 2 * k + 2)
                        {
                            string db = Common.RecvASCIIImage.Substring(index + 2 * k, 2);
                            HEXtoBYTE(Common.RecvASCIIImage.Substring(index + 2 * k, 2), ref bt[k]);
                        }
                    }

                    //2013 8/20
                    byte p1 = (byte)(b1 >> 6);
                    if (p1 == 0) CommandList[devnum, i, comcnt].PROPERTY = "C";
                    if (p1 == 1) CommandList[devnum, i, comcnt].PROPERTY = "D";
                    if (p1 == 2) CommandList[devnum, i, comcnt].PROPERTY = "E";
                    if (p1 == 3) CommandList[devnum, i, comcnt].PROPERTY = "F";

                    CommandList[devnum, i, comcnt].AUX_DATA = Convert.ToString(b2,16);

                    //////

                    switch (b1 & 0x3f)
                    {
                        case 1:
                            CommandList[devnum, i, comcnt].CommandID = CommID.定数出力;
                            CommandList[devnum, i, comcnt].CONSTANT = (bt[0] + (bt[1] << 8) + (bt[2] << 16)).ToString();
                            index += 6;
                            break;
                        case 2:
                            CommandList[devnum, i, comcnt].CommandID = CommID.引数出力;
                            CommandList[devnum, i, comcnt].OUT_CNT = ((bt[0] >> 5) + 1).ToString();
                            if ((bt[0] & 0x10) == 0) st = "DEC";
                            else st = "HEX";
                            CommandList[devnum, i, comcnt].ARG_BASE = st;
                            CommandList[devnum, i, comcnt].ARG_NUM = ((bt[0] & 0xf)+1).ToString();

                            index += 2;
                            break;
                        case 3:
                            CommandList[devnum, i, comcnt].CommandID = CommID.除算bit出力;
                            CommandList[devnum, i, comcnt].OUT_CNT = ((bt[0] >> 5) + 1).ToString();
                            if ((bt[0] & 0x10) == 0) st = "DEC";
                            else st = "HEX";
                            CommandList[devnum, i, comcnt].ARG_BASE = st;
                            CommandList[devnum, i, comcnt].ARG_NUM = ((bt[0] & 0xf)+1).ToString();
                            CommandList[devnum, i, comcnt].ARG_DIV = ((bt[1] + (bt[2] << 8))+1).ToString();

                            index += 6;
                            break;
                        case 4:
                            CommandList[devnum, i, comcnt].CommandID = CommID.除加算出力;
                            CommandList[devnum, i, comcnt].OUT_CNT = ((bt[0] >> 5) + 1).ToString();
                            if ((bt[0] & 0x10) == 0) st = "DEC";
                            else st = "HEX";
                            CommandList[devnum, i, comcnt].ARG_BASE = st;
                            CommandList[devnum, i, comcnt].ARG_NUM = ((bt[0] & 0xf)+1).ToString();
                            CommandList[devnum, i, comcnt].ARG_DIV = ((bt[1] + (bt[2] << 8))+1).ToString();
                            CommandList[devnum, i, comcnt].ARG_ADD = (bt[3] + (bt[4] << 8)).ToString();

                            index += 10;
                            break;
                        case 5:
                            CommandList[devnum, i, comcnt].CommandID = CommID.TDS出力;
                            CommandList[devnum, i, comcnt].OUT_CNT = ((bt[3] + (bt[4] << 8)) + 1).ToString();
                            if ((bt[0] & 0x10) == 0) st = "DEC";
                            else st = "HEX";
                            CommandList[devnum, i, comcnt].ARG_BASE = st;
                            CommandList[devnum, i, comcnt].ARG_NUM = ((bt[0] & 0xf)+1).ToString();
                            CommandList[devnum, i, comcnt].ARG_DIV = ((bt[1] + (bt[2] << 8))+1).ToString();

                            index += 10;
                            break;
                        case 6:
                            CommandList[devnum, i, comcnt].CommandID = CommID.Chip出力;
                            CommandList[devnum, i, comcnt].OUT_CNT = ((bt[0] >> 5) + 1).ToString();
                            if ((bt[0] & 0x10) == 0) st = "DEC";
                            else st = "HEX";
                            CommandList[devnum, i, comcnt].ARG_BASE = st;
                            CommandList[devnum, i, comcnt].ARG_NUM = ((bt[0] & 0xf)+1).ToString();

                            index += 2;
                            break;
                        case 7:
                            CommandList[devnum, i, comcnt].CommandID = CommID.指定bit出力;
                            CommandList[devnum, i, comcnt].OUT_CNT = ((bt[0] >> 5) + 1).ToString();
                            if ((bt[0] & 0x10) == 0) st = "DEC";
                            else st = "HEX";
                            CommandList[devnum, i, comcnt].ARG_BASE = st;
                            CommandList[devnum, i, comcnt].ARG_NUM = ((bt[0] & 0xf)+1).ToString();
                            CommandList[devnum, i, comcnt].ARG_LOWERBIT = bt[1].ToString();
                            CommandList[devnum, i, comcnt].ARG_UPPERBIT = bt[2].ToString();

                            index += 6;
                            break;
                        case 8:
                            CommandList[devnum, i, comcnt].CommandID = CommID.HW出力;
                            CommandList[devnum, i, comcnt].ARG_HW = (bt[0] + (bt[1] << 8) + (bt[2] << 16)).ToString();

                            index += 6;
                            break;
                        case 9:
                            CommandList[devnum, i, comcnt].CommandID = CommID.Delay;
                            CommandList[devnum, i, comcnt].ARG_DELAY = (bt[0] + (bt[1] << 8) + (bt[2] << 16)).ToString();

                            index += 6;
                            break;
                        case 0x1f:
                            comcnt--;
                            CommandList[devnum, i, comcnt].REPEAT_CNT = ((bt[0] + (bt[1] << 8))+1).ToString();

                            index += 4;
                            break;
                        default:
                            break;
                    }
                    CommandList[devnum, i, comcnt].COMMENT = "";        //COMMENT
                    comcnt++;
                }
                ComListOrg[devnum, i].COMMAND_COUNT = comcnt;
            }
            idx = index;
        }

        //ツリー再構築
        private void DevListREG()
        {

            int i = 0, j = 0, k = 0, n = 0;
            int br = 0;
            //            int cnt = 0;

            CommandSet[, ,] CommandListTemp;
            CommandINF[,] ComListTemp;
            PinSet[,] PinListTemp;

            CommandListTemp = new CommandSet[(int)ConstGui.MAX_DEV_ITEMLIST, (int)ConstGui.MAX_COM_ITEMLIST, (int)ConstGui.MAX_NR_ITEM_IN_COMMANDLIST];
            for (i = 0; i < (int)ConstGui.MAX_DEV_ITEMLIST; i++)
            {
                for (j = 0; j < (int)ConstGui.MAX_COM_ITEMLIST; j++)
                {
                    for (k = 0; k < (int)ConstGui.MAX_NR_ITEM_IN_COMMANDLIST; k++)
                    {
                        CommandListTemp[i, j, k] = new CommandSet();
                    }
                }
            }

            PinListTemp = new PinSet[(int)ConstGui.MAX_DEV_ITEMLIST, (int)ConstGui.MAX_PIN_ITEMLIST];
            for (i = 0; i < (int)ConstGui.MAX_DEV_ITEMLIST; i++)
            {
                for (j = 0; j < (int)ConstGui.MAX_PIN_ITEMLIST; j++)
                {
                    PinListTemp[i, j] = new PinSet();
                }
            }

            ComListTemp = new CommandINF[(int)ConstGui.MAX_DEV_ITEMLIST, (int)ConstGui.MAX_COMINF_ITEMLIST];
            for (i = 0; i < (int)ConstGui.MAX_DEV_ITEMLIST; i++)
            {
                for (j = 0; j < (int)ConstGui.MAX_COMINF_ITEMLIST; j++)
                {
                    ComListTemp[i, j] = new CommandINF();
                }
            }

            if (Common.DEVICE_COUNT == 0) goto nn;

            //DevList 登録時のツリー構造再構築
            for (i = 0; i < lv_DevList.Items.Count; i++)
            {
                for (j = 0, br = 0; j < Common.DEVICE_COUNT; j++)
                {
                    for (k = 0; k < DevListOrg[j].PIN_COUNT; k++)
                    {
                        if ((DevList[i].DEV_NAME == DevListOrg[j].DEV_NAME) || (DevList[i].OLD_NAME == DevListOrg[j].DEV_NAME))
                        {
                            br = 1;
                            PinList[j, k].CopyTo(PinListTemp[i, k]);
                        }
                    }
                    if (br == 1)
                    {
                        DevList[i].PIN_COUNT = DevListOrg[j].PIN_COUNT;
                        break;
                    }
                    else DevList[i].PIN_COUNT = 0;
                }
            }
            for (i = 0; i < lv_DevList.Items.Count; i++)
            {
                for (j = 0, br = 0; j < Common.DEVICE_COUNT; j++)
                {
                    for (k = 0; k < DevListOrg[j].COM_COUNT; k++)
                    {
                        if ((DevList[i].DEV_NAME == DevListOrg[j].DEV_NAME) || (DevList[i].OLD_NAME == DevListOrg[j].DEV_NAME))
                        {
                            br = 1;
                            ComListOrg[j, k].CopyTo(ComListTemp[i, k]);
                        }
                    }
                    if (br == 1)
                    {
                        DevList[i].COM_COUNT = DevListOrg[j].COM_COUNT;
                        DevList[i].COM_NUM = DevListOrg[j].COM_NUM;
                        break;
                    }
                    else
                    {
                        DevList[i].COM_COUNT = 0;
                        DevList[i].COM_NUM = "0";
                    }
                }
            }


            for (i = 0; i < lv_DevList.Items.Count; i++)
            {
                for (j = 0, br = 0; j < Common.DEVICE_COUNT; j++)
                {
                    for (k = 0; k < DevListOrg[j].COM_COUNT; k++)
                    {
                        for (n = 0; n < ComListOrg[j, k].COMMAND_COUNT; n++)
                        {
                            if ((DevList[i].DEV_NAME == DevListOrg[j].DEV_NAME) || (DevList[i].OLD_NAME == DevListOrg[j].DEV_NAME))
                            {
                                br = 1;
                                CommandList[j, k, n].CopyTo(CommandListTemp[i, k, n]);
                            }
                        }
                    }
                    if (br == 1) break;
                }
            }
        nn:
            //Temp→Org
            for (i = 0; i < (int)ConstGui.MAX_DEV_ITEMLIST; i++)
            {
                for (k = 0; k < (int)ConstGui.MAX_PIN_ITEMLIST; k++)
                {
                    PinListTemp[i, k].CopyTo(PinList[i, k]);
                }
                for (k = 0; k < (int)ConstGui.MAX_COM_ITEMLIST; k++)
                {
                    ComListTemp[i, k].CopyTo(ComListOrg[i, k]);
                    for (n = 0; n < (int)ConstGui.MAX_NR_ITEM_IN_COMMANDLIST; n++)
                    {
                        CommandListTemp[i, k, n].CopyTo(CommandList[i, k, n]);
                    }
                }
            }
        }

        private void ComListREG()
        {

            int i, j, k, n;
            int br = 0;

            CommandSet[, ,] CommandListTemp;

            CommandListTemp = new CommandSet[(int)ConstGui.MAX_DEV_ITEMLIST, (int)ConstGui.MAX_COM_ITEMLIST, (int)ConstGui.MAX_NR_ITEM_IN_COMMANDLIST];
            for (i = 0; i < (int)ConstGui.MAX_DEV_ITEMLIST; i++)
            {
                for (j = 0; j < (int)ConstGui.MAX_COM_ITEMLIST; j++)
                {
                    for (k = 0; k < (int)ConstGui.MAX_NR_ITEM_IN_COMMANDLIST; k++)
                    {
                        CommandListTemp[i, j, k] = new CommandSet();
                    }
                }
            }

            //ComList 登録時のツリー構造再構築
            for (i = 0; i < lv_ComNameList.Items.Count; i++)//LIST側
            {
                for (n = 0, br = 0; n < DevListOrg[Common.SelectDev].COM_COUNT; n++)
                {
                    if ((ComList[Common.SelectDev, i].COM_NAME == ComListOrg[Common.SelectDev, n].COM_NAME) || (ComList[Common.SelectDev, i].OLD_NAME == ComListOrg[Common.SelectDev, n].COM_NAME))
                    {
                        for (k = 0; k < ComListOrg[Common.SelectDev, n].COMMAND_COUNT; k++)
                        {
                            CommandList[Common.SelectDev, n, k].CopyTo(CommandListTemp[Common.SelectDev, i, k]);
                            br = 1;
                        }
                        ComList[Common.SelectDev, i].COMMAND_COUNT = ComListOrg[Common.SelectDev, n].COMMAND_COUNT;
                    }
                    if (br == 1) break;
                }
            }
            //


            for (i = 0; i < (int)ConstGui.MAX_COM_ITEMLIST; i++)
            {
                for (n = 0; n < (int)ConstGui.MAX_NR_ITEM_IN_COMMANDLIST; n++)
                {
                    CommandListTemp[Common.SelectDev, i, n].CopyTo(CommandList[Common.SelectDev, i, n]);
                }
            }
        }
        private void MakeOverView()
        {
            int aux = 0, data = 0;
            int i, j;

            tb_OverView.Clear();
            tb_OverView.AppendText("ID:" + tb_InfoID.Text + "      ");
            tb_OverView.AppendText(tb_InfoKind.Text + " devices" + CRLF);
            for (i = 0; i < Common.DEVICE_COUNT; i++)
            {
                tb_OverView.AppendText("DEVICE:" + DevListOrg[i].DEV_NAME + "      ");
                tb_OverView.AppendText(DevList[i].COM_COUNT + " commands      ");
                for (j = 0, aux = 0, data = 0; j < DevListOrg[i].PIN_COUNT; j++)
                {
                    if ((PinList[i, j].PinID >= PinID.DATA00) && (PinList[i, j].PinID <= PinID.DATA23)) data++;
                    if ((PinList[i, j].PinID >= PinID.AUX0) && (PinList[i, j].PinID <= PinID.AUX7)) aux++;
                }
                tb_OverView.AppendText(data + " dpins      ");
                tb_OverView.AppendText(aux + " apins      " + CRLF);
                for (j = 0; j < DevListOrg[i].COM_COUNT; j++)
                {
                    tb_OverView.AppendText("  COMMAND:" + ComListOrg[i, j].COM_NAME + CRLF);
                }

            }
        }
    }
}