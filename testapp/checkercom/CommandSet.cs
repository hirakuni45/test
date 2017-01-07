using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;


namespace checkercom
{

    /** �R�}���h�Z�b�g.
     *  �R�}���h����ъ֘A�֐�
     */
    class CommandSet
    {

        public CommID CommandID;        /**< �R�}���hID                             */
//        public string ARG1;
//        public string ARG2;
//        public string ARG3;
        public string REPEAT_CNT;
        public string PROPERTY;
        public string AUX_DATA;
        public string CONSTANT;
        public string OUT_CNT;
        public string ARG_BASE;
        public string ARG_NUM;
        public string ARG_DIV;
        public string ARG_ADD;
        public string ARG_UPPERBIT;
        public string ARG_LOWERBIT;
        public string ARG_HW;
        public string ARG_DELAY;
        public string COMMENT;

        /** �R���X�g���N�^.
         */
        public CommandSet()
        {
            CommandID = CommID.�萔�o��;
//            ARG1 = "";
//            ARG2 = "";
//            ARG3 = "";
            REPEAT_CNT = "1";
            PROPERTY="C";
            AUX_DATA="0";
            CONSTANT = "0";
            OUT_CNT="1";
            ARG_BASE="DEC";
            ARG_NUM="1";
            ARG_DIV="1";
            ARG_ADD="0";
            ARG_UPPERBIT="0";
            ARG_LOWERBIT="1";
            ARG_HW="0";
            ARG_DELAY="0";
            COMMENT = "";
        }

        /** �I�u�W�F�N�g�̃R�s�[.
         * @param com �R�s�[��R�}���h�Z�b�g�I�u�W�F�N�g
         */
        public void CopyTo(CommandSet com)
        {
//            com.ARG1 = this.ARG1;
//            com.ARG2 = this.ARG2;
//            com.ARG3 = this.ARG3;
            com.CommandID = this.CommandID;
            com.REPEAT_CNT = this.REPEAT_CNT;
            com.PROPERTY=this.PROPERTY;
            com.AUX_DATA=this.AUX_DATA;
            com.CONSTANT = this.CONSTANT;
            com.OUT_CNT=this.OUT_CNT;
            com.ARG_BASE=this.ARG_BASE;
            com.ARG_NUM=this.ARG_NUM;
            com.ARG_DIV=this.ARG_DIV;
            com.ARG_ADD=this.ARG_ADD;
            com.ARG_UPPERBIT=this.ARG_UPPERBIT;
            com.ARG_LOWERBIT=this.ARG_LOWERBIT;
            com.ARG_HW=this.ARG_HW;
            com.ARG_DELAY=this.ARG_DELAY;
            com.COMMENT = this.COMMENT;

        }

        /** �p�����[�^�e�L�X�g�����񐶐�.
         *   �R�}���h�ɉ������p�����[�^�̃��X�g���e�L�X�g������
         *  @param ParameterText �R�}���h������(�Q�ƌ^)
         */
        public void BuildParameterText(ref string ParameterText)
        {
            ParameterText = ""; // �p�����[�^���N���A

            // �p�����[�^�e�L�X�g�̐���
            switch (CommandID)
            {
                case CommID.�萔�o��:
                    ParameterText += this.PROPERTY+",0x"+this.AUX_DATA+","+this.CONSTANT;
                    break;
                case CommID.�����o��:
                    ParameterText += this.PROPERTY+",0x"+this.AUX_DATA+","+this.OUT_CNT+","+this.ARG_BASE+","+this.ARG_NUM;
                    break;
                case CommID.���Zbit�o��:
                    ParameterText += this.PROPERTY+",0x"+this.AUX_DATA+","+this.OUT_CNT+","+this.ARG_BASE+","+this.ARG_NUM+","+this.ARG_DIV;
                    break;
                case CommID.�����Z�o��:
                    ParameterText += this.PROPERTY+",0x"+this.AUX_DATA + "," + this.OUT_CNT + "," + this.ARG_BASE + "," + this.ARG_NUM + "," + this.ARG_DIV + "," + this.ARG_ADD;
                    break;
                case CommID.TDS�o��:
                    ParameterText += this.PROPERTY+",0x"+this.AUX_DATA + "," + this.OUT_CNT + "," + this.ARG_BASE + "," + this.ARG_NUM + "," + this.ARG_DIV;
                    break;
                case CommID.Chip�o��:
                    ParameterText += this.PROPERTY+",0x"+this.AUX_DATA + "," + this.OUT_CNT + "," + this.ARG_BASE + "," + this.ARG_NUM;
                    break;
                case CommID.�w��bit�o��:
                    ParameterText += this.PROPERTY+",0x"+this.AUX_DATA + "," + this.OUT_CNT + "," + this.ARG_BASE + "," + this.ARG_NUM + "," + this.ARG_LOWERBIT + "," + this.ARG_UPPERBIT;
                    break;
                case CommID.HW�o��:
                    ParameterText += this.ARG_HW;
                    break;
                case CommID.Delay:
                    ParameterText += this.ARG_DELAY;
                    break;
                default:
                    break;

            }
        }

    }

    class PinSet
    {

        public PinID PinID;        /**<  �s�� ID                             */
        public string PinName;
        public string PinNum;

        public string PinInit;
        public string PinC;
        public string PinD;
        public string PinE;
        public string PinF;

        /** �R���X�g���N�^.
         */
        public PinSet()
        {
            PinID = 0;
            PinName = "";
            PinNum = "";
            PinInit = "";
            PinC = "";
            PinD = "";
            PinE = "";
            PinF = "";
        }

        /** �I�u�W�F�N�g�̃R�s�[.
         * @param com �R�s�[��R�}���h�Z�b�g�I�u�W�F�N�g
         */
        public void CopyTo(PinSet pin)
        {
            pin.PinName = this.PinName;
            pin.PinNum = this.PinNum;

            pin.PinID = this.PinID;
            pin.PinInit = this.PinInit;
            pin.PinC = this.PinC;
            pin.PinD = this.PinD;
            pin.PinE = this.PinE;
            pin.PinF = this.PinF;

        }

        /** �p�����[�^�e�L�X�g�����񐶐�.
         *   �R�}���h�ɉ������p�����[�^�̃��X�g���e�L�X�g������
         *  @param ParameterText �R�}���h������(�Q�ƌ^)
         */
        public void BuildParameterText(ref string ParameterText)
        {
            ParameterText = ""; // �p�����[�^���N���A

            // �p�����[�^�e�L�X�g�̐���
            switch (PinID)
            {
                case PinID.TWE:
                    ParameterText += "-";
                    break;
                case PinID.CLE1:
                    break;
                case PinID.CLE2:
                    break;
                default:
                    break;

            }
        }

    }

    public class Common
    {
        public static string m_SerialPort = "";         // �V���A���|�[�g
        public static string AUXNum="0";
        public static string DATANum="0";
        public static int ReadProc=0;
        public static int DEVICE_COUNT = 0;
        public static int SelectDev = 0;
        public static int SelectCom;
        public static string SendASCIIImage;
        public static string RecvASCIIImage;
        public static int []ArgMaxCHK=new int[16];
    }

    public class DeviceINF
    {
        public int PIN_COUNT;
        public int COM_COUNT;
        public string DEV_NAME;
        public string DEV_NUM;
        public string COM_NUM;
//        public object PIN_INF;
        public string AUX_INIT;
//        public object[] COM_INF;
        public string PERIOD;
        public string PHASE;
        public string COMMENT;
        public string OLD_NAME;     //temporary
        public bool COPY_PIN_ENA;
        public int[] COPY_PIN;

        public DeviceINF()
        {
            PIN_COUNT = 0;
            COM_COUNT = 0;
            DEV_NAME = "";
            DEV_NUM = "";
            COM_NUM = "0";
            AUX_INIT = "";
            PERIOD = "";
            PHASE = "";
            COMMENT = "";
            OLD_NAME="";
            COPY_PIN_ENA = false;
            COPY_PIN = new int[24];
            for (int i = 0; i < 24; ++i) COPY_PIN[i] = -1;
//            PIN_INF = main_Form.PinList;
        }

        public void CopyTo(DeviceINF dev)
        {
            dev.PIN_COUNT = this.PIN_COUNT;
            dev.COM_COUNT = this.COM_COUNT;
            dev.DEV_NAME = this.DEV_NAME;
            dev.DEV_NUM = this.DEV_NUM;
            dev.COM_NUM = this.COM_NUM;
            dev.AUX_INIT = this.AUX_INIT;
            dev.PERIOD = this.PERIOD;
            dev.PHASE = this.PHASE;
            dev.COMMENT = this.COMMENT;
            dev.COPY_PIN_ENA = this.COPY_PIN_ENA;
            dev.COPY_PIN = this.COPY_PIN;
        }

    }

    public class CommandINF
    {
        public int COMMAND_COUNT;
        public string COM_NAME;
        public string COMMENT;
        public string OLD_NAME;
//        public object[] COMMANDSET;

        public CommandINF()
        {
            COMMAND_COUNT = 0;
            COM_NAME = "";
            COMMENT = "";
            OLD_NAME = "";
        }

        public void CopyTo(CommandINF dev)
        {
            dev.COMMAND_COUNT = this.COMMAND_COUNT;
            dev.COM_NAME = this.COM_NAME;
            dev.COMMENT = this.COMMENT;

        }

    }

}

